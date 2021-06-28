/** 
 *  Copyright (c) 1999~2017, Altibase Corp. and/or its affiliates. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License, version 3,
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
 
/***********************************************************************
 * $Id: iloLoadInsert.h 88170 2020-07-23 23:32:06Z chkim $
 **********************************************************************/

/***********************************************************************
 * FILE DESCRIPTION :
 *
 * This file includes the inline functions that break off from
 * the iloLoad::InsertFromBuff function.
 **********************************************************************/

#define PRINT_RECORD(i)                                           \
    m_LogFile.PrintOneRecord(sHandle,                             \
            &m_pTableInfo[sConnIndex],                            \
            aData->mRecordNumber[i],                              \
            i);                                                   \
    m_BadFile.PrintOneRecord(sHandle,                             \
            &m_pTableInfo[sConnIndex],                            \
            i,                                                    \
            m_DataFile.mLOBFile,                                  \
            aData->mOptionUsed);

#define ECODE (uteGetErrorCODE(sErrorMgr))
#define FAIL_BREAK                                                \
    (ECODE == 0x3b032 ||                                          \
     ECODE == 0x5003b || /* buffer full */                        \
     ECODE == 0x51043 || /* 통신 장애    */                       \
     ECODE == 0x91044 || /* Data file IO error */                 \
     ECODE == 0x91045)   /* LOB file IO error */

#define ADD_ERROR_COUNT(cnt)                                      \
    aData->mErrorCount += (cnt); /* Fail whole array records. */  \
    mErrorCount += (cnt);        /* BUG-28608 */

inline IDE_RC iloLoad::InitContextData(iloLoadInsertContext *aData)
{
    iloaderHandle *sHandle = aData->mHandle;

    aData->mArrayCount = (m_pProgOption->m_ArrayCount == 0) ?
                         1 : m_pProgOption->m_ArrayCount;
    aData->mTotal      = 0;
    aData->mLoad4Sleep = 0;
    aData->mRealCount  = 0;
    aData->mOptionUsed = ILO_FALSE;       // BUG-24583

    iloMutexLock( sHandle, &(sHandle->mParallel.mLoadInsMutex) );
    /* PROJ-1714
     * sConnIndex는 현재 Thread에서 사용하게될 Identifier 같은 역할을 한다.
     * Connection과 TableInfo를 구분할 수 있도록 한다.
     */
    aData->mConnIndex = mConnIndex++;          
    aData->mLOBColExist = IsLOBColExist( sHandle, &m_pTableInfo[aData->mConnIndex] );
    
    iloMutexUnLock( sHandle, &(sHandle->mParallel.mLoadInsMutex) );

    // BUG-28605
    aData->mErrorMgr = (uteErrorMgr *)idlOS::malloc(
                           aData->mArrayCount * ID_SIZEOF(uteErrorMgr));
    IDE_TEST_RAISE(aData->mErrorMgr == NULL, MAllocError);
    idlOS::memset(aData->mErrorMgr, 0, aData->mArrayCount * ID_SIZEOF(uteErrorMgr));
    m_pISPApiUp[aData->mConnIndex].SetErrorMgr(aData->mArrayCount, aData->mErrorMgr);
  
    // BUG-28675
    aData->mRecordNumber = (SInt *)idlOS::calloc(aData->mArrayCount, ID_SIZEOF(SInt));
    IDE_TEST_RAISE(aData->mRecordNumber == NULL, MAllocError);
    
    /* BUG-24583
     * -lob 'use_separate_files=yes' 옵션을 사용할 경우에 
     * Bad File에 FilePath + FileName을 저장한다.
     */
    if ( aData->mLOBColExist == ILO_TRUE &&
         m_pProgOption->mUseSeparateFiles == ILO_TRUE )
    {
        aData->mOptionUsed = ILO_TRUE;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION(MAllocError);
    {
        iloMutexLock( sHandle, &(sHandle->mParallel.mLoadFIOMutex) );
        uteSetErrorCode(sHandle->mErrorMgr, utERR_ABORT_memory_error,
                    __FILE__, __LINE__);
        
        if ( sHandle->mUseApi != SQL_TRUE )
        {
            utePrintfErrorCode(stdout, sHandle->mErrorMgr);
        }
        
        aData->mTotal = 0;
        iloMutexUnLock( sHandle, &(sHandle->mParallel.mLoadFIOMutex) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

inline IDE_RC iloLoad::FiniContextData(iloLoadInsertContext *aData)
{
    SInt           sConnIndex = aData->mConnIndex;
    SInt           sTotal     = aData->mTotal;
    iloaderHandle *sHandle    = aData->mHandle;

    /* BUG-30413 */
    if ( sHandle->mProgOption->mGetTotalRowCount == ILO_TRUE )
    {
        sHandle->mStatisticLog.totalCount = aData->mRecordNumber[aData->mRealCount];
    }

    iloMutexLock( sHandle, &(sHandle->mParallel.mLoadInsMutex) );
    mTotalCount += sTotal;
    mParallelLoad[sConnIndex] = sTotal;
    sHandle->mParallel.mLoadThrNum--;   //BUG-24211
    iloMutexUnLock( sHandle, &(sHandle->mParallel.mLoadInsMutex) );

    // BUG-28605
    if (aData->mErrorMgr != NULL)
    {
        m_pISPApiUp[sConnIndex].SetErrorMgr(sHandle->mNErrorMgr,
                                            sHandle->mErrorMgr);
        idlOS::free(aData->mErrorMgr);
    }

    // BUG-28675
    if (aData->mRecordNumber != NULL)
    {
        idlOS::free(aData->mRecordNumber);
    }

    return IDE_SUCCESS;
}

/* BUG-46486 Improve lock wait
 * 레코드 단위 락킹에서 array 단위 락킹으로 변경
 */
inline SInt iloLoad::ReadRecord(iloLoadInsertContext *aData)
{
    SInt           sRet = READ_ERROR;
    SInt           sLogCallback = 0;
    SInt           sConnIndex = aData->mConnIndex;
    SInt           sArrayCount = aData->mArrayCount;
    iloaderHandle *sHandle    = aData->mHandle;
    uteErrorMgr   *sErrorMgr  = aData->mErrorMgr;

    aData->mRealCount = 0;
    sLogCallback = ( sHandle->mUseApi == SQL_TRUE ) && ( sHandle->mLogCallback != NULL );

    iloMutexLock( sHandle, &(sHandle->mParallel.mLoadInsMutex) );

    while ( aData->mRealCount < sArrayCount )
    {
        aData->mNextAction = NONE;

        // BUG-24898 iloader 파싱에러 상세화
        // 에러메시지를 clear 한다.
        if (uteGetErrorCODE(sHandle->mErrorMgr) != 0x00000)
        {
            uteClearError(sHandle->mErrorMgr);
        } 

        sRet = m_DataFile.ReadOneRecordFromCBuff(sHandle, 
                &m_pTableInfo[sConnIndex],
                aData->mRealCount);

        if (sRet != READ_SUCCESS)
        {
            idlOS::memcpy(sErrorMgr, sHandle->mErrorMgr, ID_SIZEOF(uteErrorMgr));
        }

        // BUG-24879 errors 옵션 지원
        // recordcount 를 정확히 출력하기 위해서 eof 일때는 count 하지 않는다.
        if (sRet != END_OF_FILE)
        {
            aData->mRecordNumber[aData->mRealCount] = mReadRecCount += 1; //읽은 Record 수
        }

        // BUG-28208: malloc 등에 실패했을 때 iloader 바로 종료 
        if ( (sRet == SYS_ERROR) ||
             (m_pProgOption->m_bExist_L &&
              (aData->mRecordNumber[aData->mRealCount] > m_pProgOption->m_LastRow)) )
        {
            if (aData->mRealCount == 0)
            {
                aData->mNextAction = BREAK;
            }
            break;
        }

        if (sRet == END_OF_FILE && aData->mRealCount == 0)
        {
            aData->mNextAction = CONTINUE;
            break;
        }

        if ( (sRet != END_OF_FILE) &&
             (aData->mRecordNumber[aData->mRealCount] < m_pProgOption->m_FirstRow) )
        {
            aData->mNextAction = CONTINUE;
            continue;
        }

        if (sRet != END_OF_FILE)
        {
            aData->mTotal ++;
            mLoadCount++;   // for progress

            //진행 상황 표시
            // BUG-27938: 중간에 continue, break를 하는 경우가 있으므로 여기서 찍어줘야한다.
            if ( sHandle->mUseApi != SQL_TRUE )
            {
                if ((mLoadCount % 100) == 0)
                {
                    PrintProgress( sHandle, mLoadCount );
                }
            }
        }

        if (sRet == READ_ERROR)
        {
            LogError(aData, aData->mRealCount);
            continue;
        }

        if (sRet == END_OF_FILE) break;

        // BUG-24890 error 가 발생하면 sRealCount를 증가하면 안된다.
        // sRealCount 는 array 의 갯수이다. 따라서 잘못된 메모리를 읽게된다.
        aData->mRealCount++;

        aData->mLoad4Sleep++; // for sleep, BUG-18707

        if (sLogCallback)
        {
            if (Callback4Api(aData) != IDE_SUCCESS)
            {
                aData->mNextAction = BREAK;
                break;
            }
        }
    }
    iloMutexUnLock( sHandle, &(sHandle->mParallel.mLoadInsMutex) );

    return sRet;
}

inline IDE_RC iloLoad::LogError(iloLoadInsertContext *aData,
                                SInt                  aArrayIndex)
{
    SInt           sConnIndex = aData->mConnIndex;
    iloaderHandle *sHandle    = aData->mHandle;
    uteErrorMgr   *sErrorMgr  = aData->mErrorMgr;

    iloMutexLock( sHandle, &(sHandle->mParallel.mLoadFIOMutex) );

    ADD_ERROR_COUNT(1);

    // BUG-21640 iloader에서 에러메시지를 알아보기 편하게 출력하기
    // log 파일에 rowcount 를 출력한다.
    PRINT_RECORD(aArrayIndex);

    // BUG-21640 iloader에서 에러메시지를 알아보기 편하게 출력하기
    // 기존 에러메시지와 동일한 형식으로 출력하는 함수추가   
    m_LogFile.PrintLogErr(sErrorMgr);

    ILO_CALLBACK;

    iloMutexUnLock( sHandle, &(sHandle->mParallel.mLoadFIOMutex) );

    return IDE_SUCCESS;
}

inline void iloLoad::Sleep(iloLoadInsertContext *aData)
{
    PDL_Time_Value sSleepTime;

    if ((m_pProgOption->mExistWaitTime == SQL_TRUE) &&
            (m_pProgOption->mExistWaitCycle == SQL_TRUE))
    {
        /* BUG-46486 Improve lock wait, record lock -> array lock
         * array 단위로 체크하게 되어 % 연산자 사용 불가
         */
        if ( aData->mLoad4Sleep >= m_pProgOption->mWaitCycle )
        {
            aData->mLoad4Sleep = 0;
            sSleepTime.initialize(0, m_pProgOption->mWaitTime * 1000);
            idlOS::sleep( sSleepTime );
        }
    }
}

inline IDE_RC iloLoad::Callback4Api(iloLoadInsertContext *aData)
{
    SInt           sRealCount = aData->mRealCount;
    iloaderHandle *sHandle    = aData->mHandle;

    iloMutexLock( sHandle, &(sHandle->mParallel.mLoadFIOMutex) );

    /*
     * BUG-32910
     * mStopIloader flag should be checked
     * before invoking callback in iloader
     */
    if ( sHandle->mProgOption->mStopIloader == ILO_TRUE )
    {
        IDE_RAISE(fail_callback);
    }

    /* BUG-30413
     * mCBFrequencyCnt 값과 arrayCount 의 값을 비교 하여 콜백을 호출 한다.  
     */
    if ( sRealCount == aData->mArrayCount ) 
    {
        mSetCallbackLoadCnt = mSetCallbackLoadCnt + sRealCount;
    }
    if (( sHandle->mProgOption->mSetRowFrequency != 0 ) &&
            ( mCBFrequencyCnt <= mSetCallbackLoadCnt )) 
    {
        mCBFrequencyCnt +=  sHandle->mProgOption->mSetRowFrequency;

        sHandle->mStatisticLog.loadCount  = mSetCallbackLoadCnt; 
        sHandle->mStatisticLog.errorCount = mErrorCount;

        if ( sHandle->mLogCallback( ILO_STATISTIC_LOG, &sHandle->mStatisticLog )
                != ALTIBASE_ILO_SUCCESS )
        { 
            sHandle->mProgOption->mStopIloader = ILO_TRUE;
        }
    }

    iloMutexUnLock( sHandle, &(sHandle->mParallel.mLoadFIOMutex) );

    return IDE_SUCCESS;

    IDE_EXCEPTION(fail_callback);
    {
        iloMutexUnLock( sHandle, &(sHandle->mParallel.mLoadFIOMutex) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

inline IDE_RC iloLoad::SetParamInfo(iloLoadInsertContext *aData)
{
    SInt           sConnIndex = aData->mConnIndex;
    iloaderHandle *sHandle    = aData->mHandle;
    uteErrorMgr   *sErrorMgr  = aData->mErrorMgr;

    aData->mNextAction = NONE;

    IDE_TEST(m_pISPApiUp[sConnIndex].SetColwiseParamBind( sHandle,
                (UInt)aData->mRealCount,
                m_pTableInfo[sConnIndex].mStatusPtr)
             != IDE_SUCCESS);

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    iloMutexLock( sHandle, &(sHandle->mParallel.mLoadFIOMutex) );

    ADD_ERROR_COUNT(aData->mRealCount);

    if (FAIL_BREAK)
    {
        PRINT_ERROR_CODE(sErrorMgr);
        aData->mNextAction = BREAK;
    }
    else
    {
        aData->mNextAction = CONTINUE;
    }

    ILO_CALLBACK;

    m_LogFile.PrintLogErr(sErrorMgr);
    iloMutexUnLock( sHandle, &(sHandle->mParallel.mLoadFIOMutex) );

    return IDE_FAILURE;
}

//INSERT SQL EXECUTE
inline IDE_RC iloLoad::ExecuteInsertSQL(iloLoadInsertContext *aData)
{
    SInt           i;
    SInt           j;
    SInt           sConnIndex = aData->mConnIndex;
    SInt           sRealCount = aData->mRealCount;
    iloaderHandle *sHandle    = aData->mHandle;
    uteErrorMgr   *sErrorMgr  = aData->mErrorMgr;
    SQLUSMALLINT  *sStatusPtr = NULL;
    SChar          szMsg[4096];
    SInt           sInsertFailedCount = 0;

    aData->mNextAction = NONE;

    IDE_TEST(m_pISPApiUp[sConnIndex].Execute() == SQL_FALSE);

    /* BUG-48016 Fix skipped commit */
    if (sHandle->mProgOption->m_bExist_ShowCommit == ID_TRUE)
    {
        idlOS::fprintf( stderr, 
                "%sTotal success [%d]\n", 
                DEV_SHOW_COMMIT_PREFIX,
                sRealCount );
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    iloMutexLock( sHandle, &(sHandle->mParallel.mLoadFIOMutex) );

    if ( sRealCount == 1 )
    {
        ADD_ERROR_COUNT(sRealCount);

        if (FAIL_BREAK)
        {
            PRINT_ERROR_CODE(sErrorMgr);
            aData->mNextAction = BREAK;
        }
        else
        {
            PRINT_RECORD(0);
            aData->mNextAction = CONTINUE;
        }

        ILO_CALLBACK;

        m_LogFile.PrintLogErr(sErrorMgr);
    }
    else if ( sRealCount > 1 )
    {
        if (FAIL_BREAK)
        {
            ADD_ERROR_COUNT(sRealCount);

            PRINT_ERROR_CODE(sErrorMgr);
            aData->mNextAction = BREAK;
        }
        else
        {
            sStatusPtr = m_pTableInfo[sConnIndex].mStatusPtr;
            for (i = j = 0; i < sRealCount; i++)
            {
                if (sStatusPtr[i] != SQL_PARAM_SUCCESS)
                {
                    ADD_ERROR_COUNT(1);

                    // array 에러를 출력할때 정확한 rowcount 를 출력하기위해서
                    // array 의 갯수를 뺀후 +1 한다.
                    PRINT_RECORD(i);

                    if (sStatusPtr[i] == SQL_PARAM_ERROR)
                    {
                        if (uteGetErrorCODE(&sErrorMgr[j]) != 0x00000)
                        {
                            m_LogFile.PrintLogErr(&sErrorMgr[j]);
                        }
                        else
                        {
                            m_LogFile.PrintLogErr(&sErrorMgr[0]);
                        }
                        j++;
                    }
                    else if (sStatusPtr[i] == SQL_PARAM_UNUSED)
                    {
                        // BUGBUG : when happen?
                        idlOS::sprintf(szMsg, "Insert Error : unused\n");
                        m_LogFile.PrintLogMsg(szMsg);
                    }

                    sInsertFailedCount++;
                }
                else
                {
                    aData->mUncommitInsertCount++;
                }
            }
   
            /* BUG-48016 Fix skipped commit */
            if (sHandle->mProgOption->m_bExist_ShowCommit == ID_TRUE)
            {
                idlOS::fprintf( stderr, 
                        "%sPartial success [%d] failed [%d]\n", 
                        DEV_SHOW_COMMIT_PREFIX,
                        sRealCount - sInsertFailedCount, 
                        sInsertFailedCount );
            }

            /* 
             * BUG-48016 Fix skipped commit 
             * Commit candidate when accumulated of insert success record count 
             * equal or over array count.
             */
            if ( aData->mUncommitInsertCount >= aData->mArrayCount )
            {
                aData->mNextAction = NONE;
    
                if (sHandle->mProgOption->m_bExist_ShowCommit == ID_TRUE)
                {
                    idlOS::fprintf( stderr, 
                            "%sAccumulated partial success count [%d] >= array count [%d]\n",
                            DEV_SHOW_COMMIT_PREFIX,
                            aData->mUncommitInsertCount,
                            aData->mArrayCount );
                }
            }
            else
            {
                aData->mNextAction = CONTINUE;
            }

        }
    }
    iloMutexUnLock( sHandle, &(sHandle->mParallel.mLoadFIOMutex) );

    return IDE_FAILURE;
}

inline IDE_RC iloLoad::ExecuteInsertLOB(iloLoadInsertContext *aData)
{
    IDE_RC         sRC;
    SInt           i;
    SInt           sConnIndex = aData->mConnIndex;
    SInt           sRealCount = aData->mRealCount;
    iloaderHandle *sHandle    = aData->mHandle;
    uteErrorMgr   *sErrorMgr  = aData->mErrorMgr;

    aData->mNextAction = NONE;

    if (aData->mLOBColExist == ILO_TRUE)
    {
        for (i = 0; i < sRealCount; i++)
        {
            /* LOB 데이터를 파일로부터 읽어 서버로 전송. */
            iloMutexLock( sHandle, &(sHandle->mParallel.mLoadLOBMutex) );
            sRC = m_DataFile.LoadOneRecordLOBCols( sHandle,
                    aData->mRecordNumber[i],
                    &m_pTableInfo[sConnIndex], 
                    i,
                    &m_pISPApiUp[sConnIndex]);
            iloMutexUnLock( sHandle, &(sHandle->mParallel.mLoadLOBMutex) );

            if (sRC != IDE_SUCCESS)
            {
                /* 오류가 발생한 레코드의 삽입을 취소한다.
                 * m_ArrayCount와 m_CommitUnit이 1이라는
                 * 가정이 깔려 있다. */

                (void)m_pISPApiUp[sConnIndex].EndTran(ILO_FALSE);

                iloMutexLock( sHandle, &(sHandle->mParallel.mLoadFIOMutex) );

                if (FAIL_BREAK)
                {
                    aData->mNextAction = BREAK;

                    ADD_ERROR_COUNT(sRealCount - i);

                    ILO_CALLBACK;

                    m_LogFile.PrintLogErr(sErrorMgr);
                    //goto AFTER_MAIN_LOOP_LABEL;       //!!! 처리해야함
                    iloMutexUnLock( sHandle, &(sHandle->mParallel.mLoadFIOMutex) );

                    break;
                }
                else
                {
                    ADD_ERROR_COUNT(1);
                    PRINT_RECORD(i);

                    ILO_CALLBACK;

                    m_LogFile.PrintLogErr(sErrorMgr);
                    iloMutexUnLock( sHandle, &(sHandle->mParallel.mLoadFIOMutex) );
                }
            }
        }
    }
    return IDE_SUCCESS;
}

inline IDE_RC iloLoad::Commit4Dryrun(iloLoadInsertContext *aData)
{
    aData->mPrevCommitRecCnt = aData->mTotal - aData->mErrorCount;
    /* BUG-48016 Fix skipped commit */
    aData->mUncommitInsertCount = 0;

    return IDE_SUCCESS;
}

inline IDE_RC iloLoad::Commit(iloLoadInsertContext *aData)
{
    SInt           sConnIndex = aData->mConnIndex;
    SInt           sDiffErrorCount = 0; // BUG-28608
    iloaderHandle *sHandle    = aData->mHandle;
    uteErrorMgr   *sErrorMgr  = aData->mErrorMgr;

    IDE_TEST_RAISE( m_pISPApiUp[sConnIndex].EndTran(ILO_TRUE) != IDE_SUCCESS,
                    fail_commit );

    aData->mPrevCommitRecCnt = aData->mTotal - aData->mErrorCount;
    /* BUG-48016 Fix skipped commit */
    aData->mUncommitInsertCount = 0;

    return IDE_SUCCESS;

    IDE_EXCEPTION(fail_commit);
    {
        iloMutexLock( sHandle, &(sHandle->mParallel.mLoadFIOMutex) );

        ILO_CALLBACK;

        m_LogFile.PrintLogErr(aData->mErrorMgr);
        (void)m_pISPApiUp[sConnIndex].EndTran(ILO_FALSE);

        // BUG-28608
        sDiffErrorCount = aData->mTotal - aData->mErrorCount - aData->mPrevCommitRecCnt;
        aData->mErrorCount += sDiffErrorCount;
        mErrorCount += sDiffErrorCount;

        iloMutexUnLock( sHandle, &(sHandle->mParallel.mLoadFIOMutex) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/* BUG-46486 Improve lock wait */
inline
void iloLoad::PrintProgress( ALTIBASE_ILOADER_HANDLE aHandle,
                             SInt aLoadCount )
{
    SChar sTableName[MAX_OBJNAME_LEN];

    iloaderHandle *sHandle = (iloaderHandle *) aHandle;
    
    if ((aLoadCount % 5000) == 0)
    {
       /* BUG-32114 aexport must support the import/export of partition tables.
        * ILOADER IN/OUT TABLE NAME이 PARTITION 일경우 PARTITION NAME으로 변경 */
        if( sHandle->mProgOption->mPartition == ILO_TRUE )
        {
            idlOS::printf("\n%d record load(%s / %s)\n\n", 
                    aLoadCount,
                    m_pTableInfo[0].GetTransTableName(sTableName,(UInt)MAX_OBJNAME_LEN),
                    sHandle->mParser.mPartitionName );
        }
        else
        {
            idlOS::printf("\n%d record load(%s)\n\n", 
                    aLoadCount,
                    m_pTableInfo[0].GetTransTableName(sTableName,(UInt)MAX_OBJNAME_LEN));
        }
    }
    else
    {
        putchar('.');
    }
    idlOS::fflush(stdout);
}
