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
 * $Id: qmgShardSelect.h 76914 2016-08-30 04:16:27Z hykim $
 *
 * Description : Shard select graph�� ���� ����
 *
 * ��� ���� :
 *
 * ��� :
 *
 **********************************************************************/

#ifndef _O_QMG_SHARD_SELECT_H_
#define _O_QMG_SHARD_SELECT_H_ 1

#include <qc.h>
#include <qmgDef.h>
#include <qmoOneNonPlan.h>

//---------------------------------------------------
// Shard graph�� Define ���
//---------------------------------------------------

#define QMG_SHARD_FLAG_CLEAR                     (0x00000000)

//---------------------------------------------------
// Shard graph �� �����ϱ� ���� �ڷ� ����
//---------------------------------------------------

typedef struct qmgShardSELT
{
    // ���� ����
    qmgGraph          graph;

    // ���� ����
    qcNamePosition    shardQuery;
    sdiAnalyzeInfo  * shardAnalysis;
    UShort            shardParamOffset;
    UShort            shardParamCount;

    qmsLimit        * limit;            // ���� PROJ ��� ������, limit start value ������ ������ ��.
    qmoAccessMethod * accessMethod;     // full scan
    UInt              accessMethodCnt;
    UInt              flag;
} qmgShardSELT;

//---------------------------------------------------
// Shard graph �� �����ϱ� ���� �Լ�
//---------------------------------------------------

class qmgShardSelect
{
public:
    // Graph �� �ʱ�ȭ
    static IDE_RC  init( qcStatement * aStatement,
                         qmsQuerySet * aQuerySet,
                         qmsFrom     * aFrom,
                         qmgGraph   ** aGraph );

    // Graph�� ����ȭ ����
    static IDE_RC optimize( qcStatement * aStatement,
                            qmgGraph * aGraph );

    // Graph�� Plan Tree ����
    static IDE_RC makePlan( qcStatement * aStatement,
                            const qmgGraph * aParent, 
                            qmgGraph * aGraph );

    // Graph�� ���� ������ �����.
    static IDE_RC printGraph( qcStatement  * aStatement,
                              qmgGraph     * aGraph,
                              ULong          aDepth,
                              iduVarString * aString );

    static IDE_RC printAccessMethod( qcStatement     * aStatement,
                                     qmoAccessMethod * aAccessMethod,
                                     ULong             aDepth,
                                     iduVarString    * aString );

private:

};

#endif /* _O_QMG_SHARD_SELECT_H_ */
