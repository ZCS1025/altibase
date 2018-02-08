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
 * $Id: qmgPushPred.h 24873 2008-01-18 02:23:38Z sungminee $
 *
 * Description :
 *     BUG-19756 View Predicate Pushdown
 *
 * ��� ���� :
 *
 * ��� :
 *
 **********************************************************************/

#ifndef _O_QMO_PUSH_PRED_H_
#define _O_QMO_PUSH_PRED_H_ 1

#include <qc.h>
#include <qmsParseTree.h>
#include <qmgDef.h>
#include <qmoPredicate.h>

// BUG-40409 row_number_limit �ִ�ġ
#define QMO_PUSH_RANK_MAX                   1024

class qmoPushPred
{
public:
    
    // BUG-18367 SFWGH�� predicate�� view�� predicate���� pushdown ����
    static IDE_RC doPushDownViewPredicate( qcStatement  * aStatement,
                                           qmsParseTree * aViewParseTree,
                                           qmsQuerySet  * aViewQuerySet,
                                           UShort         aViewTupleId,
                                           qmsSFWGH     * aSFWGH,
                                           qmsFrom      * aFrom,
                                           qmoPredicate * aPredicate,
                                           idBool       * aIsPushed,
                                           idBool       * aIsPushedAll,
                                           idBool       * aRemainPushedPred );
    
private:

    // Push selection �ص� �Ǵ� �������� queryset ������ �˻�
    static IDE_RC canPushSelectionQuerySet( qmsParseTree * aViewParseTree,
                                            qmsQuerySet  * aViewQuerySet,
                                            idBool       * aCanPushDown,
                                            idBool       * aRemainPushedPred );
    

    // Push slection �ص� �Ǵ� predicate���� �˻� 
    static IDE_RC canPushDownPredicate( qcStatement  * aStatement,
                                        qmsSFWGH     * aViewSFWGH,
                                        qmsTarget    * aViewTarget,
                                        UShort         aViewTupleId,
                                        qtcNode      * aNode,
                                        idBool         aContainRootsNext,
                                        idBool       * aCanPushDown );

    // BUG-18367 view�� predicate���� pushdown ����
    static IDE_RC pushDownPredicate( qcStatement  * aStatement,
                                     qmsQuerySet  * aViewQuerySet,
                                     UShort         aViewTupleId,
                                     qmsSFWGH     * aSFWGH,
                                     qmsFrom      * aFrom,
                                     qmoPredicate * aPredicate );

    /* BUG-40354 pushed rank */
    static IDE_RC isPushableRankPred( qcStatement  * aStatement,
                                      qmsParseTree * aViewParseTree,
                                      qmsQuerySet  * aViewQuerySet,
                                      UShort         aViewTupleId,
                                      qtcNode      * aNode,
                                      idBool       * aCanPushDown,
                                      UInt         * aPushedRankTargetOrder,
                                      qtcNode     ** aPushedRankLimit );

    /* BUG-40354 pushed rank */
    static IDE_RC isStopKeyPred( UShort         aViewTupleId,
                                 qtcNode      * aColumn,
                                 qtcNode      * aValue,
                                 idBool       * aIsStopKey );

    /* BUG-40354 pushed rank */
    static IDE_RC pushDownRankPredicate( qcStatement  * aStatement,
                                         qmsQuerySet  * aViewQuerySet,
                                         UInt           aRankTargetOrder,
                                         qtcNode      * aRankLimit );

    // BUG-19756 view�� predicate�� view ������ table predicate���� ��ȯ
    static IDE_RC changeViewPredIntoTablePred( qcStatement  * aStatement,
                                               qmsTarget    * aViewTarget,
                                               UShort         aViewTupleId,
                                               qtcNode      * aNode,
                                               idBool         aContainRootsNext );

    // BUG-19756 view �÷��� view target column���� ��ȯ
    static IDE_RC transformToTargetColumn( qtcNode      * aNode,
                                           qtcNode      * aTargetColumn );
    
    // BUG-19756 view �÷��� view target value�� ��ȯ
    static IDE_RC transformToTargetValue( qtcNode      * aNode,
                                          qtcNode      * aTargetColumn );
                                             
    // BUG-19756 view �÷��� view�� target expression���� ��ȯ
    static IDE_RC transformToTargetExpression( qcStatement  * aStatement,
                                               qtcNode      * aNode,
                                               qtcNode      * aTargetColumn );
};

#endif /* _O_QMO_PUSH_PRED_H_ */
