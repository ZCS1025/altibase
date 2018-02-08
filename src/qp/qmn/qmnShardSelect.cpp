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
 * $Id$
 *
 * Description : SDSE(SharD SElect) Node
 *
 * ��� ���� :
 *
 * ��� :
 *
 **********************************************************************/

#include <cm.h>
#include <idl.h>
#include <ide.h>
#include <qcg.h>
#include <qmoUtil.h>
#include <qmnShardSelect.h>
#include <smi.h>

IDE_RC qmnSDSE::init( qcTemplate * aTemplate,
                      qmnPlan    * aPlan )
{
/***********************************************************************
 *
 * Description : SDSE ����� �ʱ�ȭ
 *
 * Implementation : ���� �ʱ�ȭ�� ���� ���� ��� ���� �ʱ�ȭ ����
 *
 ***********************************************************************/

    sdiClientInfo * sClientInfo = NULL;
    qmncSDSE      * sCodePlan = NULL;
    qmndSDSE      * sDataPlan = NULL;
    idBool          sJudge = ID_TRUE;

    //-------------------------------
    // ���ռ� �˻�
    //-------------------------------

    IDE_DASSERT( aTemplate != NULL );
    IDE_DASSERT( aPlan     != NULL );

    //-------------------------------
    // �⺻ �ʱ�ȭ
    //-------------------------------

    sCodePlan = (qmncSDSE*)aPlan;
    sDataPlan = (qmndSDSE*)(aTemplate->tmplate.data + aPlan->offset);
    sDataPlan->flag = &aTemplate->planFlag[sCodePlan->planID];

    // First initialization
    if ( ( *sDataPlan->flag & QMND_SDSE_INIT_DONE_MASK ) == QMND_SDSE_INIT_DONE_FALSE )
    {
        IDE_TEST( firstInit(aTemplate, sCodePlan, sDataPlan) != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    //-------------------------------
    // ������� ���� �ʱ�ȭ
    //-------------------------------

    sClientInfo = aTemplate->stmt->session->mQPSpecific.mClientInfo;

    sdi::closeDataNode( sClientInfo, sDataPlan->mDataInfo );

    //-------------------------------
    // doIt�Լ� ������ ���� Constant filter �� judgement
    //-------------------------------
    if ( sCodePlan->constantFilter != NULL )
    {
        IDE_TEST( qtc::judge( &sJudge,
                              sCodePlan->constantFilter,
                              aTemplate )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    if ( sJudge == ID_TRUE )
    {
        //------------------------------------------------
        // ���� �Լ� ����
        //------------------------------------------------
        sDataPlan->doIt = qmnSDSE::doItFirst;
        *sDataPlan->flag &= ~QMND_SDSE_ALL_FALSE_MASK;
        *sDataPlan->flag |=  QMND_SDSE_ALL_FALSE_FALSE;
    }
    else
    {
        sDataPlan->doIt = qmnSDSE::doItAllFalse;
        *sDataPlan->flag &= ~QMND_SDSE_ALL_FALSE_MASK;
        *sDataPlan->flag |= QMND_SDSE_ALL_FALSE_TRUE;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmnSDSE::firstInit( qcTemplate * aTemplate,
                           qmncSDSE   * aCodePlan,
                           qmndSDSE   * aDataPlan )
{
/***********************************************************************
 *
 * Description : Data ������ ���� �Ҵ�
 *
 * Implementation :
 *
 ***********************************************************************/

    sdiClientInfo    * sClientInfo = NULL;
    sdiDataNode        sDataNodeArg;
    sdiBindParam     * sBindParams = NULL;
    UShort             sTupleID;
    UInt               i;

    // Tuple ��ġ�� ����
    sTupleID = aCodePlan->tupleRowID;
    aDataPlan->plan.myTuple = &aTemplate->tmplate.rows[sTupleID];

    // BUGBUG
    aDataPlan->plan.myTuple->lflag &= ~MTC_TUPLE_STORAGE_MASK;
    aDataPlan->plan.myTuple->lflag |=  MTC_TUPLE_STORAGE_DISK;

    aDataPlan->nullRow        = NULL;
    aDataPlan->mCurrScanNode  = 0;
    aDataPlan->mScanDoneCount = 0;

    //-------------------------------
    // ������ �ʱ�ȭ
    //-------------------------------

    // shard linker �˻� & �ʱ�ȭ
    IDE_TEST( sdi::checkShardLinker( aTemplate->stmt ) != IDE_SUCCESS );

    IDE_TEST_RAISE( aTemplate->shardExecData.execInfo == NULL,
                    ERR_NO_SHARD_INFO );

    aDataPlan->mDataInfo = ((sdiDataNodes*)aTemplate->shardExecData.execInfo)
        + aCodePlan->shardDataIndex;

    //-------------------------------
    // shard ������ ���� �غ�
    //-------------------------------

    sClientInfo = aTemplate->stmt->session->mQPSpecific.mClientInfo;

    if ( aDataPlan->mDataInfo->mInitialized == ID_FALSE )
    {
        idlOS::memset( &sDataNodeArg, 0x00, ID_SIZEOF(sdiDataNode) );

        // data�� ������ ����(tuple�� ����) buffer ���� �Ҵ�
        sDataNodeArg.mColumnCount  = aDataPlan->plan.myTuple->columnCount;
        sDataNodeArg.mBufferLength = aDataPlan->plan.myTuple->rowOffset;
        for ( i = 0; i < SDI_NODE_MAX_COUNT; i++ )
        {
            sDataNodeArg.mBuffer[i] = (void*)( aTemplate->shardExecData.data + aCodePlan->mBuffer[i] );
            // �ʱ�ȭ
            idlOS::memset( sDataNodeArg.mBuffer[i], 0x00, sDataNodeArg.mBufferLength );
        }
        sDataNodeArg.mOffset = (UInt*)( aTemplate->shardExecData.data + aCodePlan->mOffset );
        sDataNodeArg.mMaxByteSize =
            (UInt*)( aTemplate->shardExecData.data + aCodePlan->mMaxByteSize );

        sDataNodeArg.mBindParamCount = aCodePlan->mShardParamCount;
        sDataNodeArg.mBindParams = (sdiBindParam*)
            ( aTemplate->shardExecData.data + aCodePlan->mBindParam );

        for ( i = 0; i < aDataPlan->plan.myTuple->columnCount; i++ )
        {
            sDataNodeArg.mOffset[i] = aDataPlan->plan.myTuple->columns[i].column.offset;
            sDataNodeArg.mMaxByteSize[i] = aDataPlan->plan.myTuple->columns[i].column.size;
        }

        IDE_TEST( setParamInfo( aTemplate,
                                aCodePlan,
                                sDataNodeArg.mBindParams )
                  != IDE_SUCCESS );

        IDE_TEST( sdi::initShardDataInfo( aTemplate,
                                          aCodePlan->mShardAnalysis,
                                          sClientInfo,
                                          aDataPlan->mDataInfo,
                                          & sDataNodeArg )
                  != IDE_SUCCESS );
    }
    else
    {
        if ( aCodePlan->mShardParamCount > 0 )
        {
            IDE_TEST( aTemplate->stmt->qmxMem->alloc(
                          ID_SIZEOF(sdiBindParam) * aCodePlan->mShardParamCount,
                          (void**) & sBindParams )
                      != IDE_SUCCESS );

            IDE_TEST( setParamInfo( aTemplate,
                                    aCodePlan,
                                    sBindParams )
                      != IDE_SUCCESS );

            IDE_TEST( sdi::reuseShardDataInfo( aTemplate,
                                               sClientInfo,
                                               aDataPlan->mDataInfo,
                                               sBindParams,
                                               aCodePlan->mShardParamCount )
                      != IDE_SUCCESS );
        }
        else
        {
            // Nothing to do.
        }
    }

    *aDataPlan->flag &= ~QMND_SDSE_INIT_DONE_MASK;
    *aDataPlan->flag |=  QMND_SDSE_INIT_DONE_TRUE;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NO_SHARD_INFO )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmnSDSE::firstInit",
                                  "Shard Info is not found" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmnSDSE::setParamInfo( qcTemplate   * aTemplate,
                              qmncSDSE     * aCodePlan,
                              sdiBindParam * aBindParams )
{
    qciBindParamInfo * sAllParamInfo = NULL;
    qciBindParam     * sBindParam = NULL;
    UInt               sBindOffset = 0;
    UInt               i = 0;

    // PROJ-2653
    sAllParamInfo  = aTemplate->stmt->pBindParam;

    for ( i = 0; i < aCodePlan->mShardParamCount; i++ )
    {
        sBindOffset = aCodePlan->mShardParamOffset + i;

        IDE_DASSERT( sBindOffset < aTemplate->stmt->pBindParamCount );

        sBindParam = &sAllParamInfo[sBindOffset].param;

        if ( ( sBindParam->inoutType == CMP_DB_PARAM_INPUT ) ||
             ( sBindParam->inoutType == CMP_DB_PARAM_INPUT_OUTPUT ) )
        {
            IDE_DASSERT( sAllParamInfo[sBindOffset].isParamInfoBound == ID_TRUE );
            IDE_DASSERT( sAllParamInfo[sBindOffset].isParamDataBound == ID_TRUE );
        }
        else
        {
            // Nothing to do.
        }

        aBindParams[i].mId        = i + 1;
        aBindParams[i].mInoutType = sBindParam->inoutType;
        aBindParams[i].mType      = sBindParam->type;
        aBindParams[i].mData      = sBindParam->data;
        aBindParams[i].mDataSize  = sBindParam->dataSize;
        aBindParams[i].mPrecision = sBindParam->precision;
        aBindParams[i].mScale     = sBindParam->scale;
    }

    return IDE_SUCCESS;
}

IDE_RC qmnSDSE::doIt( qcTemplate * aTemplate,
                      qmnPlan    * aPlan,
                      qmcRowFlag * aFlag )
{
    qmndSDSE * sDataPlan = (qmndSDSE*) (aTemplate->tmplate.data + aPlan->offset);

    return sDataPlan->doIt( aTemplate, aPlan, aFlag );
}

IDE_RC qmnSDSE::doItAllFalse( qcTemplate * aTemplate,
                              qmnPlan    * aPlan,
                              qmcRowFlag * aFlag )
{
/***********************************************************************
 *
 * Description : Constant Filter �˻��Ŀ� �����Ǵ� �Լ��� ���� �����ϴ�
 *               Record�� �������� �ʴ´�.
 *
 * Implementation : �׻� record ������ �����Ѵ�.
 *
 ***********************************************************************/

    qmncSDSE * sCodePlan = (qmncSDSE*)aPlan;
    qmndSDSE * sDataPlan = (qmndSDSE*)(aTemplate->tmplate.data + aPlan->offset);

    // ���ռ� �˻�
    IDE_DASSERT( sCodePlan->constantFilter != NULL );
    IDE_DASSERT( ( *sDataPlan->flag & QMND_SDSE_ALL_FALSE_MASK ) == QMND_SDSE_ALL_FALSE_TRUE );

    // ������ ������ Setting
    *aFlag &= ~QMC_ROW_DATA_MASK;
    *aFlag |= QMC_ROW_DATA_NONE;

    return IDE_SUCCESS;
}

IDE_RC qmnSDSE::doItFirst( qcTemplate * aTemplate,
                           qmnPlan    * aPlan,
                           qmcRowFlag * aFlag )
{
/***********************************************************************
 *
 * Description : data ������ ���� �ʱ�ȭ�� �����ϰ�
 *               data �� �������� ���� �Լ��� ȣ���Ѵ�.
 *
 * Implementation :
 *              - allocStmt
 *              - prepare
 *              - bindCol (PROJ-2638 ������ ����)
 *              - execute
 *
 ***********************************************************************/

    qmncSDSE       * sCodePlan = (qmncSDSE *)aPlan;
    qmndSDSE       * sDataPlan = (qmndSDSE *)(aTemplate->tmplate.data + aPlan->offset);
    sdiClientInfo  * sClientInfo = aTemplate->stmt->session->mQPSpecific.mClientInfo;

    // ������ ���� �˻�
    IDE_TEST( iduCheckSessionEvent( aTemplate->stmt->mStatistics ) != IDE_SUCCESS );

    // DataPlan �ʱ�ȭ
    sDataPlan->mCurrScanNode  = 0;
    sDataPlan->mScanDoneCount = 0;

    //-------------------------------
    // ������ ����
    //-------------------------------

    IDE_TEST( sdi::decideShardDataInfo(
                  aTemplate,
                  &(aTemplate->tmplate.rows[aTemplate->tmplate.variableRow]),
                  sCodePlan->mShardAnalysis,
                  sClientInfo,
                  sDataPlan->mDataInfo,
                  sCodePlan->mQueryPos )
              != IDE_SUCCESS );

    //-------------------------------
    // ����
    //-------------------------------

    IDE_TEST( sdi::executeSelect( aTemplate->stmt,
                                  sClientInfo,
                                  sDataPlan->mDataInfo )
              != IDE_SUCCESS );

    IDE_TEST( doItNext( aTemplate, aPlan, aFlag ) != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmnSDSE::doItNext( qcTemplate * aTemplate,
                          qmnPlan    * aPlan,
                          qmcRowFlag * aFlag )
{
/***********************************************************************
 *
 * Description : data �� �������� �Լ��� �����Ѵ�.
 *
 *    Ư�� data node �� buffer �� ���� ��Ե� ��츦 �����Ͽ�,
 *    data node �� �� ���� ���ư��鼭 �����Ѵ�.
 *
 *    ����� ���� data node �� �ǳʶٸ�,
 *    ��� data node �� doIt ����� QMC_ROW_DATA_NONE(no rows)��
 *    �� �� ���� �����Ѵ�.
 *
 * Implementation :
 *              - fetch
 *
 ***********************************************************************/

    qmncSDSE       * sCodePlan = (qmncSDSE*)aPlan;
    qmndSDSE       * sDataPlan = (qmndSDSE*)(aTemplate->tmplate.data + aPlan->offset);
    sdiClientInfo  * sClientInfo = aTemplate->stmt->session->mQPSpecific.mClientInfo;
    sdiConnectInfo * sConnectInfo = NULL;
    sdiDataNode    * sDataNode = NULL;
    mtcColumn      * sColumn = NULL;
    idBool           sJudge = ID_FALSE;
    idBool           sExist = ID_FALSE;
    UInt             i;

    while ( 1 )
    {
        if ( sDataPlan->mCurrScanNode == sClientInfo->mCount )
        {
            // ���� doIt�� ������ data node ���� ���� �Ǿ��ٸ�,
            // ù��° data node ���� �ٽ� doIt�ϵ��� �Ѵ�.
            sDataPlan->mCurrScanNode = 0;
        }
        else
        {
            // Nothing to do.
        }

        sConnectInfo = &(sClientInfo->mConnectInfo[sDataPlan->mCurrScanNode]);
        sDataNode = &(sDataPlan->mDataInfo->mNodes[sDataPlan->mCurrScanNode]);

        // ���� doIt�� ����� ������ data node �� skip�Ѵ�.
        if ( sDataNode->mState == SDI_NODE_STATE_EXECUTED )
        {
            sJudge = ID_FALSE;

            while ( sJudge == ID_FALSE )
            {
                // fetch client
                IDE_TEST( sdi::fetch( sConnectInfo, sDataNode, &sExist )
                          != IDE_SUCCESS );

                // �߸��� �����Ͱ� fetch�Ǵ� ��츦 ����Ѵ�.
                sColumn = sDataPlan->plan.myTuple->columns;
                for ( i = 0; i < sDataPlan->plan.myTuple->columnCount; i++, sColumn++ )
                {
                    IDE_TEST_RAISE( sColumn->module->actualSize(
                                        sColumn,
                                        (UChar*)sDataNode->mBuffer[sDataPlan->mCurrScanNode] +
                                        sColumn->column.offset ) >
                                    sColumn->column.size,
                                    ERR_INVALID_DATA_FETCHED );
                }

                //------------------------------
                // Data ���� ���ο� ���� ó��
                //------------------------------

                if ( sExist == ID_TRUE )
                {
                    sDataPlan->plan.myTuple->row =
                        sDataNode->mBuffer[sDataPlan->mCurrScanNode];

                    // BUGBUG nullRID
                    SMI_MAKE_VIRTUAL_NULL_GRID( sDataPlan->plan.myTuple->rid );

                    sDataPlan->plan.myTuple->modify++;

                    if ( sCodePlan->filter != NULL )
                    {
                        IDE_TEST( qtc::judge( &sJudge,
                                              sCodePlan->filter,
                                              aTemplate )
                                  != IDE_SUCCESS );
                    }
                    else
                    {
                        sJudge = ID_TRUE;
                    }

                    if ( sJudge == ID_TRUE )
                    {
                        if ( sCodePlan->subqueryFilter != NULL )
                        {
                            IDE_TEST( qtc::judge( &sJudge,
                                                  sCodePlan->subqueryFilter,
                                                  aTemplate )
                                      != IDE_SUCCESS );
                        }
                        else
                        {
                            // Nothing to do.
                        }
                    }
                    else
                    {
                        // Nothing to do.
                    }

                    if ( sJudge == ID_TRUE )
                    {
                        if ( sCodePlan->nnfFilter != NULL )
                        {
                            IDE_TEST( qtc::judge( &sJudge,
                                                  sCodePlan->nnfFilter,
                                                  aTemplate )
                                      != IDE_SUCCESS );
                        }
                        else
                        {
                            // Nothing to do.
                        }
                    }
                    else
                    {
                        // Nothing to do.
                    }

                    if ( sJudge == ID_TRUE )
                    {
                        *aFlag = QMC_ROW_DATA_EXIST;
                        sDataPlan->doIt = qmnSDSE::doItNext;
                        break;
                    }
                    else
                    {
                        // Nothing to do.
                    }
                }
                else
                {
                    // a data node fetch complete
                    sDataNode->mState = SDI_NODE_STATE_FETCHED;
                    sDataPlan->mScanDoneCount++;
                    break;
                }
            }

            if ( sJudge == ID_TRUE )
            {
                IDE_DASSERT( *aFlag == QMC_ROW_DATA_EXIST );
                break;
            }
            else
            {
                // Nothing to do.
            }
        }
        else
        {
            if ( sDataNode->mState < SDI_NODE_STATE_EXECUTED )
            {
                sDataPlan->mScanDoneCount++;
            }
            else
            {
                IDE_DASSERT( sDataNode->mState == SDI_NODE_STATE_FETCHED );
            }
        }

        if ( sDataPlan->mScanDoneCount == sClientInfo->mCount )
        {
            *aFlag = QMC_ROW_DATA_NONE;
            sDataPlan->doIt = qmnSDSE::doItFirst;
            break;
        }
        else
        {
            sDataPlan->mCurrScanNode++;
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_INVALID_DATA_FETCHED )
    {
        IDE_SET( ideSetErrorCode( mtERR_ABORT_INVALID_LENGTH ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmnSDSE::padNull( qcTemplate * aTemplate,
                         qmnPlan    * aPlan )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation :
 *
 ***********************************************************************/

    qmncSDSE * sCodePlan = (qmncSDSE*)aPlan;
    qmndSDSE * sDataPlan = (qmndSDSE*)(aTemplate->tmplate.data + aPlan->offset);

    if ( ( aTemplate->planFlag[sCodePlan->planID] & QMND_SDSE_INIT_DONE_MASK )
         == QMND_SDSE_INIT_DONE_FALSE )
    {
        // �ʱ�ȭ ���� ���� ��� �ʱ�ȭ ����
        IDE_TEST( aPlan->init( aTemplate, aPlan ) != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    if ( ( sCodePlan->plan.flag & QMN_PLAN_STORAGE_MASK ) == QMN_PLAN_STORAGE_DISK )
    {
        //-----------------------------------
        // Disk Table�� ���
        //-----------------------------------

        // Record ������ ���� ������ �ϳ��� �����ϸ�,
        // �̿� ���� pointer�� �׻� �����Ǿ�� �Ѵ�.

        if ( sDataPlan->nullRow == NULL )
        {
            //-----------------------------------
            // Null Row�� ������ ���� ���� ���
            //-----------------------------------

            // ���ռ� �˻�
            IDE_DASSERT( sDataPlan->plan.myTuple->rowOffset > 0 );

            // Null Row�� ���� ���� �Ҵ�
            IDE_TEST( aTemplate->stmt->qmxMem->cralloc( sDataPlan->plan.myTuple->rowOffset,
                                                        (void**) &sDataPlan->nullRow )
                      != IDE_SUCCESS );

            // PROJ-1705
            // ��ũ���̺��� null row�� qp���� ����/�����صΰ� ����Ѵ�.
            IDE_TEST( qmn::makeNullRow( sDataPlan->plan.myTuple,
                                        sDataPlan->nullRow )
                      != IDE_SUCCESS );

            SMI_MAKE_VIRTUAL_NULL_GRID( sDataPlan->nullRID );
        }
        else
        {
            // �̹� Null Row�� ��������.
            // Nothing to do.
        }

        // Null Row ����
        idlOS::memcpy( sDataPlan->plan.myTuple->row,
                       sDataPlan->nullRow,
                       sDataPlan->plan.myTuple->rowOffset );

        // Null RID�� ����
        idlOS::memcpy( &sDataPlan->plan.myTuple->rid,
                       &sDataPlan->nullRID,
                       ID_SIZEOF(scGRID) );
    }
    else
    {
        //-----------------------------------
        // Memory Table�� ���
        //-----------------------------------
        // data node �� tuple�� �׻� disk tuple�̴�.
        IDE_DASSERT( 1 );
    }

    sDataPlan->plan.myTuple->modify++;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmnSDSE::printPlan( qcTemplate   * aTemplate,
                           qmnPlan      * aPlan,
                           ULong          aDepth,
                           iduVarString * aString,
                           qmnDisplay     aMode )
{
/***********************************************************************
 *
 * Description : SDSE ����� ���� ������ ����Ѵ�.
 *
 * Implementation :
 *
 ***********************************************************************/

    qmncSDSE * sCodePlan = (qmncSDSE*)aPlan;
    qmndSDSE * sDataPlan = (qmndSDSE*)(aTemplate->tmplate.data + aPlan->offset);
    sdiClientInfo * sClientInfo = aTemplate->stmt->session->mQPSpecific.mClientInfo;

    sDataPlan->flag = & aTemplate->planFlag[sCodePlan->planID];

    //----------------------------
    // SDSE ��� ǥ��
    //----------------------------
    if ( QCU_TRCLOG_DETAIL_MTRNODE == 1 )
    {
        qmn::printSpaceDepth( aString, aDepth );
        iduVarStringAppend( aString, "SHARD-COORDINATOR [ " );
        iduVarStringAppendFormat( aString, "SELF: %"ID_INT32_FMT" ]\n",
                                  (SInt)sCodePlan->tupleRowID );
    }
    else
    {
        qmn::printSpaceDepth( aString, aDepth );
        iduVarStringAppend( aString, "SHARD-COORDINATOR\n" );
    }

    //----------------------------
    // Predicate ������ �� ���
    //----------------------------
    if ( QCG_GET_SESSION_TRCLOG_DETAIL_PREDICATE( aTemplate->stmt ) == 1 )
    {
        // Normal Filter ���
        if ( sCodePlan->filter != NULL )
        {
            qmn::printSpaceDepth( aString, aDepth );
            iduVarStringAppend( aString, " [ FILTER ]\n" );
            IDE_TEST( qmoUtil::printPredInPlan( aTemplate,
                                                aString,
                                                aDepth + 1,
                                                sCodePlan->filter )
                      != IDE_SUCCESS );
        }
        else
        {
            // Nothing to do.
        }

        // Constant Filter
        if ( sCodePlan->constantFilter != NULL )
        {
            qmn::printSpaceDepth( aString, aDepth );
            iduVarStringAppend( aString, " [ CONSTANT FILTER ]\n" );
            IDE_TEST( qmoUtil::printPredInPlan( aTemplate,
                                                aString,
                                                aDepth + 1,
                                                sCodePlan->constantFilter )
                      != IDE_SUCCESS );
        }
        else
        {
            // Nothing to do.
        }

        // Subquery Filter
        if ( sCodePlan->subqueryFilter != NULL )
        {
            qmn::printSpaceDepth( aString, aDepth );
            iduVarStringAppend( aString, " [ SUBQUERY FILTER ]\n" );
            IDE_TEST( qmoUtil::printPredInPlan( aTemplate,
                                                aString,
                                                aDepth + 1,
                                                sCodePlan->subqueryFilter )
                      != IDE_SUCCESS );
        }
        else
        {
            // Nothing to do.
        }

        // NNF Filter
        if ( sCodePlan->nnfFilter != NULL )
        {
            qmn::printSpaceDepth( aString, aDepth );
            iduVarStringAppend( aString, " [ NOT-NORMAL-FORM FILTER ]\n" );
            IDE_TEST( qmoUtil::printPredInPlan( aTemplate,
                                                aString,
                                                aDepth + 1,
                                                sCodePlan->nnfFilter )
                      != IDE_SUCCESS );
        }
        else
        {
            // Nothing to do.
        }

        if ( sClientInfo != NULL )
        {
            // �������� ���
            IDE_DASSERT( QMND_SDSE_INIT_DONE_TRUE == QMND_SDEX_INIT_DONE_TRUE );

            IDE_TEST( qmnSDEX::printDataInfo( aTemplate,
                                              sClientInfo,
                                              sDataPlan->mDataInfo,
                                              aDepth + 1,
                                              aString,
                                              aMode,
                                              sDataPlan->flag )
                      != IDE_SUCCESS );
        }
        else
        {
            // Nothing to do.
        }
    }
    else
    {
        // Nothing to do.
    }

    //----------------------------
    // Subquery ������ ���.
    //----------------------------
    // subquery�� constant filter, nnf filter, subquery filter���� �ִ�.
    // Constant Filter�� Subquery ���� ���
    if ( sCodePlan->constantFilter != NULL )
    {
        IDE_TEST( qmn::printSubqueryPlan( aTemplate,
                                          sCodePlan->constantFilter,
                                          aDepth,
                                          aString,
                                          aMode )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    // Subquery Filter�� Subquery ���� ���
    if ( sCodePlan->subqueryFilter != NULL )
    {
        IDE_TEST( qmn::printSubqueryPlan( aTemplate,
                                          sCodePlan->subqueryFilter,
                                          aDepth,
                                          aString,
                                          aMode )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    // NNF Filter�� Subquery ���� ���
    if ( sCodePlan->nnfFilter != NULL )
    {
        IDE_TEST( qmn::printSubqueryPlan( aTemplate,
                                          sCodePlan->nnfFilter,
                                          aDepth,
                                          aString,
                                          aMode )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    //----------------------------
    // Operator�� ��� ���� ���
    //----------------------------
    if ( QCU_TRCLOG_RESULT_DESC == 1 )
    {
        IDE_TEST( qmn::printResult( aTemplate,
                                    aDepth,
                                    aString,
                                    sCodePlan->plan.resultDesc )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}
