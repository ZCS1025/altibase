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
 
/******************************************************************************
 * $Id$
 *
 * Description : ORDER BY Elimination Transformation
 *
 * ��� ���� :
 *
 *****************************************************************************/

#include <ide.h>
#include <qcuProperty.h>
#include <qcgPlan.h>
#include <qmoOBYETransform.h>

extern mtfModule mtfCount;

IDE_RC qmoOBYETransform::doTransform( qcStatement * aStatement,
                                      qmsQuerySet * aQuerySet )
{
/***********************************************************************
 *
 * Description : BUG-41183 Inline view �� ���ʿ��� ORDER BY ����
 *
 * Implementation :
 *
 *       ���� ������ ������ ��� inline view �� order by ���� �����Ѵ�.
 *
 *       - SELECT count(*) ������ ����
 *       - FROM ���� ��� inline view �� ����
 *       - LIMIT ���� ���� ���
 *
 ***********************************************************************/

    qmsFrom      * sFrom = NULL;
    qmsParseTree * sParseTree = NULL;
    qmsQuerySet  * sQuerySet = NULL;

    IDU_FIT_POINT_FATAL( "qmoOBYETransform::doTransform::__FT__" );

    //------------------------------------------
    // ���ռ� �˻�
    //------------------------------------------

    IDE_DASSERT( aQuerySet != NULL );

    //------------------------------------------
    // ���� �˻�
    //------------------------------------------

    if ( QCU_OPTIMIZER_ORDER_BY_ELIMINATION_ENABLE == 1 )
    {
        if ( aQuerySet->setOp == QMS_NONE )
        {
            IDE_DASSERT( aQuerySet->SFWGH != NULL );

            for ( sFrom = aQuerySet->SFWGH->from; sFrom != NULL; sFrom = sFrom->next )
            {
                if ( ( sFrom->tableRef != NULL ) &&
                     ( sFrom->tableRef->view != NULL ) &&
                     ( sFrom->tableRef->tableInfo->tableType == QCM_USER_TABLE ) )
                {
                    // inline view
                    sParseTree = (qmsParseTree*)(sFrom->tableRef->view->myPlan->parseTree);
                    sQuerySet = sParseTree->querySet;

                    IDE_TEST( doTransform( aStatement,
                                           sQuerySet )
                              != IDE_SUCCESS );

                    if ( ( aQuerySet->target->targetColumn->node.module == &mtfCount ) &&
                         ( aQuerySet->target->next == NULL ) &&
                         ( sParseTree->limit == NULL ) &&
                         ( sQuerySet->setOp == QMS_NONE ) )
                    {
                        sParseTree->orderBy = NULL;
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
            }
        }
        else
        {
            IDE_TEST( doTransform( aStatement,
                                   aQuerySet->left )
                      != IDE_SUCCESS );

            IDE_TEST( doTransform( aStatement,
                                   aQuerySet->right )
                      != IDE_SUCCESS );
        }
    }
    else
    {
        // Nothing to do.
    }

    // environment�� ���
    qcgPlan::registerPlanProperty(
            aStatement,
            PLAN_PROPERTY_OPTIMIZER_ORDER_BY_ELIMINATION_ENABLE );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}
