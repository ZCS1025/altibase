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

#include <qci.h>
#include <mmm.h>
#include <mmErrorCode.h>
#include <mmcSession.h>
#include <mmcStatement.h>
#include <mmcTask.h>
#include <mmtAdminManager.h>
#include <mmuProperty.h>
#include <mmuOS.h>


mmcStmtExecFunc mmcStatement::mExecuteFunc[] =
{
    mmcStatement::executeDDL,
    mmcStatement::executeDML,
    mmcStatement::executeDCL,
    NULL,
    mmcStatement::executeSP,
    mmcStatement::executeDB,
};


IDE_RC mmcStatement::executeDDL(mmcStatement *aStmt, SLong * /*aAffectedRowCount*/, SLong * /*aFetchedRowCount*/)
{
    mmcSession   *sSession   = aStmt->getSession();
    qciStatement *sStatement = aStmt->getQciStmt();
    smiTrans     *sTrans     = sSession->getTrans(aStmt, ID_TRUE);

    ideLog::log(IDE_QP_2, QP_TRC_EXEC_DDL_BEGIN, aStmt->getQueryString());

    IDE_TEST(aStmt->endSmiStmt(SMI_STATEMENT_RESULT_SUCCESS) != IDE_SUCCESS);

    /* BUG-45371 */
    IDU_FIT_POINT_RAISE( "mmcStatement::executeDDL::beginSmiStmt",
                         BeginError );
    IDE_TEST_RAISE( aStmt->beginSmiStmt(sTrans, SMI_STATEMENT_NORMAL) != IDE_SUCCESS,
                    BeginError );

    // TASK-2401 Disk/Memory Log�и�
    //           LFG=2�϶� Trans Commit�� �α� �÷��� �ϵ��� ����
    IDE_TEST( aStmt->getSmiStmt()->getTrans()->setMetaTableModified()
              != IDE_SUCCESS )

    /* PROJ-1442 Replication Online �� DDL ���
     * DDL Transaction ���� ����Ͽ�, Replication Sender�� DML�� �����ϵ��� ��
     */
    IDE_TEST(aStmt->getSmiStmt()->getTrans()->writeDDLLog() != IDE_SUCCESS);

    IDE_TEST(qci::execute(sStatement, aStmt->getSmiStmt()) != IDE_SUCCESS);

    ideLog::log(IDE_QP_2, QP_TRC_EXEC_DDL_SUCCESS);

    return IDE_SUCCESS;

    IDE_EXCEPTION(BeginError)
    {
        /* PROJ-1832 New database link */
        IDE_ASSERT( mmcTrans::rollbackForceDatabaseLink(sTrans, sSession)
                    == IDE_SUCCESS );

        sSession->changeOpenStmt(-1);

        aStmt->setStmtBegin(ID_FALSE);
    }
    IDE_EXCEPTION_END;
    {
        /* bug-37143: unreadalbe error msg of DDL failure */
        ideLog::log(IDE_QP_2, QP_TRC_EXEC_DDL_FAILURE,
                E_ERROR_CODE(ideGetErrorCode()),
                ideGetErrorMsg(ideGetErrorCode()));
    }

    return IDE_FAILURE;
}

IDE_RC mmcStatement::executeDML(mmcStatement *aStmt, SLong *aAffectedRowCount, SLong *aFetchedRowCount)
{
    mmcSession   *sSession   = aStmt->getSession();
    qciStatement *sStatement = aStmt->getQciStmt();
    qciStmtType   sStmtType  = aStmt->getStmtType();

    switch (sStmtType)
    {
        case QCI_STMT_LOCK_TABLE:
            if (sSession->getCommitMode() == MMC_COMMITMODE_AUTOCOMMIT)
            {
                IDE_RAISE(AutocommitError);
            }
            else
            {
                // Nothing to do.
            }
            
        default:
            IDE_TEST(qci::execute(sStatement, aStmt->getSmiStmt()) != IDE_SUCCESS);

            qci::getRowCount(aStmt->getQciStmt(), aAffectedRowCount, aFetchedRowCount);

            break;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION(AutocommitError);
    {
        IDE_SET(ideSetErrorCode(mmERR_ABORT_MMC_CANT_LOCK_TABLE_IN_AUTOCOMMIT_MODE));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC mmcStatement::executeDCL(mmcStatement *aStmt, SLong * /*aAffectedRowCount*/, SLong * /*aFetchedRowCount*/)
{
    mmcSession  *sSession      = aStmt->getSession();
    qciStmtType  sStmtType     = aStmt->getStmtType();
    mmmPhase     sStartupPhase = mmm::getCurrentPhase();
    idBool       sIsDDLLogging = ID_FALSE;
    smiTrans   * sSmiTrans     = NULL;

    /* BUG-42915 STOP�� FLUSH�� �α׸� DDL Logging Level�� ����մϴ�. */
    if ( ( sStmtType == QCI_STMT_ALT_REPLICATION_STOP ) ||
         ( sStmtType == QCI_STMT_ALT_REPLICATION_FLUSH ) )
    {
        sIsDDLLogging = ID_TRUE;
    }
    else
    {
        /* Nothing to do */
    }

    if ( sIsDDLLogging == ID_TRUE )
    {
        ideLog::log( IDE_QP_2, QP_TRC_EXEC_DCL_BEGIN, aStmt->getQueryString() );
    }
    else
    {
        ideLog::log( IDE_QP_4, QP_TRC_EXEC_DCL_BEGIN, aStmt->getQueryString() );
    }

    switch (sStmtType)
    {
        case QCI_STMT_SET_PLAN_DISPLAY_ON:
        case QCI_STMT_SET_PLAN_DISPLAY_OFF:
        case QCI_STMT_SET_PLAN_DISPLAY_ONLY:
        case QCI_STMT_SET_AUTOCOMMIT_TRUE:
        case QCI_STMT_SET_AUTOCOMMIT_FALSE:
            IDE_RAISE(InvalidSessionProperty);
            break;

        case QCI_STMT_SET_SESSION_PROPERTY:
        case QCI_STMT_SET_SYSTEM_PROPERTY:
            IDE_TEST_RAISE(sStartupPhase < MMM_STARTUP_PROCESS, InvalidServerPhaseError);
            sSmiTrans = sSession->getTrans(ID_TRUE);
            break;

        case QCI_STMT_ALT_TABLESPACE_DISCARD:
        case QCI_STMT_ALT_TABLESPACE_CHKPT_PATH:
        case QCI_STMT_ALT_DATAFILE_ONOFF:
        case QCI_STMT_ALT_RENAME_DATAFILE:
            IDE_TEST_RAISE(sStartupPhase != MMM_STARTUP_CONTROL, InvalidServerPhaseError);
            sSmiTrans = sSession->getTrans(ID_TRUE);
            break;

        /* BUG-21230 */
        case QCI_STMT_COMMIT:
        case QCI_STMT_ROLLBACK:
        case QCI_STMT_SET_TX:
            IDE_TEST_RAISE(sStartupPhase < MMM_STARTUP_META, InvalidServerPhaseError);
            IDE_TEST_RAISE(sSession->getXaAssocState() !=
                           MMD_XA_ASSOC_STATE_NOTASSOCIATED, DCLNotAllowedError); 
            sSmiTrans = sSession->getTrans(ID_TRUE);
            break;
            
        case QCI_STMT_SAVEPOINT:
        case QCI_STMT_SET_REPLICATION_MODE:
        case QCI_STMT_SET_STACK:
        case QCI_STMT_SET:
        case QCI_STMT_ALT_SYS_CHKPT:
        case QCI_STMT_ALT_SYS_VERIFY:
        case QCI_STMT_ALT_SYS_MEMORY_COMPACT:
        case QCI_STMT_ALT_SYS_ARCHIVELOG:
        case QCI_STMT_ALT_REPLICATION_STOP :  /* BUG-42852 STOP�� FLUSH�� DCL�� ��ȯ�մϴ�. */
        case QCI_STMT_ALT_REPLICATION_FLUSH : /* BUG-42852 STOP�� FLUSH�� DCL�� ��ȯ�մϴ�. */
            IDE_TEST_RAISE(sStartupPhase < MMM_STARTUP_META, InvalidServerPhaseError);
            sSmiTrans = sSession->getTrans(ID_TRUE);
            break;

        case QCI_STMT_ALT_TABLESPACE_BACKUP:
        case QCI_STMT_ALT_SYS_SWITCH_LOGFILE:
        case QCI_STMT_ALT_SYS_FLUSH_BUFFER_POOL:
        case QCI_STMT_ALT_SYS_COMPACT_PLAN_CACHE:
        case QCI_STMT_ALT_SYS_RESET_PLAN_CACHE:
        case QCI_STMT_ALT_SYS_SHRINK_MEMPOOL:
        case QCI_STMT_CONTROL_DATABASE_LINKER:
        case QCI_STMT_CLOSE_DATABASE_LINK:
            IDE_TEST_RAISE(sStartupPhase != MMM_STARTUP_SERVICE, InvalidServerPhaseError);
            sSmiTrans = sSession->getTrans(ID_TRUE);
            break;

        case QCI_STMT_CONNECT:
        case QCI_STMT_DISCONNECT:
            IDE_RAISE(UnsupportedSQL);
            break;
        /* BUG-42639 Monitoring query */
        case QCI_STMT_SELECT_FOR_FIXED_TABLE:
            /* BUG-42639 Monitoring query
             * X$, V$ ���� �����ϴ� Select Query��
             * Trans �� �Ҵ��� �ʿ䰡 ����
             */
            break;
        default:
            break;
    }

    IDE_TEST(qci::executeDCL(aStmt->getQciStmt(),
                             aStmt->getSmiStmt(),
                             sSmiTrans ) != IDE_SUCCESS);

    if ( sIsDDLLogging == ID_TRUE )
    {
        ideLog::log( IDE_QP_2, QP_TRC_EXEC_DCL_SUCCESS );
    }
    else
    {
        ideLog::log( IDE_QP_4, QP_TRC_EXEC_DCL_SUCCESS );
    }
    
    return IDE_SUCCESS;

    IDE_EXCEPTION(InvalidSessionProperty);
    {
        IDE_SET(ideSetErrorCode(mmERR_ABORT_INVALID_SESSION_PROPERTY));
    }
    IDE_EXCEPTION(InvalidServerPhaseError);
    {
        IDE_SET(ideSetErrorCode(mmERR_ABORT_SERVER_PHASE_MISMATCHES_QUERY_TYPE));
    }
    IDE_EXCEPTION(UnsupportedSQL);
    {
        IDE_SET(ideSetErrorCode(mmERR_ABORT_INTERNAL_SERVER_ERROR_ARG, "Unsupported SQL"));
    }
    IDE_EXCEPTION(DCLNotAllowedError);
    {
        IDE_SET(ideSetErrorCode(mmERR_ABORT_NOT_ALLOWED_DCL));
    }
    IDE_EXCEPTION_END;
    {
        if ( sIsDDLLogging == ID_TRUE )
        {
            ideLog::log( IDE_QP_2,
                         QP_TRC_EXEC_DCL_FAILURE,
                         E_ERROR_CODE( ideGetErrorCode() ),
                         ideGetErrorMsg( ideGetErrorCode() ) );
        }
        else
        {
            ideLog::log( IDE_QP_4,
                         QP_TRC_EXEC_DCL_FAILURE,
                         E_ERROR_CODE( ideGetErrorCode() ),
                         ideGetErrorMsg( ideGetErrorCode() ) );
        }
    }
    
    return IDE_FAILURE;
}

IDE_RC mmcStatement::executeSP(mmcStatement *aStmt, SLong * aAffectedRowCount, SLong *aFetchedRowCount)
{
    IDE_TEST(qci::execute(aStmt->getQciStmt(), aStmt->getSmiStmt()) != IDE_SUCCESS);

    qci::getRowCount(aStmt->getQciStmt(), aAffectedRowCount, aFetchedRowCount);

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC mmcStatement::executeDB(mmcStatement *aStmt, SLong * /*aAffectedRowCount*/, SLong * /*aFetchedRowCount*/)
{
    IDE_TEST(qci::execute(aStmt->getQciStmt(), aStmt->getSmiStmt()) != IDE_SUCCESS);

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}
