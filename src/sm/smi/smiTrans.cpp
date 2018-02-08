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
 * $Id: smiTrans.cpp 82186 2018-02-05 05:17:56Z lswhh $
 **********************************************************************/

#include <idl.h>
#include <ide.h>
#include <smErrorCode.h>
#include <smx.h>
#include <smr.h>
#include <smiTrans.h>
#include <smiLegacyTrans.h>
#include <smiMain.h>

IDE_RC smiTrans::initialize()
{
    mTrans          = NULL;
    mStmtListHead   = NULL;

    /* smiTrans_initialize_malloc_StmtListHead.tc */
    IDU_FIT_POINT_RAISE("smiTrans::initialize::malloc::StmtListHead",insufficient_memory);
    IDE_TEST_RAISE( iduMemMgr::malloc( IDU_MEM_SM_SMI,
                                       ID_SIZEOF(smiStatement),
                                       (void **)&mStmtListHead) != IDE_SUCCESS,
                    insufficient_memory );

    /* PROJ-1381 Fetch Across Commits
     * mStmtListHead�� mAllPrev/mAllNext�� �̰������� �ʱ�ȭ �Ѵ�.
     * Commit ���Ŀ��� Legacy TX�� STMT�� ������ �� �־���ϱ� �����̴�. */
    mStmtListHead->mAllPrev =
    mStmtListHead->mAllNext = mStmtListHead;

    mCursorListHead.mAllPrev =
    mCursorListHead.mAllNext = &mCursorListHead;

    mStmtListHead->mChildStmtCnt = 0;
    mStmtListHead->mUpdate       = NULL;

    return IDE_SUCCESS;

    IDE_EXCEPTION( insufficient_memory );
    {
        IDE_SET(ideSetErrorCode(idERR_ABORT_InsufficientMemory));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smiTrans::initializeInternal( void )
{
    IDE_ASSERT( mStmtListHead   != NULL );

    mStmtListHead->mTrans        = this;
    mStmtListHead->mParent       = NULL;
    mStmtListHead->mPrev         = mStmtListHead;
    mStmtListHead->mNext         = mStmtListHead;
    mStmtListHead->mUpdate       = NULL;
    mStmtListHead->mChildStmtCnt = 0;
    mStmtListHead->mFlag         = SMI_STATEMENT_NORMAL;

    mCursorListHead.mPrev = &mCursorListHead;
    mCursorListHead.mNext = &mCursorListHead;

    SM_LSN_INIT( mBeginLSN );
    SM_LSN_INIT( mCommitLSN );

    return IDE_SUCCESS;
}

IDE_RC smiTrans::destroy( idvSQL * /*aStatistics*/ )
{
    if ( mStmtListHead != NULL )
    {
        /* PROJ-1381 Fetch Across Commits
         * for XA, replication thread */
        IDE_TEST_RAISE( mStmtListHead->mUpdate != NULL,
                        ERR_UPDATE_STATEMENT_EXIST );

        IDE_TEST_RAISE( mStmtListHead->mChildStmtCnt != 0,
                        ERR_STATEMENT_EXIST );

        IDE_ASSERT( iduMemMgr::free( mStmtListHead ) == IDE_SUCCESS );
        mStmtListHead = NULL;
    }
    else
    {
        /* nothing to do */
    }

    if ( mTrans != NULL )
    {
        IDE_TEST( smxTransMgr::freeTrans( (smxTrans*)mTrans )
                  != IDE_SUCCESS );
        mTrans = NULL;
    }
    else
    {
        /* nothing to do */
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_UPDATE_STATEMENT_EXIST );
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_smiUpdateStatementExist ) );
    }
    IDE_EXCEPTION( ERR_STATEMENT_EXIST );
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_smiStatementExist ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smiTrans::begin(smiStatement** aStatement,
                       idvSQL        *aStatistics,
                       UInt           aFlag,
                       UInt           aReplID,
                       idBool         aIgnoreRetry)
{
    /* PROJ-1381 Fetch Across Commits
     * for XA, replication */
    IDE_TEST_RAISE( mStmtListHead->mUpdate != NULL,
                    ERR_UPDATE_STATEMENT_EXIST );

    (void)initializeInternal();

    if ( mTrans == NULL )
    {
        IDU_FIT_POINT( "smiTrans::begin::alloc" );
        IDE_TEST( smxTransMgr::alloc( (smxTrans **)&mTrans,
                                      aStatistics,
                                      aIgnoreRetry )
                  != IDE_SUCCESS );
    }
    else
    {
        /* nothing to do */
    }

    IDE_TEST( ((smxTrans*)mTrans)->begin( aStatistics, aFlag, aReplID )
              != IDE_SUCCESS );

    mFlag = aFlag;

    mStmtListHead->mTransID = smxTrans::getTransID( mTrans );
    mStmtListHead->mFlag    = ( mFlag & SMI_TRANSACTION_MASK )
                                != SMI_TRANSACTION_UNTOUCHABLE ?
                           SMI_STATEMENT_NORMAL : SMI_STATEMENT_UNTOUCHABLE;

    *aStatement = mStmtListHead;

    (((smxTrans*)mTrans)->mSmiTransPtr) = this;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_UPDATE_STATEMENT_EXIST );
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_smiUpdateStatementExist ) );

        /* BUG-42584 INC-30976 �ذ��� ���� ����� �ڵ� �߰� */
        ideLog::log( IDE_SM_0,
                "Transaction has child statement.\n"
                "Statement Info\n"
                "Trans Ptr        : 0x%"ID_xPOINTER_FMT"\n"
                "Trans ID         : %"ID_UINT32_FMT"\n"
                "Child Statements : %"ID_UINT32_FMT"\n"
                "SCN              : %"ID_UINT64_FMT"\n"
                "SCN HEX          : 0x%"ID_XINT64_FMT"\n"
                "InfiniteSCN      : %"ID_UINT64_FMT"\n"
                "InfiniteSCN HEX  : 0x%"ID_XINT64_FMT"\n"
                "Flag             : %"ID_UINT32_FMT"\n"
                "Depth            : %"ID_UINT32_FMT"\n",
                mStmtListHead->mTrans,
                mStmtListHead->mTransID,
                mStmtListHead->mChildStmtCnt,
                SM_SCN_TO_LONG( mStmtListHead->mSCN ),
                SM_SCN_TO_LONG( mStmtListHead->mSCN ),
                SM_SCN_TO_LONG( mStmtListHead->mInfiniteSCN ),
                SM_SCN_TO_LONG( mStmtListHead->mInfiniteSCN ),
                mStmtListHead->mFlag,
                mStmtListHead->mDepth );

        ideLog::log( IDE_SM_0,
                "smiTrans Info\n"
                "Trans Ptr    : 0x%"ID_xPOINTER_FMT"\n"
                "Flag         : %"ID_UINT32_FMT"\n"
                "BeginLSN     : %"ID_UINT32_FMT", %"ID_UINT32_FMT"\n"
                "CommitLSN    : %"ID_UINT32_FMT", %"ID_UINT32_FMT"\n",
                mTrans,
                mFlag,
                mBeginLSN.mFileNo,
                mBeginLSN.mOffset,
                mCommitLSN.mFileNo,
                mCommitLSN.mOffset );

        if ( mTrans != NULL )
        {
            ideLog::log( IDE_SM_0,
                    "smxTrans Info\n"
                    "TransID           : %"ID_UINT32_FMT"\n"
                    "Flag              : %"ID_UINT32_FMT"\n"
                    "SessionID         : %"ID_UINT32_FMT"\n"
                    "Status            : %"ID_UINT32_FMT"\n"
                    "Repl ID           : %"ID_UINT32_FMT"\n"
                    "IsFree            : %"ID_UINT32_FMT"\n"
                    "CommitSCN         : %"ID_UINT64_FMT"\n"
                    "CommitSCN HEX     : 0x%"ID_XINT64_FMT"\n"
                    "InfiniteSCN       : %"ID_UINT64_FMT"\n"
                    "InfiniteSCN HEX   : 0x%"ID_XINT64_FMT"\n"
                    "MinMemViewSCN     : %"ID_UINT64_FMT"\n"
                    "MinMemViewSCN HEX : 0x%"ID_XINT64_FMT"\n"
                    "MinDskViewSCN     : %"ID_UINT64_FMT"\n"
                    "MinDskViewSCN HEX : 0x%"ID_XINT64_FMT"\n",
                    ((smxTrans*)mTrans)->mTransID,
                    ((smxTrans*)mTrans)->mFlag,
                    ((smxTrans*)mTrans)->mSessionID,
                    ((smxTrans*)mTrans)->mStatus,
                    ((smxTrans*)mTrans)->mReplID,
                    ((smxTrans*)mTrans)->mIsFree,
                    SM_SCN_TO_LONG( ((smxTrans*)mTrans)->mCommitSCN ),
                    SM_SCN_TO_LONG( ((smxTrans*)mTrans)->mCommitSCN ),
                    SM_SCN_TO_LONG( ((smxTrans*)mTrans)->mInfinite ),
                    SM_SCN_TO_LONG( ((smxTrans*)mTrans)->mInfinite ),
                    SM_SCN_TO_LONG( ((smxTrans*)mTrans)->mMinMemViewSCN ),
                    SM_SCN_TO_LONG( ((smxTrans*)mTrans)->mMinMemViewSCN ),
                    SM_SCN_TO_LONG( ((smxTrans*)mTrans)->mMinDskViewSCN ), 
                    SM_SCN_TO_LONG( ((smxTrans*)mTrans)->mMinDskViewSCN ) );
        }
        else
        {
            /* nothing to do ... */
        }

    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

IDE_RC smiTrans::savepoint(const SChar* aSavePoint,
                           smiStatement *aStatement )
{

    if(aStatement != NULL)
    {
        IDE_TEST_RAISE( aStatement->mParent->mParent != NULL,
                        ERR_NOT_ROOT_STATEMENT);
    }

    IDU_FIT_POINT_RAISE( "smiTrans::savepoint::ERR_UPDATE_STATEMENT_EXIST", ERR_UPDATE_STATEMENT_EXIST );

    IDE_TEST_RAISE( mStmtListHead->mUpdate != NULL,
                    ERR_UPDATE_STATEMENT_EXIST );

    IDE_TEST( ((smxTrans*)mTrans)->setExpSavepoint( aSavePoint )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_UPDATE_STATEMENT_EXIST );
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_smiUpdateStatementExist ) );
    }
    IDE_EXCEPTION( ERR_NOT_ROOT_STATEMENT );
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_smiNotRootStatement ) );
    }

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

void smiTrans::reservePsmSvp( )
{
    ((smxTrans*)mTrans)->reservePsmSvp();
}

void smiTrans::clearPsmSvp( )
{
    ((smxTrans*)mTrans)->clearPsmSvp();
}

IDE_RC smiTrans::abortToPsmSvp( )
{
    IDE_TEST( ((smxTrans *)mTrans)->abortToPsmSvp() != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}


// partial rollback �Ǵ� total rollback�� �Ѵ�.
// total rollback�� transaction slot�� free�Ѵ�.
IDE_RC smiTrans::rollback(const SChar* aSavePoint,
                          UInt         aTransReleasePolicy )
{
    IDU_FIT_POINT_RAISE( "smiTrans::rollback::ERR_UPDATE_STATEMENT_EXIST", ERR_UPDATE_STATEMENT_EXIST );

    IDE_TEST_RAISE( mStmtListHead->mUpdate != NULL,
                    ERR_UPDATE_STATEMENT_EXIST );

    if(aSavePoint != NULL)
    {
        /* partial rollback */
        IDE_TEST( ((smxTrans*)mTrans)->abortToExpSavepoint(aSavePoint)
                  != IDE_SUCCESS );
    }
    else
    {
        /* total rollback. */
        IDE_TEST_RAISE( mStmtListHead->mChildStmtCnt != 0,
                        ERR_STATEMENT_EXIST );

        IDE_TEST( ((smxTrans*)mTrans)->abort() != IDE_SUCCESS );

        if ( aTransReleasePolicy == SMI_RELEASE_TRANSACTION )
        {
            IDE_TEST( smxTransMgr::freeTrans( (smxTrans*)mTrans )
                      != IDE_SUCCESS );
            mTrans = NULL;
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_UPDATE_STATEMENT_EXIST );
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_smiUpdateStatementExist ) );
    }
    IDE_EXCEPTION( ERR_STATEMENT_EXIST );
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_smiStatementExist ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

IDE_RC smiTrans::commit(smSCN * aCommitSCN,
                        UInt    aTransReleasePolicy)
{
    idBool      sIsLegacyTrans  = ID_FALSE;
    idBool      sWriteCommitLog = ID_FALSE; /* BUG-41342 */
    void      * sLegacyTrans    = NULL;

    IDU_FIT_POINT_RAISE( "smiTrans::commit::ERR_UPDATE_STATEMENT_EXIST", ERR_UPDATE_STATEMENT_EXIST );
    IDE_TEST_RAISE( mStmtListHead->mUpdate != NULL,
                    ERR_UPDATE_STATEMENT_EXIST );

    /* PROJ-1381 Fetch Across Commits
     * commit ������ ChildStmt�� ���������� fetch�� ��� �� �� �ֵ���
     * Legacy Trans�� �����Ѵ�. */
    if ( mStmtListHead->mChildStmtCnt != 0 )
    {
        /* Autocommit Mode�̰ų� Ʈ�������� ������ ������ ����
         * Release Policy�� SMI_RELEASE_TRANSACTION���� �Ѵ�.
         * �̶��� commit ������ �����ִ� ChildStmt�� �־�� �ȵȴ�. */
        IDE_TEST_RAISE( aTransReleasePolicy == SMI_RELEASE_TRANSACTION,
                        ERR_STATEMENT_EXIST );

        sIsLegacyTrans = ID_TRUE;
    }

    IDU_FIT_POINT( "smiTrans::commit" );

    IDE_TEST( ((smxTrans*)mTrans)->commit( aCommitSCN,
                                           sIsLegacyTrans,
                                           &sLegacyTrans )
              != IDE_SUCCESS );
    sWriteCommitLog = ID_TRUE;

    /* PROJ-1381 Fetch Across Commits */
    if ( sIsLegacyTrans == ID_TRUE )
    {
        IDE_DASSERT( sLegacyTrans != NULL );

        IDE_TEST( smiLegacyTrans:: makeLegacyStmt( sLegacyTrans,
                                                   mStmtListHead )
                  != IDE_SUCCESS );
    }
    else
    {
        /* nothing to do */
    }

    SM_GET_LSN( mBeginLSN, ((smxTrans*)mTrans)->getBeginLSN() );
    SM_GET_LSN( mCommitLSN, ((smxTrans*)mTrans)->getCommitLSN() );

    if ( aTransReleasePolicy == SMI_RELEASE_TRANSACTION )
    {
        IDE_TEST( smxTransMgr::freeTrans( (smxTrans*)mTrans ) != IDE_SUCCESS );
        mTrans = NULL;
    }
    else
    {
        /* nothing to do */
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_UPDATE_STATEMENT_EXIST );
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_smiUpdateStatementExist ) );
    }
    IDE_EXCEPTION( ERR_STATEMENT_EXIST );
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_smiStatementExist ) );
    }
    IDE_EXCEPTION_END;

    /* BUG-41342 Commit Log�� ���� �Ŀ��� ����ó���ϸ� �ȵȴ�. */
    IDE_ASSERT( sWriteCommitLog == ID_FALSE );

    return IDE_FAILURE;
}

smTID smiTrans::getTransID()
{
    /* ------------------------------------------------
     *  mTrans�� NULL�� �� �ֱ� ������ �ӽ����� �� check & getID
     * ----------------------------------------------*/
    smxTrans *sTrans = (smxTrans*)mTrans;
    return (sTrans == NULL) ? 0 : ((smTID)((smxTrans*)sTrans)->mTransID);
}

/* -----------------------------------
     For Global Transaction
   ---------------------------------- */
/* BUG-18981 */
IDE_RC smiTrans::prepare(ID_XID *aXID)
{

    IDE_TEST( ((smxTrans*)mTrans)->prepare(aXID) != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}


IDE_RC smiTrans::attach( SInt aSlotID )
{

    smxTrans *sTrans;

    sTrans = smxTransMgr::getTransBySID(aSlotID);
    mTrans   = sTrans;

    return IDE_SUCCESS;

}

IDE_RC smiTrans::isReadOnly(idBool *aIsReadOnly)
{
    idBool sIsReadOnly           = ID_FALSE;
    idBool sIsVolatileTBSTouched = ID_FALSE;

    sIsReadOnly = ((smxTrans *)mTrans)->isReadOnly();
    if ( sIsReadOnly == ID_TRUE )
    {
        /* BUG-42991 smiTrans::isReadOnly()���� Volatile Tablespace�� �˻��ϵ��� �մϴ�. */
        sIsVolatileTBSTouched = ((smxTrans *)mTrans)->isVolatileTBSTouched();
        if ( sIsVolatileTBSTouched == ID_FALSE )
        {
            *aIsReadOnly = ID_TRUE;
        }
        else
        {
            *aIsReadOnly = ID_FALSE;
        }
    }
    else
    {
        *aIsReadOnly = ID_FALSE;
    }

    return IDE_SUCCESS;
}

void smiTrans::showOIDList()
{

    (void)(((smxTrans*)mTrans)->showOIDList());

}

UInt smiTrans::getFirstUpdateTime()
{
    smxTrans *sTrans;

    sTrans = (smxTrans *)mTrans; /* store it for preventing NULL-reference.
                                    this should be atomic-operation.*/

    if (sTrans != NULL)
    {
        return sTrans->getFstUpdateTime();
    }
    else
    {
        return 0; /* already committed or assigned other smiTrans */
    }
}

// QP���� Meta�� ���ٵ� ��� �� �Լ��� ȣ���Ͽ�
// Transaction�� Meta���� ���θ� �����Ѵ�
IDE_RC smiTrans::setMetaTableModified()
{
    ((smxTrans*)mTrans)->setMetaTableModified();

    return IDE_SUCCESS;
}

smSN smiTrans::getBeginSN()
{
    return SM_MAKE_SN( mBeginLSN );
}
smSN smiTrans::getCommitSN()
{
    return SM_MAKE_SN( mCommitLSN );
}

/*******************************************************************************
 * Description : DDL Transaction�� ǥ���ϴ� Log Record�� ����Ѵ�.
 ******************************************************************************/
IDE_RC smiTrans::writeDDLLog()
{
    return ((smxTrans*)mTrans)->writeDDLLog();
}

/*******************************************************************************
 * Description : Staticstics�� ��Ʈ�Ѵ�.
 *
 *  BUG-22651  smrLogMgr::updateTransLSNInfo����
 *             ����������Ǵ� ��찡 �����ֽ��ϴ�.
 ******************************************************************************/
void smiTrans::setStatistics( idvSQL * aStatistics )
{
    ((smxTrans*)mTrans)->setStatistics( aStatistics );
}

idvSQL * smiTrans::getStatistics( void )
{
    return ((smxTrans*)mTrans)->getStatistics( mTrans );
}

IDE_RC smiTrans::setReplTransLockTimeout( UInt aReplTransLockTimeout )
{
    IDE_DASSERT( mTrans != NULL );
    return ((smxTrans*)mTrans)->setReplLockTimeout( aReplTransLockTimeout );
}

UInt smiTrans::getReplTransLockTimeout( )
{
    IDE_DASSERT( mTrans != NULL );
    return ((smxTrans*)mTrans)->getReplLockTimeout();
}

idBool smiTrans::isBegin()
{
    idBool sIsBegin = ID_FALSE;

    if (mTrans != NULL)
    {
        sIsBegin = smxTrans::isTxBeginStatus((smxTrans*)mTrans);
    }
    else
    {
        /* Nothing to do */
    }

    return sIsBegin;
}
