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

#include <idsCrypt.h>
#include <qci.h>
#include <mmm.h>
#include <mmErrorCode.h>
#include <mmcSession.h>
#include <mmcStatement.h>
#include <mmuProperty.h>
#include <mmuOS.h>
#include <mmtAdminManager.h>
#include <mmtSnapshotExportManager.h>

mmcStmtBeginFunc mmcStatement::mBeginFunc[] =
{
    mmcStatement::beginDDL,
    mmcStatement::beginDML,
    mmcStatement::beginDCL,
    NULL,
    mmcStatement::beginSP,
    mmcStatement::beginDB,
};

mmcStmtEndFunc mmcStatement::mEndFunc[] =
{
    mmcStatement::endDDL,
    mmcStatement::endDML,
    mmcStatement::endDCL,
    NULL,
    mmcStatement::endSP,
    mmcStatement::endDB,
};


IDE_RC mmcStatement::beginDDL(mmcStatement *aStmt)
{
    mmcSession  *sSession  = aStmt->getSession();
    smiTrans    *sTrans;
    UInt         sFlag = 0;
    UInt         sTxIsolationLevel;
    UInt         sTxTransactionMode;
    idBool       sIsReadOnly = ID_FALSE;
    idBool       sTxBegin = ID_TRUE;

#ifdef DEBUG
    qciStmtType  sStmtType = aStmt->getStmtType();

    IDE_DASSERT(qciMisc::isStmtDDL(sStmtType) == ID_TRUE);
#endif

    if( ( sSession->getCommitMode() == MMC_COMMITMODE_NONAUTOCOMMIT ) ||
         ( aStmt->isRootStmt() == ID_FALSE ) )
    {
        sSession->setActivated(ID_TRUE);

        IDE_TEST_RAISE(sSession->isAllStmtEnd() != ID_TRUE, StmtRemainError);

        // ���� transaction �� transaction�� �Ӽ��� ����
        // autocommit�ε� child statement�� ��� �θ� statement
        // �κ��� transaction�� �޴´�.
        if( ( sSession->getCommitMode() == MMC_COMMITMODE_AUTOCOMMIT ) &&
            ( aStmt->isRootStmt() == ID_FALSE ) )
        {
            sTrans = aStmt->getParentStmt()->getTrans(ID_FALSE);
            aStmt->setTrans(sTrans);
        }
        else
        {
            // BUG-17497
            // non auto commit mode�� ���,
            // transaction�� begin �� ���̹Ƿ� Ʈ������� read only ���� �˻�
            IDE_TEST_RAISE(sSession->isReadOnlyTransaction() == ID_TRUE,
                           TransactionModeError);

            sTrans = sSession->getTrans(ID_FALSE);
            sTxBegin = sSession->getTransBegin();
        }
        
        /* BUG-42853 LOCK TABLE�� UNTIL NEXT DDL ��� �߰� */
        if ( sSession->getLockTableUntilNextDDL() == ID_TRUE )
        {
            if ( sTxBegin == ID_TRUE )
            {
                /* �����͸� �����ϴ� DML�� �Բ� ����� �� ����. */
                IDE_TEST( sTrans->isReadOnly( &sIsReadOnly ) != IDE_SUCCESS );

                IDE_TEST_RAISE( sIsReadOnly != ID_TRUE,
                                ERR_CANNOT_LOCK_TABLE_UNTIL_NEXT_DDL_WITH_DML );
            }
            else
            {
                /* Nothing to do */
            }

            /* Transaction Commit & Begin�� �������� �ʴ´�. */
        }
        else
        {
            sTxIsolationLevel  = sSession->getTxIsolationLevel(sTrans);
            sTxTransactionMode = sSession->getTxTransactionMode(sTrans);

            // ���� transaction�� commit �� ��,
            // ������ �Ӽ����� transaction�� begin ��
            IDE_TEST(mmcTrans::commit(sTrans, sSession) != IDE_SUCCESS);

            // session �Ӽ� �� isolation level�� transaction mode��
            // transaction �Ӽ����� transaction ���� �ٸ� �� �ְ�,
            // session�� transaction �Ӽ����� �ٸ� �� �ִ�.
            // ���� transaction�� ���� session info flag����
            // transaction isolation level�� transaction mode��
            // commit ���� �Ӽ��� ������ �ִٰ� begin �Ҷ�
            // �� �Ӽ��� �״�� �������� �����ؾ� �Ѵ�.
            sFlag = sSession->getSessionInfoFlagForTx();

            sFlag &= ~SMI_TRANSACTION_MASK;
            sFlag |= sTxTransactionMode;

            sFlag &= ~SMI_ISOLATION_MASK;
            sFlag |= sTxIsolationLevel;

            mmcTrans::begin(sTrans, sSession->getStatSQL(), sFlag, sSession );
        }

        sSession->setActivated(ID_FALSE);
    }
    else
    {
        // BUG-17497
        // auto commit mode�� ���,
        // transaction�� begin ���̹Ƿ� ������ read only ���� �˻�
        IDE_TEST_RAISE(sSession->isReadOnlySession() == ID_TRUE,
                       TransactionModeError);

        sSession->setActivated(ID_TRUE);

        IDE_TEST_RAISE(sSession->isAllStmtEnd() != ID_TRUE, StmtRemainError);

        sTrans = aStmt->getTrans(ID_TRUE);

        mmcTrans::begin(sTrans,
                        sSession->getStatSQL(),
                        sSession->getSessionInfoFlagForTx(),
                        sSession);
    }

    IDE_TEST_RAISE(aStmt->beginSmiStmt(sTrans, SMI_STATEMENT_NORMAL)
                   != IDE_SUCCESS, BeginError);

    sSession->changeOpenStmt(1);

    aStmt->setStmtBegin(ID_TRUE);

    return IDE_SUCCESS;

    IDE_EXCEPTION(TransactionModeError);
    {
        IDE_SET(ideSetErrorCode(mmERR_ABORT_MMC_ACCESS_MODE));
    }
    IDE_EXCEPTION(StmtRemainError);
    {
        IDE_SET(ideSetErrorCode(mmERR_ABORT_OTHER_STATEMENT_REMAINS));
    }
    IDE_EXCEPTION(BeginError);
    {
        // BUG-27953 : Rollback ���н� ���� ����� Assert ó��
        /* PROJ-1832 New database link */
        IDE_ASSERT( mmcTrans::rollbackForceDatabaseLink(
                        sTrans, sSession )
                    == IDE_SUCCESS );
    }
    IDE_EXCEPTION( ERR_CANNOT_LOCK_TABLE_UNTIL_NEXT_DDL_WITH_DML )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMX_CANNOT_LOCK_TABLE_UNTIL_NEXT_DDL_WITH_DML ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC mmcStatement::beginDML(mmcStatement *aStmt)
{
    mmcSession    *sSession  = aStmt->getSession();
    smiTrans      *sTrans;
    qciStmtType    sStmtType = aStmt->getStmtType();
    UInt           sFlag     = 0;
    mmcCommitMode  sSessionCommitMode = sSession->getCommitMode();
    smSCN          sSCN = SM_SCN_INIT;

    IDE_DASSERT(qciMisc::isStmtDML(sStmtType) == ID_TRUE);
    
    if ( sSessionCommitMode  == MMC_COMMITMODE_NONAUTOCOMMIT   )
    {
        sTrans = sSession->getTrans(ID_FALSE);
        if ( sSession->getTransBegin() == ID_FALSE )
        {
            mmcTrans::begin(sTrans,
                            sSession->getStatSQL(),
                            sSession->getSessionInfoFlagForTx(),
                            sSession);
        }
        else
        {
            /* Nothing to do */
        }

        // BUG-17497
        // non auto commit mode�� ����̸�,
        // root statement�ϼ��� �ְ�, child statement�ϼ� �ִ�.
        // transaction�� begin �� ���̹Ƿ� Ʈ������� read only ���� �˻�
        IDE_TEST_RAISE((sStmtType != QCI_STMT_SELECT) &&
                       (sSession->isReadOnlyTransaction() == ID_TRUE),
                       TransactionModeError);

        //fix BUG-24041 none-auto commit mode���� select statement begin�Ҷ�
        //mActivated�� on��Ű��ȵ�
        if(sStmtType == QCI_STMT_SELECT)
        {
            //nothing to do
        }
        else
        {
            sSession->setActivated(ID_TRUE);
        }
    }
    else
    {
        //AUTO-COMMIT-MODE, child statement.
        // fix BUG-28267 [codesonar] Ignored Return Value
        if( aStmt->isRootStmt() == ID_FALSE  )
        {
            sTrans = aStmt->getParentStmt()->getTrans(ID_FALSE);
            aStmt->setTrans(sTrans);
        }
        else
        {
            //AUTO-COMMIT-MODE,Root statement. 
            // BUG-17497
            // auto commit mode�� ���,
            // transaction�� begin ���̹Ƿ� ������ read only ���� �˻�
            IDE_TEST_RAISE((sStmtType != QCI_STMT_SELECT) &&
                           (sSession->isReadOnlySession() == ID_TRUE),
                           TransactionModeError);

            sTrans = aStmt->getTrans(ID_TRUE);

            mmcTrans::begin(sTrans,
                            sSession->getStatSQL(),
                            sSession->getSessionInfoFlagForTx(),
                            sSession);
        }//else
        sSession->setActivated(ID_TRUE);

    }//else

    if ((sStmtType == QCI_STMT_SELECT) &&
        (sSession->getTxIsolationLevel(sTrans) != SMI_ISOLATION_REPEATABLE))
    {
        sFlag = SMI_STATEMENT_UNTOUCHABLE;
    }
    else
    {
        // PROJ-2199 SELECT func() FOR UPDATE ����
        // SMI_STATEMENT_FORUPDATE �߰�
        if( sStmtType == QCI_STMT_SELECT_FOR_UPDATE )
        {
            sFlag = SMI_STATEMENT_FORUPDATE;
        }
        else
        {
            sFlag = SMI_STATEMENT_NORMAL;
        }
    }

    IDE_TEST_RAISE(aStmt->beginSmiStmt(sTrans, sFlag) != IDE_SUCCESS, BeginError);

    sSession->changeOpenStmt(1);

    /* PROJ-1381 FAC : Holdable Fetch�� ���� Stmt ���� ���� */
    if ((aStmt->getStmtType() == QCI_STMT_SELECT)
     && (aStmt->getCursorHold() == MMC_STMT_CURSOR_HOLD_ON))
    {
        sSession->changeHoldFetch(1);
    }

    /* PROJ-2626 Snapshot Export
     * iLoader �����̰� begin Snapshot �� �������̰� �� Select �����̸�
     * begin Snapshot�ÿ��� ������ SCN�� ���ͼ� �ڽ��� smiStatement
     * �� SCN�� �����Ѵ�. */
    if ( sSession->getClientAppInfoType() == MMC_CLIENT_APP_INFO_TYPE_ILOADER )
    {
        if ( ( aStmt->getStmtType() == QCI_STMT_SELECT ) &&
             ( mmtSnapshotExportManager::isBeginSnapshot() == ID_TRUE ))
        {
            /* REBUILD ������ �߻��ߴٴ� ���� begin Snapshot���� ������ SCN����
             * select �Ҷ� SCN�� �޶����ٴ� �ǹ��̴�. SCN�� �޶����ٴ� ����
             * �� Table�� DDL�� �߻��ߴٴ� ���̰� �̷���� Error�� �߻���Ų��.  */
            IDE_TEST_RAISE( ( ideGetErrorCode() & E_ACTION_MASK ) == E_ACTION_REBUILD,
                            INVALID_SNAPSHOT_SCN );

            /* Begin ���� SCN �� �����´� */
            IDE_TEST( mmtSnapshotExportManager::getSnapshotSCN( &sSCN )
                      != IDE_SUCCESS );

            /* ���� Statement�� SCN�� �����Ѵ� */
            aStmt->mSmiStmt.setScnForSnapshot( &sSCN );
        }
        else
        {
            IDU_FIT_POINT("mmc::mmcStatementBegin::beginDML::ILoader");
        }
    }
    else
    {
        /* Nothing to do */
    }

    aStmt->setStmtBegin(ID_TRUE);

    return IDE_SUCCESS;

    IDE_EXCEPTION(TransactionModeError);
    {
        IDE_SET(ideSetErrorCode(mmERR_ABORT_MMC_ACCESS_MODE));
    }
    IDE_EXCEPTION(BeginError);
    {
        if ( ( sSession->getCommitMode() != MMC_COMMITMODE_NONAUTOCOMMIT ) &&
             ( aStmt->isRootStmt() == ID_TRUE ) )
        {
            // BUG-27953 : Rollback ���н� ���� ����� Assert ó��
            /* PROJ-1832 New database link */
            IDE_ASSERT( mmcTrans::rollbackForceDatabaseLink(
                            sTrans, sSession )
                        == IDE_SUCCESS );
        }
    }
    IDE_EXCEPTION( INVALID_SNAPSHOT_SCN )
    {
        IDE_SET( ideSetErrorCode( mmERR_ABORT_INVALID_SNAPSHOT_SCN ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC mmcStatement::beginDCL(mmcStatement *aStmt)
{
    IDE_DASSERT(qciMisc::isStmtDCL(aStmt->getStmtType()) == ID_TRUE);

    aStmt->setStmtBegin(ID_TRUE);

    return IDE_SUCCESS;
}

IDE_RC mmcStatement::beginSP(mmcStatement *aStmt)
{
    mmcSession  *sSession  = aStmt->getSession();
    smiTrans    *sTrans;

#ifdef DEBUG
    qciStmtType  sStmtType = aStmt->getStmtType();

    IDE_DASSERT(qciMisc::isStmtSP(sStmtType) == ID_TRUE);
#endif

    if ( ( sSession->getCommitMode() == MMC_COMMITMODE_NONAUTOCOMMIT ) ||
         ( aStmt->isRootStmt() == ID_FALSE ) )
    {
        sSession->setActivated(ID_TRUE);

        // autocommit����̳� child statement�� ���.
        if( ( sSession->getCommitMode() == MMC_COMMITMODE_AUTOCOMMIT ) &&
            ( aStmt->isRootStmt() == ID_FALSE ) )
        {
            sTrans = aStmt->getParentStmt()->getTrans(ID_FALSE);
            aStmt->setTrans(sTrans);
        }
        else
        {
            sTrans = sSession->getTrans(ID_FALSE);
            if ( sSession->getTransBegin() == ID_FALSE )
            {
                mmcTrans::begin(sTrans,
                                sSession->getStatSQL(),
                                sSession->getSessionInfoFlagForTx(),
                                sSession);
            }
            else
            {
                /* Nothing to do */
            }

            // BUG-17497
            // non auto commit mode�� ���,
            // transaction�� begin �� ���̹Ƿ� Ʈ������� read only ���� �˻�
            IDE_TEST_RAISE(sSession->isReadOnlyTransaction() == ID_TRUE,
                           TransactionModeError);
        }

        sTrans->reservePsmSvp();
    }
    else
    {
        // BUG-17497
        // auto commit mode�� ���,
        // transaction�� begin ���̹Ƿ� ������ read only ���� �˻�
        IDE_TEST_RAISE(sSession->isReadOnlySession() == ID_TRUE,
                       TransactionModeError);

        sSession->setActivated(ID_TRUE);

        sTrans = aStmt->getTrans(ID_TRUE);

        mmcTrans::begin(sTrans,
                        sSession->getStatSQL(),
                        sSession->getSessionInfoFlagForTx(),
                        aStmt->getSession());
    }

    aStmt->setTransID(sTrans->getTransID());
    
    aStmt->setSmiStmt(sTrans->getStatement());

    aStmt->setStmtBegin(ID_TRUE);

    return IDE_SUCCESS;

    IDE_EXCEPTION(TransactionModeError);
    {
        IDE_SET(ideSetErrorCode(mmERR_ABORT_MMC_ACCESS_MODE));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

#ifdef DEBUG
IDE_RC mmcStatement::beginDB(mmcStatement *aStmt)
{
    qcStatement* sQcStmt = &((aStmt->getQciStmt())->statement);

    IDE_DASSERT(sQcStmt != NULL);

    IDE_DASSERT(qciMisc::isStmtDB(aStmt->getStmtType()) == ID_TRUE);
#else
IDE_RC mmcStatement::beginDB(mmcStatement */*aStmt*/)
{
#endif

    return IDE_SUCCESS;
}

IDE_RC mmcStatement::endDDL(mmcStatement *aStmt, idBool aSuccess)
{
    mmcSession *sSession = aStmt->getSession();
    smiTrans   *sTrans;
    IDE_RC      sRc      = IDE_SUCCESS;
    UInt        sFlag    = 0;
    UInt        sTxIsolationLevel;
    UInt        sTxTransactionMode;
    
    IDE_DASSERT(qciMisc::isStmtDDL(aStmt->getStmtType()) == ID_TRUE);

    IDE_TEST(aStmt->endSmiStmt((aSuccess == ID_TRUE) ?
                               SMI_STATEMENT_RESULT_SUCCESS : SMI_STATEMENT_RESULT_FAILURE)
             != IDE_SUCCESS);

    if ( ( sSession->getCommitMode() == MMC_COMMITMODE_NONAUTOCOMMIT ) ||
         ( aStmt->isRootStmt() == ID_FALSE ) )
    {
        // autocommit����̳� child statement�� ���.
        if( ( sSession->getCommitMode() == MMC_COMMITMODE_AUTOCOMMIT ) &&
            ( aStmt->isRootStmt() == ID_FALSE ) )
        {
            sTrans = aStmt->getTrans(ID_FALSE);
        }
        else
        {
            sTrans = sSession->getTrans(ID_FALSE);
        }

        // BUG-17495
        // non auto commit �� ���, ���� transaction �Ӽ��� ����ص״ٰ�
        // ������ �Ӽ����� transaction�� begin �ؾ� ��
        sTxIsolationLevel  = sSession->getTxIsolationLevel(sTrans);
        sTxTransactionMode = sSession->getTxTransactionMode(sTrans);

        // BUG-17878
        // session �Ӽ� �� isolation level�� transaction mode��
        // transaction �Ӽ����� transaction ���� �ٸ� �� �ְ�,
        // session�� transaction �Ӽ����� �ٸ� �� �ִ�.
        // ���� transaction�� ���� session info flag����
        // transaction isolation level�� transaction mode��
        // commit ���� �Ӽ��� ������ �ִٰ� begin �Ҷ�
        // �� �Ӽ��� �״�� �������� �����ؾ� �Ѵ�.
        sFlag = sSession->getSessionInfoFlagForTx();
        sFlag &= ~SMI_TRANSACTION_MASK;
        sFlag |= sTxTransactionMode;

        sFlag &= ~SMI_ISOLATION_MASK;
        sFlag |= sTxIsolationLevel;
    }
    else
    {
        sTrans = aStmt->getTrans(ID_FALSE);
    }

    if (aSuccess == ID_TRUE)
    {
        sRc = mmcTrans::commit(sTrans, sSession);

        if (sRc == IDE_SUCCESS)
        {
            if (sSession->getQueueInfo() != NULL)
            {
                mmqManager::freeQueue(sSession->getQueueInfo());
            }
        }
        else
        {
            /* PROJ-1832 New database link */
            IDE_ASSERT( mmcTrans::rollbackForceDatabaseLink(
                            sTrans, sSession )
                        == IDE_SUCCESS );     
        }
    }
    else
    {
        /* PROJ-1832 New database link */
        IDE_TEST( mmcTrans::rollbackForceDatabaseLink(
                      sTrans, sSession )
                  != IDE_SUCCESS );
    }

    if ( ( ( sSession->getCommitMode() == MMC_COMMITMODE_NONAUTOCOMMIT ) &&
           ( sSession->getReleaseTrans() == ID_FALSE ) ) ||
         ( aStmt->isRootStmt() == ID_FALSE ) )
    {
        // BUG-17497
        // non auto commit mode�� ���,
        // commit �� �����ص� transaction �Ӽ����� trasaction�� begin �Ѵ�.
        mmcTrans::begin( sTrans,
                         sSession->getStatSQL(),
                         sFlag,
                         aStmt->getSession() );

        // BUG-20673 : PSM Dynamic SQL
        if ( aStmt->isRootStmt() == ID_FALSE )
        {
            sTrans->reservePsmSvp();
        }
    }
    else
    {
        /* Nothing to do */
    }

    sSession->setQueueInfo(NULL);

    sSession->setActivated(ID_FALSE);
    
    /* BUG-29224
     * Change a Statement Member variable after end of method.
     */
    sSession->changeOpenStmt(-1);

    aStmt->setStmtBegin(ID_FALSE);

    IDE_TEST(sRc != IDE_SUCCESS);

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;
    {
        sSession->setQueueInfo(NULL);
    }

    return IDE_FAILURE;
}

IDE_RC mmcStatement::endDML(mmcStatement *aStmt, idBool aSuccess)
{
    mmcSession *sSession = aStmt->getSession();
    smiTrans   *sTrans;
    
    IDE_DASSERT(qciMisc::isStmtDML(aStmt->getStmtType()) == ID_TRUE);

    IDE_TEST(aStmt->endSmiStmt((aSuccess == ID_TRUE) ?
                               SMI_STATEMENT_RESULT_SUCCESS : SMI_STATEMENT_RESULT_FAILURE)
             != IDE_SUCCESS);

    if ( ( sSession->getCommitMode() == MMC_COMMITMODE_AUTOCOMMIT ) &&
         ( aStmt->isRootStmt() == ID_TRUE ) )
    {
        sTrans = aStmt->getTrans(ID_FALSE);

        if (sTrans == NULL)
        {
            /*  PROJ-1381 Fetch AcrossCommit
             * Holdable Stmt�� Tx���� ���߿� ���� �� �ִ�.
             * ������ �ƴϴ� ������ �Ѿ��. */
            IDE_ASSERT(aStmt->getCursorHold() == MMC_STMT_CURSOR_HOLD_ON);
        }
        else
        {
            if (aSuccess == ID_TRUE)
            {
                /* BUG-37674 */
                IDE_TEST_RAISE( mmcTrans::commit( sTrans, sSession ) 
                                != IDE_SUCCESS, CommitError );
            }
            else
            {
                /* PROJ-1832 New database link */
                IDE_TEST( mmcTrans::rollbackForceDatabaseLink(
                              sTrans, sSession )
                          != IDE_SUCCESS );
            }
        }

        sSession->setActivated(ID_FALSE);
    }

    /* BUG-29224
     * Change a Statement Member variable after end of method.
     */
    sSession->changeOpenStmt(-1);

    /* PROJ-1381 FAC : Holdable Fetch�� ���� Stmt ���� ���� */
    if ((aStmt->getStmtType() == QCI_STMT_SELECT)
     && (aStmt->getCursorHold() == MMC_STMT_CURSOR_HOLD_ON))
    {
        sSession->changeHoldFetch(-1);
    }

    aStmt->setStmtBegin(ID_FALSE);

    return IDE_SUCCESS;

    IDE_EXCEPTION(CommitError);
    {
        /* PROJ-1832 New database link */
        IDE_ASSERT( mmcTrans::rollbackForceDatabaseLink(
                        sTrans, sSession )
                    == IDE_SUCCESS );
        sSession->setActivated(ID_FALSE);
        
        /* BUG-29224
         * Change a Statement Memeber variable when error occured.
         */ 
        sSession->changeOpenStmt(-1);

        /* PROJ-1381 FAC : Holdable Fetch�� ���� Stmt ���� ���� */
        if ((aStmt->getStmtType() == QCI_STMT_SELECT)
         && (aStmt->getCursorHold() == MMC_STMT_CURSOR_HOLD_ON))
        {
            sSession->changeHoldFetch(-1);
        }

        aStmt->setStmtBegin(ID_FALSE);
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC mmcStatement::endDCL(mmcStatement *aStmt, idBool /*aSuccess*/)
{
    IDE_DASSERT(qciMisc::isStmtDCL(aStmt->getStmtType()) == ID_TRUE);

    aStmt->setStmtBegin(ID_FALSE);

    return IDE_SUCCESS;
}

IDE_RC mmcStatement::endSP(mmcStatement *aStmt, idBool aSuccess)
{
    mmcSession *sSession = aStmt->getSession();
    smiTrans   *sTrans;
    idBool      sTxBegin = ID_TRUE;

    IDE_DASSERT(qciMisc::isStmtSP(aStmt->getStmtType()) == ID_TRUE);

    aStmt->setStmtBegin(ID_FALSE);

    if ( ( sSession->getCommitMode() == MMC_COMMITMODE_NONAUTOCOMMIT ) ||
         ( aStmt->isRootStmt() == ID_FALSE ) )
    {
        if( ( sSession->getCommitMode() == MMC_COMMITMODE_AUTOCOMMIT ) &&
            ( aStmt->isRootStmt() == ID_FALSE ) )
        {
            sTrans = aStmt->getParentStmt()->getTrans(ID_FALSE);
            aStmt->setTrans(sTrans);
        }
        else
        {
            sTrans = sSession->getTrans(ID_FALSE);
            sTxBegin = sSession->getTransBegin();
        }

        if ( sTxBegin == ID_TRUE )
        {
            if (aSuccess == ID_TRUE)
            {
                sTrans->clearPsmSvp();
            }
            else
            {
                IDE_TEST(sTrans->abortToPsmSvp() != IDE_SUCCESS);
            }
        }
        else
        {
            /* Nothing to do */
        }
    }
    else
    {
        // BUG-36203 PSM Optimize
        IDE_TEST( aStmt->freeChildStmt( ID_TRUE,
                                        ID_FALSE )
                  != IDE_SUCCESS );

        sTrans = aStmt->getTrans(ID_FALSE);

        if (aSuccess == ID_TRUE)
        {
            /* BUG-37674 */
            IDE_TEST_RAISE( mmcTrans::commit( sTrans, sSession ) 
                            != IDE_SUCCESS, CommitError );
        }
        else
        {
            /* PROJ-1832 New database link */
            IDE_TEST( mmcTrans::rollbackForceDatabaseLink(
                          sTrans, sSession )
                     != IDE_SUCCESS );
        }

        sSession->setActivated(ID_FALSE);
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION(CommitError);
    {
        // BUG-27953 : Rollback ���н� ���� ����� Assert ó��
        /* PROJ-1832 New database link */
        IDE_ASSERT ( mmcTrans::rollbackForceDatabaseLink(
                         sTrans, sSession )
                     == IDE_SUCCESS );
        sSession->setActivated(ID_FALSE);
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC mmcStatement::endDB(mmcStatement * /*aStmt*/, idBool /*aSuccess*/)
{
    return IDE_SUCCESS;
}
