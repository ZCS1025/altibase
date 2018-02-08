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
 

#ifndef _O_SMI_TEMP_TABLE_H_
#define _O_SMI_TEMP_TABLE_H_ 1

#include <idu.h>
#include <smDef.h>
#include <smiDef.h>
#include <smiMisc.h>
#include <sdtDef.h>

class smiTempTable
{
public:
    static IDE_RC initializeStatic();
    static IDE_RC destroyStatic();

    static IDE_RC create( idvSQL              * aStatistics,
                          scSpaceID             aSpaceID,
                          ULong                 aWorkAreaSize,
                          smiStatement        * aStatement,
                          UInt                  aFlag,
                          const smiColumnList * aColumnList,
                          const smiColumnList * aKeyColumns,
                          UInt                  aWorkGroupRatio,
                          const void         ** aTable);
    static IDE_RC resetKeyColumn( void                * aTable,
                                  const smiColumnList * aKeyColumns );
    static IDE_RC drop(void   * aTable);
    static IDE_RC clear(void   * aTable);
    static IDE_RC clearHitFlag(void   * aTable);

    static IDE_RC insert(void     * aTable, 
                         smiValue * aValue, 
                         UInt       aHashValue,
                         scGRID   * aGRID,
                         idBool   * aResult );
    static IDE_RC update(smiTempCursor * aCursor, 
                         smiValue      * aValue);
    static IDE_RC setHitFlag(smiTempCursor * aCursor);
    static idBool isHitFlagged( smiTempCursor * aCursor );
    static IDE_RC sort( void          * aTable);
    static IDE_RC scan( void          * aTable);
    static IDE_RC fetchFromGRID( void          * aTable,
                                 scGRID          aGRID,
                                 void          * aDestRowBuf );

    static IDE_RC openCursor(void                * aTable, 
                             UInt                  aFlag,
                             const smiColumnList * aColumns,
                             const smiRange      * aKeyRange,
                             const smiRange      * aKeyFilter,
                             const smiCallBack   * aRowFilter,
                             UInt                  aHashValue,
                             smiTempCursor      ** aCursor );
    static IDE_RC restartCursor(smiTempCursor       * aCursor,
                                UInt                  aFlag,
                                const smiRange      * aKeyRange,
                                const smiRange      * aKeyFilter,
                                const smiCallBack   * aRowFilter,
                                UInt                  aHashValue );
    static IDE_RC fetch( smiTempCursor  * aTempCursor,
                         UChar         ** aRow,
                         scGRID         * aRowGRID );
    static IDE_RC storeCursor(smiTempCursor    * aCursor,
                              smiTempPosition ** aPosition );
    static IDE_RC restoreCursor(smiTempCursor    * aCursor,
                                smiTempPosition  * aPosition,
                                UChar           ** aRow,
                                scGRID           * aRowGRID );
    static IDE_RC closeAllCursor(void * aTable );
    static IDE_RC getNullRow(void   * aTable,
                             UChar ** aNullRowPtr);
    static IDE_RC getDisplayInfo(void  * aTable,
                                 ULong * aPageCount,
                                 SLong * aRecordCount );
private:
    static IDE_RC resetCursor(smiTempCursor * aCursor);
    static IDE_RC closeCursor(smiTempCursor * aCursor);

    static void   initStatsPtr(smiTempTableHeader * aHeader,
                               smiStatement       * aStatement );
    static void   registWatchArray(smiTempTableHeader * aHeader);
    static void   generateStats(smiTempTableHeader * aTable);
    static void   accumulateStats( smiTempTableHeader * aTable );
    inline static IDE_RC checkSessionAndStats(smiTempTableHeader * aTable,
                                              smiTempTableOpr      aOpr );
    inline static void checkEndTime( smiTempTableHeader * aTable );

    static void checkAndDump( smiTempTableHeader * aHeader );

public:
    /************************************************************
     * Build fixed table
     * FixedTable X$TEMPTABLE_STATS����µ� ���ȴ�.
     ************************************************************/
    static IDE_RC buildTempTableStatsRecord( idvSQL              * /*aStatistics*/,
                                             void                * aHeader,
                                             void                * aDumpObj,
                                             iduFixedTableMemory * aMemory );

    /************************************************************
     * Build fixed table
     * FixedTable X$TEMPINFO�� ����µ� ���ȴ�.
     ************************************************************/
    static IDE_RC buildTempInfoRecord( idvSQL              * /*aStatistics*/,
                                       void                * aHeader,
                                       void                * aDumpObj,
                                       iduFixedTableMemory * aMemory );


    /************************************************************
     * Build fixed table
     * FixedTable X$TEMPTABLE_OPER����µ� ���ȴ�.
     ************************************************************/
    static IDE_RC buildTempTableOprRecord( idvSQL              * /*aStatistics*/,
                                           void                * aHeader,
                                           void                * aDumpObj,
                                           iduFixedTableMemory * aMemory );

private:
    /* ���� �߻��� TempTable�� �ڼ��� ������� ���� ���Ϸ� Dump�Ѵ�.
     * �׸��� Dump�� ��ü�� ���� ������ altibase_dump.trc���� ����Ѵ�. */
    static void   dumpToFile( smiTempTableHeader * aHeader );
    static void   dumpTempTableHeader( void   * aTableHeader, 
                                       SChar  * aOutBuf, 
                                       UInt     aOutSize );
    static void   dumpTempCursor( void   * aTempCursor,
                                  SChar  * aOutBuf, 
                                  UInt     aOutSize );
    static void   dumpTempStats( smiTempTableStats * aTempStats,
                                 SChar             * aOutBuf, 
                                 UInt                aOutSize );

    static void   makeStatsPerf( smiTempTableStats      * aTempStats,
                                 smiTempTableStats4Perf * aPerf );


    static SChar               mOprName[][ SMI_TT_STR_SIZE ];
    static SChar               mTTStateName[][ SMI_TT_STR_SIZE ];

    static iduMemPool          mTempTableHdrPool;
    static iduMemPool          mTempCursorPool;
    static iduMemPool          mTempPositionPool;

    static smiTempTableStats   mGlobalStats;
    static smiTempTableStats * mTempTableStatsWatchArray;
    static UInt                mStatIdx;
};

/**************************************************************************
 * Description :
 * ��踦 �����ϰ�, Session ���� ���θ� üũ�մϴ�.
 *
 * <IN>
 * aHeader        - ��� Table
 ***************************************************************************/
IDE_RC smiTempTable::checkSessionAndStats(smiTempTableHeader * aHeader,
                                          smiTempTableOpr      aOpr )
{
    smiTempTableStats * sStats;
    idvTime             sCurTime;

    sStats = aHeader->mStatsPtr;
    if(iduProperty::getTimedStatistics() == IDV_TIMED_STATISTICS_ON )
    {
        IDV_TIME_GET( &sCurTime );

        // ó���ҷ����� Create�ÿ���, ���� Time�� ���� 
        if( aOpr != SMI_TTOPR_CREATE )
        {
            sStats->mOprPrepareTime[ aOpr ] += 
                IDV_TIME_DIFF_MICRO(&sStats->mLastOprTime, &sCurTime);
        }

        sStats->mLastOprTime = sCurTime;

        // Operation�� Nȸ �����ϸ� ����ó����. �׽�Ʈ��
        
        IDE_TEST_RAISE( 
            ( aHeader->mCheckCnt == smuProperty::getSmTempOperAbort() ) &&
            ( smuProperty::getSmTempOperAbort() > 0 ),
            ABORT_INTERNAL_ERROR );
    }

    sStats->mOprCount[ aOpr ] ++;
    sStats->mTTLastOpr = aOpr;
    aHeader->mCheckCnt ++;

    /* Fetch, Insert, Update, setHitFlagó�� ���� �Ҹ��� ������ �ƴϰų�,
     * ������ ���� �Ǿ��ٸ� */
    if( ( aOpr == SMI_TTOPR_CREATE ) ||
        ( aOpr == SMI_TTOPR_DROP )   ||
        ( aOpr == SMI_TTOPR_SORT )   ||
        ( aOpr == SMI_TTOPR_CLEAR )   ||
        ( ( aHeader->mCheckCnt % SMI_TT_STATS_INTERVAL ) == 0 ) )
    {
        // Drop�� TimeOut�Ǿ �����ؾ� �ϱ� ������ üũ�� ���� 
        if( aOpr != SMI_TTOPR_DROP )
        {
            IDE_TEST( iduCheckSessionEvent(aHeader->mStatistics) 
                      != IDE_SUCCESS);
        }
        else
        {
            sStats = aHeader->mStatsPtr;
            sStats->mDropTV = smiGetCurrTime();
        }

        smiTempTable::generateStats(aHeader);
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ABORT_INTERNAL_ERROR )
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_INTERNAL ) );
    }
    IDE_EXCEPTION_END;

    aHeader->mCheckCnt ++;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * ���� �ð��� ����Ͽ�, ��踦 �����մϴ�. 
 *
 * <IN>
 * aHeader        - ��� Table
 ***************************************************************************/
void smiTempTable::checkEndTime( smiTempTableHeader * aHeader)
{
    smiTempTableStats * sStats;
    idvTime             sCurTime;

    if(iduProperty::getTimedStatistics() == IDV_TIMED_STATISTICS_ON )
    {
        sStats = aHeader->mStatsPtr;
        IDV_TIME_GET( &sCurTime );

        sStats->mOprTime[ sStats->mTTLastOpr ] += 
            IDV_TIME_DIFF_MICRO(&sStats->mLastOprTime, &sCurTime);

        sStats->mLastOprTime = sCurTime;
    }
}

#endif /* _O_SMI_TEMP_TABLE_H_ */
