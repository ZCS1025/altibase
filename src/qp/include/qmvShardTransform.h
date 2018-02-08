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
 * $Id: qmvShardTransform.h 23857 2008-03-19 02:36:53Z sungminee $
 *
 * Description :
 *     Shard View Transform�� ���� �ڷ� ���� ����
 *
 * ��� ���� :
 *
 * ��� :
 *
 **********************************************************************/

#ifndef _O_QMV_SHARD_TRANSFORM_H_
#define _O_QMV_SHARD_TRANSFORM_H_ 1

#include <qc.h>
#include <qmsParseTree.h>
#include <qmoDependency.h>
#include <sdi.h>

class qmvShardTransform
{
public:

    //------------------------------------------
    // transform shard view�� ����
    //------------------------------------------

    static IDE_RC  doTransform( qcStatement  * aStatement );

    static IDE_RC  doTransformForExpr( qcStatement  * aStatement,
                                       qtcNode      * aExpr );

    static IDE_RC  raiseInvalidShardQuery( qcStatement  * aStatement );

private:

    //------------------------------------------
    // transform shard view�� ����
    //------------------------------------------

    // query block�� ��ȸ�ϸ� transform ����
    static IDE_RC  processTransform( qcStatement  * aStatement );

    // query set�� ��ȸ�ϸ� transform ����
    static IDE_RC  processTransformForQuerySet( qcStatement  * aStatement,
                                                qmsQuerySet  * aQuerySet );

    // from shard table�� transform ����
    static IDE_RC  processTransformForFrom( qcStatement  * aStatement,
                                            qmsFrom      * aFrom );

    // expression�� ��ȸ�ϸ� subquery�� transform ����
    static IDE_RC  processTransformForExpr( qcStatement  * aStatement,
                                            qtcNode      * aExpr );

    static IDE_RC  processTransformForDML( qcStatement  * aStatement );

    //------------------------------------------
    // shard query �˻�
    //------------------------------------------

    // query block�� shard query���� �˻�
    static IDE_RC  isShardQuery( qcStatement     * aStatement,
                                 qcNamePosition  * aParsePosition,
                                 idBool          * aIsShardQuery,
                                 sdiAnalyzeInfo ** aShardAnalysis,
                                 UShort          * aShardParamOffset,
                                 UShort          * aShardParamCount );

    //------------------------------------------
    // shard view ����
    //------------------------------------------

    // for qcStatement
    static IDE_RC  makeShardStatement( qcStatement    * aStatement,
                                       qcNamePosition * aParsePosition,
                                       qcShardStmtType  aShardStmtType,
                                       sdiAnalyzeInfo * aShardAnalysis,
                                       UShort           aShardParamOffset,
                                       UShort           aShardParamCount );

    // for querySet
    static IDE_RC  makeShardQuerySet( qcStatement    * aStatement,
                                      qmsQuerySet    * aQuerySet,
                                      qcNamePosition * aParsePosition,
                                      sdiAnalyzeInfo * aShardAnalysis,
                                      UShort           aShardParamOffset,
                                      UShort           aShardParamCount );

    // for from
    static IDE_RC  makeShardView( qcStatement    * aStatement,
                                  qmsTableRef    * aTableRef,
                                  sdiAnalyzeInfo * aShardAnalysis );

    //------------------------------------------
    // PROJ-2687 shard aggregation transform
    //------------------------------------------

    static IDE_RC isTransformAbleQuery( qcStatement     * aStatement,
                                        qcNamePosition  * aParsePosition,
                                        idBool          * aIsTransformAbleQuery );

    static IDE_RC processAggrTransform( qcStatement    * aStatement,
                                        qmsQuerySet    * aQuerySet,
                                        sdiAnalyzeInfo * aShardAnalysis,
                                        idBool         * aIsTransformed );

    static IDE_RC addColumnListToText( qtcNode        * aNode,
                                       SChar          * aQueryBuf,
                                       UInt             aQueryBufSize,
                                       qcNamePosition * aQueryPosition,
                                       UInt           * aAddedColumnCount,
                                       UInt           * aAddedTotal );

    static IDE_RC addAggrListToText( qtcNode        * aNode,
                                     SChar          * aQueryBuf,
                                     UInt             aQueryBufSize,
                                     qcNamePosition * aQueryPosition,
                                     UInt           * aAddedCount,
                                     UInt           * aAddedTotal,
                                     idBool         * aUnsupportedAggr );

    static IDE_RC addSumMinMaxCountToText( qtcNode        * aNode,
                                           SChar          * aQueryBuf,
                                           UInt             aQueryBufSize,
                                           qcNamePosition * aQueryPosition );

    static IDE_RC addAvgToText( qtcNode        * aNode,
                                SChar          * aQueryBuf,
                                UInt             aQueryBufSize,
                                qcNamePosition * aQueryPosition );

    static IDE_RC getFromEnd( qmsFrom * aFrom,
                              UInt    * aFromWhereEnd );

    static IDE_RC modifyOrgAggr( qcStatement  * aStatement,
                                 qtcNode     ** aNode,
                                 UInt         * aViewTargetOrder );

    static IDE_RC changeAggrExpr( qcStatement  * aStatement,
                                  qtcNode     ** aNode,
                                  UInt         * aViewTargetOrder );
};

#endif /* _O_QMV_VIEW_MERGING_H_ */
