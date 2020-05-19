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
 *     Shard View Transform을 위한 자료 구조 정의
 *
 * 용어 설명 :
 *
 * 약어 :
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
    // transform shard view의 수행
    //------------------------------------------

    static IDE_RC  doTransform( qcStatement  * aStatement );

    static IDE_RC  doTransformForExpr( qcStatement  * aStatement,
                                       qtcNode      * aExpr );

    static IDE_RC  raiseInvalidShardQuery( qcStatement  * aStatement );

private:

    //------------------------------------------
    // transform shard view의 수행
    //------------------------------------------

    // query block을 순회하며 transform 수행
    static IDE_RC  processTransform( qcStatement  * aStatement );

    // query set을 순회하며 transform 수행
    static IDE_RC  processTransformForQuerySet( qcStatement  * aStatement,
                                                qmsQuerySet  * aQuerySet,
                                                ULong          aTransformSMN );

    // from shard table의 transform 수행
    static IDE_RC  processTransformForFrom( qcStatement  * aStatement,
                                            qmsFrom      * aFrom );

    // expression을 순회하며 subquery의 transform 수행
    static IDE_RC  processTransformForExpr( qcStatement  * aStatement,
                                            qtcNode      * aExpr );

    static IDE_RC  processTransformForDML( qcStatement  * aStatement );

    //------------------------------------------
    // shard query 검사
    //------------------------------------------

    // query block이 shard query인지 검사
    static IDE_RC  isShardQuery( qcStatement     * aStatement,
                                 qcNamePosition  * aParsePosition,
                                 ULong             aSMN,
                                 idBool          * aIsShardQuery,
                                 sdiAnalyzeInfo ** aShardAnalysis,
                                 UShort          * aShardParamOffset,
                                 UShort          * aShardParamCount );

    //------------------------------------------
    // shard view 생성
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
                                  sdiAnalyzeInfo * aShardAnalysis,
                                  qcNamePosition * aFilter );

    //------------------------------------------
    // PROJ-2687 shard aggregation transform
    //------------------------------------------

    static IDE_RC isTransformAbleQuery( qcStatement     * aStatement,
                                        qcNamePosition  * aParsePosition,
                                        ULong             aTransformSMN,
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

    //------------------------------------------
    // PROJ-2701 Sharding online data rebuild
    //------------------------------------------    
    static IDE_RC rebuildTransform( qcStatement    * aStatement,
                                    qcNamePosition * aStmtPos,
                                    ULong            aSessionSMN,
                                    ULong            aDataSMN );

    static IDE_RC checkRebuildTransformable( qcStatement    * aStatement,
                                             sdiAnalyzeInfo * aSessionSMNAnalysis,
                                             idBool           aDataSMNIsShardQuery,
                                             sdiAnalyzeInfo * aDataSMNAnalysis,
                                             idBool         * aIsTransformable );

    static void getNodeIdFromName( sdiNodeInfo * aNodeInfo,
                                   SChar       * aNodeName,
                                   UShort      * aNodeId );

    static void getNodeNameFromId( sdiNodeInfo * aNodeInfo,
                                   UShort        aNodeId,
                                   SChar      ** aNodeName );

    static IDE_RC makeSdiValueWithBindParam( qcStatement   * aStatement,
                                             UInt            aKeyDataType,
                                             UShort          aValueCount,
                                             sdiValueInfo  * aValues,
                                             sdiValue      * aTargetValue );
    
    static IDE_RC makeExecNodes4ShardQuery( qcStatement     * aStatement,
                                            sdiRebuildInfo  * aRebuildInfo,
                                            qcShardNodes   ** aExecNodes );

    static IDE_RC makeExecNodeWithValue( qcStatement     * aStatement,
                                         sdiRebuildInfo  * aRebuildInfo,
                                         qcShardNodes   ** aExecNodes );
    
    static IDE_RC getExecNodeId( sdiSplitMethod   aSplitMethod,
                                 UInt             aKeyDataType,
                                 UShort           aDefaultNodeId,
                                 sdiRangeInfo   * aRangeInfo,
                                 sdiValue       * aShardKeyValue,
                                 UShort         * aExecNodeId );
    
    static IDE_RC makeExecNode( qcStatement    * aStatement,
                                sdiNodeInfo    * aNodeInfo,
                                UShort           aMyNodeId,
                                UShort           aSessionSMNExecNodeCount,
                                UShort         * aSessionSMNExecNodeId,
                                UShort           aDataSMNExecNodeCount,
                                UShort         * aDataSMNExecNodeId,
                                qcShardNodes  ** aExecNodes );
    
    static IDE_RC makeExecNodeWithoutValue( qcStatement     * aStatement,
                                            sdiRebuildInfo  * aRebuildInfo,
                                            qcShardNodes   ** aExecNodes );

    static IDE_RC makeExecNodeForCloneSelect( qcStatement     * aStatement,
                                              sdiRebuildInfo  * aRebuildInfo,
                                              qcShardNodes   ** aExecNodes );

    static void getExecNodeIdWithoutValue( sdiAnalyzeInfo * aAnalysis,
                                           UShort         * aExecNodeCount,
                                           UShort         * aExecNodeId );

    static IDE_RC rebuildNonShardTransform( qcStatement    * aStatement,
                                            sdiRebuildInfo * aRebuildInfo );

    static IDE_RC rebuildTransformQuerySet( qcStatement    * aStatement,
                                            sdiRebuildInfo * aRebuildInfo,
                                            qmsQuerySet    * aQuerySet );

    static IDE_RC rebuildTransformFrom( qcStatement    * aStatement,
                                        sdiRebuildInfo * aRebuildInfo, 
                                        qmsFrom        * aFrom );

    static idBool checkRebuildTransformable4NonShard( sdiObjectInfo * aSessionSMNObj,
                                                      sdiObjectInfo * aDataSMNObj );

    static IDE_RC makeRebuildRanges( qcStatement          * aStatement,
                                     UShort                 aMyNodeId,
                                     sdiSplitMethod         aSplitMethod,
                                     UInt                   aKeyDataType,
                                     sdiRangeInfo         * aSessionSMNRangeInfo,
                                     UShort                 aSessionSMNDefaultNodeId,
                                     sdiRangeInfo         * aDataSMNRangeInfo,
                                     UShort                 aDataSMNDefaultNodeId,
                                     sdiRebuildRangeList ** aRebuildRanges,
                                     idBool               * aIsTransformNeeded );

    static IDE_RC makeRebuildFilterString( qcStatement          * aStatement,
                                           sdiObjectInfo        * aShardObject,
                                           sdiRebuildInfo       * aRebuildInfo,
                                           sdiRebuildRangeList  * aRebuildRanges,
                                           qcNamePosition       * aRebuildFilter );

    static IDE_RC makeFilterStringWithValue( qcStatement         * aStatement,
                                             sdiObjectInfo       * aShardObject,
                                             sdiRebuildInfo      * aRebuildInfo,
                                             SChar               * aShardKeyCol,
                                             sdiRebuildRangeList * aRebuildRanges,
                                             qcNamePosition      * aRebuildFilter );

    static IDE_RC getFilterType( sdiObjectInfo       * aShardObject,
                                 sdiRebuildRangeList * aRebuildRanges,
                                 sdiValue            * aFilterValue,
                                 sdiRebuildRangeType * aFilterType );

    static IDE_RC getValueStr( UInt       aKeyDataType,
                               sdiValue * aValue,
                               sdiValue * aValueStr );

    static IDE_RC makeFilterStringWithRange( sdiObjectInfo       * aShardObject,
                                             SChar               * aShardKeyCol,
                                             sdiRebuildRangeList * aRebuildRanges,
                                             qcNamePosition      * aRebuildFilter,
                                             SInt                  aRebuildFilterMaxSize );

    static IDE_RC makeHashFilter( UInt                  aKeyDataType,
                                  SChar               * aShardKeyCol,
                                  sdiRebuildRangeList * aRebuildRanges,
                                  qcNamePosition      * aRebuildFilter,
                                  SInt                  aRebuildFilterMaxSize );

    static IDE_RC makeRangeFilter( UInt                  aKeyDataType,
                                   SChar               * aShardKeyCol,
                                   sdiRebuildRangeList * aRebuildRanges,
                                   qcNamePosition      * aRebuildFilter,
                                   SInt                  aRebuildFilterMaxSize );

    static IDE_RC makeListFilter( UInt                  aKeyDataType,
                                  SChar               * aShardKeyCol,
                                  sdiRebuildRangeList * aRebuildRanges,
                                  qcNamePosition      * aRebuildFilter,
                                  SInt                  aRebuildFilterMaxSize );

    static IDE_RC rebuildTransformExpr( qcStatement    * aStatement,
                                        sdiRebuildInfo * aRebuildInfo,
                                        qtcNode        * aExpr );
};

#endif /* _O_QMV_VIEW_MERGING_H_ */
