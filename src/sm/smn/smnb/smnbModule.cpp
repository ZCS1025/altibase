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
 * $Id: smnbModule.cpp 82075 2018-01-17 06:39:52Z jina.kim $
 **********************************************************************/

#include <idl.h>
#include <ide.h>
#include <idu.h>
#include <idCore.h>
#include <smc.h>
#include <smm.h>
#include <smu.h>
#include <smDef.h>
#include <smErrorCode.h>
#include <smnReq.h>
#include <smnManager.h>
#include <smnbModule.h>
#include <sgmManager.h>
#include <smiMisc.h>

extern smiGlobalCallBackList gSmiGlobalCallBackList;

smmSlotList  smnbBTree::mSmnbNodePool;
void*        smnbBTree::mSmnbFreeNodeList;

UInt         smnbBTree::mNodeSize;
UInt         smnbBTree::mIteratorSize;
UInt         smnbBTree::mDefaultMaxKeySize; /* PROJ-2433 */
UInt         smnbBTree::mNodeSplitRate;     /* BUG-40509 */


//fix BUG-23007
/* Index Node Pool */

smnIndexModule smnbModule =
{
    SMN_MAKE_INDEX_MODULE_ID( SMI_TABLE_MEMORY,
                              SMI_BUILTIN_B_TREE_INDEXTYPE_ID ),
    SMN_RANGE_ENABLE | SMN_DIMENSION_DISABLE | SMN_DEFAULT_ENABLE |
        SMN_BOTTOMUP_BUILD_ENABLE | SMN_FREE_NODE_LIST_ENABLE,
    SMP_VC_PIECE_MAX_SIZE - 1,  // BUG-23113
    (smnMemoryFunc)       smnbBTree::prepareIteratorMem,
    (smnMemoryFunc)       smnbBTree::releaseIteratorMem,
    (smnMemoryFunc)       smnbBTree::prepareFreeNodeMem,
    (smnMemoryFunc)       smnbBTree::releaseFreeNodeMem,
    (smnCreateFunc)       smnbBTree::create,
    (smnDropFunc)         smnbBTree::drop,

    (smTableCursorLockRowFunc)  smnManager::lockRow,
    (smnDeleteFunc)             smnbBTree::deleteNA,
    (smnFreeFunc)               smnbBTree::freeSlot,
    (smnExistKeyFunc)           smnbBTree::existKey,
    (smnInsertRollbackFunc)     NULL,
    (smnDeleteRollbackFunc)     NULL,
    (smnAgingFunc)              NULL,

    (smInitFunc)         smnbBTree::init,
    (smDestFunc)         smnbBTree::dest,
    (smnFreeNodeListFunc) smnbBTree::freeAllNodeList,
    (smnGetPositionFunc)  smnbBTree::getPosition,
    (smnSetPositionFunc)  smnbBTree::setPosition,

    (smnAllocIteratorFunc) smnbBTree::allocIterator,
    (smnFreeIteratorFunc)  smnbBTree::freeIterator,
    (smnReInitFunc)        NULL,
    (smnInitMetaFunc)      NULL,
    (smnMakeDiskImageFunc) smnbBTree::makeDiskImage,

    (smnBuildIndexFunc)     smnbBTree::buildIndex,
    (smnGetSmoNoFunc)       NULL,
    (smnMakeKeyFromRow)     NULL,
    (smnMakeKeyFromSmiValue)NULL,
    (smnRebuildIndexColumn) NULL,
    (smnSetIndexConsistency)NULL,
    (smnGatherStat)         smnbBTree::gatherStat
};

static const  smSeekFunc smnbSeekFunctions[32][12] =
{
    { /* SMI_TRAVERSE_FORWARD  SMI_PREVIOUS_DISABLE SMI_LOCK_READ            */
        (smSeekFunc)smnbBTree::fetchNext,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::beforeFirst,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::beforeFirst,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::beforeFirst
    },
    { /* SMI_TRAVERSE_FORWARD  SMI_PREVIOUS_DISABLE SMI_LOCK_WRITE           */
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::beforeFirstU,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::beforeFirst,
        (smSeekFunc)smnbBTree::fetchNext,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::beforeFirst
    },
    { /* SMI_TRAVERSE_FORWARD  SMI_PREVIOUS_DISABLE SMI_LOCK_REPEATABLE      */
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::beforeFirstR,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::beforeFirst,
        (smSeekFunc)smnbBTree::fetchNext,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::beforeFirst,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::beforeFirst
    },
    { /* SMI_TRAVERSE_FORWARD  SMI_PREVIOUS_DISABLE SMI_LOCK_TABLE_SHARED    */
        (smSeekFunc)smnbBTree::fetchNext,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::beforeFirst,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::beforeFirst,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::beforeFirst
    },
    { /* SMI_TRAVERSE_FORWARD  SMI_PREVIOUS_DISABLE SMI_LOCK_TABLE_EXCLUSIVE */
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::beforeFirstU,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::beforeFirst,
        (smSeekFunc)smnbBTree::fetchNext,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::beforeFirst
    },
    { /* UNUSED                                                              */
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
    },
    { /* UNUSED                                                              */
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
    },
    { /* UNUSED                                                              */
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
    },
    { /* SMI_TRAVERSE_FORWARD  SMI_PREVIOUS_ENABLE  SMI_LOCK_READ            */
        (smSeekFunc)smnbBTree::fetchNext,
        (smSeekFunc)smnbBTree::fetchPrev,
        (smSeekFunc)smnbBTree::beforeFirst,
        (smSeekFunc)smnbBTree::afterLast,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::beforeFirst,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::beforeFirst
    },
    { /* SMI_TRAVERSE_FORWARD  SMI_PREVIOUS_ENABLE  SMI_LOCK_WRITE           */
        (smSeekFunc)smnbBTree::fetchNextU,
        (smSeekFunc)smnbBTree::fetchPrevU,
        (smSeekFunc)smnbBTree::beforeFirst,
        (smSeekFunc)smnbBTree::afterLast,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::beforeFirst,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::beforeFirst
    },
    { /* SMI_TRAVERSE_FORWARD  SMI_PREVIOUS_ENABLE  SMI_LOCK_REPEATABLE      */
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::beforeFirstR,
        (smSeekFunc)smnbBTree::afterLastR,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::beforeFirst,
        (smSeekFunc)smnbBTree::fetchNext,
        (smSeekFunc)smnbBTree::fetchPrev,
        (smSeekFunc)smnbBTree::beforeFirst,
        (smSeekFunc)smnbBTree::afterLast,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::beforeFirst
    },
    { /* SMI_TRAVERSE_FORWARD  SMI_PREVIOUS_ENABLE  SMI_LOCK_TABLE_SHARED    */
        (smSeekFunc)smnbBTree::fetchNext,
        (smSeekFunc)smnbBTree::fetchPrev,
        (smSeekFunc)smnbBTree::beforeFirst,
        (smSeekFunc)smnbBTree::afterLast,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::beforeFirst,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::beforeFirst
    },
    { /* SMI_TRAVERSE_FORWARD  SMI_PREVIOUS_ENABLE  SMI_LOCK_TABLE_EXCLUSIVE */
        (smSeekFunc)smnbBTree::fetchNextU,
        (smSeekFunc)smnbBTree::fetchPrevU,
        (smSeekFunc)smnbBTree::beforeFirst,
        (smSeekFunc)smnbBTree::afterLast,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::beforeFirst,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::beforeFirst
    },
    { /* UNUSED                                                              */
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
    },
    { /* UNUSED                                                              */
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA
    },
    { /* UNUSED                                                              */
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA
    },
    { /* SMI_TRAVERSE_BACKWARD SMI_PREVIOUS_DISABLE SMI_LOCK_READ            */
        (smSeekFunc)smnbBTree::fetchPrev,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::afterLast,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::afterLast,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::afterLast
    },
    { /* SMI_TRAVERSE_BACKWARD SMI_PREVIOUS_DISABLE SMI_LOCK_WRITE           */
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::afterLastU,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::afterLast,
        (smSeekFunc)smnbBTree::fetchPrev,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::afterLast
    },
    { /* SMI_TRAVERSE_BACKWARD SMI_PREVIOUS_DISABLE SMI_LOCK_REPEATABLE      */
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::afterLastR,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::afterLast,
        (smSeekFunc)smnbBTree::fetchPrev,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::afterLast,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::afterLast
    },
    { /* SMI_TRAVERSE_BACKWARD SMI_PREVIOUS_DISABLE SMI_LOCK_TABLE_SHARED    */
        (smSeekFunc)smnbBTree::fetchPrev,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::afterLast,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::afterLast,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::afterLast
    },
    { /* SMI_TRAVERSE_BACKWARD SMI_PREVIOUS_DISABLE SMI_LOCK_TABLE_EXCLUSIVE */
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::afterLastU,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::afterLast,
        (smSeekFunc)smnbBTree::fetchPrev,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::afterLast
    },
    { /* UNUSED                                                              */
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA
    },
    { /* UNUSED                                                              */
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA
    },
    { /* UNUSED                                                              */
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA
    },
    { /* SMI_TRAVERSE_BACKWARD SMI_PREVIOUS_ENABLE  SMI_LOCK_READ            */
        (smSeekFunc)smnbBTree::fetchPrev,
        (smSeekFunc)smnbBTree::fetchNext,
        (smSeekFunc)smnbBTree::afterLast,
        (smSeekFunc)smnbBTree::beforeFirst,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::afterLast,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::afterLast
    },
    { /* SMI_TRAVERSE_BACKWARD SMI_PREVIOUS_ENABLE  SMI_LOCK_WRITE           */
        (smSeekFunc)smnbBTree::fetchPrevU,
        (smSeekFunc)smnbBTree::fetchNextU,
        (smSeekFunc)smnbBTree::afterLast,
        (smSeekFunc)smnbBTree::beforeFirst,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::afterLast,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::afterLast
    },
    { /* SMI_TRAVERSE_BACKWARD SMI_PREVIOUS_ENABLE  SMI_LOCK_REPEATABLE      */
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::afterLastR,
        (smSeekFunc)smnbBTree::beforeFirstR,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::afterLast,
        (smSeekFunc)smnbBTree::fetchPrev,
        (smSeekFunc)smnbBTree::fetchNext,
        (smSeekFunc)smnbBTree::afterLast,
        (smSeekFunc)smnbBTree::beforeFirst,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::afterLast
    },
    { /* SMI_TRAVERSE_BACKWARD SMI_PREVIOUS_ENABLE  SMI_LOCK_TABLE_SHARED    */
        (smSeekFunc)smnbBTree::fetchPrev,
        (smSeekFunc)smnbBTree::fetchNext,
        (smSeekFunc)smnbBTree::afterLast,
        (smSeekFunc)smnbBTree::beforeFirst,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::afterLast,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::afterLast
    },
    { /* SMI_TRAVERSE_BACKWARD SMI_PREVIOUS_ENABLE  SMI_LOCK_TABLE_EXCLUSIVE */
        (smSeekFunc)smnbBTree::fetchPrevU,
        (smSeekFunc)smnbBTree::fetchNextU,
        (smSeekFunc)smnbBTree::afterLast,
        (smSeekFunc)smnbBTree::beforeFirst,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::afterLast,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::retraverse,
        (smSeekFunc)smnbBTree::afterLast
    },
    { /* UNUSED                                                              */
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
    },
    { /* UNUSED                                                              */
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA
    },
    { /* UNUSED                                                              */
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
        (smSeekFunc)smnbBTree::NA,
    }
};

void smnbBTree::setIndexProperties()
{
    SInt sNodeCntPerPage = 0;

    /* PROJ-2433
     * NODE SIZE�� property __MEM_BTREE_INDEX_NODE_SIZE ���̴�.
     * ��, ���������� ������ ���ϴ� ������ ���̱����� �����Ѵ�.
     * ( mNodeSize >= __MEM_BTREE_INDEX_NODE_SIZE,
     *                except for __MEM_BTREE_INDEX_NODE_SIZE == 32K ) */

    mNodeSize       = smuProperty::getMemBtreeNodeSize();
    sNodeCntPerPage = SMM_TEMP_PAGE_BODY_SIZE / idlOS::align( mNodeSize );
    sNodeCntPerPage = ( sNodeCntPerPage <= 0 ) ? 1 : sNodeCntPerPage; /* page�� �ּ� �Ѱ��� node �Ҵ�*/
    mNodeSize       = idlOS::align( (SInt)( ( SMM_TEMP_PAGE_BODY_SIZE / sNodeCntPerPage )
                                            - idlOS::align(1) + 1 ) );

    /* PROJ-2433
     * mIteratorSize�� btree index �������� ����ϴ� smnbIterator�� �Ҵ��� �޸� ũ���̴�.
     * smnbIterator�� ��� + rows[] �� �����Ǹ�,
     * rows[]���� fetch�� node�� row pointer���� ����ȴ�.
     * ���� row[]�� �ִ�ũ��� (node ũ�� - smnbLNode ũ��) �̴�.
     * highfence�� ����ϱ⶧���� row poniter + 1�� �߰��뷮�� Ȯ���Ѵ� */

    mIteratorSize = ( ID_SIZEOF( smnbIterator ) +
                      mNodeSize - ID_SIZEOF( smnbLNode ) +
                      ID_SIZEOF( SChar * ) );

    mDefaultMaxKeySize = smuProperty::getMemBtreeDefaultMaxKeySize();

    smnbBTree::setNodeSplitRate(); /* BUG-40509 */
}

IDE_RC smnbBTree::prepareIteratorMem( const smnIndexModule* )
{
    setIndexProperties();

    return IDE_SUCCESS;
}

IDE_RC smnbBTree::releaseIteratorMem(const smnIndexModule* )
{
    return IDE_SUCCESS;
}


IDE_RC smnbBTree::prepareFreeNodeMem( const smnIndexModule* )
{
    IDE_TEST( mSmnbNodePool.initialize( mNodeSize,
                                        SMNB_NODE_POOL_MAXIMUM,
                                        SMNB_NODE_POOL_CACHE )
              != IDE_SUCCESS );

    IDE_TEST( smnManager::allocFreeNodeList(
                                        (smnFreeNodeFunc)smnbBTree::freeNode,
                                        &mSmnbFreeNodeList)
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smnbBTree::releaseFreeNodeMem(const smnIndexModule* )
{
    smnManager::destroyFreeNodeList(mSmnbFreeNodeList);

    IDE_TEST( mSmnbNodePool.release() != IDE_SUCCESS );
    IDE_TEST( mSmnbNodePool.destroy() != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smnbBTree::freeAllNodeList(idvSQL         * /*aStatistics*/,
                                  smnIndexHeader * aIndex,
                                  void           * /*aTrans*/)
{
    /* BUG-37643 Node�� array�� ���������� �����ϴµ�
     * compiler ����ȭ�� ���ؼ� ���������� ���ŵ� �� �ִ�.
     * ���� �̷��� ������ volatile�� �����ؾ� �Ѵ�. */
    volatile smnbLNode  * sCurLNode;
    volatile smnbINode  * sFstChildINode;
    volatile smnbINode  * sCurINode;
    smnbHeader          * sHeader;
    smmSlot               sSlots = { &sSlots, &sSlots };
    smmSlot             * sFreeNode;
    SInt                  sSlotCount = 0;

    IDE_ASSERT( aIndex != NULL );

    sHeader = (smnbHeader*)(aIndex->mHeader);

    if ( sHeader != NULL )
    {
        sFstChildINode = (smnbINode*)(sHeader->root);

        while(sFstChildINode != NULL)
        {
            sCurINode = sFstChildINode;
            sFstChildINode = (smnbINode*)(sFstChildINode->mChildPtrs[0]);

            if ( (sCurINode->flag & SMNB_NODE_TYPE_MASK)== SMNB_NODE_TYPE_LEAF )
            {
                sCurLNode = (smnbLNode*)sCurINode;

                while(sCurLNode != NULL)
                {
                    sSlotCount++;

                    destroyNode( (smnbNode*)sCurLNode );

                    sFreeNode = (smmSlot*)sCurLNode;

                    sFreeNode->prev = sSlots.prev;
                    sFreeNode->next = &sSlots;

                    sSlots.prev->next = sFreeNode;
                    sSlots.prev = sFreeNode;

                    sCurLNode = sCurLNode->nextSPtr;
                }

                sFreeNode = sSlots.next;

                while(sFreeNode != &sSlots)
                {
                    sFreeNode = sFreeNode->next;
                }

                sSlots.next->prev = sSlots.prev;
                sSlots.prev->next = sSlots.next;

                IDE_TEST( mSmnbNodePool.releaseSlots( sSlotCount, sSlots.next )
                          != IDE_SUCCESS );

                break;
            }
            else
            {
                while( sCurINode != NULL )
                {
                    sSlotCount++;

                    destroyNode( (smnbNode*)sCurINode );

                    sFreeNode = (smmSlot*)sCurINode;

                    sFreeNode->prev = sSlots.prev;
                    sFreeNode->next = &sSlots;

                    sSlots.prev->next = sFreeNode;
                    sSlots.prev = sFreeNode;

                    sCurINode = sCurINode->nextSPtr;
                }
            }
        }

        sHeader->pFstNode = NULL;
        sHeader->pLstNode = NULL;
        sHeader->root     = NULL;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smnbBTree::freeNode( smnbNode* aNodes )
{
    smmSlot* sSlots;
    smmSlot* sSlot;
    UInt     sCount;

    if ( aNodes != NULL )
    {
        destroyNode( aNodes );

        sSlots       = (smmSlot*)aNodes;
        aNodes       = (smnbNode*)(aNodes->freeNodeList);
        sSlots->prev = sSlots;
        sSlots->next = sSlots;
        sCount       = 1;

        while( aNodes != NULL )
        {
            destroyNode( aNodes );

            sSlot             = (smmSlot*)aNodes;
            aNodes            = (smnbNode*)(aNodes->freeNodeList);
            sSlot->prev       = sSlots;
            sSlot->next       = sSlots->next;
            sSlots->next      = sSlot;
            sSlot->next->prev = sSlot;
            sCount++;
        }

        IDE_TEST( mSmnbNodePool.releaseSlots( sCount, sSlots )
                      != IDE_SUCCESS );
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smnbBTree::create( idvSQL               */*aStatistics*/,
                          smcTableHeader*       aTable,
                          smnIndexHeader*       aIndex,
                          smiSegAttr   *        /*aSegAttr*/,
                          smiSegStorageAttr*    /*aSegStoAttr */,
                          smnInsertFunc*        aInsert,
                          smnIndexHeader**      aRebuildIndexHeader,
                          ULong                 /*aSmoNo*/ )
{
    smnbHeader       * sHeader;
    smnbHeader       * sSrcIndexHeader;
    UInt               sStage = 0;
    UInt               sIndexCount;
    const smiColumn  * sColumn;
    smnbColumn       * sIndexColumn;
    smnbColumn       * sIndexColumn4Build;
    UInt               sAlignVal;
    UInt               sMaxAlignVal = 0;
    UInt               sOffset = 0;
    SChar              sBuffer[IDU_MUTEX_NAME_LEN];
    UInt               sNonStoringSize = 0;
    UInt               sCompareFlags   = 0;

    //TC/FIT/Limit/sm/smn/smnb/smnbBTree_create_malloc1.sql
    IDU_FIT_POINT_RAISE( "smnbBTree::create::malloc1",
                          ERR_INSUFFICIENT_MEMORY );

    // fix BUG-22898
    // �޸� b-tree Run Time Header ����
    IDE_TEST_RAISE( iduMemMgr::malloc( IDU_MEM_SM_SMN,
                                       ID_SIZEOF(smnbHeader),
                                       (void**)&sHeader ) != IDE_SUCCESS,
                    ERR_INSUFFICIENT_MEMORY );
    sStage = 1;

    // BUG-28856 logging ���� ���� (NATIVE -> POSIX)
    // TASK-4102���� POSIX�� �� ȿ�����ΰ����� �Ǵܵ�
    idlOS::snprintf(sBuffer, 
                    IDU_MUTEX_NAME_LEN, 
                    "SMNB_MUTEX_%"ID_UINT64_FMT,
                    (ULong)(aIndex->mId));
    IDE_TEST( sHeader->mTreeMutex.initialize(
                                sBuffer,
                                IDU_MUTEX_KIND_POSIX,
                                IDV_WAIT_INDEX_NULL )
                != IDE_SUCCESS );

    // fix BUG-23007
    // Index Node Pool �ʱ�ȭ
    IDE_TEST( sHeader->mNodePool.initialize( mNodeSize,
                                         SMM_SLOT_LIST_MAXIMUM_DEFAULT,
                                         SMM_SLOT_LIST_CACHE_DEFAULT,
                                         &mSmnbNodePool )
              != IDE_SUCCESS );
    sStage = 2;

    //TC/TC/Limit/sm/smn/smnb/smnbBTree_create_malloc2.sql
    IDU_FIT_POINT_RAISE( "smnbBTree::create::malloc2",
                          ERR_INSUFFICIENT_MEMORY );

    // fix BUG-22898
    // Index Header�� Columns ����
    IDE_TEST_RAISE( iduMemMgr::malloc( IDU_MEM_SM_SMN,
                                 ID_SIZEOF(smnbColumn) * (aIndex->mColumnCount),
                                 (void**)&(sHeader->columns) ) != IDE_SUCCESS,
                    ERR_INSUFFICIENT_MEMORY );

    //TC/FIT/Limit/sm/smn/smnb/smnbBTree_create_malloc3.sql
    IDU_FIT_POINT_RAISE( "smnbBTree::create::malloc3",
                          ERR_INSUFFICIENT_MEMORY );

    IDE_TEST_RAISE( iduMemMgr::malloc( IDU_MEM_SM_SMN,
                                 ID_SIZEOF(smnbColumn) * (aIndex->mColumnCount),
                                 (void**)&(sHeader->columns4Build) ) != IDE_SUCCESS,
                    ERR_INSUFFICIENT_MEMORY );

    sHeader->fence = sHeader->columns + aIndex->mColumnCount;
    sHeader->fence4Build = sHeader->columns4Build + aIndex->mColumnCount;

    sStage = 3;

    sHeader->mIsCreated    = ID_FALSE;
    sHeader->mIsConsistent = ID_TRUE;
    sHeader->root          = NULL;
    sHeader->pFstNode      = NULL;
    sHeader->pLstNode      = NULL;
    sHeader->nodeCount     = 0;
    sHeader->tempPtr       = NULL;
    sHeader->cRef          = 0;
    sHeader->mSpaceID      = aTable->mSpaceID; /* FIXED TABLE "X$MEM_BTREE_HEADER" ������ ����. */
    sHeader->mIsMemTBS     = smLayerCallback::isMemTableSpace( aTable->mSpaceID );

    // To fix BUG-17726
    if ( ( aIndex->mFlag & SMI_INDEX_TYPE_MASK ) == SMI_INDEX_TYPE_PRIMARY_KEY )
    {
        sHeader->mIsNotNull = ID_TRUE;
    }
    else
    {
        sHeader->mIsNotNull = ID_FALSE;
    }

    sHeader->mIndexHeader = aIndex;

    sOffset = 0;
    for( sIndexCount = 0; sIndexCount < aIndex->mColumnCount; sIndexCount++ )
    {
        sIndexColumn = &sHeader->columns[sIndexCount];
        sIndexColumn4Build = &sHeader->columns4Build[sIndexCount];

        IDE_TEST_RAISE( (aIndex->mColumns[sIndexCount] & SMI_COLUMN_ID_MASK )
                        >= aTable->mColumnCount,
                        ERR_COLUMN_NOT_FOUND );
        sColumn = smcTable::getColumn(aTable,
                                      ( aIndex->mColumns[sIndexCount] & SMI_COLUMN_ID_MASK));

        IDE_TEST_RAISE( sColumn->id != aIndex->mColumns[sIndexCount],
                        ERR_COLUMN_NOT_FOUND );

        idlOS::memcpy( &sIndexColumn->column, sColumn, ID_SIZEOF(smiColumn) );

        idlOS::memcpy( &sIndexColumn4Build->column, sColumn, ID_SIZEOF(smiColumn) );

        idlOS::memcpy( &sIndexColumn4Build->keyColumn,
                       &sIndexColumn4Build->column,
                       ID_SIZEOF(smiColumn) );

        if ( ( ( sIndexColumn4Build->keyColumn.flag & SMI_COLUMN_TYPE_MASK ) == SMI_COLUMN_TYPE_VARIABLE ) ||
             ( ( sIndexColumn4Build->keyColumn.flag & SMI_COLUMN_TYPE_MASK ) == SMI_COLUMN_TYPE_VARIABLE_LARGE ) )
        {
            sIndexColumn4Build->keyColumn.flag &= ~SMI_COLUMN_TYPE_MASK;
            sIndexColumn4Build->keyColumn.flag |= SMI_COLUMN_TYPE_FIXED;
        }

        IDE_TEST(gSmiGlobalCallBackList.getAlignValue(sColumn,
                                                      &sAlignVal)
                 != IDE_SUCCESS );

        sOffset = idlOS::align( sOffset, sAlignVal );

        sIndexColumn4Build->keyColumn.offset = sOffset;
        sOffset += sIndexColumn4Build->column.size;

        /* PROJ-2433
         * direct key index �ΰ��
         * ù��° �÷��� direct key compare�� �����Ѵ�. */
        if ( ( sIndexCount == 0 ) &&
             ( ( aIndex->mFlag & SMI_INDEX_DIRECTKEY_MASK ) == SMI_INDEX_DIRECTKEY_TRUE ) )
        {
            sCompareFlags = ( aIndex->mColumnFlags[sIndexCount] | SMI_COLUMN_COMPARE_DIRECT_KEY ) ;
        }
        else
        {
            sCompareFlags = aIndex->mColumnFlags[sIndexCount];
        }

        IDE_TEST( gSmiGlobalCallBackList.findCompare( sColumn,
                                                      sCompareFlags,
                                                      &sIndexColumn->compare )
                  != IDE_SUCCESS );

        /* PROJ-2433 
         * bottom-build�� ���Ǵ� compare �Լ���
         * direct key�� ������� �ʴ� compare �Լ��� �����Ѵ�. */
        IDE_TEST( gSmiGlobalCallBackList.findCompare( sColumn,
                                                      aIndex->mColumnFlags[sIndexCount],
                                                      &sIndexColumn4Build->compare )
                  != IDE_SUCCESS );

        IDE_TEST( gSmiGlobalCallBackList.findKey2String( sColumn,
                                                         aIndex->mColumnFlags[sIndexCount],
                                                         &sIndexColumn->key2String)
                 != IDE_SUCCESS );
        sIndexColumn4Build->key2String = sIndexColumn->key2String;

        IDE_TEST( gSmiGlobalCallBackList.findIsNull( sColumn,
                                                     aIndex->mColumnFlags[sIndexCount],
                                                     &sIndexColumn->isNull )
                  != IDE_SUCCESS );
        sIndexColumn4Build->isNull = sIndexColumn->isNull;

        IDE_TEST( gSmiGlobalCallBackList.findNull( sColumn,
                                                   aIndex->mColumnFlags[sIndexCount],
                                                   &sIndexColumn->null )
                  != IDE_SUCCESS );
        sIndexColumn4Build->null = sIndexColumn->null;

        /* BUG-24449
         * Ű�� ��� ũ��� Ÿ�Կ� ���� �ٸ���. ���� MT �Լ��� ���̸� ȹ���ϰ�,
         * ���̸� �����ϰ�, ���̸� �����ؾ� �Ѵ�.
         * ActualSize�Լ��� ���� ���̸� �˰�, makeMtdValue �Լ��� ���� MT Type��
         * �� �����Ѵ�.
         * �׸��� ��� ���̵� MT �Լ��κ��� ��´�
         */
        IDE_TEST( gSmiGlobalCallBackList.findActualSize(
                      sColumn,
                      &sIndexColumn->actualSize )
                  != IDE_SUCCESS );
        sIndexColumn4Build->actualSize = sIndexColumn->actualSize;

        /* PROJ-2429 Dictionary based data compress for on-disk DB
         * memory index �� min/max���� smiGetCompressionColumn�� �̿���
         * ���Ѵ��� �����Ѵ�. ���� dictionary compression�� �������
         * �ش� data type�� �Լ��� ����ؾ� �Ѵ�. */
        IDE_TEST( gSmiGlobalCallBackList.findCopyDiskColumnValue4DataType(
                      sColumn,
                      &sIndexColumn->makeMtdValue)
                  != IDE_SUCCESS );
        sIndexColumn4Build->makeMtdValue = sIndexColumn->makeMtdValue;
        
        IDE_TEST( gSmiGlobalCallBackList.getNonStoringSize( sColumn,
                                                            &sNonStoringSize )
                  != IDE_SUCCESS );
        sIndexColumn->headerSize       = sNonStoringSize;
        sIndexColumn4Build->headerSize = sIndexColumn->headerSize ;

        if ( sAlignVal > sMaxAlignVal )
        {
            sMaxAlignVal = sAlignVal;
        }

        if ( sIndexCount == 0 )
        {
            /* PROJ-2433
             * ���⼭ direct key index ���õ� ������ �����Ѵ�. */
            IDE_TEST_RAISE ( setDirectKeyIndex( sHeader,
                                                aIndex,
                                                sColumn,
                                                sAlignVal ) != IDE_SUCCESS,
                             ERR_TOO_SMALL_DIRECT_KEY_SIZE )
        }
        else
        {
            /* nothing to do */
        }
    }

    if ( sMaxAlignVal < 8 )
    {
        sHeader->mSlotAlignValue = 4;
    }
    else
    {
        sHeader->mSlotAlignValue = 8;
    }

    sHeader->mFixedKeySize = sOffset;

    /* index statistics */
    IDE_TEST( smnManager::initIndexStatistics( aIndex,
                                               (smnRuntimeHeader*)sHeader,
                                               aIndex->mId )
              != IDE_SUCCESS );

    aIndex->mHeader = (smnRuntimeHeader*)sHeader;

    if ( aRebuildIndexHeader != NULL )
    {
        if ( *aRebuildIndexHeader != NULL )
        {
            sHeader->tempPtr = *aRebuildIndexHeader;
            sSrcIndexHeader = (smnbHeader*)((*aRebuildIndexHeader)->mHeader);
            sSrcIndexHeader->cRef++;
        }
        else
        {
            *aRebuildIndexHeader = aIndex;
        }
    }
    else
    {
        sHeader->tempPtr = NULL;
    }
    
    /* PROJ-2334 PMT */
    if ( ( ( aIndex->mFlag & SMI_INDEX_UNIQUE_MASK ) == SMI_INDEX_UNIQUE_ENABLE ) ||
         ( ( aIndex->mFlag & SMI_INDEX_LOCAL_UNIQUE_MASK ) == SMI_INDEX_LOCAL_UNIQUE_ENABLE ) )
    {
        *aInsert = smnbBTree::insertRowUnique;
    }
    else
    {
        *aInsert = smnbBTree::insertRow;
    }

    if ( smuProperty::getDBMSStatMethod() == SMU_DBMS_STAT_METHOD_AUTO )
    {
        aIndex->mStat.mKeyCount = 
            ((smcTableHeader *)aTable)->mFixed.mMRDB.mRuntimeEntry->mInsRecCnt;
    }

    idlOS::memset( &(sHeader->mStmtStat), 0x00, ID_SIZEOF(smnbStatistic) );
    idlOS::memset( &(sHeader->mAgerStat), 0x00, ID_SIZEOF(smnbStatistic) );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_COLUMN_NOT_FOUND );
    {
        IDE_SET( ideSetErrorCode( smERR_FATAL_smnColumnNotFound ) );
    }
    IDE_EXCEPTION( ERR_INSUFFICIENT_MEMORY );
    {
        IDE_SET( ideSetErrorCode( idERR_ABORT_InsufficientMemory ) );
    }
    IDE_EXCEPTION( ERR_TOO_SMALL_DIRECT_KEY_SIZE );
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_TooSmallDirectKeySize ) );
    }
    IDE_EXCEPTION_END;

    IDE_PUSH();
    switch( sStage )
    {
            //fix BUG-22898
        case 3:
            (void) iduMemMgr::free( sHeader->columns );
            (void) iduMemMgr::free( sHeader->columns4Build );
        case 2:
            (void) sHeader->mNodePool.release();
            (void) sHeader->mNodePool.destroy();
        case 1:
            //fix BUG-22898
            (void) iduMemMgr::free( sHeader );
            break;
        default:
            break;
    }
    IDE_POP();

    aIndex->mHeader = NULL;

    return IDE_FAILURE;
}

IDE_RC smnbBTree::buildIndex( idvSQL*               aStatistics,
                              void*                 aTrans,
                              smcTableHeader*       aTable,
                              smnIndexHeader*       aIndex,
                              smnGetPageFunc        aGetPageFunc,
                              smnGetRowFunc         aGetRowFunc,
                              SChar*                aNullRow,
                              idBool                aIsNeedValidation,
                              idBool                aIsEnableTable,
                              UInt                  aParallelDegree,
                              UInt                /*aBuildFlag*/,
                              UInt                /*aTotalRecCnt*/ )
{
    smnIndexModule*     sIndexModules;
    UInt                sThreadCnt;
    UInt                sKeyValueSize;
    UInt                sKeySize;
    UInt                sRunSize;
    ULong               sMemoryIndexBuildValueThreshold;
    idBool              sBuildWithKeyValue = ID_TRUE;
    UInt                sPageCnt;

    IDE_ASSERT( aGetPageFunc != NULL );
    IDE_ASSERT( aGetRowFunc  != NULL );
    IDE_ASSERT( aNullRow     != NULL );

    sIndexModules = aIndex->mModule;

    sPageCnt = aTable->mFixed.mMRDB.mRuntimeEntry->mAllocPageList->mPageCount;

    if ( aParallelDegree == 0 )
    {
        sThreadCnt = smuProperty::getIndexBuildThreadCount();
    }
    else
    {
        sThreadCnt = aParallelDegree;
    }

    if ( sPageCnt < sThreadCnt )
    {
        sThreadCnt = sPageCnt;
    }

    if ( sThreadCnt == 0 )
    {
        sThreadCnt = 1;
    }

    sKeyValueSize = getMaxKeyValueSize( aIndex );

    // BUG-19249 : sKeySize = key value ũ�� + row ptr ũ��
    sKeySize      = getKeySize( sKeyValueSize );

    sMemoryIndexBuildValueThreshold =
        smuProperty::getMemoryIndexBuildValueLengthThreshold();

    sRunSize        = smuProperty::getMemoryIndexBuildRunSize();

    // PROJ-1629 : Memory Index Build
    // ������ row pointer�� �̿��ϰų� key value�� �̿��� index build��
    // ���������� ��밡��.
    // row pointer�� �̿��ϴ� ��� :
    // C-1. key value lenght�� property�� ���ǵ� ������ Ŭ ��
    // C-2. �Ѱ��� key size�� run�� ũ�⺸�� Ŭ ��
    // C-3. persistent index�� ��
    // BUG-19249 : C-2 �߰�
    if ( ( sKeyValueSize > sMemoryIndexBuildValueThreshold ) ||   // C-1
         ( sKeySize > (sRunSize - ID_SIZEOF(UShort)) )      ||   // C-2
         ( (aIndex->mFlag & SMI_INDEX_PERSISTENT_MASK) ==        // C-3
           SMI_INDEX_PERSISTENT_ENABLE ) )
    {
        sBuildWithKeyValue = ID_FALSE;
    }

    if ( sBuildWithKeyValue == ID_TRUE )
    {
        IDE_TEST( smnbValuebaseBuild::buildIndex( aTrans,
                                                  aStatistics,
                                                  aTable,
                                                  aIndex,
                                                  sThreadCnt,
                                                  aIsNeedValidation,
                                                  sKeySize,
                                                  sRunSize,
                                                  aGetPageFunc,
                                                  aGetRowFunc )
                  != IDE_SUCCESS);

        // BUG-19249
        ((smnbHeader*)aIndex->mHeader)->mBuiltType = 'V';
    }
    else
    {
        IDE_TEST( smnbPointerbaseBuild::buildIndex( aTrans,
                                                    aStatistics,
                                                    aTable,
                                                    aIndex,
                                                    sThreadCnt,
                                                    aIsEnableTable,
                                                    aIsNeedValidation,
                                                    aGetPageFunc,
                                                    aGetRowFunc,
                                                    aNullRow )
                  != IDE_SUCCESS);

        // BUG-19249
        ((smnbHeader*)aIndex->mHeader)->mBuiltType = 'P';
    }

    IDE_TEST_RAISE( sIndexModules == NULL, ERR_NOT_FOUND );

    (void)smnManager::setIndexCreated( aIndex, ID_TRUE );

    return IDE_SUCCESS;

    /* �ش� type�� �ε����� ã�� �� �����ϴ�. */
    IDE_EXCEPTION( ERR_NOT_FOUND );
    IDE_SET( ideSetErrorCode( smERR_FATAL_smnNotSupportedIndex ) );

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smnbBTree::drop( smnIndexHeader * aIndex )
{
    /* BUG-37643 Node�� array�� ���������� �����ϴµ�
     * compiler ����ȭ�� ���ؼ� ���������� ���ŵ� �� �ִ�.
     * ���� �̷��� ������ volatile�� �����ؾ� �Ѵ�. */
    volatile smnbLNode  *s_pCurLNode;
    volatile smnbINode  *s_pFstChildINode;
    volatile smnbINode  *s_pCurINode;
    smnbHeader          *s_pHeader;
    smmSlot              s_slots = { &s_slots, &s_slots };
    smmSlot             *s_pFreeNode;
    SInt                 s_slotCount = 0;
    SInt                 s_cSlot;

    s_pHeader = (smnbHeader*)(aIndex->mHeader);

    if ( s_pHeader != NULL )
    {
        s_pFstChildINode = (smnbINode*)(s_pHeader->root);

        while(s_pFstChildINode != NULL)
        {
            s_pCurINode = s_pFstChildINode;
            s_pFstChildINode = (smnbINode*)(s_pFstChildINode->mChildPtrs[0]);

            if ( (s_pCurINode->flag & SMNB_NODE_TYPE_MASK)== SMNB_NODE_TYPE_LEAF )
            {
                s_pCurLNode = (smnbLNode*)s_pCurINode;

                while(s_pCurLNode != NULL)
                {
                    s_slotCount++;
                    
                    destroyNode( (smnbNode*)s_pCurLNode );

                    s_pFreeNode = (smmSlot*)s_pCurLNode;

                    s_pFreeNode->prev = s_slots.prev;
                    s_pFreeNode->next = &s_slots;

                    s_slots.prev->next = s_pFreeNode;
                    s_slots.prev = s_pFreeNode;

                    s_pCurLNode = s_pCurLNode->nextSPtr;
                }

                s_pFreeNode = s_slots.next;
                s_cSlot = s_slotCount;

                while(s_pFreeNode != &s_slots)
                {
                    s_pFreeNode = s_pFreeNode->next;
                    s_cSlot--;
                }

                s_slots.next->prev = s_slots.prev;
                s_slots.prev->next = s_slots.next;

                IDE_TEST( s_pHeader->mNodePool.releaseSlots(
                                                s_slotCount,
                                                s_slots.next,
                                                SMM_SLOT_LIST_MUTEX_NEEDLESS )
                          != IDE_SUCCESS );

                break;
            }
            else
            {
                while(s_pCurINode != NULL)
                {
                    s_slotCount++;

                    destroyNode( (smnbNode*)s_pCurINode );

                    s_pFreeNode = (smmSlot*)s_pCurINode;

                    s_pFreeNode->prev = s_slots.prev;
                    s_pFreeNode->next = &s_slots;

                    s_slots.prev->next = s_pFreeNode;
                    s_slots.prev = s_pFreeNode;

                    s_pCurINode = s_pCurINode->nextSPtr;
                }
            }
        }

        IDE_TEST( s_pHeader->mNodePool.release() != IDE_SUCCESS );
        IDE_TEST( s_pHeader->mNodePool.destroy() != IDE_SUCCESS );

        IDE_TEST( s_pHeader->mTreeMutex.destroy() != IDE_SUCCESS );

        IDE_TEST( smnManager::destIndexStatistics( 
                        aIndex,
                        (smnRuntimeHeader*)s_pHeader )
                  != IDE_SUCCESS );

        IDE_TEST( iduMemMgr::free( s_pHeader->columns) != IDE_SUCCESS );
        IDE_TEST( iduMemMgr::free( s_pHeader->columns4Build ) != IDE_SUCCESS );

        IDE_TEST( iduMemMgr::free( s_pHeader ) != IDE_SUCCESS );

        aIndex->mHeader = NULL;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smnbBTree::init( idvSQL *              /* aStatistics */,
                        smnbIterator *        aIterator,
                        void *                aTrans,
                        smcTableHeader *      aTable,
                        smnIndexHeader *      aIndex,
                        void*                 /* aDumpObject */,
                        const smiRange *      aKeyRange,
                        const smiRange *      /* aKeyFilter */,
                        const smiCallBack *   aRowFilter,
                        UInt                  aFlag,
                        smSCN                 aSCN,
                        smSCN                 aInfinite,
                        idBool                aUntouchable,
                        smiCursorProperties * aProperties,
                        const smSeekFunc **  aSeekFunc )
{
    idvSQL                    *sSQLStat;

    aIterator->SCN             = aSCN;
    aIterator->infinite        = aInfinite;
    aIterator->trans           = aTrans;
    aIterator->table           = aTable;
    aIterator->curRecPtr       = NULL;
    aIterator->lstFetchRecPtr  = NULL;
    aIterator->lstReturnRecPtr = NULL;
    SC_MAKE_NULL_GRID( aIterator->mRowGRID );
    aIterator->tid             = smLayerCallback::getTransID( aTrans );
    aIterator->flag            = ( aUntouchable == ID_TRUE ) ? SMI_ITERATOR_READ : SMI_ITERATOR_WRITE;
    aIterator->mProperties     = aProperties;
    aIterator->index           = (smnbHeader*)aIndex->mHeader;
    aIterator->mKeyRange       = aKeyRange;
    aIterator->mRowFilter      = aRowFilter;

    IDL_MEM_BARRIER;

    *aSeekFunc = smnbSeekFunctions[aFlag&(SMI_TRAVERSE_MASK|
                                          SMI_PREVIOUS_MASK|
                                          SMI_LOCK_MASK)];

    sSQLStat = aIterator->mProperties->mStatistics;
    IDV_SQL_ADD(sSQLStat, mMemCursorIndexScan, 1);

    if ( sSQLStat != NULL )
    {
        IDV_SESS_ADD(sSQLStat->mSess, IDV_STAT_INDEX_MEM_CURSOR_IDX_SCAN, 1);
    }

    return IDE_SUCCESS;
}

IDE_RC smnbBTree::dest( smnbIterator * /*aIterator*/ )
{
    return IDE_SUCCESS;
}

IDE_RC smnbBTree::isRowUnique( void*              aTrans,
                               smnbStatistic*     aIndexStat,
                               smSCN              aStmtSCN,
                               void*              aRow,
                               smTID*             aTid,
                               SChar**            aExistUniqueRow )
{
    smSCN sCreateSCN;
    smSCN sRowSCN;
    smSCN sNullSCN;
    smTID sRowTID;
    smTID sMyTID;

    smSCN sLimitSCN;
    smSCN sNxtSCN;
    smTID sNxtTID;

    /* aTrans�� NULL�̸� uniqueness�� �˻����� �ʴ´�. */
    IDE_TEST_CONT( aTrans == NULL, SKIP_UNIQUE_CHECK );

    sMyTID = smLayerCallback::getTransID( aTrans );

    SM_SET_SCN( &sCreateSCN, &(((smpSlotHeader*)aRow)->mCreateSCN) );
    SM_SET_SCN( &sLimitSCN,  &(((smpSlotHeader*)aRow)->mLimitSCN)  );

    SMX_GET_SCN_AND_TID( sCreateSCN, sRowSCN, sRowTID );
    SMX_GET_SCN_AND_TID( sLimitSCN,  sNxtSCN, sNxtTID );

    aIndexStat->mKeyValidationCount++;

    /* BUG-32655
     * SCN�� DeleteBit�� ������ ���. �� ���� Rollback������
     * �ƿ� ��ȿ���� ���� Row�� �� ����̱⿡
     * �����ؾ� �ȴ�. */
    IDE_TEST_CONT( SM_SCN_IS_DELETED( sRowSCN ) , SKIP_UNIQUE_CHECK );

    if ( SM_SCN_IS_INFINITE( sRowSCN ) )
    {
        if ( sRowTID != sMyTID )
        {
            *aTid = sRowTID;
            IDE_ERROR_RAISE( *aTid != 0, ERR_CORRUPTED_INDEX );
            IDE_ERROR_RAISE( *aTid != SM_NULL_TID, ERR_CORRUPTED_INDEX );
        }
        else
        {
            IDE_TEST_RAISE( SM_SCN_IS_FREE_ROW( sNxtSCN ),
                            ERR_UNIQUE_VIOLATION );
        }
    }
    else
    {
        /* BUG-14953: PK�� �ΰ� �Ѿ: ���⼭ Next OID�� �ٸ�
           Transaction�а� �ִ� �߿� Update�� �ɼ� �ֱ� ������ ����
           �����ؼ� Test�ؾ��Ѵ�.*/

        IDE_TEST_RAISE( SM_SCN_IS_FREE_ROW( sNxtSCN ),
                        ERR_UNIQUE_VIOLATION );

        if ( SM_SCN_IS_LOCK_ROW( sNxtSCN ) )
        {
            //�ڱ� �ڽ��� �̹� lock�� ��� �ִ� row��� unique����
            IDE_TEST_RAISE( sNxtTID == sMyTID, ERR_UNIQUE_VIOLATION );

            //���� row�� ��� �ִٸ� �� Ʈ������� ������ ��ٸ���.
            *aTid = sNxtTID;
            IDE_ERROR_RAISE( *aTid != 0, ERR_CORRUPTED_INDEX );
            IDE_ERROR_RAISE( *aTid != SM_NULL_TID, ERR_CORRUPTED_INDEX );
        }
        else
        {
            if ( SM_SCN_IS_INFINITE( sNxtSCN ) )
            {
                if ( sNxtTID != sMyTID )
                {
                    *aTid = sNxtTID;
                    IDE_ERROR_RAISE( *aTid != 0, ERR_CORRUPTED_INDEX );
                    IDE_ERROR_RAISE( *aTid != SM_NULL_TID, ERR_CORRUPTED_INDEX );
                }
            }

            SM_INIT_SCN( &sNullSCN );

            if ( SM_SCN_IS_NOT_INFINITE( sNxtSCN ) &&
                 ( !SM_SCN_IS_EQ( &aStmtSCN, &sNullSCN ) ) &&
                 SM_SCN_IS_LT( &aStmtSCN, &sNxtSCN ) )
            {
                // BUG-15097
                // unique check�� ���� �� row�� commit scn��
                // ���� transaction�� begin scn���� ũ��
                // ���� transaction�� �� row�� �� �� ����.
                // �̶� retry error�� �߻����Ѿ� �Ѵ�.
                IDE_RAISE( ERR_ALREADY_MODIFIED );
            }
        }
    }

    IDE_EXCEPTION_CONT( SKIP_UNIQUE_CHECK );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_UNIQUE_VIOLATION );
    IDE_SET( ideSetErrorCode( smERR_ABORT_smnUniqueViolation ) );

    // PROJ-2264
    if ( aExistUniqueRow != NULL )
    {
        *aExistUniqueRow = (SChar*)aRow;
    }

    IDE_EXCEPTION( ERR_ALREADY_MODIFIED );
    IDE_SET( ideSetErrorCode( smERR_RETRY_Already_Modified ) );

    IDE_EXCEPTION( ERR_CORRUPTED_INDEX )
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_INCONSISTENT_INDEX ) );

        ideLog::log( IDE_SM_0, "Index Corrupted Detected : Wrong RID / SCN" );
    }

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*
   TASK-6743 
   ���� compareRows() �Լ��� ȣ��Ƚ���� ���Ƽ�,
   �뵵�� �°� ����ϵ��� 3���� �Լ��� ����.

    compareRows()    : ���� ��.
    compareRowsAndPtr()  : ���� �����ϸ� �����ͷ� ��.
    compareRowsAndPtr2() : compareRowsAndPtr()�� �����ϳ�, ���� ���������� �Ű������� �˷���.
 */
SInt smnbBTree::compareRows( smnbStatistic    * aIndexStat,
                             const smnbColumn * aColumns,
                             const smnbColumn * aFence,
                             const void       * aRow1,
                             const void       * aRow2 )
{
    SInt          sResult;
    smiColumn   * sColumn;
    smiValueInfo  sValueInfo1;
    smiValueInfo  sValueInfo2;

    if ( aRow1 == NULL )
    {
        if ( aRow2 == NULL )
        {
            return 0;
        }

        return 1;
    }
    else
    {
        if ( aRow2 == NULL )
        {
            return -1;
        }
    }

    SMI_SET_VALUEINFO(
        &sValueInfo1, NULL, aRow1, 0, SMI_OFFSET_USE,
        &sValueInfo2, NULL, aRow2, 0, SMI_OFFSET_USE );

    for ( ; aColumns < aFence; aColumns++ )
    {
        aIndexStat->mKeyCompareCount++;
        sColumn = (smiColumn*)&(aColumns->column);

        if ( ( aColumns->column.flag & SMI_COLUMN_TYPE_MASK ) ==
               SMI_COLUMN_TYPE_FIXED )
        {
            sValueInfo1.column = sColumn;
            sValueInfo2.column = sColumn;

            sResult = aColumns->compare( &sValueInfo1,
                                         &sValueInfo2 );
        }
        else
        {
            sResult = compareVarColumn( aColumns,
                                        aRow1,
                                        aRow2 );
        }

        if ( sResult != 0 )
        {
            return sResult;
        }
    }

    return 0;
}

SInt smnbBTree::compareRowsAndPtr( smnbStatistic    * aIndexStat,
                                   const smnbColumn * aColumns,
                                   const smnbColumn * aFence,
                                   const void       * aRow1,
                                   const void       * aRow2 )
{
    SInt          sResult;
    smiColumn   * sColumn;
    smiValueInfo  sValueInfo1;
    smiValueInfo  sValueInfo2;

    if ( aRow1 == NULL )
    {
        if ( aRow2 == NULL )
        {
            return 0;
        }

        return 1;
    }
    else
    {
        if ( aRow2 == NULL )
        {
            return -1;
        }
    }

    SMI_SET_VALUEINFO(
        &sValueInfo1, NULL, aRow1, 0, SMI_OFFSET_USE,
        &sValueInfo2, NULL, aRow2, 0, SMI_OFFSET_USE );

    for ( ; aColumns < aFence; aColumns++ )
    {
        aIndexStat->mKeyCompareCount++;
        sColumn = (smiColumn*)&(aColumns->column);

        if ( ( aColumns->column.flag & SMI_COLUMN_TYPE_MASK ) ==
             SMI_COLUMN_TYPE_FIXED )
        {
            sValueInfo1.column = sColumn;
            sValueInfo2.column = sColumn;

            sResult = aColumns->compare( &sValueInfo1,
                                         &sValueInfo2 );
        }
        else
        {
            sResult = compareVarColumn( (smnbColumn*)aColumns,
                                        aRow1,
                                        aRow2 );
        }

        if ( sResult != 0 )
        {
            return sResult;
        }
    }

    /* BUG-39043 Ű ���� ���� �� ��� pointer ��ġ�� ������ ���ϵ��� �� ���
     * ������ ���� �ø� ��� ��ġ�� ����Ǳ� ������ persistent index�� ����ϴ� ���
     * ����� index�� ���� ������ ���� �ʾ� ������ ���� �� �ִ�.
     * �̸� �ذ��ϱ� ���� Ű ���� ���� �� ��� OID�� ������ ���ϵ��� �Ѵ�. */
    if( SMP_SLOT_GET_OID( aRow1 ) > SMP_SLOT_GET_OID( aRow2 ) )
    {
        return 1;
    }
    if( SMP_SLOT_GET_OID( aRow1 ) < SMP_SLOT_GET_OID( aRow2 ) )
    {
        return -1;
    }

    return 0;
}

SInt smnbBTree::compareRowsAndPtr2( smnbStatistic    * aIndexStat,
                                    const smnbColumn * aColumns,
                                    const smnbColumn * aFence,
                                    SChar            * aRow1,
                                    SChar            * aRow2,
                                    idBool           * aIsEqual )
{
    SInt          sResult;
    smiColumn   * sColumn;
    smiValueInfo  sValueInfo1;
    smiValueInfo  sValueInfo2;

    *aIsEqual = ID_FALSE;

    if(aRow1 == NULL)
    {
        if(aRow2 == NULL)
        {
            *aIsEqual = ID_TRUE;

            return 0;
        }

        return 1;
    }
    else
    {
        if(aRow2 == NULL)
        {
            return -1;
        }
    }

    SMI_SET_VALUEINFO(
        &sValueInfo1, NULL, aRow1, 0, SMI_OFFSET_USE,
        &sValueInfo2, NULL, aRow2, 0, SMI_OFFSET_USE );

    for( ; aColumns < aFence; aColumns++ )
    {
        aIndexStat->mKeyCompareCount++;
        sColumn = (smiColumn*)&(aColumns->column);

        if( (aColumns->column.flag & SMI_COLUMN_TYPE_MASK) ==
            SMI_COLUMN_TYPE_FIXED )
        {
            sValueInfo1.column = sColumn;
            sValueInfo2.column = sColumn;

            sResult = aColumns->compare( &sValueInfo1,
                                         &sValueInfo2 );
        }
        else
        {
            sResult = compareVarColumn( (smnbColumn*)aColumns,
                                        aRow1,
                                        aRow2 );
        }

        if( sResult != 0 )
        {
            return sResult;
        }
    }

    *aIsEqual = ID_TRUE;

    /* BUG-39043 Ű ���� ���� �� ��� pointer ��ġ�� ������ ���ϵ��� �� ���
     * ������ ���� �ø� ��� ��ġ�� ����Ǳ� ������ persistent index�� ����ϴ� ���
     * ����� index�� ���� ������ ���� �ʾ� ������ ���� �� �ִ�.
     * �̸� �ذ��ϱ� ���� Ű ���� ���� �� ��� OID�� ������ ���ϵ��� �Ѵ�. */
    if( SMP_SLOT_GET_OID( aRow1 ) > SMP_SLOT_GET_OID( aRow2 ) )
    {
        return 1;
    }
    if( SMP_SLOT_GET_OID( aRow1 ) < SMP_SLOT_GET_OID( aRow2 ) )
    {
        return -1;
    }

    return 0;
}

/*********************************************************************
 * FUNCTION DESCRIPTION : smnbBTree::compareKeys                     *
 * ------------------------------------------------------------------*
 * PROJ-2433 Direct Key Index
 * row�� direct key�� ���ϴ� �Լ�.
 *
 *  - row�� row�� ���ϴ� compareRowsAndPtr() �Լ��� ������
 *  - direct key�� �����ϴ� slot�� ������� Ȯ���ϱ����� aRow1�� �ʿ���.
 *
 * aIndexStat      - [IN]  �������
 * aColumns        - [IN]  �÷�����
 * aFence          - [IN]  �÷����� Fence (������ �÷����� ��ġ)
 * aKey1           - [IN]  direct key pointer
 * aRow1           - [IN]  direct key�� ���� slot�� row pointer
 * aPartialKeySize - [IN]  direct key�� partial key�� ����� ����
 * aRow2           - [IN]  ���� row pointer
 *********************************************************************/
SInt smnbBTree::compareKeys( smnbStatistic      * aIndexStat,
                             const smnbColumn   * aColumns,
                             const smnbColumn   * aFence,
                             void               * aKey1,
                             SChar              * aRow1,
                             UInt                 aPartialKeySize,
                             SChar              * aRow2,
                             idBool             * aIsSuccess )
{
    SInt          sResult = 0;
    smiColumn     sDummyColumn;
    smiColumn   * sColumn = NULL;
    smiValueInfo  sValueInfo1;
    smiValueInfo  sValueInfo2;
    UInt          sFlag = 0;

    if ( aRow1 == NULL )
    {
        if ( aRow2 == NULL )
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        if ( aRow2 == NULL )
        {
            return -1;
        }
        else
        {
            /* nothing to do */
        }
    }

    IDE_ERROR_RAISE( aColumns < aFence, ERR_CORRUPTED_INDEX );

    aIndexStat->mKeyCompareCount++;

    if ( ( aColumns->column.flag & SMI_COLUMN_TYPE_MASK ) ==
         SMI_COLUMN_TYPE_FIXED )
    {
        sDummyColumn.offset = 0;
        sDummyColumn.flag   = 0;

        sColumn = (smiColumn*)&(aColumns->column);

        sFlag = SMI_OFFSET_USELESS;

        if ( aPartialKeySize != 0 )
        {
            sFlag &= ~SMI_PARTIAL_KEY_MASK;
            sFlag |= SMI_PARTIAL_KEY_ON;
        }
        else
        {
            sFlag &= ~SMI_PARTIAL_KEY_MASK;
            sFlag |= SMI_PARTIAL_KEY_OFF;
        }

        SMI_SET_VALUEINFO( &sValueInfo1,
                           &sDummyColumn,
                           aKey1,
                           aPartialKeySize,
                           sFlag,
                           &sValueInfo2,
                           sColumn,
                           aRow2,
                           0,
                           SMI_OFFSET_USE );

        sResult = aColumns->compare( &sValueInfo1,
                                     &sValueInfo2 );
    }
    else
    {
        sResult = compareKeyVarColumn( (smnbColumn*)aColumns,
                                       aKey1,
                                       aPartialKeySize,
                                       aRow2,
                                       aIsSuccess );
        IDE_TEST( *aIsSuccess != ID_TRUE );
    }

    *aIsSuccess = ID_TRUE;

    return sResult;

    IDE_EXCEPTION( ERR_CORRUPTED_INDEX )
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_INCONSISTENT_INDEX ) );

        ideLog::log( IDE_SM_0, "Index Corrupted Detected : Wrong Index Order" );
    }
    IDE_EXCEPTION_END;

    *aIsSuccess = ID_FALSE;

    return sResult;
}

/*********************************************************************
 * FUNCTION DESCRIPTION : smnbBTree::compareKeyVarColumn             *
 * ------------------------------------------------------------------*
 * PROJ-2433 Direct Key Index
 * variable row�� direct key�� ���ϴ� �Լ�.
 *
 *  - variable row�� row�� ���ϴ� compareVarColumn() �Լ��� ������
 *
 * aColumns        - [IN]  �÷�����
 * aKey            - [IN]  direct key pointer
 * aPartialKeySize - [IN]  direct key�� partial key�� ����� ����
 * aRow            - [IN]  ���� row pointer
 *********************************************************************/
SInt smnbBTree::compareKeyVarColumn( smnbColumn * aColumn,
                                     void       * aKey,
                                     UInt         aPartialKeySize,
                                     SChar      * aRow,
                                     idBool     * aIsSuccess )
{
    SInt              sResult = 0;
    smiColumn         sDummyColumn;
    smiColumn         sColumn;
    smVCDesc        * sColumnVCDesc = NULL;
    ULong             sInPageBuffer = 0;
    smiValueInfo      sValueInfo1;
    smiValueInfo      sValueInfo2;
    UInt              sFlag = 0;

    sColumn = aColumn->column;

    // BUG-37533
    if ( ( sColumn.flag & SMI_COLUMN_COMPRESSION_MASK )
         == SMI_COLUMN_COMPRESSION_FALSE )
    {
        if ( (sColumn.flag & SMI_COLUMN_TYPE_MASK) == SMI_COLUMN_TYPE_VARIABLE_LARGE )
        {
            // BUG-30068 : malloc ���н� ������ �����մϴ�.
            sColumnVCDesc = (smVCDesc*)(aRow + sColumn.offset);
            IDE_ERROR_RAISE( sColumnVCDesc->length <= SMP_VC_PIECE_MAX_SIZE, ERR_CORRUPTED_INDEX );
        }
        else
        {
            /* Nothing to do */
        }
    }
    else
    {
        // compressed column�̸� smVCDesc�� length�� �˻����� �ʴ´�.
    }

    sDummyColumn.offset = 0;
    sDummyColumn.flag   = 0;

    sColumn.value = (void*)&sInPageBuffer;
    *(ULong *)(sColumn.value) = 0;

    sFlag = SMI_OFFSET_USELESS;

    if ( aPartialKeySize != 0 )
    {
        sFlag &= ~SMI_PARTIAL_KEY_MASK;
        sFlag |= SMI_PARTIAL_KEY_ON;
    }
    else
    {
        sFlag &= ~SMI_PARTIAL_KEY_MASK;
        sFlag |= SMI_PARTIAL_KEY_OFF;
    }

    SMI_SET_VALUEINFO( &sValueInfo1,
                       (const smiColumn*)&sDummyColumn,
                       aKey,
                       aPartialKeySize,
                       sFlag,
                       &sValueInfo2,
                       (const smiColumn*)&sColumn,
                       aRow,
                       0,
                       SMI_OFFSET_USE );

    sResult = aColumn->compare( &sValueInfo1,
                                &sValueInfo2 );
    *aIsSuccess = ID_TRUE;

    return sResult;

    IDE_EXCEPTION( ERR_CORRUPTED_INDEX )
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_INCONSISTENT_INDEX ) );

        ideLog::log( IDE_SM_0, "Index Corrupted Detected : Wrong VC Piece Size" );
    }
    IDE_EXCEPTION_END;    

    *aIsSuccess = ID_FALSE;

    return sResult;
}

/********************************************************
 * To fix BUG-21235
 * OUT-VARCHAR�� ���� Compare Function
 ********************************************************/
SInt smnbBTree::compareVarColumn( const smnbColumn * aColumn,
                                  const void       * aRow1,
                                  const void       * aRow2 )
{
    SInt        sResult;
    smiColumn   sColumn1;
    smiColumn   sColumn2;
    smVCDesc  * sColumnVCDesc1;
    smVCDesc  * sColumnVCDesc2;
    ULong       sInPageBuffer1;
    ULong       sInPageBuffer2;
    smiValueInfo  sValueInfo1;
    smiValueInfo  sValueInfo2;

    sColumn1 = aColumn->column;
    sColumn2 = aColumn->column;

    // BUG-37533
    if ( ( sColumn1.flag & SMI_COLUMN_COMPRESSION_MASK )
         == SMI_COLUMN_COMPRESSION_FALSE )
    {
        if ( ( sColumn1.flag & SMI_COLUMN_TYPE_MASK ) == SMI_COLUMN_TYPE_VARIABLE_LARGE )
        {
            // BUG-30068 : malloc ���н� ������ �����մϴ�.
            sColumnVCDesc1 = (smVCDesc*)((SChar *)aRow1 + sColumn1.offset);
            IDE_ASSERT( sColumnVCDesc1->length <= SMP_VC_PIECE_MAX_SIZE );

            sColumnVCDesc2 = (smVCDesc*)((SChar *)aRow2 + sColumn2.offset);
            IDE_ASSERT( sColumnVCDesc2->length <= SMP_VC_PIECE_MAX_SIZE );
        }
        else
        {
            /* Nothing to do */
        }
    }
    else
    {
        // compressed column�̸� smVCDesc�� length�� �˻����� �ʴ´�.
    }

    sColumn1.value = (void*)&sInPageBuffer1;
    *(ULong*)(sColumn1.value) = 0;
    sColumn2.value = (void*)&sInPageBuffer2;
    *(ULong*)(sColumn2.value) = 0;

    SMI_SET_VALUEINFO( &sValueInfo1, (const smiColumn*)&sColumn1, aRow1, 0, SMI_OFFSET_USE,
                       &sValueInfo2, (const smiColumn*)&sColumn2, aRow2, 0, SMI_OFFSET_USE );

    sResult = aColumn->compare( &sValueInfo1,
                                &sValueInfo2 );

    return sResult;
}


idBool smnbBTree::isNullColumn( smnbColumn * aColumn,
                                smiColumn  * aSmiColumn,
                                SChar      * aRow )
{
    idBool  sResult;
    UInt    sLength = 0;
    SChar * sKeyPtr = NULL;

    if ( ( aSmiColumn->flag & SMI_COLUMN_COMPRESSION_MASK )
           != SMI_COLUMN_COMPRESSION_TRUE )
    {
        if ( (aSmiColumn->flag & SMI_COLUMN_TYPE_MASK) ==
              SMI_COLUMN_TYPE_FIXED )
        {
            sResult = aColumn->isNull( NULL,
                                       aRow + aSmiColumn->offset );
        }
        else
        {
            sResult = isVarNull( aColumn, aSmiColumn, aRow );
        }
    }
    else
    {
        // BUG-37464 
        // compressed column�� ���ؼ� index ������ null �˻縦 �ùٷ� ���� ����
        sKeyPtr = (SChar*)smiGetCompressionColumn( aRow,
                                                   aSmiColumn,
                                                   ID_TRUE, // aUseColumnOffset
                                                   &sLength );

        if ( sKeyPtr == NULL )
        {
            sResult = ID_TRUE;
        }
        else
        {
            sResult = aColumn->isNull( NULL, sKeyPtr );
        }
    }

    return sResult;
}

/********************************************************
 * To fix BUG-21235
 * OUT-VARCHAR�� ���� Is Null Function
 ********************************************************/
idBool smnbBTree::isVarNull( smnbColumn * aColumn,
                             smiColumn  * aSmiColumn,
                             SChar      * aRow )
{
    idBool      sResult;
    smiColumn   sColumn;
    smVCDesc  * sColumnVCDesc;
    UChar     * sOutPageBuffer = NULL;
    ULong       sInPageBuffer;
    UChar     * sOrgValue;
    SChar     * sValue;
    UInt        sLength = 0;

    sColumn = *aSmiColumn;

    sOrgValue = (UChar*)sColumn.value;
    if ( ( sColumn.flag & SMI_COLUMN_TYPE_MASK ) == SMI_COLUMN_TYPE_VARIABLE_LARGE )
    {
        sColumnVCDesc = (smVCDesc*)(aRow + sColumn.offset);
        if ( sColumnVCDesc->length > SMP_VC_PIECE_MAX_SIZE )
        {
            IDE_ASSERT( iduMemMgr::malloc( IDU_MEM_SM_SMN,
                        sColumnVCDesc->length + ID_SIZEOF( ULong ),
                        (void**)&sOutPageBuffer ) == IDE_SUCCESS );
            sColumn.value = sOutPageBuffer;
        }
        else
        {
            sColumn.value = (void*)&sInPageBuffer;
        }
        *(ULong*)(sColumn.value) = 0;
    }
    else
    {
        sColumn.value = (void*)&sInPageBuffer;
    }
    *(ULong*)(sColumn.value) = 0;

    sValue = sgmManager::getVarColumn(aRow, &sColumn, &sLength);

    if ( sValue == NULL )
    {
        sResult = ID_TRUE;
    }
    else
    {
        sResult = aColumn->isNull( &sColumn, sValue );
    }

    sColumn.value = sOrgValue;
    if ( sOutPageBuffer != NULL )
    {
        IDE_ASSERT( iduMemMgr::free( sOutPageBuffer ) == IDE_SUCCESS );
    }

    return sResult;
}

/* BUG-30074 disk table�� unique index���� NULL key ���� ��/
 *           non-unique index���� deleted key �߰� ��
 *           Cardinality�� ��Ȯ���� ���� �������ϴ�.
 * Key ��ü�� Null���� Ȯ���Ѵ�. ��� Null�̾�� �Ѵ�. */

idBool smnbBTree::isNullKey( smnbHeader * aIndex,
                             SChar      * aRow )
{

    smnbColumn * sColumn;
    idBool       sIsNullKey   = ID_TRUE;

    for( sColumn = &aIndex->columns[0];
         sColumn < aIndex->fence;
         sColumn++)
    {
        if ( smnbBTree::isNullColumn( sColumn,
                                      &(sColumn->column),
                                      aRow ) == ID_FALSE )
        {
            sIsNullKey = ID_FALSE;
            break;
        }
    }

    return sIsNullKey;
}

UInt smnbBTree::getMaxKeyValueSize( smnIndexHeader *aIndexHeader )
{
    UInt             sTotalSize;

    IDE_ASSERT( aIndexHeader != NULL );

    sTotalSize = idlOS::align8(((smnbHeader*)aIndexHeader->mHeader)->mFixedKeySize);

    return sTotalSize;
}

IDE_RC smnbBTree::updateStat4Insert( smnIndexHeader   * aPersIndex,
                                     smnbHeader       * aIndex,
                                     smnbLNode        * aLeafNode,
                                     SInt               aSlotPos,
                                     SChar            * aRow,
                                     idBool             aIsUniqueIndex )
{
    smnbLNode        * sPrevNode = NULL;
    smnbLNode        * sNextNode = NULL;
    SChar            * sPrevRowPtr;
    SChar            * sNextRowPtr;
    IDU_LATCH          sVersion;
    smnbColumn       * sColumn;
    SLong            * sNumDistPtr;
    SInt               sCompareResult;

    // get next slot
    if ( aSlotPos == ( aLeafNode->mSlotCount - 1 ) )
    {
        if ( aLeafNode->nextSPtr == NULL )
        {
            sNextRowPtr = NULL;
        }
        else
        {
            while(1)
            {
                sNextNode = (smnbLNode*)aLeafNode->nextSPtr;
                sVersion  = getLatchValueOfLNode( sNextNode ) & IDU_LATCH_UNMASK;

                IDL_MEM_BARRIER;

                sNextRowPtr = sNextNode->mRowPtrs[0];

                IDL_MEM_BARRIER;

                if ( sVersion == getLatchValueOfLNode(sNextNode) )
                {
                    break;
                }
            }
        }
    }
    else
    {
        sNextRowPtr = aLeafNode->mRowPtrs[aSlotPos + 1];
    }
    // get prev slot
    if ( aSlotPos == 0 )
    {
        if ( aLeafNode->prevSPtr == NULL )
        {
            sPrevRowPtr = NULL;
        }
        else
        {
            while(1)
            {
                sPrevNode = (smnbLNode*)aLeafNode->prevSPtr;
                sVersion  = getLatchValueOfLNode( sPrevNode ) & IDU_LATCH_UNMASK;

                IDL_MEM_BARRIER;

                sPrevRowPtr = sPrevNode->mRowPtrs[sPrevNode->mSlotCount - 1];

                IDL_MEM_BARRIER;

                if ( sVersion == getLatchValueOfLNode(sPrevNode) )
                {
                    break;
                }
            }
        }
    }
    else
    {
        IDE_DASSERT( ( aSlotPos - 1 ) >= 0 );
        sPrevRowPtr = aLeafNode->mRowPtrs[aSlotPos - 1];
    }

    sColumn = &aIndex->columns[0];
    // update index statistics

    if ( isNullColumn( sColumn,
                       &(sColumn->column),
                       aRow ) != ID_TRUE )
    {
        /* MinValue */
        if ( sPrevRowPtr == NULL )
        {
            IDE_TEST( setMinMaxStat( aPersIndex,
                                     aRow,
                                     sColumn,
                                     ID_TRUE ) != IDE_SUCCESS ); // aIsMinStat
        }

        /* MaxValue */
        if ( ( sNextRowPtr == NULL) ||
             ( isNullColumn( sColumn,
                             &(sColumn->column),
                             sNextRowPtr ) == ID_TRUE) )
        {
            IDE_TEST( setMinMaxStat( aPersIndex,
                                     aRow,
                                     sColumn,
                                     ID_FALSE ) != IDE_SUCCESS ); // aIsMinStat
        }
    }

    /* NumDist */
    sNumDistPtr = &aPersIndex->mStat.mNumDist;

    while(1)
    {
        if ( ( aIsUniqueIndex == ID_TRUE ) && ( aIndex->mIsNotNull == ID_TRUE ) )
        {
            idCore::acpAtomicInc64( (void*) sNumDistPtr );
            break;
        }

        if ( sPrevRowPtr != NULL )
        {
            sCompareResult = compareRows( &(aIndex->mStmtStat),
                                          aIndex->columns,
                                          aIndex->fence,
                                          aRow,
                                          sPrevRowPtr ); 
            if ( sCompareResult < 0 )
            {
                smnManager::logCommonHeader( aIndex->mIndexHeader );
                logIndexNode( aIndex->mIndexHeader, (smnbNode*)sPrevNode );
                IDE_ERROR_RAISE( 0, ERR_CORRUPTED_INDEX );
            }
            if ( sCompareResult == 0 )
            {
                break;
            }
        }

        if ( sNextRowPtr != NULL )
        {
            sCompareResult = compareRows( &(aIndex->mStmtStat),
                                          aIndex->columns,
                                          aIndex->fence,
                                          aRow,
                                          sNextRowPtr );
            if ( sCompareResult > 0 )
            {
                smnManager::logCommonHeader( aIndex->mIndexHeader );
                logIndexNode( aIndex->mIndexHeader, (smnbNode*)sNextNode );
                IDE_ERROR_RAISE( 0, ERR_CORRUPTED_INDEX );
            }
            if ( sCompareResult == 0 )
            {
                break;
            }
        }

        /* BUG-30074 - disk table�� unique index���� NULL key ���� ��/
         *             non-unique index���� deleted key �߰� �� NumDist��
         *             ��Ȯ���� ���� �������ϴ�.
         *
         * Null Key�� ��� NumDist�� �������� �ʵ��� �Ѵ�.
         * Memory index�� NumDist�� ������ ��å���� �����Ѵ�. */
        if ( smnbBTree::isNullKey( aIndex,
                                   aRow ) != ID_TRUE )
        {
            idCore::acpAtomicInc64( (void*) sNumDistPtr );
        }

        break;
    }

    /* BUG-32943 [sm-disk-index] After executing DELETE ROW, the KeyCount
     * of DRDB Index is not decreased  */
    idCore::acpAtomicInc64( (void*)&aPersIndex->mStat.mKeyCount );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_CORRUPTED_INDEX )
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_INCONSISTENT_INDEX ) );

        ideLog::log( IDE_SM_0, "Index Corrupted Detected : Wrong Index Order" );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smnbBTree::insertIntoLeafNode( smnbHeader      * aHeader,
                                      smnbLNode       * aLeafNode,
                                      SChar           * aRow,
                                      SInt              aSlotPos )
{
    /* PROJ-2433 */
    if ( aSlotPos <= ( aLeafNode->mSlotCount - 1 ) )
    {
        shiftLeafSlots( aLeafNode,
                        (SShort)aSlotPos,
                        (SShort)( aLeafNode->mSlotCount - 1 ),
                        1 );
    }
    else
    {
        /* nothing to do */
    }

    setLeafSlot( aLeafNode,
                 (SShort)aSlotPos,
                 aRow,
                 NULL ); /* direct key�� �Ʒ��� ���� */

    if ( SMNB_IS_DIRECTKEY_IN_NODE( aLeafNode ) == ID_TRUE )
    {
        IDE_TEST( smnbBTree::makeKeyFromRow( aHeader, 
                                             aRow,
                                             SMNB_GET_KEY_PTR( aLeafNode, aSlotPos ) ) 
                  != IDE_SUCCESS );
    }
    else
    {
        /* nothing to do */
    }

    IDL_MEM_BARRIER;

    aLeafNode->mSlotCount++;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smnbBTree::insertIntoInternalNode( smnbHeader  * aHeader,
                                          smnbINode   * a_pINode,
                                          void        * a_pRow,
                                          void        * aKey,
                                          void        * a_pChild )
{
    SInt s_nSlotPos;

    IDE_TEST( findSlotInInternal( aHeader,
                                  a_pINode,
                                  a_pRow,
                                  &s_nSlotPos ) != IDE_SUCCESS );

    /* PROJ-2433 */
    if ( s_nSlotPos <= ( a_pINode->mSlotCount - 1 ) )
    {
        shiftInternalSlots( a_pINode,
                            (SShort)s_nSlotPos,
                            (SShort)( a_pINode->mSlotCount - 1 ),
                            1 );
    }
    else
    {
        /* nothing to do */
    }

    setInternalSlot( a_pINode,
                     (SShort)s_nSlotPos,
                     (smnbNode *)a_pChild,
                     (SChar *)a_pRow,
                     aKey );

    IDL_MEM_BARRIER;

    a_pINode->mSlotCount++;
    
    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*********************************************************************
 * FUNCTION DESCRIPTION : smnbBTree::findSlotInInternal              *
 * ------------------------------------------------------------------*
 * INTERNAL NODE ������ row�� ��ġ�� ã�´�.
 *
 * - �Ϲ� INDEX�� ���
 *   �Լ� findRowInInternal() �� ����� �˻��Ѵ�.
 *
 * - (PROJ-2433) Direct Key Index�� ������̶��
 *   �Լ� findKeyInInternal() �� ����� direct key ������� �˻���
 *   �Լ� compareRowsAndPtr()�� findRowInInternal() �� ����� row ������� ��˻��Ѵ�.
 *
 *  => direct key�� ���� row ������� �Ǵٽ� ���ؾ� �ϴ� ����.
 *    1. ������ key ���� �������̸� row pointer���� ���� ��Ȯ�� slot�� ã�ƾ��ϱ� �����̴�.
 *    2. direct key�� partail key �ΰ�� ��Ȯ�� ��ġ�� ã���� ���⶧���̴�
 *
 * aHeader   - [IN]  INDEX ���
 * aNode     - [IN]  INTERNAL NODE
 * aRow      - [IN]  �˻��� row pointer
 * aSlot     - [OUT] ã���� slot ��ġ
 *********************************************************************/
IDE_RC smnbBTree::findSlotInInternal( smnbHeader  * aHeader,
                                      smnbINode   * aNode,
                                      void        * aRow,
                                      SInt        * aSlot )
{
    SInt    sMinimum = 0;
    SInt    sMaximum = aNode->mSlotCount - 1;

    if ( sMaximum == -1 )
    {
        *aSlot = 0;

        return IDE_SUCCESS;
    }
    else
    {
        /* nothing to do */
    }

    if ( SMNB_IS_DIRECTKEY_IN_NODE( aNode ) == ID_TRUE )
    {
        IDE_TEST( findKeyInInternal( aHeader,
                                     aNode,
                                     sMinimum,
                                     sMaximum,
                                     (SChar *)aRow,
                                     aSlot ) != IDE_SUCCESS );

        if ( *aSlot > sMaximum )
        {
            /* �������� �ش��ϴ°��̾��� */
            return IDE_SUCCESS;
        }
        else
        {
            /* nothing to do */
        }

        if ( compareRowsAndPtr( &aHeader->mAgerStat,
                                aHeader->columns,
                                aHeader->fence,
                                aNode->mRowPtrs[*aSlot],
                                aRow )
             >= 0 )
        {
            return IDE_SUCCESS;
        }
        else
        {
            sMinimum = *aSlot + 1;
        }
    }
    else
    {
        /* nothing to do */
    }

    if ( sMinimum <= sMaximum )
    {
        findRowInInternal( aHeader,
                           aNode,
                           sMinimum,
                           sMaximum,
                           (SChar *)aRow,
                           aSlot );
    }
    else
    {
        /* PROJ-2433
         * node �������� �����ϴ°��̾���. */
        *aSlot = sMaximum + 1;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smnbBTree::findKeyInInternal( smnbHeader   * aHeader,
                                   smnbINode    * aNode,
                                   SInt           aMinimum,
                                   SInt           aMaximum,
                                   SChar        * aRow,
                                   SInt         * aSlot )
{
    idBool  sIsSuccess = ID_TRUE;
    SInt    sMedium = 0;
    UInt    sPartialKeySize = ( aHeader->mIsPartialKey == ID_TRUE ) ? (aNode->mKeySize) : 0;

    do
    {
        sMedium = (aMinimum + aMaximum) >> 1;

        if ( compareKeys( &aHeader->mAgerStat,
                          aHeader->columns,
                          aHeader->fence,
                          SMNB_GET_KEY_PTR( aNode, sMedium ),
                          aNode->mRowPtrs[sMedium],
                          sPartialKeySize,
                          aRow,
                          &sIsSuccess )
             >= 0 )
        {
            aMaximum = sMedium - 1;
            *aSlot   = sMedium;
        }
        else
        {
            aMinimum = sMedium + 1;
            *aSlot   = aMinimum;
        }

        IDE_TEST( sIsSuccess != ID_TRUE );
    }
    while ( aMinimum <= aMaximum );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

void smnbBTree::findRowInInternal( smnbHeader   * aHeader,
                                   smnbINode    * aNode,
                                   SInt           aMinimum,
                                   SInt           aMaximum,
                                   SChar        * aRow,
                                   SInt         * aSlot )
{
    SInt sMedium = 0;

    do
    {
        sMedium = (aMinimum + aMaximum) >> 1;

        if ( compareRowsAndPtr( &aHeader->mAgerStat,
                                aHeader->columns,
                                aHeader->fence,
                                aNode->mRowPtrs[sMedium],
                                aRow )
             >= 0 )
        {
            aMaximum = sMedium - 1;
            *aSlot   = sMedium;
        }
        else
        {
            aMinimum = sMedium + 1;
            *aSlot   = aMinimum;
        }
    }
    while ( aMinimum <= aMaximum );
}

/*********************************************************************
 * FUNCTION DESCRIPTION : smnbBTree::findSlotInLeaf                  *
 * ------------------------------------------------------------------*
 * LEAF NODE ������ row�� ��ġ�� ã�´�.
 *
 * - �Ϲ� INDEX�� ���
 *   �Լ� findRowInLeaf() �� ����� �˻��Ѵ�.
 *
 * - (PROJ-2433) Direct Key Index�� ������̶��
 *   �Լ� findKeyInLeaf() �� ����� direct key ������� �˻���
 *   �Լ� compareRowsAndPtr()�� findRowInLeaf() �� ����� row ������� ��˻��Ѵ�.
 *
 *  => direct key�� ���� row ������� �Ǵٽ� ���ؾ� �ϴ� ����.
 *    1. direct key�� partail key �ΰ�� ��Ȯ�� ��ġ�� ã���� ���⶧���̴�
 *    2. ������ key ���� �������̸� row pointer���� ���� ��Ȯ�� slot�� ã�ƾ��ϱ� �����̴�.
 *
 * aHeader   - [IN]  INDEX ���
 * aNode     - [IN]  LEAF NODE
 * aRow      - [IN]  �˻��� row pointer
 * aSlot     - [OUT] ã���� slot ��ġ
 *********************************************************************/
IDE_RC smnbBTree::findSlotInLeaf( smnbHeader  * aHeader,
                                  smnbLNode   * aNode,
                                  void        * aRow,
                                  SInt        * aSlot )
{
    SInt        sMinimum = 0;
    SInt        sMaximum = aNode->mSlotCount - 1;

    if ( sMaximum == -1 )
    {
        *aSlot = 0;

        return IDE_SUCCESS;
    }
    else
    {
        /* nothing to do */
    }

    if ( SMNB_IS_DIRECTKEY_IN_NODE( aNode ) == ID_TRUE )
    {
        IDE_TEST( findKeyInLeaf( aHeader,
                                 aNode,
                                 sMinimum,
                                 sMaximum,
                                 (SChar *)aRow,
                                 aSlot ) != IDE_SUCCESS );

        if ( *aSlot > sMaximum )
        {
            /* ������������. */
            return IDE_SUCCESS;
        }
        else
        {
            /* nothing to do */
        }
 
        if ( compareRowsAndPtr( &aHeader->mAgerStat,
                                aHeader->columns,
                                aHeader->fence,
                                aNode->mRowPtrs[*aSlot],
                                aRow )
             >= 0 )
        {
            return IDE_SUCCESS;
        }
        else
        {
            sMinimum = *aSlot + 1; /* *aSlot�� ã�� slot�� �ƴѰ��� Ȯ���ߴ�. skip �Ѵ�. */
        }

    }
    else
    {
        /* nothing to do */
    }

    if ( sMinimum <= sMaximum )
    {
        findRowInLeaf( aHeader,
                       aNode,
                       sMinimum,
                       sMaximum,
                       (SChar *)aRow,
                       aSlot );
    }
    else
    {
        /* PROJ-2433
         * node �������� �����ϴ°��̾���. */
        *aSlot = sMaximum + 1;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smnbBTree::findKeyInLeaf( smnbHeader   * aHeader,
                                 smnbLNode    * aNode,
                                 SInt           aMinimum,
                                 SInt           aMaximum,
                                 SChar        * aRow,
                                 SInt         * aSlot )
{
    idBool  sIsSuccess = ID_TRUE;
    SInt    sMedium = 0;
    UInt    sPartialKeySize = ( aHeader->mIsPartialKey == ID_TRUE ) ? (aNode->mKeySize) : 0;

    do
    {
        sMedium = (aMinimum + aMaximum) >> 1;

        if ( compareKeys( &aHeader->mAgerStat,
                          aHeader->columns,
                          aHeader->fence,
                          SMNB_GET_KEY_PTR( aNode, sMedium ),
                          aNode->mRowPtrs[sMedium],
                          sPartialKeySize,
                          aRow,
                          &sIsSuccess )
             >= 0 )
        {
            aMaximum = sMedium - 1;
            *aSlot   = sMedium;
        }
        else
        {
            aMinimum = sMedium + 1;
            *aSlot   = aMinimum;
        }

        IDE_TEST( sIsSuccess != ID_TRUE );
    }
    while ( aMinimum <= aMaximum );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

void smnbBTree::findRowInLeaf( smnbHeader   * aHeader,
                               smnbLNode    * aNode,
                               SInt           aMinimum,
                               SInt           aMaximum,
                               SChar        * aRow,
                               SInt         * aSlot )
{
    SInt sMedium = 0;

    do
    {
        sMedium = (aMinimum + aMaximum) >> 1;

        if ( compareRowsAndPtr( &aHeader->mAgerStat,
                                aHeader->columns,
                                aHeader->fence,
                                aNode->mRowPtrs[sMedium],
                                aRow )
             >= 0 )
        {
            aMaximum = sMedium - 1;
            *aSlot   = sMedium;
        }
        else
        {
            aMinimum = sMedium + 1;
            *aSlot   = aMinimum;
        }
    }
    while ( aMinimum <= aMaximum );
}

IDE_RC smnbBTree::findPosition( const smnbHeader   * a_pHeader,
                                SChar              * a_pRow,
                                SInt               * a_pDepth,
                                smnbStack          * a_pStack )
{
    /* BUG-37643 Node�� array�� ���������� �����ϴµ�
     * compiler ����ȭ�� ���ؼ� ���������� ���ŵ� �� �ִ�.
     * ���� �̷��� ������ volatile�� �����ؾ� �Ѵ�. */
    volatile smnbINode * s_pCurINode;
    volatile smnbINode * s_pChildINode;
    volatile smnbINode * s_pCurSINode;
    SInt                 s_nDepth;
    smnbStack          * s_pStack;
    SInt                 s_nLstReadPos;
    IDU_LATCH            s_version;
    SInt                 s_slotCount;

retry:
    s_nDepth = *a_pDepth;

    while(1)
    {
        if ( s_nDepth < 0 )
        {
            s_pStack       = a_pStack;
            s_pCurINode    = (smnbINode*)(a_pHeader->root);

            if ( s_pCurINode != NULL )
            {
                IDL_MEM_BARRIER;
                s_version = getLatchValueOfINode(s_pCurINode) & (~SMNB_SCAN_LATCH_BIT);
                IDL_MEM_BARRIER;
            }

            break;
        }
        else
        {
            s_nDepth     = *a_pDepth;

            while(s_nDepth >= 0)
            {
                s_pCurINode  = (smnbINode*)(a_pStack[s_nDepth].node);

                IDL_MEM_BARRIER;
                if ( getLatchValueOfINode(s_pCurINode)
                      == a_pStack[s_nDepth].version )
                {
                    break;
                }

                s_nDepth--;
            }

            if ( s_nDepth < 0 )
            {
                continue;
            }

            s_pCurINode  = (smnbINode*)(a_pStack[s_nDepth].node);

            IDL_MEM_BARRIER;
            s_version = getLatchValueOfINode(s_pCurINode) & (~SMNB_SCAN_LATCH_BIT);
            IDL_MEM_BARRIER;

            s_pStack     = a_pStack + s_nDepth;
            s_nDepth--;

            break;
        }
    }

    if ( s_pCurINode != NULL )
    {
        while((s_pCurINode->flag & SMNB_NODE_TYPE_MASK) == SMNB_NODE_TYPE_INTERNAL)
        {
            while(1)
            {
                IDL_MEM_BARRIER;
                s_version = getLatchValueOfINode(s_pCurINode) & (~SMNB_SCAN_LATCH_BIT);
                IDL_MEM_BARRIER;

                IDE_TEST( findSlotInInternal( (smnbHeader *)a_pHeader,
                                              (smnbINode *)s_pCurINode,
                                              a_pRow,
                                              &s_nLstReadPos ) != IDE_SUCCESS );

                s_pChildINode   = (smnbINode*)(s_pCurINode->mChildPtrs[s_nLstReadPos]);
                s_slotCount     = s_pCurINode->mSlotCount;
                s_pCurSINode    = s_pCurINode->nextSPtr;

                IDL_MEM_BARRIER;

                if ( s_version == getLatchValueOfINode(s_pCurINode) )
                {
                    break;
                }
            }

            /* free�� node�� ���Դ�... �ٽ�. */
            if ( s_slotCount == 0 )
            {
                goto retry;
            }

            if ( s_nLstReadPos >= s_slotCount )
            {
                if ( s_version != getLatchValueOfINode(s_pCurINode) )
                {
                    /* Ž�� �� Ÿ Tx�� ���� key reorganization�� ����� ���
                     * ���� ��尡 ���յǸ鼭 ���ͳ� ����� slotcount�� �پ���
                     * �̿� ���� �̵����� ���� ���¶� �ش� slot�� ��ġ�� slotcount���� Ŭ �� �ִ�.
                     * �� ��쿡�� Ž���� ������Ѵ�. */
                    goto retry;
                }
                else
                {
                    if ( s_pCurSINode != NULL )
                    {
                        /* node split �ΰ�� next node�� Ž�� */
                        s_pCurINode = s_pCurSINode;

                        continue;
                    }
                    else
                    {
                        /* BUG-45573
                           �ſ� ���� Ȯ��������, �Ʒ� �������� �����ϸ� �߻��Ҽ��ִ�.
                           �̰�� ��Ž���Ѵ�.

                           1. s_pCurINode ����� ���� ū key�� Ager�� ���� �����Ǿ���,
                           �̹��� �߰��Ϸ��� ���ο� key�� ��忡�� ���� ū���̴�.
                           (s_nLstReadPos >= s_slotCount ������ ����.)

                           2. Ager�� ���� next node�� �����Ǿ� s_pCurINode->nextSPtre = NULL�� ���õǾ���.
                         */
                        goto retry;
                    }
                } 
            }

            s_pStack->version    = s_version;
            s_pStack->node       = (smnbINode*)s_pCurINode;
            s_pStack->lstReadPos = s_nLstReadPos;
            s_pStack->slotCount  = s_slotCount;

            s_nDepth++;
            s_pStack++;

            s_pCurINode = s_pChildINode;
        }

        IDL_MEM_BARRIER;
        s_pStack->version    = getLatchValueOfINode(s_pCurINode);
        s_pStack->node       = (smnbINode*)s_pCurINode;
        s_nDepth++;
    }

    *a_pDepth = s_nDepth;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smnbBTree::splitInternalNode(smnbHeader      * a_pIndexHeader,
                                    void            * a_pRow,
                                    void            * aKey,
                                    void            * a_pChild,
                                    SInt              a_nSlotPos,
                                    smnbINode       * a_pINode,
                                    smnbINode       * a_pNewINode,
                                    void           ** a_pSepKeyRow,
                                    void           ** aSepKey )
{
    SInt i = 0;

    IDL_MEM_BARRIER;

    // BUG-18292 : V$MEM_BTREE_HEADER ���� �߰�
    a_pIndexHeader->nodeCount++;

    // To fix BUG-18671
    initInternalNode( a_pNewINode,
                      a_pIndexHeader,
                      IDU_LATCH_UNLOCKED );

    IDL_MEM_BARRIER; /* ����� �ʱ�ȭ�Ŀ� link�� �����ؾ� ��*/

    a_pNewINode->nextSPtr  = a_pINode->nextSPtr;
    a_pNewINode->prevSPtr  = a_pINode;

    if ( a_pINode->nextSPtr != NULL )
    {
        SMNB_SCAN_LATCH(a_pINode->nextSPtr);
        a_pINode->nextSPtr->prevSPtr = a_pNewINode;
        SMNB_SCAN_UNLATCH(a_pINode->nextSPtr);
    }

    a_pINode->nextSPtr = a_pNewINode;

    if ( a_nSlotPos <= SMNB_INTERNAL_SLOT_SPLIT_COUNT( a_pIndexHeader ) )
    {
        /* PROJ-2433 */
        if ( SMNB_INTERNAL_SLOT_SPLIT_COUNT(a_pIndexHeader) <= ( a_pINode->mSlotCount - 1 ) )
        {
            copyInternalSlots( a_pNewINode,
                               0,
                               a_pINode,
                               (SShort)( SMNB_INTERNAL_SLOT_SPLIT_COUNT(a_pIndexHeader) ),
                               (SShort)( a_pINode->mSlotCount - 1 ) );
        }
        else
        {
            /* ����� �ü� ����. */
            IDE_ERROR_RAISE( 0, ERR_CORRUPTED_INDEX );
        }

        a_pNewINode->mSlotCount = a_pINode->mSlotCount - SMNB_INTERNAL_SLOT_SPLIT_COUNT(a_pIndexHeader);
        a_pINode   ->mSlotCount = SMNB_INTERNAL_SLOT_SPLIT_COUNT(a_pIndexHeader);

        if ( a_nSlotPos <= ( a_pINode->mSlotCount - 1 ) )
        {
            shiftInternalSlots( a_pINode,
                                (SShort)a_nSlotPos,
                                (SShort)( a_pINode->mSlotCount - 1 ),
                                1 );
        }
        else
        {
            /* nothing to do */
        }

        setInternalSlot( a_pINode,
                         (SShort)a_nSlotPos,
                         (smnbNode *)a_pChild,
                         (SChar *)a_pRow,
                         aKey );

        (a_pINode->mSlotCount)++;
    }
    else
    {
        /* PROJ-2433 */
        if ( SMNB_INTERNAL_SLOT_SPLIT_COUNT(a_pIndexHeader) <= ( a_nSlotPos - 1 ) )
        {
            copyInternalSlots( a_pNewINode,
                               0,
                               a_pINode,
                               (SShort)( SMNB_INTERNAL_SLOT_SPLIT_COUNT( a_pIndexHeader ) ),
                               (SShort)( a_nSlotPos - 1 ) );

            i = a_nSlotPos - SMNB_INTERNAL_SLOT_SPLIT_COUNT( a_pIndexHeader );
        }
        else
        {
            /* ����� �ü� ����. */
            IDE_ERROR_RAISE( 0, ERR_CORRUPTED_INDEX );
        }

        setInternalSlot( a_pNewINode,
                         (SShort)i,
                         (smnbNode *)a_pChild,
                         (SChar *)a_pRow,
                         aKey );

        i++;

        copyInternalSlots( a_pNewINode,
                           (SShort)i,
                           a_pINode,
                           (SShort)a_nSlotPos,
                           (SShort)( a_pINode->mSlotCount - 1 ) );

        a_pNewINode->mSlotCount = a_pINode->mSlotCount - SMNB_INTERNAL_SLOT_SPLIT_COUNT( a_pIndexHeader ) + 1;
        a_pINode   ->mSlotCount = SMNB_INTERNAL_SLOT_SPLIT_COUNT( a_pIndexHeader );
    }

    *a_pSepKeyRow = NULL;
    *aSepKey      = NULL;

    getInternalSlot( NULL,
                     (SChar **)a_pSepKeyRow,
                     aSepKey,
                     a_pINode,
                     (SShort)( a_pINode->mSlotCount - 1 ) );

    IDL_MEM_BARRIER;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_CORRUPTED_INDEX )
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_INCONSISTENT_INDEX ) );

        ideLog::log( IDE_SM_0, "Index Corrupted Detected : Wrong Index SlotCount" );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

void smnbBTree::splitLeafNode( smnbHeader    * a_pIndexHeader,
                               SInt            a_nSlotPos,
                               smnbLNode     * a_pLeafNode,
                               smnbLNode     * a_pNewLeafNode,
                               smnbLNode    ** aTargetLeaf,
                               SInt          * aTargetSeq )
{
    IDL_MEM_BARRIER;

    // BUG-18292 : V$MEM_BTREE_HEADER ���� �߰�
    a_pIndexHeader->nodeCount++;

    // To fix BUG-18671
    initLeafNode( a_pNewLeafNode,
                  a_pIndexHeader,
                  IDU_LATCH_LOCKED );
    
    SMNB_SCAN_UNLATCH( a_pNewLeafNode );

    IDL_MEM_BARRIER; /* ����� �ʱ�ȭ�Ŀ� link�� �����ؾ� ��*/

    /* new node�� key �й谡 �Ϸ�ɶ����� ��Ƴ����Ѵ�.
     * �׷��� ������, no latch�� ���� ��带 �����Ҷ� ���� key �й谡 �ȵ�
     * ���¿��� �� �� �ִ�.
     * ���� ���, ��� ���Ž� ���� ��带 �����Ҷ�..
     * ���� new node�� tree�� �����ϱ� ���� latch bit�� �����ؾ� �Ѵ�. */
    SMNB_SCAN_LATCH( a_pNewLeafNode );

    a_pNewLeafNode->nextSPtr  = a_pLeafNode->nextSPtr;
    a_pNewLeafNode->prevSPtr  = a_pLeafNode;

    if ( a_pLeafNode->nextSPtr != NULL)
    {
        /* split�� �ϱ� ���� tree latch ���� ��Ȳ�̶� Tree ������ �������
         * �ʴ´�. backward scan�ÿ��� tree latch�� ��� ������ ���� ����. */
        a_pLeafNode->nextSPtr->prevSPtr = a_pNewLeafNode;
    }

    a_pLeafNode->nextSPtr = a_pNewLeafNode;

    if ( a_nSlotPos <= SMNB_LEAF_SLOT_SPLIT_COUNT( a_pIndexHeader ) )
    {
        /* PROJ-2433 */
        if ( SMNB_LEAF_SLOT_SPLIT_COUNT( a_pIndexHeader ) <= ( a_pLeafNode->mSlotCount - 1 ) )
        {
            copyLeafSlots( a_pNewLeafNode,
                           0,
                           a_pLeafNode,
                           (SShort)( SMNB_LEAF_SLOT_SPLIT_COUNT( a_pIndexHeader ) ),
                           (SShort)( a_pLeafNode->mSlotCount - 1 ) );
        }
        else
        {
            /* nothing to do */
        }

        a_pNewLeafNode->mSlotCount = a_pLeafNode->mSlotCount - SMNB_LEAF_SLOT_SPLIT_COUNT( a_pIndexHeader );
        a_pLeafNode   ->mSlotCount = SMNB_LEAF_SLOT_SPLIT_COUNT( a_pIndexHeader );

        *aTargetLeaf = a_pLeafNode;
        *aTargetSeq = a_nSlotPos;
    }
    else
    {
        /* PROJ-2433 */
        if ( SMNB_LEAF_SLOT_SPLIT_COUNT( a_pIndexHeader ) <= ( a_pLeafNode->mSlotCount - 1 ) )
        {
            copyLeafSlots( a_pNewLeafNode,
                           0,
                           a_pLeafNode,
                           (SShort)( SMNB_LEAF_SLOT_SPLIT_COUNT( a_pIndexHeader ) ),
                           (SShort)( a_pLeafNode->mSlotCount - 1 ) );
        }
        else
        {
            /* nothing to do */
        }

        a_pLeafNode   ->mSlotCount = SMNB_LEAF_SLOT_SPLIT_COUNT( a_pIndexHeader );
        a_pNewLeafNode->mSlotCount = ( SMNB_LEAF_SLOT_MAX_COUNT( a_pIndexHeader ) -
                                       SMNB_LEAF_SLOT_SPLIT_COUNT( a_pIndexHeader ) );

        *aTargetLeaf = a_pNewLeafNode;
        *aTargetSeq  = a_nSlotPos - SMNB_LEAF_SLOT_SPLIT_COUNT( a_pIndexHeader );
    }

    SMNB_SCAN_UNLATCH( a_pNewLeafNode );
}

IDE_RC smnbBTree::checkUniqueness( smnIndexHeader        * aIndexHeader,
                                   void                  * a_pTrans,
                                   smnbStatistic         * aIndexStat,
                                   const smnbColumn      * a_pColumns,
                                   const smnbColumn      * a_pFence,
                                   smnbLNode             * a_pLeafNode,
                                   SInt                    a_nSlotPos,
                                   SChar                 * a_pRow,
                                   smSCN                   aStmtSCN,
                                   idBool                  aIsTreeLatched,
                                   smTID                 * a_pTransID,
                                   idBool                * aIsRetraverse,
                                   SChar                ** aExistUniqueRow,
                                   SChar                ** a_diffRow,
                                   SInt                  * a_diffSlotPos )
{
    SInt          i;
    SInt          j;
    SInt          s_nSlotPos;
    smnbLNode   * s_pCurLeafNode;
    UInt          sState = 0;
    SInt          sSlotCount = 0;
    SInt          sCompareResult;
    idBool        sIsReplTx = ID_FALSE;
    smnbLNode   * sTempNode = NULL;
    idBool        sCheckEnd = ID_FALSE;
    UInt          sTempLockState = 0;

    *a_pTransID    = SM_NULL_TID;
    *aIsRetraverse = ID_FALSE;

    sIsReplTx = smxTransMgr::isReplTrans( a_pTrans );

    /* �̿� ��带 ������ �ʿ䰡 �ִ� ��� Tree latch�� ��ƾ� �Ѵ�.
     * ���� �����ϰ��� �ϴ� ��忡�� latch�� �����ִ� �����̹Ƿ�
     * �ش� ��常 ���� �¿� �̿���带 �����ϴ��� �Ǵ��ؾ� �Ѵ�.
     * ������ ��ġ�� ó�� �Ǵ� ������ �̰ų� ������ row�� ù��°, ������ row��
     * ���� �����ϸ� �¿� �̿� ��带 �����Ѵٰ� �Ǵ��Ѵ�. */
    if ( aIsTreeLatched == ID_FALSE )
    {
        if ( ( a_nSlotPos == a_pLeafNode->mSlotCount ) ||
             ( compareRows( aIndexStat,
                            a_pColumns,
                            a_pFence,
                            a_pRow,
                            a_pLeafNode->mRowPtrs[a_pLeafNode->mSlotCount - 1] ) == 0 ) )
        {
            *aIsRetraverse = ID_TRUE;
        }
        else
        {
            /* nothing to do */
        }

        if ( ( a_nSlotPos == 0 ) ||
             ( compareRows( aIndexStat,
                            a_pColumns,
                            a_pFence,
                            a_pRow,
                            a_pLeafNode->mRowPtrs[0] ) == 0 ) )
        {
            *aIsRetraverse = ID_TRUE;
        }
        else
        {
            /* nothing to do */
        }

        /* Tree latch ��� �ٽ� �õ��ؾ� �Ѵ�. */
        if ( *aIsRetraverse == ID_TRUE )
        {
            IDE_CONT( need_to_retraverse );
        }
    }

    /*
     * uniqueness üũ�� �����Ѵ�.
     */

    s_pCurLeafNode = a_pLeafNode;

    /* backward Ž��
     * backward�� lock�� ��µ�, Tree latch�� ȹ���� �����̱� ������
     * ����� ������ ����.
     *
     * lock�� ��� ������ ���� �� ó�� ��忡�� ���� ���� ���� key�� �ٸ� ����
     * ���� key�� �����ֱ� ������ Tree latch���� insert/freeSlot�� ����� ��
     * �ֱ��, �̶����� slot ��ġ�� ����� �� �ֱ� �����̴�. */
    while ( s_pCurLeafNode != NULL )
    {
        /* ������ ���� �̹� latch�� ���������Ƿ�, �� ������ �ȵȴ�. */
        if ( a_pLeafNode != s_pCurLeafNode )
        {
            /* �̿� ��带 �����Ѵٸ� Tree latch�� �����־�� �Ѵ�. */
            IDE_ASSERT( aIsTreeLatched == ID_TRUE );

            IDE_TEST( lockNode(s_pCurLeafNode) != IDE_SUCCESS );
            sState = 1;

            /* ������ slot���� ����. */
            s_nSlotPos = s_pCurLeafNode->mSlotCount;
        }
        else
        {
            /* ������ ���� ������ ��ġ ���� ���� */
            s_nSlotPos = a_nSlotPos;
        }

        for (i = s_nSlotPos - 1; i >= 0; i--)
        {
            sCompareResult = compareRows( aIndexStat,
                                          a_pColumns,
                                          a_pFence,
                                          a_pRow,
                                          s_pCurLeafNode->mRowPtrs[i] );
            if ( sCompareResult > 0 )
            {
                break;
            }

            if ( sCompareResult != 0 )
            {
                smnManager::logCommonHeader( aIndexHeader );
                logIndexNode( aIndexHeader, (smnbNode*)s_pCurLeafNode );
                IDE_ERROR_RAISE( 0, ERR_CORRUPTED_INDEX );
            }

            IDE_TEST( isRowUnique( a_pTrans,
                                   aIndexStat,
                                   aStmtSCN,
                                   s_pCurLeafNode->mRowPtrs[i],
                                   a_pTransID,
                                   aExistUniqueRow )
                      != IDE_SUCCESS );

            if ( *a_pTransID != SM_NULL_TID )
            {
                *a_diffSlotPos = i;
                *a_diffRow = (SChar*)s_pCurLeafNode->mRowPtrs[i];

                /* BUG-45524 replication ���� reciever�ʿ����� Tx ��⸦ ���� �ʱ� ������ 
                 *           �Ͻ������� ������ Ű�� �ټ� ������ �� �ֽ��ϴ�.
                 *           �ߺ��� Ű�� sender���� rollback�α׿� ���� �ڿ������� ���� �Ǳ�������
                 *           ���ŵ� ������ Ű�� unique violation üũ�� �� ������� ���� ���¿���
                 *           local query�� log ������ �߻��� ��� ������ �߻��� �� �ֽ��ϴ�.
                 *           �̸� ���� ���� ������ Ű�� �ټ� ������ ��쿡 �̵� ��
                 *           �̵� ��θ� ������� �� ������ �����ؾ� �մϴ�. */
                if( sIsReplTx != ID_TRUE )
                {
                    /* replication�� �ƴ϶�� Ÿ slot�� Ž���� �ʿ䰡 ����. */
                    break;
                }
                else
                {
                    /* ������ Ű ���� �ִ� row�� �� �ִ��� ���� slot�� Ȯ���Ѵ�.
                     * ������ Ű ���� ���� row�� commit�� row�� �ִٸ� �ش� row�� ���·� unique check�� �ؾ� >�Ѵ�. */
                    j = i - 1;

                    while( j >= 0 )
                    {
                        sCompareResult = compareRows( aIndexStat,
                                                      a_pColumns,
                                                      a_pFence,
                                                      a_pRow,
                                                      s_pCurLeafNode->mRowPtrs[j] );

                        if( sCompareResult == 0 )
                        {
                            /* ������ Ű ���� ���� row�� �ִٸ� isRowUnique���� unique üũ�� �Ѵ�. */
                            IDE_TEST(isRowUnique(a_pTrans,
                                                 aIndexStat,
                                                 aStmtSCN,
                                                 s_pCurLeafNode->mRowPtrs[j],
                                                 a_pTransID,
                                                 aExistUniqueRow )
                                     != IDE_SUCCESS);

                            /* unique check�� ��� �ߴٸ� ���� slot�� ���ؼ��� �ݺ��Ѵ�. */
                            j--;
                        }
                        else
                        {
                            /* ������ Ű���� ���� row�� ���� slot�� ���� */
                            break;
                        }
                    }

                    if( ( j != -1 ) || ( s_pCurLeafNode->prevSPtr == NULL) )
                    {
                        /* ��� ������ �������� ���� Ű�� ã�Ұų� ���� ��尡 ���ٸ� 
                         * ���� ����� Ž���� ���� ���� �ʴ´�. */
                        break;
                    }
                    else
                    {
                        /* Ű ���� Ž���� ��� ��ü�� Ž���� �Ŀ��� ���� Ű ���� ��� �߰ߵȴٸ�
                         * ���� ��忡 ���ؼ��� Ž���� �����ؾ� �Ѵ�. */
                    }

                    /* ��忡 lock�� �ɾ�� �ϹǷ� tree latch�� ��� ���� �ʴٸ� tree latch�� ��� ������Ѵ�. */
                    if( aIsTreeLatched == ID_FALSE )
                    {
                        if( sState == 1)
                        {
                            sState = 0;
                            IDE_TEST( unlockNode(s_pCurLeafNode) != IDE_SUCCESS );
                        }

                        *aIsRetraverse = ID_TRUE;
                        IDE_CONT( need_to_retraverse );
                    }
                    else
                    {
                        sTempNode = s_pCurLeafNode->prevSPtr;

                        while( ( sCheckEnd == ID_FALSE ) && ( sTempNode != NULL ) )
                        {
                            IDE_TEST( lockNode( sTempNode ) != IDE_SUCCESS );
                            sTempLockState = 1;

                            for( j = ( sTempNode->mSlotCount - 1) ; j>= 0; j-- )
                            {
                                sCompareResult = compareRows( aIndexStat,
                                                              a_pColumns,
                                                              a_pFence,
                                                              a_pRow,
                                                              sTempNode->mRowPtrs[j] );

                                if( sCompareResult != 0 )
                                {
                                    /* �������� ���� Ű�� �߰ߵ� ��� �� �̻� Ž���� �ʿ� ����. */
                                    sTempLockState = 0;
                                    IDE_TEST( unlockNode( sTempNode ) != IDE_SUCCESS );

                                    sCheckEnd = ID_TRUE;

                                    break;
                                }
                                else
                                {
                                    /* ������ Ű ���� �߰ߵ� ��� unique check�� �����Ѵ�. */
                                    IDE_TEST(isRowUnique(a_pTrans,
                                                         aIndexStat,
                                                         aStmtSCN,
                                                         sTempNode->mRowPtrs[j],
                                                         a_pTransID,
                                                         aExistUniqueRow )
                                             != IDE_SUCCESS);
                                }
                            }

                            if( sCheckEnd == ID_TRUE )
                            {
                                break;
                            }
                            /* ���� ��� �ϳ��� �� Ž���������� ���� Ű ���� ��� �߰ߵȴٸ�
                             * �ٽ� �� �� ��忡 ���ؼ��� Ž���� �����Ѵ�.*/

                            sTempLockState = 0;
                            IDE_TEST( unlockNode( sTempNode ) != IDE_SUCCESS );

                            sTempNode = sTempNode->prevSPtr;
                        }
                    } 
                }
                break;
            }
        }

        if ( sState == 1 )
        {
            sState = 0;
            IDE_TEST( unlockNode(s_pCurLeafNode) != IDE_SUCCESS );
        }

        if ( *a_pTransID != SM_NULL_TID )
        {
            IDE_CONT( need_to_wait );
        }

        if ( i != -1 )
        {
            break;
        }

        s_pCurLeafNode = s_pCurLeafNode->prevSPtr;
    }

    /* forward */
    s_pCurLeafNode = a_pLeafNode;

    while (s_pCurLeafNode != NULL)
    {
        if ( a_pLeafNode != s_pCurLeafNode )
        {
            /* �̿� ��带 �����Ѵٸ� Tree latch�� �����־�� �Ѵ�. */
            IDE_ASSERT( aIsTreeLatched == ID_TRUE );

            IDE_TEST( lockNode(s_pCurLeafNode) != IDE_SUCCESS );
            sState = 1;

            /* �̿� ����� ��� ó������ ���� */
            s_nSlotPos = -1;
        }
        else
        {
            /* ������ ���� ������ ��ġ���� ���� */
            s_nSlotPos = a_nSlotPos - 1;
        }

        sSlotCount = s_pCurLeafNode->mSlotCount;

        for (i = s_nSlotPos + 1; i < sSlotCount; i++)
        {
            sCompareResult = compareRows( aIndexStat,
                                          a_pColumns,
                                          a_pFence,
                                          a_pRow,
                                          s_pCurLeafNode->mRowPtrs[i] );
            if ( sCompareResult < 0 )
            {
                break;
            }

            if ( sCompareResult != 0 )
            {
                smnManager::logCommonHeader( aIndexHeader );
                logIndexNode( aIndexHeader, (smnbNode*)s_pCurLeafNode );
                IDE_ERROR_RAISE( 0, ERR_CORRUPTED_INDEX );
            }

            IDE_TEST( isRowUnique( a_pTrans,
                                   aIndexStat,
                                   aStmtSCN,
                                   s_pCurLeafNode->mRowPtrs[i],
                                   a_pTransID,
                                   aExistUniqueRow )
                      != IDE_SUCCESS );

            if ( *a_pTransID != SM_NULL_TID )
            {
                *a_diffSlotPos = i;
                *a_diffRow = s_pCurLeafNode->mRowPtrs[i];

                /* BUG-45524 replication ���� reciever�ʿ����� Tx��⸦ ���� �ʱ� ������ 
                 *           �Ͻ������� ������ Ű�� �ټ� ������ �� �ֽ��ϴ�.
                 *           �ߺ��� Ű�� sender���� rollback�α׿� ���� �ڿ������� ���� �Ǳ�������
                 *           ���ŵ� ������ Ű�� unique violation üũ�� �� ������� ���� ���¿���
                 *           local query�� log ������ �߻��� ��� ������ �߻��� �� �ֽ��ϴ�.
                 *           �̸� ���� ���� ������ Ű�� �ټ� ������ ��쿡 
                 *           �̵� ��θ� ������� �� ������ �����ؾ� �մϴ�. */
                if( sIsReplTx != ID_TRUE )
                {    
                    /* replication�� �ƴ϶�� Ÿ slot�� Ž���� �ʿ䰡 ����. */
                    break;
                }    
                else 
                {    
                    /* ������ Ű ���� �ִ� row�� �� �ִ��� ���� slot�� Ȯ���Ѵ�.
                     * ������ Ű ���� ���� row�� commit�� row�� �ִٸ� �ش� row�� ���·� unique check�� �ؾ� �Ѵ�. */
                    j = i + 1;

                    while( j < sSlotCount )
                    {    
                        sCompareResult = compareRows( aIndexStat,
                                                      a_pColumns,
                                                      a_pFence,
                                                      a_pRow,
                                                      s_pCurLeafNode->mRowPtrs[j] );

                        if( sCompareResult == 0 )
                        {   
                            /* ������ Ű ���� ���� row�� �ִٸ� isRowUnique���� unique üũ�� �Ѵ�. */
                            IDE_TEST(isRowUnique(a_pTrans,
                                                 aIndexStat,
                                                 aStmtSCN,
                                                 s_pCurLeafNode->mRowPtrs[j],
                                                 a_pTransID,
                                                 aExistUniqueRow ) 
                                     != IDE_SUCCESS);

                            /* unique check�� ��� �ߴٸ� ���� slot�� ���ؼ��� �ݺ��Ѵ�. */
                            j++;
                        }    
                        else 
                        {    
                            /* ������ Ű���� ���� row�� ���� slot�� ���� */
                            break;
                        }
                    }

                    if( ( j != sSlotCount ) || ( s_pCurLeafNode->nextSPtr == NULL) )
                    {
                        /* ��� ������ �������� ���� Ű�� ã�Ұų� ���� ��尡 ���ٸ� 
                         * ���� ����� Ž���� �������� �ʴ´�. */
                        break;
                    }
                    else
                    {
                        /* Ű ���� Ž���� ��� ��ü�� Ž���� �Ŀ��� ���� Ű ���� ��� �߰ߵȴٸ�
                         * ���� ��忡 ���ؼ��� Ž���� �����ؾ� �Ѵ�. */
                    }

                    /* ��忡 lock�� �ɾ�� �ϹǷ� tree latch�� ��� ���� �ʴٸ� tree latch�� ��� ������Ѵ�. */
                    if( aIsTreeLatched == ID_FALSE )
                    {
                        if( sState == 1 )
                        {
                            sState = 0;
                            IDE_TEST( unlockNode(s_pCurLeafNode) != IDE_SUCCESS );
                        }

                        *aIsRetraverse = ID_TRUE;
                        IDE_CONT( need_to_retraverse );
                    }
                    else
                    {
                        sTempNode = s_pCurLeafNode->nextSPtr;

                        while( ( sCheckEnd == ID_FALSE ) && ( sTempNode != NULL ) )
                        {
                            IDE_TEST( lockNode( sTempNode ) != IDE_SUCCESS );
                            sTempLockState = 1;

                            for( j = 0 ; j < sTempNode->mSlotCount ; j++ )
                            {
                                sCompareResult =
                                    compareRows( aIndexStat,
                                                 a_pColumns,
                                                 a_pFence,
                                                 a_pRow,
                                                 sTempNode->mRowPtrs[j]);

                                if( sCompareResult != 0)
                                {
                                    /* �������� ���� Ű�� �߰ߵ� ��� �� �̻� Ž���� �ʿ� ����. */
                                    sTempLockState = 0;
                                    IDE_TEST( unlockNode( sTempNode ) != IDE_SUCCESS );

                                    sCheckEnd = ID_TRUE;

                                    break;
                                }
                                else
                                {
                                    /* ������ Ű ���� �߰ߵ� ��� unique check�� �����Ѵ�. */
                                    IDE_TEST(isRowUnique(a_pTrans,
                                                         aIndexStat,
                                                         aStmtSCN,
                                                         sTempNode->mRowPtrs[j],
                                                         a_pTransID,
                                                         aExistUniqueRow )
                                             != IDE_SUCCESS);
                                }
                            }

                            if( sCheckEnd == ID_TRUE )
                            {
                                break;
                            }
                            /* ���� ��� �ϳ��� �� Ž���������� ���� Ű ���� ��� �߰ߵȴٸ�
                             * �ٽ� �� ���� ��忡 ���ؼ��� Ž���� �����Ѵ�.*/

                            sTempLockState = 0;
                            IDE_TEST( unlockNode( sTempNode ) != IDE_SUCCESS );

                            sTempNode = sTempNode->nextSPtr;
                        }
                    }
                }
                break;
            }
        }

        if ( sState == 1 )
        {
            sState = 0;
            IDE_TEST( unlockNode(s_pCurLeafNode) != IDE_SUCCESS );
        }

        if ( *a_pTransID != SM_NULL_TID )
        {
            IDE_CONT( need_to_wait );
        }

        if ( i != sSlotCount )
        {
            break;
        }

        IDE_ASSERT( aIsTreeLatched == ID_TRUE );

        s_pCurLeafNode = s_pCurLeafNode->nextSPtr;
    }


    IDE_EXCEPTION_CONT( need_to_wait );
    IDE_EXCEPTION_CONT( need_to_retraverse );

    IDE_ASSERT( sState == 0 );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_CORRUPTED_INDEX )
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_INCONSISTENT_INDEX ) );

        ideLog::log( IDE_SM_0, "Index Corrupted Detected : Wrong Index Order" );
    }
    IDE_EXCEPTION_END;

    if ( ideGetErrorCode() == smERR_ABORT_INCONSISTENT_INDEX )
    {
        smnManager::setIsConsistentOfIndexHeader( aIndexHeader, ID_FALSE );
    }

    if( sTempLockState == 1 )
    {
        IDE_ASSERT( unlockNode( sTempNode ) == IDE_SUCCESS );
    }

    if ( sState == 1 )
    {
        IDE_ASSERT( unlockNode(s_pCurLeafNode) == IDE_SUCCESS );
    }

    return IDE_FAILURE;
}

void smnbBTree::getPreparedNode( smnbNode    ** aNodeList,
                                 UInt         * aNodeCount,
                                 void        ** aNode )
{
    smnbNode    *sNode;
    smnbNode    *sPrev;
    smnbNode    *sNext;

    IDE_ASSERT( aNodeList  != NULL );
    IDE_ASSERT( aNodeCount != NULL );
    IDE_ASSERT( aNode      != NULL );
    IDE_ASSERT( *aNodeCount > 0 );

    sNode = *aNodeList;
    sPrev = (smnbNode*)((*aNodeList)->prev);
    sNext = (smnbNode*)((*aNodeList)->next);

    sPrev->next = sNext;
    sNext->prev = sPrev;

    sNode->prev = NULL;
    sNode->next = NULL;

    *aNode = sNode;
    (*aNodeCount)--;

    if ( *aNodeCount == 0 )
    {
        *aNodeList = NULL;
    }
    else
    {
        *aNodeList = sNext;
    }
}


IDE_RC smnbBTree::prepareNodes( smnbHeader    * aIndexHeader,
                                smnbStack     * aStack,
                                SInt            aDepth,
                                smnbNode     ** aNewNodeList,
                                UInt          * aNewNodeCount )
{
    smnbNode        * sNode;
    smnbNode        * sNewNodeList;
    smnbStack       * sStack;
    SInt              sCurDepth;
    SInt              sNewNodeCount;

    IDE_ASSERT( aIndexHeader != NULL );
    IDE_ASSERT( aStack       != NULL );
    IDE_ASSERT( aNewNodeList != NULL );
    IDE_ASSERT( aNewNodeCount != NULL );

    sStack    = aStack;
    sCurDepth = aDepth;

    *aNewNodeList  = NULL;
    *aNewNodeCount = 0;

    /* Leaf node�� full �Ǿ� split �ϴ� ���̱� ������ Leaf node�� ���� ���ο�
     * ���� �� �ʿ��ϰ� �ȴ�. �׷��� �ʿ��� ��� ������ 1���� ���� */
    sNewNodeCount = 1;

    for ( sCurDepth = aDepth; sCurDepth >= 0; sCurDepth-- )
    {
        sNode = (smnbNode*)sStack[sCurDepth].node;

        if ( (sNode->flag & SMNB_NODE_TYPE_MASK) == SMNB_NODE_TYPE_INTERNAL )
        {
            /* slot count�� max�� �����ߴٸ� split �������� ���ο� node��
             * �ʿ���ϰ� �ȴ�. */
            if ( sStack[sCurDepth].slotCount == SMNB_INTERNAL_SLOT_MAX_COUNT( aIndexHeader ) )
            {
                sNewNodeCount++;
            }
            else
            {
                IDE_ERROR_RAISE( sStack[sCurDepth].slotCount < SMNB_INTERNAL_SLOT_MAX_COUNT( aIndexHeader ), ERR_CORRUPTED_INDEX );
                break;
            }
        }
    }

    IDE_ERROR_RAISE( sNewNodeCount >= 1, ERR_CORRUPTED_INDEX );

    /* Root Node���� split �Ǿ�� �Ѵٸ�, ���ο� Root Node�� �����Ǿ�� �Ѵ�. */
    if ( sCurDepth < 0 )
    {
        sNewNodeCount++;
    }

    IDE_TEST( aIndexHeader->mNodePool.allocateSlots(
                                            sNewNodeCount,
                                            (smmSlot**)&sNewNodeList,
                                            SMM_SLOT_LIST_MUTEX_NEEDLESS )
              != IDE_SUCCESS );

    *aNewNodeList  = sNewNodeList;
    *aNewNodeCount = sNewNodeCount;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_CORRUPTED_INDEX )
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_INCONSISTENT_INDEX ) );

        ideLog::log( IDE_SM_0, "Index Corrupted Detected : Wrong Index SlotCount" );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

IDE_RC smnbBTree::insertRowUnique( idvSQL*           aStatistics,
                                   void*             aTrans,
                                   void *            aTable,
                                   void*             aIndex,
                                   smSCN             aInfiniteSCN,
                                   SChar*            aRow,
                                   SChar*            aNull,
                                   idBool            aUniqueCheck,
                                   smSCN             aStmtSCN,
                                   void*             aRowSID,
                                   SChar**           aExistUniqueRow,
                                   ULong             aInsertWaitTime )
{
    smnbHeader* sHeader;

    sHeader  = (smnbHeader*)((smnIndexHeader*)aIndex)->mHeader;

    if ( compareRows( &(sHeader->mStmtStat),
                      sHeader->columns,
                      sHeader->fence,
                      aRow,
                      aNull )
         != 0 )
    {
        aNull = NULL;
    }

    IDE_TEST( insertRow( aStatistics,
                         aTrans, 
                         aTable, 
                         aIndex, 
                         aInfiniteSCN,
                         aRow, 
                         aNull, 
                         aUniqueCheck, 
                         aStmtSCN, 
                         aRowSID,
                         aExistUniqueRow, 
                         aInsertWaitTime )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smnbBTree::insertRow( idvSQL*           aStatistics,
                             void*             a_pTrans,
                             void *            /* aTable */,
                             void*             a_pIndex,
                             smSCN             /* aInfiniteSCN */,
                             SChar*            a_pRow,
                             SChar*            a_pNull,
                             idBool            aUniqueCheck,
                             smSCN             aStmtSCN,
                             void*             /*aRowSID*/,
                             SChar**           aExistUniqueRow,
                             ULong             aInsertWaitTime )
{
    smnbHeader*  s_pIndexHeader;
    SInt         s_nDepth;
    SInt         s_nSlotPos;
    smnbStack    s_stack[SMNB_STACK_DEPTH];
    void*        s_pLeftNode;
    void       * s_pNewNode     = NULL;
    smnbNode*    s_pNewNodeList = NULL;
    UInt         s_nNewNodeCount = 0;
    smnbLNode  * s_pCurLNode    = NULL;
    smnbINode  * s_pCurINode    = NULL;
    smnbINode  * s_pRootINode   = NULL;
    smnbLNode  * sTargetNode    = NULL;
    SInt         sTargetSeq;
    smTID        s_transID = SM_NULL_TID;
    void       * s_pSepKeyRow1  = NULL;
    void       * sSepKey1       = NULL;
    idBool       sIsUniqueIndex;
    idBool       sIsTreeLatched = ID_FALSE;
    idBool       sIsNodeLatched = ID_FALSE;
    idBool       sIsRetraverse = ID_FALSE;
    idBool       sNeedTreeLatch = ID_FALSE;
    idBool       sIsNxtNodeLatched = ID_FALSE;
    smnIndexHeader * sIndex = ((smnIndexHeader*)a_pIndex);
    SChar          * s_diffRow = NULL;
    SInt             s_diffSlotPos = 0;
    smpSlotHeader  * s_diffRowSlot = NULL;
    SInt             s_pKeyRedistributeCount = 0;
    smnbLNode      * s_pNxtLNode = NULL;
    SChar          * s_pOldKeyRow = NULL;

    s_pIndexHeader = (smnbHeader*)sIndex->mHeader;
    sIsUniqueIndex = (((smnIndexHeader*)a_pIndex)->mFlag & SMI_INDEX_UNIQUE_MASK)
                         == SMI_INDEX_UNIQUE_ENABLE ? ID_TRUE : ID_FALSE;

restart:
    /* BUG-32742 [sm] Create DB fail in Altibase server
     *           Solaris x86���� DB ������ ���� hang �ɸ�.
     *           ������ �����Ϸ� ����ȭ ������ ���̰�, �̸� ȸ���ϱ� ����
     *           �Ʒ��� ���� ������ */
    if ( sNeedTreeLatch == ID_TRUE )
    {
        IDE_TEST( lockTree(s_pIndexHeader) != IDE_SUCCESS );
        sIsTreeLatched = ID_TRUE;
        sNeedTreeLatch = ID_FALSE;
    }

    s_nDepth = -1;

    //check version and retraverse.
    IDE_TEST( findPosition( s_pIndexHeader,
                            a_pRow,
                            &s_nDepth,
                            s_stack ) != IDE_SUCCESS );

    if ( s_nDepth < 0 )
    {
        /* root node�� ���� ���. Tree latch���, ��Ʈ ��� �����Ѵ�. */
        if ( sIsTreeLatched == ID_FALSE )
        {
            IDE_TEST( lockTree(s_pIndexHeader) != IDE_SUCCESS );
            sIsTreeLatched = ID_TRUE;
        }

        if ( s_pIndexHeader->root == NULL )
        {
            // Tree is empty...
            IDE_TEST( s_pIndexHeader->mNodePool.allocateSlots(
                                                1,
                                                (smmSlot**)&s_pCurLNode,
                                                SMM_SLOT_LIST_MUTEX_NEEDLESS )
                      != IDE_SUCCESS );

            // BUG-18292 : V$MEM_BTREE_HEADER ���� �߰�
            s_pIndexHeader->nodeCount++;

            initLeafNode( s_pCurLNode,
                          s_pIndexHeader,
                          IDU_LATCH_UNLOCKED );

            IDE_TEST( insertIntoLeafNode( s_pIndexHeader,
                                          s_pCurLNode,
                                          a_pRow,
                                          0 ) != IDE_SUCCESS );

            if ( needToUpdateStat( s_pIndexHeader->mIsMemTBS ) == ID_TRUE )
            {
                IDE_TEST( updateStat4Insert( sIndex,
                                             s_pIndexHeader,
                                             s_pCurLNode,
                                             0,
                                             a_pRow,
                                             sIsUniqueIndex ) != IDE_SUCCESS );
            }
            else
            {
                /* nothing to do ... */
            }

            IDL_MEM_BARRIER;

            s_pIndexHeader->root = s_pCurLNode;
        }
        else
        {
            goto restart;
        }
    }
    else
    {
        s_pCurLNode = (smnbLNode*)(s_stack[s_nDepth].node);

        IDE_ERROR_RAISE( ( s_pCurLNode->flag & SMNB_NODE_TYPE_MASK ) == SMNB_NODE_TYPE_LEAF, 
                         ERR_CORRUPTED_INDEX_FLAG );

        IDU_FIT_POINT( "smnbBTree::insertRow::beforeLock" );

        IDE_TEST( lockNode(s_pCurLNode) != IDE_SUCCESS );
        sIsNodeLatched = ID_TRUE;

        IDU_FIT_POINT( "smnbBTree::insertRow::afterLock" );

        /* �� ��忡 ���Դ�.
         * lock ��� ���� �̹� ��� freeSlot�Ǿ��� �� �ִ�. */
        if ( ( s_pCurLNode->mSlotCount == 0 ) || 
             ( ( s_pCurLNode->flag & SMNB_NODE_VALID_MASK ) == SMNB_NODE_INVALID ) )
        {
            IDE_ASSERT( sIsTreeLatched == ID_FALSE );

            sIsNodeLatched = ID_FALSE;
            IDE_TEST( unlockNode(s_pCurLNode) != IDE_SUCCESS );

            goto restart;
        }
        else
        {
            /* nothing to do */
        }

        IDE_TEST( findSlotInLeaf( s_pIndexHeader,
                                  s_pCurLNode,
                                  a_pRow,
                                  &s_nSlotPos ) != IDE_SUCCESS );

        if ( s_pCurLNode->mSlotCount > s_nSlotPos )
        {
            if ( s_pCurLNode->mRowPtrs[s_nSlotPos] == a_pRow )
            {
                IDE_CONT( skip_insert_row );
            }
            else
            {
                /* nothing to do */
            }
        }
        else
        {
            /* ������ leaf node�� ã�� leaf node���� ���� ��ġ�� ã�� ����
             * split�� �߻��ϰ� �Ǹ�, next node�� �����ؾ� ������
             * ���� ������ slot�� ���Ե� �� �ִ�.
             * �� ���·δ� next node ��� �����ؾ� �ϴ��� �� �� �����Ƿ�
             * �ٽ� �õ� �Ѵ�. */
            if ( s_pCurLNode->nextSPtr != NULL )
            {
                IDE_ASSERT( sIsTreeLatched == ID_FALSE );

                sIsNodeLatched = ID_FALSE;
                IDE_TEST( unlockNode(s_pCurLNode) != IDE_SUCCESS );

                goto restart;
            }
        }

        if ( ( a_pNull == NULL ) && ( aUniqueCheck == ID_TRUE ) )
        {
            IDE_TEST( checkUniqueness( sIndex,
                                       a_pTrans,
                                       &(s_pIndexHeader->mStmtStat),
                                       s_pIndexHeader->columns,
                                       s_pIndexHeader->fence,
                                       s_pCurLNode,
                                       s_nSlotPos,
                                       a_pRow,
                                       aStmtSCN,
                                       sIsTreeLatched,
                                       &s_transID,
                                       &sIsRetraverse,
                                       aExistUniqueRow,
                                       &s_diffRow,
                                       &s_diffSlotPos )
                      != IDE_SUCCESS );

            if ( sIsRetraverse == ID_TRUE )
            {
                IDE_ASSERT( sIsTreeLatched == ID_FALSE );

                sNeedTreeLatch = ID_TRUE;

                sIsNodeLatched = ID_FALSE;
                IDE_TEST( unlockNode(s_pCurLNode) != IDE_SUCCESS );

                goto restart;
            }

            // PROJ-1553 Replication self-deadlock
            // s_transID�� 0�� �ƴϸ� wait�ؾ� ������
            // ���� transaction�� sTransID�� transaction��
            // ��� replication tx�̰�, ���� ID�̸�
            // ������� �ʰ� �׳� pass�Ѵ�.
            if ( s_transID != SM_NULL_TID )
            {
                /* TASK-6548 duplicated unique key */
                IDE_TEST_RAISE( ( smxTransMgr::isReplTrans( a_pTrans ) == ID_TRUE ) &&
                                ( smxTransMgr::isSameReplID( a_pTrans, s_transID ) == ID_TRUE ) &&
                                ( smxTransMgr::isTransForReplConflictResolution( s_transID ) == ID_TRUE ),
                                ERR_UNIQUE_VIOLATION_REPL );

                if ( smLayerCallback::isWaitForTransCase( a_pTrans,
                                                          s_transID )
                     == ID_TRUE )
                {
                    IDE_RAISE( ERR_WAIT );
                }
                else
                {
                    /* Nothing to do */
                }
            }
            else
            {
                /* Nothing to do */
            }
        }

        if ( s_pCurLNode->mSlotCount < SMNB_LEAF_SLOT_MAX_COUNT( s_pIndexHeader ) )
        {
            SMNB_SCAN_LATCH(s_pCurLNode);

            IDE_ERROR_RAISE( s_pCurLNode->mSlotCount != 0, ERR_CORRUPTED_INDEX_SLOTCOUNT );

            IDE_TEST( insertIntoLeafNode( s_pIndexHeader,
                                s_pCurLNode,
                                a_pRow,
                                s_nSlotPos ) != IDE_SUCCESS );

            SMNB_SCAN_UNLATCH(s_pCurLNode);

            if ( needToUpdateStat( s_pIndexHeader->mIsMemTBS ) == ID_TRUE )
            {
                IDE_TEST( updateStat4Insert( sIndex,
                                             s_pIndexHeader,
                                             s_pCurLNode,
                                             s_nSlotPos,
                                             a_pRow,
                                             sIsUniqueIndex) != IDE_SUCCESS );
            }
            else
            {
                /* nothing to do ... */
            }

        }
        else
        {
            /* split/key redistribution�� �ʿ�.. Tree latch�� ��������� ��� �ٽ� �õ�. */
            if ( sIsTreeLatched == ID_FALSE )
            {
                sNeedTreeLatch = ID_TRUE;

                sIsNodeLatched = ID_FALSE;
                IDE_TEST( unlockNode(s_pCurLNode) != IDE_SUCCESS );

                goto restart;
            }

            /* PROJ-2613 Key Redistibution in MRDB Index */
            /* checkEnableKeyRedistribution�� ������ ���ڷ� s_pCurINode�� ���� ���
               compiler�� optimizing�� ���� code reordering�� �߻��Ͽ�
               s_pCurINode�� ���� �ޱ��� checkEnableKeyRedistribution�� ȣ���Ͽ� �״� ��찡 �߻��Ѵ�.
               �̸� ���� ���� checkEnableKeyRedistribution���� ���� stack���� ���� ���� �ѱ��. */

            /* BUG-42899 split ��� ��尡 �ֻ��� ����� ��� stack���� �θ��带 ������ �� ����. */
            if ( s_nDepth > 0 )
            {
                s_pCurINode = (smnbINode*)(s_stack[s_nDepth - 1].node);
            }
            else
            {
                /* �ش� ��尡 �ֻ��� ����� ��� Ű ��й踦 ������ �� ����. */
                s_pCurINode = NULL;
            }

            if ( ( s_pCurINode != NULL ) && 
                 ( checkEnableKeyRedistribution( s_pIndexHeader, 
                                                 s_pCurLNode,
                                                 (smnbINode*)(s_stack[s_nDepth - 1].node) ) 
                   == ID_TRUE ) )
            {
                /* Ű ��й踦 �����Ѵ�. */
                s_pNxtLNode = s_pCurLNode->nextSPtr;

                IDE_TEST( lockNode( s_pNxtLNode ) != IDE_SUCCESS );
                sIsNxtNodeLatched = ID_TRUE;
                IDU_FIT_POINT( "BUG-45283@smnbBTree::insertRow::lockNode" ); 

                if ( s_pNxtLNode->mSlotCount >= 
                     ( ( SMNB_LEAF_SLOT_MAX_COUNT( s_pIndexHeader ) *
                         smuProperty::getMemIndexKeyRedistributionStandardRate() / 100 ) ) )
                {
                    /* checkEnableKeyRedistribution ���� �� �̿� ��忡 lock�� ��� ���̿� �̿� ��忡
                       ���Կ����� �߻��Ͽ� ������ �ʰ��� ��쿡�� Ű ��й踦 �������� �ʴ´�. */
                    sNeedTreeLatch = ID_FALSE;
                    sIsTreeLatched = ID_FALSE;
                    IDE_TEST( unlockTree(s_pIndexHeader) != IDE_SUCCESS );

                    sIsNodeLatched = ID_FALSE;
                    IDE_TEST( unlockNode( s_pNxtLNode) != IDE_SUCCESS );

                    sIsNxtNodeLatched = ID_FALSE;
                    IDE_TEST( unlockNode( s_pCurLNode) != IDE_SUCCESS );

                    goto restart;
                }
                else
                {
                    /* do nothing... */
                }

                /* Ű ��й�� �̵� ��ų slot�� ���� ���Ѵ�. */
                s_pKeyRedistributeCount = calcKeyRedistributionPosition( s_pIndexHeader,
                                                                         s_pCurLNode->mSlotCount,
                                                                         s_pNxtLNode->mSlotCount );

                s_pOldKeyRow = s_pCurLNode->mRowPtrs[s_pCurLNode->mSlotCount - 1 ];

                /* Ű ��й踦 ������ slot�� �Ϻθ� �̿� ���� �̵��Ѵ�. */
                if ( keyRedistribute( s_pIndexHeader,
                                      s_pCurLNode, 
                                      s_pNxtLNode, 
                                      s_pKeyRedistributeCount ) != IDE_SUCCESS )
                {
                    /* �ش� ��� ������ �ٸ� Tx�� ���� split/aging�� �߻��� ����̹Ƿ� ������Ѵ�. */
                    sNeedTreeLatch = ID_FALSE;
                    sIsTreeLatched = ID_FALSE;
                    IDE_TEST( unlockTree(s_pIndexHeader) != IDE_SUCCESS );

                    sIsNodeLatched = ID_FALSE;
                    IDE_TEST( unlockNode( s_pNxtLNode) != IDE_SUCCESS );

                    sIsNxtNodeLatched = ID_FALSE;
                    IDE_TEST( unlockNode( s_pCurLNode) != IDE_SUCCESS );

                    goto restart; 
                }
                else
                {
                    /* do nothing... */
                }

                sIsNxtNodeLatched = ID_FALSE;
                IDE_TEST( unlockNode( s_pNxtLNode ) != IDE_SUCCESS );

                IDE_ASSERT( sIsNodeLatched == ID_TRUE );

                /* �θ� ��带 �����ϴ�. */
                IDE_TEST( keyRedistributionPropagate( s_pIndexHeader,
                                                     s_pCurINode,
                                                     s_pCurLNode,
                                                     s_pOldKeyRow) != IDE_SUCCESS );

                /* ���� ������ �����ϱ� ���� latch�� Ǯ�� ������Ѵ�. */
                sNeedTreeLatch = ID_FALSE;
                sIsTreeLatched = ID_FALSE;
                IDE_TEST( unlockTree(s_pIndexHeader) != IDE_SUCCESS );

                sIsNodeLatched = ID_FALSE;
                IDE_TEST( unlockNode(s_pCurLNode) != IDE_SUCCESS );

                goto restart;
            }
            else
            {   
                /* split�� �����Ѵ�. */

                /* split�� �ʿ��� ��� ������ŭ �̸� �Ҵ��صд�. */
                IDE_TEST( prepareNodes( s_pIndexHeader,
                                        s_stack,
                                        s_nDepth,
                                        &s_pNewNodeList,
                                        &s_nNewNodeCount )
                          != IDE_SUCCESS );

                IDE_ERROR_RAISE( s_pNewNodeList != NULL, ERR_CORRUPTED_INDEX_NEWNODE );
                IDE_ERROR_RAISE( s_nNewNodeCount > 0, ERR_CORRUPTED_INDEX_NEWNODE );


                // PROJ-1617
                s_pIndexHeader->mStmtStat.mNodeSplitCount++;

                getPreparedNode( &s_pNewNodeList,
                                 &s_nNewNodeCount,
                                 &s_pNewNode );

                IDE_ERROR_RAISE( s_pNewNode != NULL, ERR_CORRUPTED_INDEX_NEWNODE );

                SMNB_SCAN_LATCH(s_pCurLNode);

                splitLeafNode( s_pIndexHeader,
                               s_nSlotPos,
                               s_pCurLNode,
                               (smnbLNode*)s_pNewNode,
                               &sTargetNode,
                               &sTargetSeq );

                IDE_TEST( insertIntoLeafNode( s_pIndexHeader,
                                              sTargetNode,
                                              a_pRow,
                                              sTargetSeq ) != IDE_SUCCESS );

                /* PROJ-2433 */
                s_pSepKeyRow1 = NULL;
                sSepKey1      = NULL;

                getLeafSlot( (SChar **)&s_pSepKeyRow1,
                             &sSepKey1,
                             s_pCurLNode,
                             (SShort)( s_pCurLNode->mSlotCount - 1 ) );

                SMNB_SCAN_UNLATCH(s_pCurLNode);

                if ( needToUpdateStat( s_pIndexHeader->mIsMemTBS ) == ID_TRUE )
                {
                    IDE_TEST( updateStat4Insert( sIndex,
                                                 s_pIndexHeader,
                                                 sTargetNode,
                                                 sTargetSeq,
                                                 a_pRow,
                                                 sIsUniqueIndex ) != IDE_SUCCESS );
                }
                else
                {
                    /* nothing to do ... */
                }

                s_pLeftNode = s_pCurLNode;

                s_nDepth--;

                while (s_nDepth >= 0)
                {
                    s_pCurINode = (smnbINode*)(s_stack[s_nDepth].node);

                    SMNB_SCAN_LATCH(s_pCurINode);

                    /* PROJ-2433
                     * ���ο� new split���� child pointer ������ */
                    s_pCurINode->mChildPtrs[s_stack[s_nDepth].lstReadPos] = (smnbNode *)s_pNewNode;

                    IDE_TEST( findSlotInInternal( s_pIndexHeader,
                                                  s_pCurINode,
                                                  s_pSepKeyRow1,
                                                  &s_nSlotPos ) != IDE_SUCCESS );
                    IDE_ERROR_RAISE(s_nSlotPos == s_stack[s_nDepth].lstReadPos, ERR_CORRUPTED_INDEX_SLOTCOUNT);

                    if ( s_pCurINode->mSlotCount < SMNB_INTERNAL_SLOT_MAX_COUNT( s_pIndexHeader ) )
                    {
                        IDE_TEST( insertIntoInternalNode( s_pIndexHeader,
                                                          s_pCurINode,
                                                          s_pSepKeyRow1,
                                                          sSepKey1,
                                                          s_pLeftNode ) != IDE_SUCCESS );

                        SMNB_SCAN_UNLATCH(s_pCurINode);

                        break;
                    }
                    else
                    {
                        /* nothing to do */
                    }

                    getPreparedNode( &s_pNewNodeList,
                                     &s_nNewNodeCount,
                                     &s_pNewNode );

                    IDE_ERROR_RAISE( s_pNewNode != NULL, ERR_CORRUPTED_INDEX_NEWNODE );

                    IDE_TEST( splitInternalNode( s_pIndexHeader,
                                                 s_pSepKeyRow1,
                                                 sSepKey1,
                                                 s_pLeftNode,
                                                 s_nSlotPos,
                                                 s_pCurINode,
                                                 (smnbINode*)s_pNewNode,
                                                 &s_pSepKeyRow1, 
                                                 &sSepKey1 ) != IDE_SUCCESS );

                    SMNB_SCAN_UNLATCH(s_pCurINode);

                    s_pLeftNode = s_pCurINode;
                    s_nDepth--;
                }

                if ( s_nDepth < 0 )
                {
                    getPreparedNode( &s_pNewNodeList,
                                     &s_nNewNodeCount,
                                     (void**)&s_pRootINode );

                    IDE_ERROR_RAISE( s_pRootINode != NULL, ERR_CORRUPTED_INDEX_NEWNODE );

                    // BUG-18292 : V$MEM_BTREE_HEADER ���� �߰�
                    s_pIndexHeader->nodeCount++;

                    initInternalNode( s_pRootINode,
                                      s_pIndexHeader,
                                      IDU_LATCH_UNLOCKED );

                    s_pRootINode->mSlotCount = 2;

                    setInternalSlot( s_pRootINode,
                                     0,
                                     (smnbNode *)s_pLeftNode,
                                     (SChar *)s_pSepKeyRow1,
                                     sSepKey1 );

                    setInternalSlot( s_pRootINode,
                                     1,
                                     (smnbNode *)s_pNewNode,
                                     (SChar *)NULL,
                                     (void *)NULL );

                    IDL_MEM_BARRIER;
                    s_pIndexHeader->root = s_pRootINode;
                }
                else
                {
                    /* nothing to do */
                }
            }
        }
    }

    IDE_EXCEPTION_CONT( skip_insert_row );

    if ( sIsNodeLatched == ID_TRUE )
    {
        sIsNodeLatched = ID_FALSE;
        IDE_TEST( unlockNode(s_pCurLNode) != IDE_SUCCESS );
    }

    if ( ( sIsNxtNodeLatched == ID_TRUE ) && ( s_pNxtLNode != NULL ) )
    {
        sIsNxtNodeLatched = ID_FALSE;
        IDE_TEST( unlockNode(s_pNxtLNode) != IDE_SUCCESS );
    }
    else
    {
        /* do nothing... */
    }

    if ( sIsTreeLatched == ID_TRUE )
    {
        sIsTreeLatched = ID_FALSE;
        IDE_TEST( unlockTree(s_pIndexHeader) != IDE_SUCCESS );
    }

    if ( s_nNewNodeCount > 0 )
    {
        IDE_DASSERT( 0 );
        IDE_TEST( s_pIndexHeader->mNodePool.releaseSlots(
                                             s_nNewNodeCount,
                                             (smmSlot*)s_pNewNodeList,
                                             SMM_SLOT_LIST_MUTEX_NEEDLESS )
                  != IDE_SUCCESS );
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_WAIT );

    IDE_ERROR_RAISE( s_pNewNodeList == NULL, ERR_CORRUPTED_INDEX_NEWNODE );

    if ( sIsNodeLatched == ID_TRUE )
    {
        sIsNodeLatched = ID_FALSE;
        IDE_TEST( unlockNode(s_pCurLNode) != IDE_SUCCESS );
    }

    if ( ( sIsNxtNodeLatched == ID_TRUE ) && ( s_pNxtLNode != NULL ) )
    {
        sIsNxtNodeLatched = ID_FALSE;
        IDE_TEST( unlockNode(s_pNxtLNode) != IDE_SUCCESS );
    }
    else
    {
        /* do nothing... */
    }

    if ( sIsTreeLatched == ID_TRUE )
    {
        sIsTreeLatched = ID_FALSE;
        IDE_TEST( unlockTree(s_pIndexHeader) != IDE_SUCCESS );
    }

    /* BUG-38198 Session�� ���� ����ִ��� üũ�Ͽ�
     * timeout������ ������ٸ� �ش� ������ dump�� fail ó���Ѵ�. */
    IDE_TEST_RAISE( iduCheckSessionEvent( aStatistics ) != IDE_SUCCESS, 
                    err_SESSIONEND );

    IDE_TEST( smLayerCallback::waitForTrans( a_pTrans,    /* aTrans        */ 
                                             s_transID,   /* aWaitTransID  */  
                                             aInsertWaitTime /* aLockWaitTime */)
              != IDE_SUCCESS);

    goto restart;

    IDE_EXCEPTION( err_SESSIONEND );
    {
        s_diffRowSlot = (smpSlotHeader*)s_diffRow;

        ideLog::logMem( IDE_DUMP_0,
                        (UChar*)SMP_GET_PERS_PAGE_HEADER( s_diffRow ),
                        SM_PAGE_SIZE,
                        "[ Index Insert Timeout Infomation]\n"
                        "PageID     :%"ID_UINT32_FMT"\n"
                        "TID        :%"ID_UINT32_FMT"\n"
                        "Slot Number:%"ID_UINT32_FMT"\n"
                        "Slot Offset:%"ID_UINT64_FMT"\n"
                        "CreateSCN  :%"ID_UINT64_FMT"\n",
                        SMP_SLOT_GET_PID( s_diffRowSlot ),
                        s_transID, 
                        s_diffSlotPos,
                        (ULong)SMP_SLOT_GET_OFFSET( s_diffRowSlot ),
                        SM_SCN_TO_LONG( s_diffRowSlot->mCreateSCN ) ); 
    }
    IDE_EXCEPTION( ERR_CORRUPTED_INDEX_FLAG )
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_INCONSISTENT_INDEX ) );

        ideLog::log( IDE_SM_0, "Index Corrupted Detected : Wrong Index Flag" );
    }
    IDE_EXCEPTION( ERR_CORRUPTED_INDEX_SLOTCOUNT )
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_INCONSISTENT_INDEX ) );

        ideLog::log( IDE_SM_0, "Index Corrupted Detected : Wrong Index Node SlotCount" );
    }
    IDE_EXCEPTION( ERR_CORRUPTED_INDEX_NEWNODE )
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_INCONSISTENT_INDEX ) );

        ideLog::log( IDE_SM_0, "Index Corrupted Detected : Cannot Alloc New Node" );
    }
    IDE_EXCEPTION( ERR_UNIQUE_VIOLATION_REPL )
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_smnUniqueViolationInReplTrans ) );
    }
    IDE_EXCEPTION_END;

    if (sIsNodeLatched == ID_TRUE)
    {
        sIsNodeLatched = ID_FALSE;
        IDE_ASSERT( unlockNode(s_pCurLNode) == IDE_SUCCESS );
    }

    if ( ( sIsNxtNodeLatched == ID_TRUE ) && ( s_pNxtLNode != NULL ) )
    {
        sIsNxtNodeLatched = ID_FALSE;
        IDE_ASSERT( unlockNode(s_pNxtLNode) == IDE_SUCCESS );
    }

    if ( sIsTreeLatched == ID_TRUE )
    {
        sIsTreeLatched = ID_FALSE;
        IDE_ASSERT( unlockTree(s_pIndexHeader) == IDE_SUCCESS );
    }

    if ( s_nNewNodeCount > 0 )
    {
        IDE_ASSERT( s_pIndexHeader->mNodePool.releaseSlots(
                                               s_nNewNodeCount,
                                               (smmSlot*)s_pNewNodeList,
                                               SMM_SLOT_LIST_MUTEX_NEEDLESS )
                    == IDE_SUCCESS );
    }

    if ( ideGetErrorCode() == smERR_ABORT_INCONSISTENT_INDEX )
    {
        smnManager::setIsConsistentOfIndexHeader( sIndex, ID_FALSE );
    }

    return IDE_FAILURE;
}

/*********************************************************************
 * FUNCTION DESCRIPTION : smnbBTree::findFirstSlotInInternal         *
 * ------------------------------------------------------------------*
 * INTERNAL NODE ������ ���� ù��°������ row�� ��ġ�� ã�´�.
 * ( ������ key�� �����ΰ�� ���� ����(ù��°)�� slot ��ġ�� ã�� )
 *
 * case1. �Ϲ� index�̰ų�, partial key index�� �ƴѰ��
 *   case1-1. direct key index�� ���
 *            : �Լ� findFirstKeyInInternal() ������ direct key ������� �˻�
 *   case1-2. �Ϲ� index�� ���
 *            : �Լ� findFirstRowInInternal() ������ row ������� �˻�
 *
 * case2. partial key index�ΰ��
 *        : �Լ� findFirstKeyInInternal() ������ direct key ������� �˻���
 *          �Լ� callback() �Ǵ� findFirstRowInInternal() ������ ��˻�
 *
 *  => partial key ���� ��˻��� �ʿ��� ����
 *    1. partial key�� �˻��� ��ġ�� ��Ȯ�� ��ġ�� �ƴҼ��ֱ⶧����
 *       full key�� ��˻��� �Ͽ� Ȯ���Ͽ��� �Ѵ�
 *
 * aHeader   - [IN]  INDEX ���
 * aCallBack - [IN]  Range Callback
 * aNode     - [IN]  INTERNAL NODE
 * aMinimum  - [IN]  �˻��� slot���� �ּҰ�
 * aMaximum  - [IN]  �˻��� slot���� �ִ밪
 * aSlot     - [OUT] ã���� slot ��ġ
 *********************************************************************/
inline void smnbBTree::findFirstSlotInInternal( smnbHeader          * aHeader,
                                                const smiCallBack   * aCallBack,
                                                const smnbINode     * aNode,
                                                SInt                  aMinimum,
                                                SInt                  aMaximum,
                                                SInt                * aSlot )
{
    idBool  sResult;

    if ( aHeader->mIsPartialKey == ID_FALSE )
    {
        /* full direct key */
        if ( SMNB_IS_DIRECTKEY_IN_NODE( aNode ) == ID_TRUE )
        {
            /* ����Ű�ΰ�� ù�÷���key�� ���÷�����row��ó���Ѵ� */
            findFirstKeyInInternal( aHeader,
                                    aCallBack,
                                    aNode,
                                    aMinimum,
                                    aMaximum,
                                    aSlot );
            return;
        }
        /* direct key ���� */
        else
        {
            findFirstRowInInternal( aCallBack,
                                    aNode,
                                    aMinimum,
                                    aMaximum,
                                    aSlot );
            return;
        }
    }
    else
    {
        /* partial key */
        findFirstKeyInInternal( aHeader,
                                aCallBack,
                                aNode,
                                aMinimum,
                                aMaximum,
                                aSlot );

        if ( *aSlot > aMaximum )
        {
            /* ������������. */
            return;
        }
        else
        {
            if ( aNode->mRowPtrs[*aSlot] == NULL )
            {
                /* ������������. */
                return;
            }
            else
            {
                /* nothing to do */
            }
        }

        /* partial key��, �ش� node�� key�� ��� ���Ե� �����, key�� ���Ѵ�  */
        if ( isFullKeyInInternalSlot( aHeader,
                                      aNode,
                                      (SShort)(*aSlot) ) == ID_TRUE )
        {
            aCallBack->callback( &sResult,
                                 aNode->mRowPtrs[*aSlot],
                                 SMNB_GET_KEY_PTR( aNode, *aSlot ),
                                 0,
                                 SC_NULL_GRID,
                                 aCallBack->data );
        }
        else
        {
            aCallBack->callback( &sResult,
                                 aNode->mRowPtrs[*aSlot],
                                 NULL,
                                 0,
                                 SC_NULL_GRID,
                                 aCallBack->data );
        }

        if ( sResult == ID_TRUE )
        {
            return;
        }
        else
        {
            aMinimum = *aSlot + 1; /* *aSlot�� ã�� slot�� �ƴѰ��� Ȯ���ߴ�. skip �Ѵ�. */

            if ( aMinimum <= aMaximum )
            {
                findFirstRowInInternal( aCallBack,
                                        aNode,
                                        aMinimum,
                                        aMaximum,
                                        aSlot );
            }
            else
            {
                /* PROJ-2433
                 * node �������� �����ϴ°��̾���. */
                *aSlot = aMaximum + 1;
            }

            return;
        }
    }
}

void smnbBTree::findFirstRowInInternal( const smiCallBack   * aCallBack,
                                        const smnbINode     * aNode,
                                        SInt                  aMinimum,
                                        SInt                  aMaximum,
                                        SInt                * aSlot )
{
    SInt      sMedium = 0;
    idBool    sResult;
    SChar   * sRow    = NULL;

    IDE_ASSERT( aNode->mRowPtrs != NULL );

    do
    {
        sMedium = (aMinimum + aMaximum) >> 1;
        sRow    = aNode->mRowPtrs[sMedium];

        if ( sRow == NULL )
        {
            sResult = ID_TRUE;
        }
        else
        {
            // BUG-27948 �̹� ������ Row�� ���� �ִ��� Ȯ��
            IDE_DASSERT( !SM_SCN_IS_FREE_ROW(((smpSlotHeader *)sRow)->mCreateSCN) );

            (void)aCallBack->callback( &sResult,
                                       sRow,
                                       NULL,
                                       0,
                                       SC_NULL_GRID,
                                       aCallBack->data );
        }

        if ( sResult == ID_TRUE )
        {
            aMaximum = sMedium - 1;
            *aSlot   = (UInt)sMedium;
        }
        else
        {
            aMinimum = sMedium + 1;
            *aSlot   = (UInt)aMinimum;
        }
    }
    while (aMinimum <= aMaximum);
}

void smnbBTree::findFirstKeyInInternal( smnbHeader          * aHeader,
                                        const smiCallBack   * aCallBack,
                                        const smnbINode     * aNode,
                                        SInt                  aMinimum,
                                        SInt                  aMaximum,
                                        SInt                * aSlot )
{
    SInt      sMedium           = 0;
    idBool    sResult;
    SChar   * sRow              = NULL;
    void    * sKey              = NULL;
    UInt      sPartialKeySize   = ( aHeader->mIsPartialKey == ID_TRUE ) ? (aNode->mKeySize) : 0;

    do
    {
        sMedium = (aMinimum + aMaximum) >> 1;
        sKey    = SMNB_GET_KEY_PTR( aNode, sMedium );
        sRow    = aNode->mRowPtrs[sMedium];
     
        if ( sRow == NULL )
        {
            sResult = ID_TRUE;
        }
        else
        {
            (void)aCallBack->callback( &sResult,
                                       sRow,
                                       sKey,
                                       sPartialKeySize,
                                       SC_NULL_GRID,
                                       aCallBack->data );
        }

        if ( sResult == ID_TRUE )
        {
            aMaximum = sMedium - 1;
            *aSlot   = (UInt)sMedium;
        }
        else
        {
            aMinimum = sMedium + 1;
            *aSlot   = (UInt)aMinimum;
        }
    }
    while (aMinimum <= aMaximum);
}

/*********************************************************************
 * FUNCTION DESCRIPTION : smnbBTree::findFirstSlotInLeaf             *
 * ------------------------------------------------------------------*
 * LEAF NODE ������ ���� ù��°������ row�� ��ġ�� ã�´�.
 * ( ������ key�� �����ΰ�� ���� ����(ù��°)�� slot ��ġ�� ã�� )
 *
 * case1. �Ϲ� index�̰ų�, partial key index�� �ƴѰ��
 *   case1-1. direct key index�� ���
 *            : �Լ� findFirstKeyInLeaf() ������ direct key ������� �˻�
 *   case1-2. �Ϲ� index�� ���
 *            : �Լ� findFirstRowInLeaf() ������ row ������� �˻�
 *
 * case2. partial key index�ΰ��
 *        : �Լ� findFirstKeyInLeaf() ������ direct key ������� �˻���
 *          �Լ� callback() �Ǵ� findFirstRowInLeaf() ������ ��˻�
 *
 *  => partial key ���� ��˻��� �ʿ��� ����
 *    1. partial key�� �˻��� ��ġ�� ��Ȯ�� ��ġ�� �ƴҼ��ֱ⶧����
 *       full key�� ��˻��� �Ͽ� Ȯ���Ͽ��� �Ѵ�
 *
 * aHeader   - [IN]  INDEX ���
 * aCallBack - [IN]  Range Callback
 * aNode     - [IN]  LEAF NODE
 * aMinimum  - [IN]  �˻��� slot���� �ּҰ�
 * aMaximum  - [IN]  �˻��� slot���� �ִ밪
 * aSlot     - [OUT] ã���� slot ��ġ
 *********************************************************************/
inline void smnbBTree::findFirstSlotInLeaf( smnbHeader          * aHeader,
                                            const smiCallBack   * aCallBack,
                                            const smnbLNode     * aNode,
                                            SInt                  aMinimum,
                                            SInt                  aMaximum,
                                            SInt                * aSlot )
{
    idBool  sResult;

    if ( aHeader->mIsPartialKey == ID_FALSE )
    {
        /* full direct key */
        if ( SMNB_IS_DIRECTKEY_IN_NODE( aNode ) == ID_TRUE )
        {
            /* ����Ű�ΰ�� ù�÷���key�� ���÷����� row��ó���Ѵ� */
            findFirstKeyInLeaf( aHeader,
                                aCallBack,
                                aNode,
                                aMinimum,
                                aMaximum,
                                aSlot );
            IDE_CONT( end );
        }
        /* direct key ���� */
        else
        {
            findFirstRowInLeaf( aCallBack,
                                aNode,
                                aMinimum,
                                aMaximum,
                                aSlot );
            IDE_CONT( end );
        }
    }
    /* partial key */
    else
    {
        findFirstKeyInLeaf( aHeader,
                            aCallBack,
                            aNode,
                            aMinimum,
                            aMaximum,
                            aSlot );

        if ( *aSlot > aMaximum )
        {
            /* ������������. */
            IDE_CONT( end );
        }
        else
        {
            if ( aNode->mRowPtrs[*aSlot] == NULL )
            {
                /* ������������. */
                IDE_CONT( end );
            }
            else
            {
                /* nothing to do */
            }
        }

        if ( isFullKeyInLeafSlot( aHeader,
                                  aNode,
                                  (SShort)(*aSlot) ) == ID_TRUE )
        {
            aCallBack->callback( &sResult,
                                 aNode->mRowPtrs[*aSlot],
                                 SMNB_GET_KEY_PTR( aNode, *aSlot ),
                                 0,
                                 SC_NULL_GRID,
                                 aCallBack->data );
        }
        else
        {
            aCallBack->callback( &sResult,
                                 aNode->mRowPtrs[*aSlot],
                                 NULL,
                                 0,
                                 SC_NULL_GRID,
                                 aCallBack->data );
        }

        if ( sResult == ID_TRUE )
        {
            IDE_CONT( end );
        }
        else
        {
            aMinimum = *aSlot + 1; /* *aSlot�� ã�� slot�� �ƴѰ��� Ȯ���ߴ�. skip �Ѵ�. */

            if ( aMinimum <= aMaximum )
            {
                findFirstRowInLeaf( aCallBack,
                                    aNode,
                                    aMinimum,
                                    aMaximum,
                                    aSlot );
            }
            else
            {
                /* PROJ-2433
                 * node �������� �����ϴ°��̾���. */
                *aSlot = aMaximum + 1;
            }

            IDE_CONT( end );
        }
    }

    IDE_EXCEPTION_CONT( end );

    return;
}

void smnbBTree::findFirstRowInLeaf( const smiCallBack   * aCallBack,
                                    const smnbLNode     * aNode,
                                    SInt                  aMinimum,
                                    SInt                  aMaximum,
                                    SInt                * aSlot )
{
    SInt      sMedium   = 0;
    idBool    sResult;
    SChar   * sRow      = NULL;

    do
    {
        sMedium = (aMinimum + aMaximum) >> 1;
        sRow    = aNode->mRowPtrs[sMedium];

        if ( sRow == NULL )
        {
            sResult = ID_TRUE;
        }
        else
        {
            // BUG-27948 �̹� ������ Row�� ���� �ִ��� Ȯ��
            IDE_DASSERT( !SM_SCN_IS_FREE_ROW(((smpSlotHeader *)sRow)->mCreateSCN) );

            (void)aCallBack->callback( &sResult,
                                       sRow,
                                       NULL,
                                       0,
                                       SC_NULL_GRID,
                                       aCallBack->data );
        }

        if ( sResult == ID_TRUE )
        {
            aMaximum = sMedium - 1;
            *aSlot   = (UInt)sMedium;
        }
        else
        {
            aMinimum = sMedium + 1;
            *aSlot   = (UInt)aMinimum;
        }
    }
    while (aMinimum <= aMaximum);
}

void smnbBTree::findFirstKeyInLeaf( smnbHeader          * aHeader,
                                    const smiCallBack   * aCallBack,
                                    const smnbLNode     * aNode,
                                    SInt                  aMinimum,
                                    SInt                  aMaximum,
                                    SInt                * aSlot )
{
    SInt      sMedium           = 0;
    idBool    sResult;
    SChar   * sRow              = NULL;
    void    * sKey              = NULL;
    UInt      sPartialKeySize   = ( aHeader->mIsPartialKey == ID_TRUE ) ? (aNode->mKeySize) : 0;

    do
    {
        sMedium = (aMinimum + aMaximum) >> 1;
        sKey    = SMNB_GET_KEY_PTR( aNode, sMedium );
        sRow    = aNode->mRowPtrs[sMedium];

        if ( sRow == NULL )
        {
            sResult = ID_TRUE;
        }
        else
        {
            (void)aCallBack->callback( &sResult,
                                       sRow, /* composite key index�� ù��°���� �÷��� ���ϱ����ؼ�*/
                                       sKey,
                                       sPartialKeySize,
                                       SC_NULL_GRID,
                                       aCallBack->data );
        }

        if ( sResult == ID_TRUE )
        {
            aMaximum = sMedium - 1;
            *aSlot   = (UInt)sMedium;
        }
        else
        {
            aMinimum = sMedium + 1;
            *aSlot   = (UInt)aMinimum;
        }
    }
    while (aMinimum <= aMaximum);
}

IDE_RC smnbBTree::findFirst( smnbHeader        * a_pIndexHeader,
                             const smiCallBack * a_pCallBack,
                             SInt              * a_pDepth,
                             smnbStack         * a_pStack )
{
    SInt          s_nDepth    = -1;
    smnbLNode   * s_pCurLNode = NULL;

    if ( a_pIndexHeader->root != NULL )
    {
        findFstLeaf( a_pIndexHeader,
                     a_pCallBack,
                     &s_nDepth,
                     a_pStack,
                     &s_pCurLNode );

        if ( s_pCurLNode != NULL )
        {
            IDE_ERROR_RAISE( -1 <= s_nDepth, ERR_CORRUPTED_INDEX_NODE_DEPTH );

            s_nDepth++;
            a_pStack[s_nDepth].node = s_pCurLNode;
        }
        else
        {
            /* nothing to do */
        }
    }
    else
    {
        /* nothing to do */
    }

    // BUG-26625 �޸� �ε��� ��ĵ�� SMO�� �����ϴ� ������ ������ �ֽ��ϴ�.
    IDE_ERROR_RAISE( ( s_nDepth < 0 ) ||
                     ( ( s_pCurLNode->flag & SMNB_NODE_TYPE_MASK ) == SMNB_NODE_TYPE_LEAF ),
                     ERR_CORRUPTED_INDEX_FLAG );

    *a_pDepth = s_nDepth;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_CORRUPTED_INDEX_NODE_DEPTH )
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_INCONSISTENT_INDEX ) );

        ideLog::log( IDE_SM_0, "Index Corrupted Detected : Wrong Index Node Depth" ); 
    }
    IDE_EXCEPTION( ERR_CORRUPTED_INDEX_FLAG )
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_INCONSISTENT_INDEX ) );

        ideLog::log( IDE_SM_0, "Index Corrupted Detected : Wrong Index Node Flag" ); 
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/* BUG-26625 [SN] �޸� �ε��� ��ĵ�� SMO�� �����ϴ� ������ ������ �ֽ��ϴ�.
 *
 * findFstLeaf�� ����� �� ����
 * 1) 0 <= a_pDepth
 *      ���������� Traverse�Ͽ� Leaf��带 Ž���� -> ����
 *
 * 2) -1 == a_pDepth
 *      root node �ϳ��� �����Ͽ�, root�� leaf�� ��Ȳ. while�� ���� �ʰ�
 *      �ٷ� ������ -> ����
 *
 * 3) slotCount == 0
 *      �ش� ��带 Ž���ϱ� ���� ��尡 ������
 *      3.1) Stack up, ���� ��� ��Ž��
 *          (Stack�� ������� Root���� �ٽ�)
 *      3.2) 3.1�� ��������� ��Ž���� ��尡 ����
 *          Ž���� ����� ������ ����̱� ������ ����� ����� ����.
 *          �� Ž���� �ʿ䰡 ���� -> �� ã��, LNode = NULL
 *
 *  4) slotPos == slotCount
 *      �ش� ��带 Ž���ϱ� ���� SMO�� �Ͼ ��ǥ Ű�� �̿� ��忡 ����
 *      4.1) �̿� ��尡 ����
 *           ���������� �̿� ��带 �� ã�Ƶ�. -> �̿���� ��Ž��
 *      4.2) �̿� ��尡 ����
 *           ���� ������ ��Ž��. (3.1�� ����)
 * */

void smnbBTree::findFstLeaf( smnbHeader         * a_pIndexHeader,
                             const smiCallBack  * a_pCallBack,
                             SInt               * a_pDepth,
                             smnbStack          * a_pStack,
                             smnbLNode         ** a_pLNode)
{
    /* BUG-37643 Node�� array�� ���������� �����ϴµ�
     * compiler ����ȭ�� ���ؼ� ���������� ���ŵ� �� �ִ�.
     * ���� �̷��� ������ volatile�� �����ؾ� �Ѵ�. */
    SInt                 s_nSlotCount;
    SInt                 s_nLstReadPos;
    SInt                 s_nDepth;
    SInt                 s_nSlotPos;
    volatile smnbINode * s_pCurINode;
    volatile smnbINode * s_pChildINode;
    volatile smnbINode * s_pCurSINode;
    IDU_LATCH            s_version;
    IDU_LATCH            sNewVersion = IDU_LATCH_UNLOCKED;

    s_nDepth = *a_pDepth;

    if ( s_nDepth == -1 )
    {
        s_pCurINode    = (smnbINode*)a_pIndexHeader->root;
        s_nLstReadPos  = -1;
    }
    else
    {
        s_pCurINode    = (smnbINode*)(a_pStack->node);
        s_nLstReadPos  = a_pStack->lstReadPos;
    }

    if ( s_pCurINode != NULL )
    {
        while((s_pCurINode->flag & SMNB_NODE_TYPE_MASK) == SMNB_NODE_TYPE_INTERNAL)
        {
            s_nLstReadPos++;
            s_version     = getLatchValueOfINode(s_pCurINode) & (~SMNB_SCAN_LATCH_BIT);
            IDL_MEM_BARRIER;

            while(1)
            {
                s_nSlotCount  = s_pCurINode->mSlotCount;
                s_pCurSINode  = s_pCurINode->nextSPtr;

                if ( s_nSlotCount != 0 )
                {
                    findFirstSlotInInternal( a_pIndexHeader,
                                             a_pCallBack,
                                             (smnbINode*)s_pCurINode,
                                             s_nLstReadPos,
                                             s_nSlotCount - 1,
                                             &s_nSlotPos );
                    s_pChildINode = (smnbINode*)(s_pCurINode->mChildPtrs[s_nSlotPos]);
                }
                else
                {
                    s_pChildINode = NULL;
                }

                IDL_MEM_BARRIER;

                sNewVersion = getLatchValueOfINode(s_pCurINode);

                IDL_MEM_BARRIER;

                if ( s_version == sNewVersion )
                {
                    break;
                }
                else
                {
                    s_version = sNewVersion & (~SMNB_SCAN_LATCH_BIT);
                }

                IDL_MEM_BARRIER;
            }

            s_nLstReadPos = -1;

            if ( s_nSlotCount == 0 ) //Case 3)
            {
                if ( s_nDepth == -1 ) //Case 3.1)
                {
                    s_pCurINode = (smnbINode*)a_pIndexHeader->root;
                }
                else
                {
                    s_nDepth--;
                    a_pStack--;
                    s_pCurINode = (smnbINode*)(a_pStack->node);
                }

                if ( s_pCurINode == NULL ) //Case 3.2)
                {
                    s_nDepth = -1;
                    break;
                }

                continue;
            }

            if ( s_nSlotCount == s_nSlotPos ) //Case 4
            {
                s_pCurINode = s_pCurSINode;

                if ( s_pCurINode == NULL ) //Case 4.2
                {
                    if ( s_nDepth == -1 )
                    {
                        s_pCurINode = (smnbINode*)a_pIndexHeader->root;
                    }
                    else
                    {
                        s_nDepth--;
                        a_pStack--;
                        s_pCurINode = (smnbINode*)(a_pStack->node);
                    }
                }

                continue;
            }

            //Case 1)
            a_pStack->node       = (smnbINode*)s_pCurINode;
            a_pStack->version    = s_version;
            a_pStack->lstReadPos = s_nSlotPos;

            s_nDepth++;
            a_pStack++;

            s_pCurINode   = s_pChildINode;
            s_nLstReadPos = -1;

            // BUG-26625 �޸� �ε��� ��ĵ�� SMO�� �����ϴ� ������ ������ �ֽ��ϴ�.
        }
    }

    *a_pDepth = s_nDepth;
    *a_pLNode = (smnbLNode*)s_pCurINode;
}

IDE_RC smnbBTree::updateStat4Delete( smnIndexHeader  * aPersIndex,
                                     smnbHeader      * aIndex,
                                     smnbLNode       * a_pLeafNode,
                                     SChar           * a_pRow,
                                     SInt            * a_pSlotPos,
                                     idBool            aIsUniqueIndex )
{
    SInt               s_nSlotPos;
    smnbLNode        * sNextNode = NULL;
    smnbLNode        * sPrevNode = NULL;
    SChar            * sPrevRowPtr;
    SChar            * sNextRowPtr;
    IDU_LATCH          sVersion;
    smnbColumn       * sFirstColumn = &aIndex->columns[0];  // To fix BUG-26469
    SInt               sCompareResult;
    SChar            * sStatRow;
    SLong            * sNumDistPtr;

    s_nSlotPos = *a_pSlotPos;

    if ( ( s_nSlotPos == a_pLeafNode->mSlotCount ) ||
         ( a_pLeafNode->mRowPtrs[s_nSlotPos] != a_pRow ) )
    {
        IDE_CONT( skip_update_stat );
    }
    else
    {
        /* nothing to do */
    }

    if ( smnbBTree::isNullKey( aIndex, a_pRow ) == ID_TRUE )
    {
        IDE_CONT( skip_update_stat );
    }

    // get next slot
    if ( s_nSlotPos == ( a_pLeafNode->mSlotCount - 1 ) )
    {
        if ( a_pLeafNode->nextSPtr == NULL )
        {
            sNextRowPtr = NULL;
        }
        else
        {
            while(1)
            {
                sNextNode = (smnbLNode*)a_pLeafNode->nextSPtr;
                sVersion  = getLatchValueOfLNode( sNextNode ) & IDU_LATCH_UNMASK;

                IDL_MEM_BARRIER;

                sNextRowPtr = sNextNode->mRowPtrs[0];

                IDL_MEM_BARRIER;

                if ( sVersion == getLatchValueOfLNode(sNextNode) )
                {
                    break;
                }
            }
        }
    }
    else
    {
        IDE_DASSERT( ( s_nSlotPos + 1 ) >= 0 );
        sNextRowPtr = a_pLeafNode->mRowPtrs[s_nSlotPos + 1];
    }
    // get prev slot
    if ( s_nSlotPos == 0 )
    {
        if ( a_pLeafNode->prevSPtr == NULL )
        {
            sPrevRowPtr = NULL;
        }
        else
        {
            while(1)
            {
                sPrevNode = (smnbLNode*)a_pLeafNode->prevSPtr;
                sVersion  = getLatchValueOfLNode( sPrevNode ) & IDU_LATCH_UNMASK;

                IDL_MEM_BARRIER;

                sPrevRowPtr = sPrevNode->mRowPtrs[sPrevNode->mSlotCount - 1];

                IDL_MEM_BARRIER;

                if ( sVersion == getLatchValueOfLNode(sPrevNode) )
                {
                    break;
                }
            }
        }
    }
    else
    {
        sPrevRowPtr = a_pLeafNode->mRowPtrs[s_nSlotPos - 1];
    }

    // update index statistics

    // To fix BUG-26469
    // sColumn[0] �� ����ϸ� �ȵȴ�.
    // sColumn �� ��ü Ű�÷��� NULL ���� üũ�ϴ� �������� position �� �Ű�����.
    // �׷��� sColumn[0] �� ���� 0��°�� �ƴ� �Ű��� position �̴�.
    if ( isNullColumn( sFirstColumn,
                       &(sFirstColumn->column),
                       a_pRow ) != ID_TRUE )
    {
        if ( (a_pLeafNode->prevSPtr == NULL) && (s_nSlotPos == 0) )
        {
            if ( ( sNextRowPtr == NULL ) ||
                 ( isNullColumn( sFirstColumn,
                                 &(sFirstColumn->column),
                                 sNextRowPtr ) == ID_TRUE ) )
            {
                /* setMinMaxStat ���� Row�� NULL�̸� ������� �ʱ�ȭ��*/
                sStatRow = NULL;
            }
            else
            {
                sStatRow = sNextRowPtr;
            }

            IDE_TEST( setMinMaxStat( aPersIndex,
                                     sStatRow,
                                     sFirstColumn,
                                     ID_TRUE ) != IDE_SUCCESS ); // aIsMinStat
        }

        if ( ( sNextRowPtr == NULL ) ||
             ( isNullColumn( sFirstColumn,
                             &(sFirstColumn->column),
                             sNextRowPtr ) == ID_TRUE ) )
        {
            if ( sPrevRowPtr == NULL )
            {
                /* setMinMaxStat ���� Row�� NULL�̸� ������� �ʱ�ȭ��*/
                sStatRow = NULL;
            }
            else
            {
                sStatRow = sPrevRowPtr;
            }

            IDE_TEST( setMinMaxStat( aPersIndex,
                                     sStatRow,
                                     sFirstColumn,
                                     ID_FALSE ) != IDE_SUCCESS ); // aIsMinStat
        }
    }

    /* NumDist */
    sNumDistPtr = &aPersIndex->mStat.mNumDist;

    while(1)
    {
        if ( ( aIsUniqueIndex == ID_TRUE ) && ( aIndex->mIsNotNull == ID_TRUE ) )
        {
            idCore::acpAtomicDec64( (void*) sNumDistPtr );
            break;
        }
        else
        {
            /* nothing to do */
        }

        if ( sPrevRowPtr != NULL )
        {
            sCompareResult = compareRows( &(aIndex->mAgerStat),
                                          aIndex->columns,
                                          aIndex->fence,
                                          a_pRow,
                                          sPrevRowPtr ) ;
            if ( sCompareResult < 0 )
            {
                smnManager::logCommonHeader( aIndex->mIndexHeader );
                logIndexNode( aIndex->mIndexHeader, (smnbNode*)sPrevNode );
                IDE_ERROR_RAISE( 0, ERR_CORRUPTED_INDEX );
            }
            else
            {
                /* nothing to do */
            }

            if ( sCompareResult == 0 )
            {
                break;
            }
            else
            {
                /* nothing to do */
            }
        }
        else
        {
            /* nothing to do */
        }

        if ( sNextRowPtr != NULL )
        {
            sCompareResult = compareRows( &(aIndex->mAgerStat),
                                          aIndex->columns,
                                          aIndex->fence,
                                          a_pRow,
                                          sNextRowPtr );
            if ( sCompareResult > 0 )
            {
                smnManager::logCommonHeader( aIndex->mIndexHeader );
                logIndexNode( aIndex->mIndexHeader, (smnbNode*)sNextNode );
                IDE_ERROR_RAISE( 0, ERR_CORRUPTED_INDEX );
            }
            else
            {
                /* nothing to do */
            }

            if ( sCompareResult == 0 )
            {
                break;
            }
            else
            {
                /* nothing to do */
            }
        }
        else
        {
            /* nothing to do */
        }

        /* BUG-30074 - disk table�� unique index���� NULL key ���� ��/
         *             non-unique index���� deleted key �߰� �� Cardinality��
         *             ��Ȯ���� ���� �������ϴ�.
         *
         * Null Key�� ��� NumDist�� �������� �ʵ��� �Ѵ�.
         * Memory index�� NumDist�� ������ ��å���� �����Ѵ�. */
        if ( smnbBTree::isNullColumn( &(aIndex->columns[0]),
                                      &(aIndex->columns[0].column),
                                      a_pRow ) != ID_TRUE )
        {
            idCore::acpAtomicDec64( (void*) sNumDistPtr );
        }
        else
        {
            /* nothing to do */
        }
        break;
    }

    IDE_EXCEPTION_CONT( skip_update_stat );

    /* BUG-32943 [sm-disk-index] After executing DELETE ROW, the KeyCount
     * of DRDB Index is not decreased  */
    idCore::acpAtomicDec64( (void*)&aPersIndex->mStat.mKeyCount );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_CORRUPTED_INDEX )
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_INCONSISTENT_INDEX ) );

        ideLog::log( IDE_SM_0, "Index Corrupted Detected : Wrong Index Order" );
    }
    IDE_EXCEPTION_END;
    
    return IDE_FAILURE;
}

void smnbBTree::deleteSlotInLeafNode( smnbLNode     * a_pLeafNode,
                                      SChar         * a_pRow,
                                      SInt          * a_pSlotPos )
{
    SInt  s_nSlotPos;

    s_nSlotPos  = *a_pSlotPos;
    *a_pSlotPos = -1;

    // To fix PR-14107
    if ( ( s_nSlotPos != a_pLeafNode->mSlotCount ) &&
         ( a_pLeafNode->mRowPtrs[s_nSlotPos] == a_pRow ) )
    {
        //delete slot
        /* PROJ-2433 */
        if ( ( s_nSlotPos + 1 ) <= ( a_pLeafNode->mSlotCount - 1 ) )
        {
            shiftLeafSlots( a_pLeafNode,
                            (SShort)( s_nSlotPos + 1 ),
                            (SShort)( a_pLeafNode->mSlotCount - 1 ),
                            (SShort)(-1) );
        }
        else
        {
            /* nothing to do */
        }

        // BUG-29582: leaf node���� ������ slot�� delete�ɶ�, ������ slot��
        // row pointer�� NULL�� �ʱ�ȭ�Ѵ�.
        setLeafSlot( a_pLeafNode,
                     (SShort)( a_pLeafNode->mSlotCount - 1 ),
                     NULL,
                     NULL );

        (a_pLeafNode->mSlotCount)--;

        *a_pSlotPos = s_nSlotPos;
    }
    else
    {
        /* nothing to do */
    }
}

IDE_RC smnbBTree::deleteSlotInInternalNode(smnbINode*        a_pInternalNode,
                                           SInt              a_nSlotPos)
{
    // BUG-26941 CodeSonar::Type Underrun
    IDE_ERROR_RAISE( a_nSlotPos >= 0, ERR_CORRUPTED_INDEX );

    if ( a_pInternalNode->mSlotCount > 1 )
    {
        if ( a_pInternalNode->mRowPtrs[a_nSlotPos] == NULL )
        {
            // BUG-27456 Klocwork SM(4)
            IDE_ERROR_RAISE( a_nSlotPos != 0, ERR_CORRUPTED_INDEX );

            /*
             * PROJ-2433
             * child pointer�� �״�� �����ϰ�
             * row pointer, direct key ����
             */
            setInternalSlot( a_pInternalNode,
                             (SShort)( a_nSlotPos - 1 ),
                             a_pInternalNode->mChildPtrs[a_nSlotPos - 1],
                             NULL,
                             NULL );
        }
        else
        {
            /* nothing to do */
        }
    }
    else
    {
        /* nothing to do */
    }

    /* PROJ-2433 */
    if ( ( a_nSlotPos + 1 ) <= ( a_pInternalNode->mSlotCount - 1 ) )
    {
        shiftInternalSlots( a_pInternalNode,
                            (SShort)( a_nSlotPos + 1 ),
                            (SShort)( a_pInternalNode->mSlotCount - 1 ),
                            (SShort)(-1) );
    }
    else
    {
        /* nothing to do */
    }

    // BUG-29582: internal node���� ������ slot�� delete�ɶ�, ������ slot��
    // row pointer�� NULL�� �ʱ�ȭ�Ѵ�.
    setInternalSlot( a_pInternalNode,
                     (SShort)( a_pInternalNode->mSlotCount - 1 ),
                     NULL,
                     NULL,
                     NULL );

    (a_pInternalNode->mSlotCount)--;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_CORRUPTED_INDEX )
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_INCONSISTENT_INDEX ) );

        ideLog::log( IDE_SM_0, "Index Corrupted Detected : Wrong Index Slot Position" );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

void smnbBTree::merge(smnbINode* a_pLeftNode,
                      smnbINode* a_pRightNode)
{
    copyInternalSlots( a_pLeftNode,
                       a_pLeftNode->mSlotCount,
                       a_pRightNode,
                       0,
                       (SShort)( a_pRightNode->mSlotCount - 1 ) );
}

IDE_RC smnbBTree::deleteNA( idvSQL          * /* aStatistics */,
                            void            * /* a_pTrans */,
                            void            * /* a_pIndex */,
                            SChar           * /* a_pRow */,
                            smiIterator     * /* aIterator */,
                            sdSID             /* aRowSID */ )
{
    // memory b tree�� row�� delete�ÿ� index key�� delete mark��
    // ���� �ʴ´�.

    return IDE_SUCCESS;
}

IDE_RC smnbBTree::freeSlot( void            * a_pIndex,
                            SChar           * a_pRow,
                            idBool            aIgnoreNotFoundKey,
                            idBool          * aIsExistFreeKey )

{
    smnbHeader*       s_pIndexHeader;
    SInt              s_nDepth;
    smnbLNode*        s_pCurLeafNode = NULL;
    smnbINode*        s_pCurInternalNode = NULL;
    smnbNode*         s_pFreeNode = NULL;
    SInt              s_nSlotPos;
    smnbStack         s_stack[SMNB_STACK_DEPTH];
    void*             s_pDeleteRow = NULL;
    SChar           * s_pInsertRow = NULL;
    void            * sInsertKey   = NULL;
    idBool            sIsUniqueIndex;
    idBool            sIsTreeLatched = ID_FALSE;
    idBool            sNeedTreeLatch = ID_FALSE;
    smnbLNode       * sLatchedNode[3];   /* prev/curr/next �ִ� 3�� �������ִ� */
    UInt              sLatchedNodeCount = 0;
    UInt              sDeleteNodeCount = 0;
    smOID             sRowOID;

    IDE_ASSERT( a_pIndex        != NULL );
    IDE_ASSERT( a_pRow          != NULL );
    IDE_ASSERT( aIsExistFreeKey != NULL );

    *aIsExistFreeKey = ID_TRUE;

    s_pIndexHeader  = (smnbHeader*)((smnIndexHeader*)a_pIndex)->mHeader;
    sIsUniqueIndex = (((smnIndexHeader*)a_pIndex)->mFlag&SMI_INDEX_UNIQUE_MASK)
        == SMI_INDEX_UNIQUE_ENABLE ? ID_TRUE : ID_FALSE;

restart:
    /* BUG-32742 [sm] Create DB fail in Altibase server
     *           Solaris x86���� DB ������ ���� hang �ɸ�.
     *           ������ �����Ϸ� ����ȭ ������ ���̰�, �̸� ȸ���ϱ� ����
     *           �Ʒ��� ���� ������ */
    if ( sNeedTreeLatch == ID_TRUE )
    {
        IDU_FIT_POINT( "smnbBTree::freeSlot::beforeLockTree" );

        IDE_TEST( lockTree(s_pIndexHeader) != IDE_SUCCESS );
        sIsTreeLatched = ID_TRUE;
        sNeedTreeLatch = ID_FALSE;

        IDU_FIT_POINT( "smnbBTree::freeSlot::afterLockTree" );
    }

    s_nDepth = -1;

    IDE_TEST( findPosition( s_pIndexHeader,
                            a_pRow,
                            &s_nDepth,
                            s_stack ) != IDE_SUCCESS );

    /* BUG-32655 [sm-mem-index] The MMDB Ager must not ignore the failure of
     * index aging.
     * BUG-32984 [SM] during rollback after updateInplace in memory index,
     *                it attempts to delete a key that does not exist.
     * updateInplace rollback�� ��� Ű�� �������� ���� �� �ִ�. */
    if ( s_nDepth == -1 )
    {
        *aIsExistFreeKey = ID_FALSE;

        if ( aIgnoreNotFoundKey == ID_TRUE )
        {
            IDE_CONT( SKIP_FREE_SLOT );
        }
        else
        {
            /* goto exception */
            IDE_TEST( 1 );
        }
    }

    while(s_nDepth >= 0)
    {
        s_pCurLeafNode = (smnbLNode*)(s_stack[s_nDepth].node);
        s_pDeleteRow   = a_pRow;

        /* BUG-32781 during update MMDB Index statistics, a reading slot 
         *           can be removed.
         *
         * Tree latch�� ��� ��� prev/next/curr node�� ��� updateStat �߿�
         * row/node�� free�Ǵ� ��츦 ���ش�. */
        if ( sIsTreeLatched == ID_TRUE )
        {
            /* prev node */
            if ( s_pCurLeafNode->prevSPtr != NULL )
            {
                IDE_TEST( lockNode(s_pCurLeafNode->prevSPtr) != IDE_SUCCESS );
                sLatchedNode[sLatchedNodeCount] = s_pCurLeafNode->prevSPtr;
                sLatchedNodeCount++;
            }
            else
            {
                /* nothing to do */
            }

            /* curr node */
            IDE_TEST( lockNode(s_pCurLeafNode) != IDE_SUCCESS );
            sLatchedNode[sLatchedNodeCount] = s_pCurLeafNode;
            sLatchedNodeCount++;

            /* next node */
            if ( s_pCurLeafNode->nextSPtr != NULL )
            {
                IDE_TEST( lockNode(s_pCurLeafNode->nextSPtr) != IDE_SUCCESS );
                sLatchedNode[sLatchedNodeCount] = s_pCurLeafNode->nextSPtr;
                sLatchedNodeCount++;
            }
            else
            {
                /* nothing to do */
            }
        }
        else
        {
            IDE_TEST( lockNode(s_pCurLeafNode) != IDE_SUCCESS );
            sLatchedNode[sLatchedNodeCount] = s_pCurLeafNode;
            sLatchedNodeCount++;
        }

        IDE_ASSERT( sLatchedNodeCount <= 3 );

        /* node latch�� ����� split �Ǿ� ������ Ű�� �ٸ� ���� �̵�����
         * ���� �ִ�. �׷��� �Ǹ� �� ����� ��� Key�� ������ �� �ִ�. */
        if ( ( s_pCurLeafNode->mSlotCount == 0 ) ||
             ( ( s_pCurLeafNode->flag & SMNB_NODE_VALID_MASK ) == SMNB_NODE_INVALID ) )
        {
            IDE_ASSERT( sIsTreeLatched == ID_FALSE );
            IDE_ASSERT( sLatchedNodeCount == 1 );

            sLatchedNodeCount = 0;
            IDE_TEST( unlockNode(s_pCurLeafNode) != IDE_SUCCESS );

            goto restart;
        }
        else
        {
            /* nothing to do */
        }

        /* ��� �������� �ʿ��� ��� Tree latch�� ��ƾ� �Ѵ�. */
        if ( sIsTreeLatched == ID_FALSE )
        {
            if ( ( s_pCurLeafNode->mSlotCount - 1 ) == 0 )
            {
                IDE_ASSERT( sLatchedNodeCount == 1 );
                sNeedTreeLatch = ID_TRUE;

                sLatchedNodeCount = 0;
                IDE_TEST( unlockNode(s_pCurLeafNode) != IDE_SUCCESS );

                goto restart;
            }
            else
            {
                /* nothing to do */
            }
        }

        /* Ű ��ġ�� ã�� */
        IDE_TEST( findSlotInLeaf( s_pIndexHeader,
                                  s_pCurLeafNode,
                                  s_pDeleteRow,
                                  &s_nSlotPos ) != IDE_SUCCESS );

        /* ������ Ű�� ã�� ���� split �� ���.. */
        if ( s_nSlotPos == s_pCurLeafNode->mSlotCount )
        {
            if ( s_pCurLeafNode->nextSPtr != NULL )
            {
                IDE_ASSERT( sIsTreeLatched == ID_FALSE );
                IDE_ASSERT( sLatchedNodeCount == 1 );

                sLatchedNodeCount = 0;
                IDE_TEST( unlockNode(s_pCurLeafNode) != IDE_SUCCESS );

                goto restart;
            }
        }
        else
        {
            /* nothing to do */
        }

        /* ������ Ű ��ġ�� ������ slot�̸� ���� ����� slot�� ����Ǳ� ������
         * Tree latch�� ��´�. */
        if ( sIsTreeLatched == ID_FALSE )
        {
            if ( s_nSlotPos == ( s_pCurLeafNode->mSlotCount - 1 ) )
            {
                IDE_ASSERT( sLatchedNodeCount == 1 );

                sNeedTreeLatch = ID_TRUE;

                sLatchedNodeCount = 0;
                IDE_TEST( unlockNode(s_pCurLeafNode) != IDE_SUCCESS );

                goto restart;
            }
            else
            {
                /* nothing to do */
            }
        }

        if ( needToUpdateStat( s_pIndexHeader->mIsMemTBS ) == ID_TRUE )
        {
            IDE_TEST( updateStat4Delete( (smnIndexHeader*)a_pIndex,
                                         s_pIndexHeader,
                                         s_pCurLeafNode,
                                         (SChar*)s_pDeleteRow,
                                         &s_nSlotPos,
                                         sIsUniqueIndex ) != IDE_SUCCESS );
        }
        else
        {
            /* nothing to do ... */
        }

        SMNB_SCAN_LATCH(s_pCurLeafNode);

        deleteSlotInLeafNode( s_pCurLeafNode,
                              (SChar*)s_pDeleteRow,
                              &s_nSlotPos );

        SMNB_SCAN_UNLATCH(s_pCurLeafNode);

        /* BUG-32655 [sm-mem-index] The MMDB Ager must not ignore the failure of
         * index aging.
         * BUG-32984 [SM] during rollback after updateInplace in memory index,
         *                it attempts to delete a key that does not exist.
         * updateInplace rollback�� ��� Ű�� �������� ���� �� �ִ�. */
        if ( s_nSlotPos == -1 )
        {
            *aIsExistFreeKey = ID_FALSE;

            if ( aIgnoreNotFoundKey == ID_TRUE )
            {
                IDE_CONT( SKIP_FREE_SLOT );
            }
            else
            {
                /* goto exception */
                IDE_TEST( 1 );
            }
        }

        if ( s_pCurLeafNode->mSlotCount == 0 )
        {
            IDE_ASSERT( sIsTreeLatched == ID_TRUE );

            //remove from leaf node list
            if ( s_pCurLeafNode->prevSPtr != NULL )
            {
                ((smnbLNode*)(s_pCurLeafNode->prevSPtr))->nextSPtr = s_pCurLeafNode->nextSPtr;
            }

            if ( s_pCurLeafNode->nextSPtr != NULL )
            {
                ((smnbLNode*)(s_pCurLeafNode->nextSPtr))->prevSPtr = s_pCurLeafNode->prevSPtr;
            }

            s_pCurLeafNode->freeNodeList = s_pFreeNode;
            s_pFreeNode = (smnbNode*)s_pCurLeafNode;

            // BUG-18292 : V$MEM_BTREE_HEADER ���� �߰�
            sDeleteNodeCount++;

            if ( s_pCurLeafNode == s_pIndexHeader->root )
            {
                IDL_MEM_BARRIER;
                s_pIndexHeader->root = NULL;
            }
        }
        else
        {
            if ( s_nSlotPos == s_pCurLeafNode->mSlotCount )
            {
                /* PROJ-2433 */
                s_pInsertRow = NULL;
                sInsertKey   = NULL;

                getLeafSlot( &s_pInsertRow,
                             &sInsertKey, 
                             s_pCurLeafNode,
                             (SShort)( s_nSlotPos - 1 ) );
            }
            else
            {
                break;
            }
        }

        for(s_nDepth--; s_nDepth >= 0; s_nDepth--)
        {
            s_pCurInternalNode = (smnbINode*)(s_stack[s_nDepth].node);

            SMNB_SCAN_LATCH(s_pCurInternalNode);

            s_nSlotPos = s_stack[s_nDepth].lstReadPos;

            if ( s_pInsertRow != NULL )
            {
                if ( s_pCurInternalNode->mRowPtrs[s_nSlotPos] != NULL )
                {
                    /* PROJ-2433
                     * child pointer�� �����ϰ�
                     * row pointer�� direct key������ */
                    setInternalSlot( s_pCurInternalNode,
                                     (SShort)s_nSlotPos,
                                     s_pCurInternalNode->mChildPtrs[s_nSlotPos],
                                     s_pInsertRow,
                                     sInsertKey );
                }
                else
                {
                    SMNB_SCAN_UNLATCH(s_pCurInternalNode);

                    break;
                }
            }
            else
            {
                IDE_TEST( deleteSlotInInternalNode(s_pCurInternalNode, s_nSlotPos)
                          != IDE_SUCCESS );
                s_nSlotPos--;
            }

            SMNB_SCAN_UNLATCH(s_pCurInternalNode);

            // BUG-26625 �޸� �ε��� ��ĵ�� SMO�� �����ϴ� ������ ������ �ֽ��ϴ�.

            if ( s_pCurInternalNode->mSlotCount == 0 )
            {
                //remove from sibling node list
                if ( s_pCurInternalNode->prevSPtr != NULL )
                {
                    ((smnbINode*)(s_pCurInternalNode->prevSPtr))->nextSPtr = s_pCurInternalNode->nextSPtr;
                }

                if ( s_pCurInternalNode->nextSPtr != NULL )
                {
                    ((smnbINode*)(s_pCurInternalNode->nextSPtr))->prevSPtr = s_pCurInternalNode->prevSPtr;
                }

                if ( s_pCurInternalNode == s_pIndexHeader->root )
                {
                    IDL_MEM_BARRIER;
                    s_pIndexHeader->root = NULL;
                }

                s_pCurInternalNode->freeNodeList = s_pFreeNode;
                s_pFreeNode  = (smnbNode*)s_pCurInternalNode;
                s_pInsertRow = NULL;
                sInsertKey   = NULL;

                s_pIndexHeader->mAgerStat.mNodeDeleteCount++;

                // BUG-18292 : V$MEM_BTREE_HEADER ���� �߰�
                sDeleteNodeCount++;
            }
            else
            {
                if ( s_nSlotPos == ( s_pCurInternalNode->mSlotCount - 1 ) )
                {
                    if ( s_pInsertRow == NULL )
                    {
                        /* PROJ-2433 */
                        s_pInsertRow = NULL;
                        sInsertKey   = NULL;

                        getInternalSlot( NULL,
                                         &s_pInsertRow,
                                         &sInsertKey,
                                         s_pCurInternalNode,
                                         (SShort)s_nSlotPos );
                    }

                    if ( s_pInsertRow == NULL )
                    {
                        s_pCurInternalNode =
                            (smnbINode*)(s_pCurInternalNode->mChildPtrs[s_pCurInternalNode->mSlotCount - 1]);

                        while((s_pCurInternalNode->flag & SMNB_NODE_TYPE_MASK) == SMNB_NODE_TYPE_INTERNAL)
                        {
                            SMNB_SCAN_LATCH(s_pCurInternalNode);

                            IDE_TEST( s_pCurInternalNode->mSlotCount < 1 );

                            /*
                             * PROJ-2433
                             * child pointer�� ����
                             */
                            setInternalSlot( s_pCurInternalNode,
                                             (SShort)( s_pCurInternalNode->mSlotCount - 1 ),
                                             s_pCurInternalNode->mChildPtrs[s_pCurInternalNode->mSlotCount - 1],
                                             NULL,
                                             NULL );

                            SMNB_SCAN_UNLATCH(s_pCurInternalNode);

                            s_pCurInternalNode =
                                (smnbINode*)(s_pCurInternalNode->mChildPtrs[s_pCurInternalNode->mSlotCount - 1]);
                        }

                        break;
                    }
                }
                else
                {
                    break;
                }
            }
        }

        break;
    }//while

    IDE_EXCEPTION_CONT( SKIP_FREE_SLOT );

    while ( sLatchedNodeCount > 0 )
    {
        sLatchedNodeCount--;
        IDE_TEST( unlockNode(sLatchedNode[sLatchedNodeCount]) != IDE_SUCCESS );
    }

    if ( sIsTreeLatched == ID_TRUE )
    {
        sIsTreeLatched = ID_FALSE;
        IDE_TEST( unlockTree(s_pIndexHeader) != IDE_SUCCESS );
    }

    if ( s_pFreeNode != NULL )
    {
        IDE_TEST( s_pFreeNode == s_pFreeNode->freeNodeList );

        // BUG-18292 : V$MEM_BTREE_HEADER ���� �߰�
        s_pIndexHeader->nodeCount -= sDeleteNodeCount;

        smLayerCallback::addNodes2LogicalAger( mSmnbFreeNodeList,
                                               (smnNode*)s_pFreeNode );
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    smnManager::logCommonHeader( (smnIndexHeader*)a_pIndex );

    if ( s_pCurInternalNode != NULL )
    {
        logIndexNode( (smnIndexHeader*)a_pIndex,
                      (smnbNode*)s_pCurInternalNode );
    }

    if ( s_pCurLeafNode != NULL )
    {
        logIndexNode( (smnIndexHeader*)a_pIndex,
                      (smnbNode*)s_pCurLeafNode );
    }

    sRowOID = SMP_SLOT_GET_OID( a_pRow );
    ideLog::log( IDE_SM_0,
                 "RowPtr:%16"ID_vULONG_FMT" %16"ID_vULONG_FMT,
                 a_pRow,
                 sRowOID );
    ideLog::logMem( IDE_SM_0,
                    (UChar*)a_pRow,
                    s_pIndexHeader->mFixedKeySize
                    + SMP_SLOT_HEADER_SIZE );
    IDE_DASSERT( 0 );

    while ( sLatchedNodeCount > 0 )
    {
        sLatchedNodeCount--;
        IDE_ASSERT( unlockNode(sLatchedNode[sLatchedNodeCount]) == IDE_SUCCESS );
    }

    if ( sIsTreeLatched == ID_TRUE )
    {
        sIsTreeLatched = ID_FALSE;
        IDE_ASSERT( unlockTree(s_pIndexHeader) == IDE_SUCCESS );
    }

    if ( ideGetErrorCode() == smERR_ABORT_INCONSISTENT_INDEX )
    {
        smnManager::setIsConsistentOfIndexHeader( (smnIndexHeader*)a_pIndex, ID_FALSE );
    }

    return IDE_FAILURE;
}

IDE_RC smnbBTree::existKey( void   * aIndex,
                            SChar  * aRow,
                            idBool * aExistKey )

{
    smnbHeader*  sIndexHeader;
    SInt         sDepth;
    smnbLNode*   sCurLeafNode = NULL;
    SInt         sSlotPos;
    smnbStack    sStack[SMNB_STACK_DEPTH];
    idBool       sExistKey = ID_FALSE;
    smOID        sRowOID;
    UInt         sState = 0;

    sIndexHeader  = (smnbHeader*)((smnIndexHeader*)aIndex)->mHeader;
    sDepth = -1;

    IDE_TEST( lockTree( sIndexHeader ) != IDE_SUCCESS );
    sState = 1;

    IDE_TEST( findPosition( sIndexHeader,
                            aRow,
                            &sDepth,
                            sStack ) != IDE_SUCCESS );

    if ( sDepth != -1 )
    {
        /* �ش� Node ã���� */
        sCurLeafNode = (smnbLNode*)(sStack[sDepth].node);

        IDE_TEST( findSlotInLeaf( sIndexHeader,
                                  sCurLeafNode,
                                  aRow,
                                  &sSlotPos ) != IDE_SUCCESS );

        if ( ( sSlotPos != sCurLeafNode->mSlotCount ) &&
             ( sCurLeafNode->mRowPtrs[ sSlotPos ] == aRow ) )
        {
            /* Delete�Ǿ����� Ȯ���Ϸ� ���Դµ�, �����ϸ� �̻��� */
            sExistKey = ID_TRUE;

            smnManager::logCommonHeader( (smnIndexHeader*)aIndex );
            logIndexNode( (smnIndexHeader*)aIndex,
                          (smnbNode*)sCurLeafNode );
            sRowOID = SMP_SLOT_GET_OID( aRow );
            ideLog::log( IDE_SM_0, 
                         "RowPtr:%16lu %16lu",
                         aRow,
                         sRowOID );
            ideLog::logMem( IDE_SM_0,
                            (UChar*)aRow,
                            sIndexHeader->mFixedKeySize 
                            + SMP_SLOT_HEADER_SIZE );
        }
        else
        {
            sExistKey = ID_FALSE;
        }
    }
    else
    {
        sExistKey = ID_FALSE;
    }

    sState = 0;
    IDE_TEST( unlockTree( sIndexHeader ) != IDE_SUCCESS );

    (*aExistKey) = sExistKey;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    smnManager::logCommonHeader( (smnIndexHeader*)aIndex );

    if ( sCurLeafNode != NULL )
    {
        logIndexNode( (smnIndexHeader*)aIndex,
                      (smnbNode*)sCurLeafNode );
        sRowOID = SMP_SLOT_GET_OID( aRow );
        ideLog::log( IDE_SM_0, 
                     "RowPtr:%16lu %16lu",
                     aRow,
                     sRowOID );
        ideLog::logMem( IDE_SM_0,
                        (UChar*)aRow,
                        sIndexHeader->mFixedKeySize + SMP_SLOT_HEADER_SIZE );

        IDE_DASSERT( 0 );
    }

    if ( sState != 0)
    {
        IDE_PUSH();
        IDE_ASSERT( unlockTree( sIndexHeader ) == IDE_SUCCESS );
        IDE_POP();
    }

    if ( ideGetErrorCode() == smERR_ABORT_INCONSISTENT_INDEX )
    {
        smnManager::setIsConsistentOfIndexHeader( (smnIndexHeader*)aIndex, ID_FALSE );
    }

    (*aExistKey) = ID_FALSE;

    return IDE_FAILURE;
}

/*********************************************************************
 * FUNCTION DESCRIPTION : smnbBTree::gatherStat                      *
 * ------------------------------------------------------------------*
 * TASK-4990 changing the method of collecting index statistics
 * ���� ������� ���� ���
 * index�� ��������� �����Ѵ�.
 *
 * Statistics    - [IN]  IDLayer �������
 * Trans         - [IN]  �� �۾��� ��û�� Transaction
 * Percentage    - [IN]  Sampling Percentage
 * Degree        - [IN]  Parallel Degree
 * Header        - [IN]  ��� TableHeader
 * Index         - [IN]  ��� index
 *********************************************************************/
IDE_RC smnbBTree::gatherStat( idvSQL         * /* aStatistics */, 
                              void           * aTrans,
                              SFloat           aPercentage,
                              SInt             /* aDegree */,
                              smcTableHeader * aHeader,
                              void           * aTotalTableArg,
                              smnIndexHeader * aIndex,
                              void           * aStats,
                              idBool           aDynamicMode )
{
    smnbHeader       * sIdxHdr = NULL;
    smiIndexStat       sIndexStat;
    UInt               sAnalyzedNodeCount = 0;
    UInt               sTotalNodeCount    = 0;
    SLong              sKeyCount          = 0;
    volatile SLong     sNumDist           = 0; /* BUG-40691 */
    volatile SLong     sClusteringFactor  = 0; /* BUG-40691 */
    UInt               sIndexHeight       = 0;
    SLong              sMetaSpace         = 0;
    SLong              sUsedSpace         = 0;
    SLong              sAgableSpace       = 0;
    SLong              sFreeSpace         = 0;
    SLong              sAvgSlotCnt        = 0;
    idBool             sIsNodeLatched     = ID_FALSE;
    idBool             sIsFreeNodeLocked  = ID_FALSE;
    smnbLNode        * sCurLeafNode;
    smxTrans         * sSmxTrans          = (smxTrans *)aTrans;
    UInt               sStatFlag          = SMI_INDEX_BUILD_RT_STAT_UPDATE;

    IDE_ASSERT( aIndex != NULL );
    IDE_ASSERT( aTotalTableArg == NULL ); 

    /* BUG-44794 �ε��� ����� �ε��� ��� ������ �������� �ʴ� ���� ������Ƽ �߰� */
    SMI_INDEX_BUILD_NEED_RT_STAT( sStatFlag, sSmxTrans );

    sIdxHdr       = (smnbHeader*)(aIndex->mHeader);
    idlOS::memset( &sIndexStat, 0, ID_SIZEOF( smiIndexStat ) );

    IDE_TEST( smLayerCallback::beginIndexStat( aHeader,
                                               aIndex,
                                               aDynamicMode )
              != IDE_SUCCESS );

    /* FreeNode�� ���´�. Node��Ȱ���� ���Ƽ� TreeLatch ����
     * Node�� FullScan �ϱ� �����̴�. */
    smLayerCallback::blockFreeNode();
    sIsFreeNodeLocked = ID_TRUE;

    /************************************************************
     * Node �м��� ���� ������� ȹ�� ����
     ************************************************************/
    /* BeforeFirst�� ù �������� ���� */
    sCurLeafNode = NULL;
    IDE_TEST( getNextNode4Stat( sIdxHdr,
                                ID_FALSE, /* BackTraverse */
                                &sIsNodeLatched,
                                &sIndexHeight,
                                &sCurLeafNode )
              != IDE_SUCCESS );

    while( sCurLeafNode != NULL )
    {
        /*Sampling Percentage�� P�� ������, �����Ǵ� �� C�� �ΰ� C�� P��
         * �������� 1�� �Ѿ�� ��쿡 Sampling �ϴ� ������ �Ѵ�.
         *
         * ��)
         * P=1, C=0
         * ù��° ������ C+=P  C=>0.25
         * �ι�° ������ C+=P  C=>0.50
         * ����° ������ C+=P  C=>0.75
         * �׹�° ������ C+=P  C=>1(Sampling!)  C--; C=>0
         * �ټ���° ������ C+=P  C=>0.25
         * ������° ������ C+=P  C=>0.50
         * �ϰ���° ������ C+=P  C=>0.75
         * ������° ������ C+=P  C=>1(Sampling!)  C--; C=>0 */
        if ( ( (UInt)( aPercentage * sTotalNodeCount
                       + SMI_STAT_SAMPLING_INITVAL ) ) !=
             ( (UInt)( aPercentage * (sTotalNodeCount+1)
                       + SMI_STAT_SAMPLING_INITVAL ) ) )
        {
            sAnalyzedNodeCount ++;
            IDE_TEST( analyzeNode( aIndex,
                                   sIdxHdr,
                                   sCurLeafNode,
                                   (SLong *)&sClusteringFactor,
                                   (SLong *)&sNumDist,
                                   &sKeyCount,
                                   &sMetaSpace,
                                   &sUsedSpace,
                                   &sAgableSpace,
                                   &sFreeSpace )
                      != IDE_SUCCESS );
        }
        else
        {
            /* Skip�� */
        }
        sTotalNodeCount ++;

        IDE_TEST( getNextNode4Stat( sIdxHdr,
                                    ID_FALSE, /* BackTraverse */
                                    &sIsNodeLatched,
                                    NULL, /* sIndexHeight */
                                    &sCurLeafNode )
                  != IDE_SUCCESS );
    }

    /************************************************************
     * ������� ������
     ************************************************************/
    if ( sAnalyzedNodeCount == 0 )
    {
        /* ��ȸ����� ������ ���� �ʿ� ���� */
    }
    else
    {
        if ( sKeyCount == sAnalyzedNodeCount )
        {
            /* Node�� Key�� �ϳ��� �� ���. ũ�⺸���� �� */
            sNumDist = sNumDist * sTotalNodeCount / sAnalyzedNodeCount; 
            sClusteringFactor = 
                sClusteringFactor * sTotalNodeCount / sAnalyzedNodeCount; 
        }
        else
        {
            /* NumDist�� �����Ѵ�. ���� ���� �������� ���������� ��ģ Ű
             * ����� ������ �ʱ� ������, �� ���� �����Ѵ�.
             *
             * C     = ����Cardinaliy
             * K     = �м��� Key����
             * P     = �м��� Node����
             * (K-P) = �м��� Key���� ��
             * (K-1) = ���� Key���� ��
             *
             * C / ( K - P ) * ( K - 1 ) */

            sNumDist = (SLong)
                ( ( ( ( (SDouble) sNumDist ) / ( sKeyCount - sAnalyzedNodeCount ) )
                    * ( sKeyCount - 1 ) * sTotalNodeCount / sAnalyzedNodeCount )
                  + 1 + SMNB_COST_EPS );

            sClusteringFactor = (SLong)
                ( ( ( ( (SDouble) sClusteringFactor ) / ( sKeyCount - sAnalyzedNodeCount ) )
                  * ( sKeyCount - 1 ) * sTotalNodeCount / sAnalyzedNodeCount )
                  + SMNB_COST_EPS );
        }
        sAvgSlotCnt  = sKeyCount / sAnalyzedNodeCount;
        sUsedSpace   = sUsedSpace * sTotalNodeCount / sAnalyzedNodeCount;
        sAgableSpace = sAgableSpace * sTotalNodeCount / sAnalyzedNodeCount;
        sFreeSpace   = sFreeSpace * sTotalNodeCount / sAnalyzedNodeCount;
        sKeyCount    = sKeyCount * sTotalNodeCount / sAnalyzedNodeCount;
    }

    sIndexStat.mSampleSize        = aPercentage;
    sIndexStat.mNumPage           = sIdxHdr->nodeCount;
    sIndexStat.mAvgSlotCnt        = sAvgSlotCnt;
    sIndexStat.mIndexHeight       = sIndexHeight;
    sIndexStat.mClusteringFactor  = sClusteringFactor;
    sIndexStat.mNumDist           = sNumDist; 
    sIndexStat.mKeyCount          = sKeyCount;
    sIndexStat.mMetaSpace         = sMetaSpace;
    sIndexStat.mUsedSpace         = sUsedSpace;
    sIndexStat.mAgableSpace       = sAgableSpace;
    sIndexStat.mFreeSpace         = sFreeSpace;

    /************************************************************
     * MinMax ������� ����
     ************************************************************/
    /* PROJ-2492 Dynamic sample selection */
    // ���̳��� ����϶� index �� min,max �� �������� �ʴ´�.
    if ( aDynamicMode == ID_FALSE )
    {
        /* BUG-44794 �ε��� ����� �ε��� ��� ������ �������� �ʴ� ���� ������Ƽ �߰� */
        if ( ( sStatFlag & SMI_INDEX_BUILD_RT_STAT_MASK )
                == SMI_INDEX_BUILD_RT_STAT_UPDATE )
        {
            IDE_TEST( rebuildMinMaxStat( aIndex,
                                         sIdxHdr,
                                         ID_TRUE ) /* aMinStat */
                      != IDE_SUCCESS );
            IDE_TEST( rebuildMinMaxStat( aIndex,
                                         sIdxHdr,
                                         ID_FALSE ) /* aMaxStat */
                      != IDE_SUCCESS );
        }
    }
    else
    {
        // Nothing To Do
    }

    IDE_TEST( smLayerCallback::setIndexStatWithoutMinMax( aIndex,
                                                          aTrans,
                                                          (smiIndexStat*)aStats,
                                                          &sIndexStat,
                                                          aDynamicMode,
                                                          sStatFlag )
              != IDE_SUCCESS );

    sIsFreeNodeLocked = ID_FALSE;
    smLayerCallback::unblockFreeNode();

    IDE_TEST( smLayerCallback::endIndexStat( aHeader,
                                             aIndex,
                                             aDynamicMode )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    smnManager::logCommonHeader( aIndex );

    ideLog::log( IDE_SM_0,
                 "========================================\n"
                 " REBUILD INDEX STATISTICS: END(FAIL)    \n"
                 "========================================\n"
                 " Index Name: %s\n"
                 " Index ID:   %u\n"
                 "========================================\n",
                 aIndex->mName,
                 aIndex->mId );

    if ( sIsNodeLatched == ID_TRUE )
    {
        sIsNodeLatched = ID_FALSE;
        IDE_ASSERT( unlockNode(sCurLeafNode) == IDE_SUCCESS );
    }

    if ( sIsFreeNodeLocked == ID_TRUE )
    {
        sIsFreeNodeLocked = ID_FALSE;
        smLayerCallback::unblockFreeNode();
    }

    if ( ideGetErrorCode() == smERR_ABORT_INCONSISTENT_INDEX )
    {
        smnManager::setIsConsistentOfIndexHeader( aIndex, ID_FALSE );
    }

    return IDE_FAILURE;
}


/*********************************************************************
 * FUNCTION DESCRIPTION : smnbBTree::rebuildMinMaxstat               *
 * ------------------------------------------------------------------*
 * TASK-4990 changing the method of collecting index statistics
 * MinMax ��������� �����Ѵ�.
 *
 * aPersistentIndexHeader - [IN]  ��� Index�� PersistentHeader
 * aRuntimeIndexHeader    - [IN]  ��� Index�� RuntimeHeader
 * aMinStat               - [IN]  MinStat�ΰ�?
 *********************************************************************/
IDE_RC smnbBTree::rebuildMinMaxStat( smnIndexHeader * aPersistentIndexHeader,
                                     smnbHeader     * aRuntimeIndexHeader,
                                     idBool           aMinStat )
{
    smnbLNode        * sCurLeafNode;
    idBool             sIsNodeLatched    = ID_FALSE;
    idBool             sBacktraverse;
    idBool             sSet = ID_FALSE;
    SInt               i;

    if ( aMinStat == ID_TRUE )
    {
        /* ������ Ž���ϸ鼭 ���� ���� Ű�� ã�� */
        sBacktraverse = ID_FALSE;
    }
    else
    {
        /* ������ Ž���ϸ鼭 ���� ū Ű�� ã�� */
        sBacktraverse = ID_TRUE;
    }

    /* MinValue ã�� ������ */
    sCurLeafNode = NULL;
    IDE_TEST( getNextNode4Stat( aRuntimeIndexHeader,
                                sBacktraverse, 
                                &sIsNodeLatched,
                                NULL, /* sIndexHeight */
                                &sCurLeafNode )
              != IDE_SUCCESS );
    while( sCurLeafNode != NULL )
    {
        if ( sCurLeafNode->mSlotCount > 0 )
        {
            /* Key�� �ִ� Node�� ã����. */
            if ( isNullColumn( aRuntimeIndexHeader->columns,
                               &(aRuntimeIndexHeader->columns->column),
                               sCurLeafNode->mRowPtrs[ 0 ] )
                 == ID_TRUE )
            {
                /* ù Key�� NULL�̸� ���� NULL */
                if ( aMinStat == ID_TRUE )
                {
                    /* Min���� ã�� ���� ������ Ž���ߴµ� �� Null�̶���
                     * Min���� ���ٴ� �ǹ� */
                    sIsNodeLatched = ID_FALSE;
                    IDE_TEST( unlockNode(sCurLeafNode) != IDE_SUCCESS );
                    break;
                }
                else
                {
                    /*MaxStat�� ã�� ���� ���� Node�� ã���� �� */
                }
                
            }
            else
            {
                /* Null �ƴ� Ű�� ���� */
                if ( aMinStat == ID_TRUE )
                {
                    /* Min�̸� 0��° Key�� �ٷ� ���� */
                    IDE_TEST( setMinMaxStat( aPersistentIndexHeader,
                                             sCurLeafNode->mRowPtrs[ 0 ],
                                             aRuntimeIndexHeader->columns,
                                             ID_TRUE ) != IDE_SUCCESS ); // aIsMinStat
                    sSet = ID_TRUE;
                }
                else
                {
                    /* Max�� Null�� �ƴ� Ű�� Ž�� */
                    for ( i = sCurLeafNode->mSlotCount - 1 ; i >= 0 ; i-- )
                    {
                        if ( isNullColumn( 
                                aRuntimeIndexHeader->columns,
                                &(aRuntimeIndexHeader->columns->column),
                                sCurLeafNode->mRowPtrs[ i ] )
                            == ID_FALSE )
                        {
                            IDE_TEST( setMinMaxStat( aPersistentIndexHeader,
                                                     sCurLeafNode->mRowPtrs[ i ],
                                                     aRuntimeIndexHeader->columns,
                                                     ID_FALSE ) != IDE_SUCCESS ); // aIsMinStat
                            sSet = ID_TRUE;
                            break;
                        }
                        else
                        {
                            /* Null��. ���� Row�� */
                        }
                    }
                    /* ��ȸ �� NULL�� �ƴ� ���� ã�Ƽ� i�� 0�� ���ų� Ŀ����*/
                    IDE_ERROR( i >= 0 );
                }
                sIsNodeLatched = ID_FALSE;
                IDE_TEST( unlockNode(sCurLeafNode) != IDE_SUCCESS );
                break;
            }
        }
        else
        {
            /* Node�� Key�� ����*/
        }
        IDE_TEST( getNextNode4Stat( aRuntimeIndexHeader,
                                    sBacktraverse, 
                                    &sIsNodeLatched,
                                    NULL, /* sIndexHeight */
                                    &sCurLeafNode )
                  != IDE_SUCCESS );
    }

    if ( sSet == ID_FALSE )
    {
        IDE_TEST( setMinMaxStat( aPersistentIndexHeader,
                       NULL, /*RowPointer */
                       aRuntimeIndexHeader->columns,
                       aMinStat ) != IDE_SUCCESS ); // aIsMinStat
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    logIndexNode( aPersistentIndexHeader,
                  (smnbNode*)sCurLeafNode );

    return IDE_FAILURE;
}

/*********************************************************************
 * FUNCTION DESCRIPTION : smnbBTree::analyzeNode                     *
 * ------------------------------------------------------------------*
 * TASK-4990 changing the method of collecting index statistics
 * �ش� Node�� �м��Ѵ�.
 *
 * aPersistentIndexHeader - [IN]  ��� Index�� PersistentHeader
 * aRuntimeIndexHeader    - [IN]  ��� Index�� RuntimeHeader
 * aTargetNode            - [IN]  ��� Node
 * aClusteringFactor      - [OUT] ȹ���� ClusteringFactor
 * aNumDist               - [OUT] ȹ���� NumDist
 * aKeyCount              - [OUT] ȹ���� KeyCount
 * aMetaSpace             - [OUT] PageHeader, ExtDir�� Meta ����
 * aUsedSpace             - [OUT] ���� ������� ����
 * aAgableSpace           - [OUT] ���߿� Aging������ ����
 * aFreeSpace             - [OUT] Data������ ������ �� ����
 *********************************************************************/
IDE_RC smnbBTree::analyzeNode( smnIndexHeader * aPersistentIndexHeader,
                               smnbHeader     * aRuntimeIndexHeader,
                               smnbLNode      * aTargetNode,
                               SLong          * aClusteringFactor,
                               SLong          * aNumDist,
                               SLong          * aKeyCount,
                               SLong          * aMetaSpace,
                               SLong          * aUsedSpace,
                               SLong          * aAgableSpace, 
                               SLong          * aFreeSpace )
{
    SInt                sCompareResult;
    SChar             * sCurRow          = NULL;
    SChar             * sPrevRow         = NULL;
    SInt                sDeletedKeyCount = 0;
    SLong               sNumDist         = 0;
    SLong               sClusteringFactor = 0;
    scPageID            sPrevPID = SC_NULL_PID;
    scPageID            sCurPID  = SC_NULL_PID;
    UInt                i;

    for ( i = 0 ; i < (UInt)aTargetNode->mSlotCount ; i++ )
    {
        sPrevRow = sCurRow;
        sCurRow  = aTargetNode->mRowPtrs[ i ];

        /* KeyCount ��� */
        if ( ( !( SM_SCN_IS_FREE_ROW( ((smpSlotHeader*)sCurRow)->mLimitSCN ) ) ) ||
             ( SM_SCN_IS_LOCK_ROW( ((smpSlotHeader*)sCurRow)->mLimitSCN ) ) )
        {
            /* Next�� ������ ������ Row */
            sDeletedKeyCount ++;
        }

        /* NumDist ��� */
        if ( sPrevRow != NULL )
        {
            if ( isNullColumn( aRuntimeIndexHeader->columns,
                              &(aRuntimeIndexHeader->columns->column),
                              sCurRow ) == ID_FALSE )
            {
                /* Null�� �ƴѰ�쿡�� NumDist ����� */
                sCompareResult = compareRows( &aRuntimeIndexHeader->mStmtStat,
                                              aRuntimeIndexHeader->columns,
                                              aRuntimeIndexHeader->fence,
                                              sCurRow,
                                              sPrevRow );
                IDE_ERROR( sCompareResult >= 0 );
                if ( sCompareResult > 0 )
                {
                    sNumDist ++;
                }
                else
                {
                    /* ������ */
                }
            }
            else
            {
                /* NullKey*/
            }
            
            sPrevPID = sCurPID;
            sCurPID = SMP_SLOT_GET_PID( sCurRow );
            if ( sPrevPID != sCurPID )
            {
                sClusteringFactor ++;
            }
        }
    }

    /* PROJ-2433 */
    *aNumDist          += sNumDist;
    *aKeyCount         += aTargetNode->mSlotCount;
    *aClusteringFactor += sClusteringFactor;
    *aMetaSpace         = 0;
    *aUsedSpace        += ( ( aTargetNode->mSlotCount - sDeletedKeyCount ) *
                            ( ID_SIZEOF( aTargetNode->mRowPtrs[0] ) + aTargetNode->mKeySize ) ); 
    *aAgableSpace      += ( sDeletedKeyCount *
                            ( ID_SIZEOF( aTargetNode->mRowPtrs[0] ) + aTargetNode->mKeySize ) );
    *aFreeSpace        += ( ( SMNB_LEAF_SLOT_MAX_COUNT(aRuntimeIndexHeader) - aTargetNode->mSlotCount ) *
                            ( ID_SIZEOF( aTargetNode->mRowPtrs[0] ) + aTargetNode->mKeySize ) );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    logIndexNode( aPersistentIndexHeader,
                  (smnbNode*)aTargetNode );

    return IDE_FAILURE;
}

/*********************************************************************
 * FUNCTION DESCRIPTION : smnbBTree::getNextNode4Stat                *
 * ------------------------------------------------------------------*
 * TASK-4990 changing the method of collecting index statistics
 * ���� Node�� �����´�.
 *
 * aIdxHdr       - [IN]     ��� Index
 * aBackTraverse - [IN]     ��Ž���� ���.
 * aNodeLatched  - [IN/OUT] NodeLatch�� ��Ҵ°�?
 * aIndexHeight  - [OUT]    NULL�� �ƴ� ���, Index ���� ��ȯ��.
 * aCurNode      - [OUT]    ���� Node
 *********************************************************************/
IDE_RC smnbBTree::getNextNode4Stat( smnbHeader     * aIdxHdr,
                                    idBool           aBackTraverse,
                                    idBool         * aNodeLatched,
                                    UInt           * aIndexHeight,
                                    smnbLNode     ** aCurNode )
{
    idBool        sIsTreeLatched    = ID_FALSE;
    UInt          sIndexHeight      = 0;
    smnbINode   * sCurInternalNode;
    smnbLNode   * sCurLeafNode;

    sCurLeafNode = *aCurNode;

    if ( sCurLeafNode == NULL )
    {
        /* ���� Ž��, BeforeFirst *
         * TreeLatch�� �ɾ� SMO�� ���� Leftmost�� Ÿ�� ������ */
        IDE_TEST( lockTree(aIdxHdr) != IDE_SUCCESS );
        sIsTreeLatched = ID_TRUE;

        if ( aIdxHdr->root == NULL )
        {
            sCurLeafNode = NULL; /* ����� ! */
        }
        else
        {
            sCurInternalNode = (smnbINode*)aIdxHdr->root;
            while( (sCurInternalNode->flag & SMNB_NODE_TYPE_MASK ) 
                   == SMNB_NODE_TYPE_INTERNAL )
            {
                sIndexHeight ++;
                if ( aBackTraverse == ID_FALSE )
                {
                    sCurInternalNode = 
                        (smnbINode*)sCurInternalNode->mChildPtrs[0];
                }
                else
                {
                    sCurInternalNode = 
                        (smnbINode*)sCurInternalNode->mChildPtrs[
                            sCurInternalNode->mSlotCount - 1 ];
                }
            }

            sCurLeafNode = (smnbLNode*)sCurInternalNode;
            IDE_TEST( lockNode(sCurLeafNode) != IDE_SUCCESS );
            *aNodeLatched = ID_TRUE;
        }

        sIsTreeLatched = ID_FALSE;
        IDE_TEST( unlockTree(aIdxHdr) != IDE_SUCCESS );
    }
    else
    {
        /* ���� Node Ž��.
         * �׳� ��ũ�� Ÿ�� ���ư��� �� */
        *aNodeLatched = ID_FALSE;
        IDE_TEST( unlockNode(sCurLeafNode) != IDE_SUCCESS );

        if ( aBackTraverse == ID_FALSE )
        {
            sCurLeafNode = sCurLeafNode->nextSPtr;
        }
        else
        {
            sCurLeafNode = sCurLeafNode->prevSPtr;
        }
        if ( sCurLeafNode == NULL )
        {
            /* �ٿԴ� ! */
        }
        else
        {
            IDE_TEST( lockNode(sCurLeafNode) != IDE_SUCCESS );
            *aNodeLatched = ID_TRUE;
        }
    }

    /* NodeLatch�� ��������� ��ȯ�� LeafNode�� ����� �ϰ�
     * NodeLatch�� ������� LeafNode�� �־�� �Ѵ�. */
    IDE_ASSERT( 
        ( ( *aNodeLatched == ID_FALSE ) && ( sCurLeafNode == NULL ) ) ||
        ( ( *aNodeLatched == ID_TRUE  ) && ( sCurLeafNode != NULL ) ) );
    *aCurNode = sCurLeafNode;

    if ( aIndexHeight != NULL )
    {
         ( *aIndexHeight ) = sIndexHeight;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if ( sIsTreeLatched == ID_TRUE )
    {
        sIsTreeLatched = ID_FALSE;
        IDE_ASSERT( unlockTree(aIdxHdr) == IDE_SUCCESS );
    }

    return IDE_FAILURE;

}

IDE_RC smnbBTree::NA( void )
{
    IDE_SET( ideSetErrorCode( smERR_ABORT_smiTraverseNotApplicable ) );

    return IDE_FAILURE;
}

IDE_RC smnbBTree::beforeFirst( smnbIterator* a_pIterator,
                               const smSeekFunc** )
{
    for( a_pIterator->mKeyRange      = a_pIterator->mKeyRange;
         a_pIterator->mKeyRange->prev != NULL;
         a_pIterator->mKeyRange        = a_pIterator->mKeyRange->prev ) ;

    IDE_TEST( beforeFirstInternal( a_pIterator ) != IDE_SUCCESS );

    a_pIterator->flag  = a_pIterator->flag & (~SMI_RETRAVERSE_MASK);
    a_pIterator->flag |= SMI_RETRAVERSE_BEFORE;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smnbBTree::beforeFirstInternal( smnbIterator* a_pIterator )
{
    /* BUG-37643 Node�� array�� ���������� �����ϴµ�
     * compiler ����ȭ�� ���ؼ� ���������� ���ŵ� �� �ִ�.
     * ���� �̷��� ������ volatile�� �����ؾ� �Ѵ�. */
    const smiRange* s_pRange;
    volatile SChar* s_pRow;
    idBool          s_bResult;
    SInt            s_nSlot;
    SInt            s_cSlot;
    SInt            i = 0;
    SInt            s_nMin;
    SInt            s_nMax;
    SInt            sLoop;
    IDU_LATCH       s_version;
    smnbStack       sStack[SMNB_STACK_DEPTH];
    SInt            sDepth = -1;
    smnbLNode*      s_pCurLNode = NULL;
    smnbLNode*      s_pNxtLNode = NULL;
    smnbLNode*      s_pPrvLNode = NULL;
    void          * sKey        = NULL;
    SChar         * sRowPtr     = NULL;

    s_pRange = a_pIterator->mKeyRange;

    if ( a_pIterator->mProperties->mReadRecordCount > 0)
    {
        IDE_TEST( smnbBTree::findFirst( a_pIterator->index,
                                        &s_pRange->minimum,
                                        &sDepth,
                                        sStack) != IDE_SUCCESS );

        while( (sDepth < 0) && (s_pRange->next != NULL) )
        {
            s_pRange = s_pRange->next;

            IDE_TEST( smnbBTree::findFirst( a_pIterator->index,
                                            &s_pRange->minimum,
                                            &sDepth,
                                            sStack) != IDE_SUCCESS );

            a_pIterator->mKeyRange = s_pRange;
        }
    }

    if ( ( sDepth >= 0 ) && ( a_pIterator->mProperties->mReadRecordCount > 0 ) )
    {
        s_pCurLNode = (smnbLNode*)(sStack[sDepth].node);
        s_version   = getLatchValueOfLNode(s_pCurLNode) & (~SMNB_SCAN_LATCH_BIT);
        IDL_MEM_BARRIER;

        while(1)
        {
            s_cSlot      = s_pCurLNode->mSlotCount;
            s_pPrvLNode  = s_pCurLNode->prevSPtr;
            s_pNxtLNode  = s_pCurLNode->nextSPtr;

            if ( s_cSlot == 0 )
            {
                IDL_MEM_BARRIER;
                if ( s_version == getLatchValueOfLNode(s_pCurLNode) )
                {
                    s_pCurLNode = s_pNxtLNode;

                    if ( s_pCurLNode == NULL )
                    {
                        break;
                    }
                }

                s_nSlot      = 0;

                IDL_MEM_BARRIER;
                s_version    = getLatchValueOfLNode(s_pCurLNode) & (~SMNB_SCAN_LATCH_BIT);
                IDL_MEM_BARRIER;

                continue;
            }
            else
            {
                findFirstSlotInLeaf( a_pIterator->index,
                                     &s_pRange->minimum,
                                     s_pCurLNode,
                                     0,
                                     s_cSlot - 1,
                                     &s_nSlot );
            }

            a_pIterator->version  = s_version;
            a_pIterator->node     = s_pCurLNode;

            a_pIterator->prvNode  = s_pPrvLNode;
            a_pIterator->nxtNode  = s_pNxtLNode;

            a_pIterator->least    = ID_TRUE;
            a_pIterator->highest  = ID_FALSE;

            s_nMin = s_nSlot;
            s_nMax = s_cSlot - 1;

            if ( ( s_nSlot + 1 ) < s_cSlot )
            {
                s_pRow  = a_pIterator->node->mRowPtrs[s_nSlot];

                if ( s_pRow == NULL )
                {
                    IDL_MEM_BARRIER;
                    s_version = getLatchValueOfLNode(s_pCurLNode) & (~SMNB_SCAN_LATCH_BIT);
                    IDL_MEM_BARRIER;

                    continue;
                }
                else
                {
                    /* nothing to do */
                }

                /*
                 * PROJ-2433
                 * ���ݰ��� �ִ밪 ���ǵ� �����ϴ��� Ȯ���ϴºκ��̴�.
                 * direct key�� full key���߸� ����ε� �񱳰� �����ϴ�.
                 */
                if ( isFullKeyInLeafSlot( a_pIterator->index,
                                          a_pIterator->node,
                                          (SShort)s_nSlot ) == ID_TRUE )
                {
                    sKey = SMNB_GET_KEY_PTR( a_pIterator->node, s_nSlot );
                }
                else
                {
                    sKey = NULL;
                }

#ifdef DEBUG
                // BUG-27948 �̹� ������ Row�� ���� �ִ��� Ȯ��
                // BUG-29106 debugging code
                if ( SM_SCN_IS_FREE_ROW(((smpSlotHeader *)s_pRow)->mCreateSCN) )
                {
                    int  sSlotIdx;

                    ideLog::log( IDE_SERVER_0, "s_pRow : %lu\n", s_pRow );
                    ideLog::log( IDE_SERVER_0,
                                 "s_nSlot : %d, s_cSlot : %d\n",
                                 s_nSlot, s_cSlot );
                    for ( sSlotIdx = 0 ; sSlotIdx < s_cSlot ; sSlotIdx++ )
                    {
                        ideLog::logMem( IDE_SERVER_0,
                                        (UChar *)(a_pIterator->node->mRowPtrs[sSlotIdx]),
                                        64 );
                    }
                    ideLog::log( IDE_SERVER_0, "s_pCurLNode : %lu\n", s_pCurLNode);
                    ideLog::logMem( IDE_SERVER_0, (UChar *)a_pIterator->node,
                                    mNodeSize );
                    ideLog::logMem( IDE_SERVER_0, (UChar *)a_pIterator,
                                    mIteratorSize );
                    IDE_ASSERT( 0 );
                }
#endif

                s_pRange->maximum.callback( &s_bResult,
                                            (SChar*)s_pRow,
                                            sKey,
                                            0,
                                            SC_NULL_GRID,
                                            s_pRange->maximum.data );

                if ( s_bResult == ID_TRUE )
                {
                    if ( ( s_nSlot + 2 ) < s_cSlot )
                    {
                        s_pRow = s_pCurLNode->mRowPtrs[s_nSlot + 1];

                        if ( s_pRow == NULL )
                        {
                            IDL_MEM_BARRIER;
                            s_version = getLatchValueOfLNode(s_pCurLNode) & (~SMNB_SCAN_LATCH_BIT);
                            IDL_MEM_BARRIER;

                            continue;
                        }

                        /* PROJ-2433
                         * �ִ밪 ���ǿ� �����ϴ��� Ȯ���ϴºκ��̴�.
                         * direct key�� full key���߸� ����ε� �񱳰� �����ϴ�.
                         */
                        if ( isFullKeyInLeafSlot( a_pIterator->index,
                                                  s_pCurLNode,
                                                  (SShort)( s_nSlot + 1 ) ) == ID_TRUE )
                        {
                            sKey = SMNB_GET_KEY_PTR( s_pCurLNode, s_nSlot + 1 );
                        }
                        else
                        {
                            sKey = NULL;
                        }

#ifdef DEBUG
                        // BUG-27948 �̹� ������ Row�� ���� �ִ��� Ȯ��
                        // BUG-29106 debugging code
                        if ( SM_SCN_IS_FREE_ROW(((smpSlotHeader *)s_pRow)->mCreateSCN) )
                        {
                            int sSlotIdx;

                            ideLog::log( IDE_SERVER_0, "s_pRow : %lu\n", s_pRow );
                            ideLog::log( IDE_SERVER_0,
                                         "s_nSlot : %d, s_cSlot : %d\n",
                                         s_nSlot, s_cSlot );
                            for ( sSlotIdx = 0 ; sSlotIdx < s_cSlot ; sSlotIdx++ )
                            {
                                ideLog::logMem( IDE_SERVER_0,
                                                (UChar *)(s_pCurLNode->mRowPtrs[sSlotIdx]),
                                                64 );
                            }
                            ideLog::log( IDE_SERVER_0, "s_pCurLNode : %lu\n", s_pCurLNode);
                            ideLog::logMem( IDE_SERVER_0, (UChar *)s_pCurLNode,
                                            mNodeSize );
                            ideLog::logMem( IDE_SERVER_0, (UChar *)a_pIterator,
                                            mIteratorSize );
                            IDE_ASSERT( 0 );
                        }
                        else
                        {
                            /* nothing to do */
                        }
#endif

                        s_pRange->maximum.callback( &s_bResult,
                                                    (SChar*)s_pRow,
                                                    sKey,
                                                    0,
                                                    SC_NULL_GRID,
                                                    s_pRange->maximum.data);

                        if ( s_bResult == ID_FALSE )
                        {
                            a_pIterator->highest   = ID_TRUE;
                            s_nMax                 = s_nSlot;
                        }
                        else
                        {
                            /* nothing to do */
                        }
                    }
                }
                else
                {
                    a_pIterator->highest  = ID_TRUE;
                    s_nMax                = -1;
                }
            }

            if ( a_pIterator->highest == ID_FALSE )
            {
                s_pRow = s_pCurLNode->mRowPtrs[s_cSlot - 1];

                if ( s_pRow == NULL )
                {
                    IDL_MEM_BARRIER;
                    s_version = getLatchValueOfLNode(s_pCurLNode) & (~SMNB_SCAN_LATCH_BIT);
                    IDL_MEM_BARRIER;

                    continue;
                }
                else
                {
                    /* nothing to do */
                }

                /*
                 * PROJ-2433
                 * �ִ밪 ���ǿ� �����ϴ��� Ȯ���ϴºκ��̴�.
                 * direct key�� full key���߸� ����ε� �񱳰� �����ϴ�.
                 */
                if ( isFullKeyInLeafSlot( a_pIterator->index,
                                          s_pCurLNode,
                                          SShort( s_cSlot - 1 ) ) == ID_TRUE )
                {
                    sKey = SMNB_GET_KEY_PTR( s_pCurLNode, s_cSlot - 1 );
                }
                else
                {
                    sKey = NULL;
                }

#ifdef DEBUG
                // BUG-27948 �̹� ������ Row�� ���� �ִ��� Ȯ��
                // BUG-29106 debugging code
                if ( SM_SCN_IS_FREE_ROW(((smpSlotHeader *)s_pRow)->mCreateSCN) )
                {
                    int sSlotIdx;

                    ideLog::log( IDE_SERVER_0, "s_pRow : %lu\n", s_pRow );
                    ideLog::log( IDE_SERVER_0,
                                 "s_nSlot : %d, s_cSlot : %d\n",
                                 s_nSlot, s_cSlot );
                    for( sSlotIdx = 0; sSlotIdx < s_cSlot; sSlotIdx ++ )
                    {
                        ideLog::logMem( IDE_SERVER_0,
                                        (UChar *)(s_pCurLNode->mRowPtrs[sSlotIdx]),
                                        64 );
                    }
                    ideLog::log( IDE_SERVER_0, "s_pCurLNode : %lu\n", s_pCurLNode);
                    ideLog::logMem( IDE_SERVER_0, (UChar *)s_pCurLNode,
                                    mNodeSize );
                    ideLog::logMem( IDE_SERVER_0, (UChar *)a_pIterator,
                                    mIteratorSize );
                    IDE_ASSERT( 0 );
                }
#endif

                s_pRange->maximum.callback( &s_bResult,
                                            (SChar*)s_pRow,
                                            sKey,
                                            0,
                                            SC_NULL_GRID,
                                            s_pRange->maximum.data );
                if ( s_bResult == ID_TRUE )
                {
                    s_nMax = s_cSlot - 1;
                }
                else
                {
                    smnbBTree::findLastSlotInLeaf( a_pIterator->index,
                                                   &(s_pRange->maximum),
                                                   s_pCurLNode,
                                                   0,
                                                   s_cSlot - 1,
                                                   &s_nSlot );

                    a_pIterator->highest   = ID_TRUE;
                    s_nMax = s_nSlot;
                }
            }

            /* BUG-44043
               fetchNext()�� ���ʷ� ȣ��ɶ� scn�� �����ϴ� row�� �ϳ��� �־��
               aIterator->lstReturnRecPtr ������ ���õǾ,
               Key Redistribution�� �߻��ص� ���������� Ž���ؼ� ��Ȯ�� ���� row�� ã�Ƴ����ִ�. */
            i = -1;
            for ( sLoop = s_nMin; sLoop <= s_nMax; sLoop++ )
            {
                sRowPtr = s_pCurLNode->mRowPtrs[sLoop];

                if ( sRowPtr == NULL )
                {
                    break;
                }

                if ( smnManager::checkSCN( (smiIterator *)a_pIterator, sRowPtr )
                     == ID_TRUE )
                {
                    i++;
                    a_pIterator->rows[i] = sRowPtr;
                }
            }

            IDL_MEM_BARRIER;
            if ( s_version == getLatchValueOfLNode(s_pCurLNode) )
            {
                if ( i != -1 )
                {
                    break;
                }
                else
                {
                    /* nothing to do */
                }

                s_pCurLNode = s_pNxtLNode;

                if ( ( s_pCurLNode == NULL ) || ( a_pIterator->highest == ID_TRUE ) )
                {
                    s_pCurLNode = NULL;
                    break;
                }
            }

            s_nSlot   = 0;

            IDL_MEM_BARRIER;
            s_version = getLatchValueOfLNode(s_pCurLNode) & (~SMNB_SCAN_LATCH_BIT);
            IDL_MEM_BARRIER;
        }

        /* PROJ-2433 */
        a_pIterator->lowFence  = a_pIterator->rows;
        a_pIterator->highFence = a_pIterator->rows + i;
        a_pIterator->slot      = a_pIterator->rows - 1;
    }

    if ( s_pCurLNode == NULL )
    {
        a_pIterator->least     =
        a_pIterator->highest   = ID_TRUE;
        a_pIterator->slot      = (SChar**)&(a_pIterator->slot);
        a_pIterator->lowFence  = a_pIterator->slot + 1;
        a_pIterator->highFence = a_pIterator->slot - 1;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;
    
    if ( ideGetErrorCode() == smERR_ABORT_INCONSISTENT_INDEX )
    {
        smnManager::setIsConsistentOfIndexHeader( a_pIterator->index->mIndexHeader, ID_FALSE );
    }

    return IDE_FAILURE;
}

IDE_RC smnbBTree::findLast( smnbHeader       * aIndexHeader,
                            const smiCallBack* a_pCallBack,
                            SInt*              a_pDepth,
                            smnbStack*         a_pStack,
                            SInt*              /*a_pSlot*/ )
{
    SInt        s_nDepth;
    smnbLNode*  s_pCurLNode;

    s_nDepth = -1;

    if ( aIndexHeader->root != NULL )
    {
        IDE_TEST( findLstLeaf( aIndexHeader,
                     a_pCallBack,
                     &s_nDepth,
                     a_pStack,
                     &s_pCurLNode) != IDE_SUCCESS );

        // backward scan(findLast)�� Tree ��ü�� Lock ��� ����
        IDE_ERROR_RAISE( -1 <= s_nDepth, ERR_CORRUPTED_INDEX_NODE_DEPTH );

        s_nDepth++;
        a_pStack[s_nDepth].node = s_pCurLNode;
    }
    else
    {
        /* nothing to do */
    }

    // BUG-26625 �޸� �ε��� ��ĵ�� SMO�� �����ϴ� ������ ������ �ֽ��ϴ�.
    IDE_ERROR_RAISE( (s_nDepth < 0) ||
                     ((s_pCurLNode->flag & SMNB_NODE_TYPE_MASK) == SMNB_NODE_TYPE_LEAF),
                     ERR_CORRUPTED_INDEX_FLAG );

    *a_pDepth = s_nDepth;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_CORRUPTED_INDEX_NODE_DEPTH )
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_INCONSISTENT_INDEX ) );

        ideLog::log( IDE_SM_0, "Index Corrupted Detected : Wrong Index Node Depth" );
    }
    IDE_EXCEPTION( ERR_CORRUPTED_INDEX_FLAG )
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_INCONSISTENT_INDEX ) );

        ideLog::log( IDE_SM_0, "Index Corrupted Detected : Wrong Index Flag" );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*********************************************************************
 * FUNCTION DESCRIPTION : smnbBTree::findLastSlotInLeaf              *
 * ------------------------------------------------------------------*
 * LEAF NODE ������ ���� �������� ������ row�� ��ġ�� ã�´�.
 * ( ������ key�� �����ΰ�� ���� ������(������)�� slot ��ġ�� ã�� )
 *
 * case1. �Ϲ� index�̰ų�, partial key index�� �ƴѰ��
 *   case1-1. direct key index�� ���
 *            : �Լ� findLastKeyInLeaf() ������ direct key ������� �˻�
 *   case1-2. �Ϲ� index�� ���
 *            : �Լ� findLastRowInLeaf() ������ row ������� �˻�
 *
 * case2. partial key index�ΰ��
 *        : �Լ� findLastKeyInLeaf() ������ direct key ������� �˻���
 *          �Լ� callback() �Ǵ� findLastRowInLeaf() ������ ��˻�
 *
 *  => partial key ���� ��˻��� �ʿ��� ����
 *    1. partial key�� �˻��� ��ġ�� ��Ȯ�� ��ġ�� �ƴҼ��ֱ⶧����
 *       full key�� ��˻��� �Ͽ� Ȯ���Ͽ��� �Ѵ�
 *
 * aHeader   - [IN]  INDEX ���
 * aCallBack - [IN]  Range Callback
 * aNode     - [IN]  LEAF NODE
 * aMinimum  - [IN]  �˻��� slot���� �ּҰ�
 * aMaximum  - [IN]  �˻��� slot���� �ִ밪
 * aSlot     - [OUT] ã���� slot ��ġ
 *********************************************************************/
inline void smnbBTree::findLastSlotInLeaf( smnbHeader          * aHeader,
                                           const smiCallBack   * aCallBack,
                                           const smnbLNode     * aNode,
                                           SInt                  aMinimum,
                                           SInt                  aMaximum,
                                           SInt                * aSlot )
{
    idBool  sResult;

    if ( aHeader->mIsPartialKey == ID_FALSE )
    {
        /* full direct key */
        if ( SMNB_IS_DIRECTKEY_IN_NODE( aNode ) == ID_TRUE )
        {
            /* ����Ű�ΰ�� ù�÷���key�� ���÷����� row��ó���Ѵ� */
            findLastKeyInLeaf( aHeader,
                               aCallBack,
                               aNode,
                               aMinimum,
                               aMaximum,
                               aSlot );
            IDE_CONT( end );
        }
        /* direct key ���� */
        else
        {
            findLastRowInLeaf( aCallBack,
                               aNode,
                               aMinimum,
                               aMaximum,
                               aSlot );
            IDE_CONT( end );
        }
    }
    /* partial key */
    else
    {
        findLastKeyInLeaf( aHeader,
                           aCallBack,
                           aNode,
                           aMinimum,
                           aMaximum,
                           aSlot );

        if ( *aSlot < aMinimum )
        {
            /* ������������. */
            IDE_CONT( end );
        }
        else
        {
            if ( aNode->mRowPtrs[*aSlot] == NULL )
            {
                /* ������������. */
                IDE_CONT( end );
            }
            else
            {
                /* nothing to do */
            }
        }

        /* partial key��, �ش� node�� key�� ��� ���Ե� �����, key�� ���Ѵ�  */
        if ( isFullKeyInLeafSlot( aHeader,
                                  aNode,
                                  (SShort)(*aSlot) ) == ID_TRUE )
        {
            aCallBack->callback( &sResult,
                                 aNode->mRowPtrs[*aSlot],
                                 SMNB_GET_KEY_PTR( aNode, *aSlot ),
                                 0,
                                 SC_NULL_GRID,
                                 aCallBack->data );
        }
        else
        {
            aCallBack->callback( &sResult,
                                 aNode->mRowPtrs[*aSlot],
                                 NULL,
                                 0,
                                 SC_NULL_GRID,
                                 aCallBack->data );
        }

        if ( sResult == ID_TRUE )
        {
            /* ã�� slot�� �ٷε� slot �� �´°��� ã������ full key�� �ٽ� �˻��Ѵ� */

            aMinimum = *aSlot;

            if ( aMaximum >= ( *aSlot + 1 ) )
            {
                aMaximum = *aSlot + 1;
            }
            else
            {
                /* ã�� slot �ڷ� �ٸ� slot�� ����. ã�� slot�� �´�. */
                IDE_CONT( end );
            }
        }
        else
        {
            aMaximum = *aSlot;
        }

        if ( aMinimum <= aMaximum )
        {
            findLastRowInLeaf( aCallBack,
                               aNode,
                               aMinimum,
                               aMaximum,
                               aSlot );
        }
        else
        {
            /* PROJ-2433
             * node �������� �����ϴ°��̾���. */
            *aSlot = aMinimum - 1;
        }
    }

    IDE_EXCEPTION_CONT( end );

    return;
}

void smnbBTree::findLastRowInLeaf( const smiCallBack   * aCallBack,
                                   const smnbLNode     * aNode,
                                   SInt                  aMinimum,
                                   SInt                  aMaximum,
                                   SInt                * aSlot )
{
    /* BUG-37643 Node�� array�� ���������� �����ϴµ�
     * compiler ����ȭ�� ���ؼ� ���������� ���ŵ� �� �ִ�.
     * ���� �̷��� ������ volatile�� �����ؾ� �Ѵ�. */
    SInt              sMedium   = 0;
    idBool            sResult;
    volatile SChar  * sRow      = NULL;

    do
    {
        sMedium = (aMinimum + aMaximum) >> 1;
        sRow    = aNode->mRowPtrs[sMedium];

        if ( sRow == NULL )
        {
            sResult = ID_FALSE;
        }
        else
        {
            // BUG-27948 �̹� ������ Row�� ���� �ִ��� Ȯ��
            IDE_DASSERT( !SM_SCN_IS_FREE_ROW(((smpSlotHeader *)sRow)->mCreateSCN) );

            (void)aCallBack->callback( &sResult,
                                       (SChar *)sRow,
                                       NULL,
                                       0,
                                       SC_NULL_GRID,
                                       aCallBack->data );
        }

        if ( sResult == ID_TRUE )
        {
            aMinimum = sMedium + 1;
            *aSlot   = (UInt)sMedium;
        }
        else
        {
            aMaximum = sMedium - 1;
            *aSlot   = (UInt)aMaximum;
        }
    }
    while ( aMinimum <= aMaximum );
}

void smnbBTree::findLastKeyInLeaf( smnbHeader          * aHeader,
                                   const smiCallBack   * aCallBack,
                                   const smnbLNode     * aNode,
                                   SInt                  aMinimum,
                                   SInt                  aMaximum,
                                   SInt                * aSlot )
{
    SInt      sMedium           = 0;
    idBool    sResult;
    SChar   * sRow              = NULL;
    void    * sKey              = NULL;
    UInt      sPartialKeySize   = ( aHeader->mIsPartialKey == ID_TRUE ) ? (aNode->mKeySize) : 0;

    do
    {
        sMedium = (aMinimum + aMaximum) >> 1;
        sKey    = SMNB_GET_KEY_PTR( aNode, sMedium );
        sRow    = aNode->mRowPtrs[sMedium];
                         
        if ( sRow == NULL )
        {
            sResult = ID_FALSE;
        }
        else
        {
            (void)aCallBack->callback( &sResult,
                                       sRow,
                                       sKey,
                                       sPartialKeySize,
                                       SC_NULL_GRID,
                                       aCallBack->data );
        }

        if ( sResult == ID_TRUE )
        {
            aMinimum = sMedium + 1;
            *aSlot   = (UInt)sMedium;
        }
        else
        {
            aMaximum = sMedium - 1;
            *aSlot   = (UInt)aMaximum;
        }
    }
    while ( aMinimum <= aMaximum );
}

IDE_RC smnbBTree::findLstLeaf(smnbHeader       * aIndexHeader,
                              const smiCallBack* a_pCallBack,
                              SInt*              a_pDepth,
                              smnbStack*         a_pStack,
                              smnbLNode**        a_pLNode)
{
    /* BUG-37643 Node�� array�� ���������� �����ϴµ�
     * compiler ����ȭ�� ���ؼ� ���������� ���ŵ� �� �ִ�.
     * ���� �̷��� ������ volatile�� �����ؾ� �Ѵ�. */
    SInt                 s_nSlotCount   = 0;
    SInt                 s_nLstReadPos  = 0;
    SInt                 s_nDepth       = 0;
    SInt                 s_nSlotPos     = 0;
    volatile smnbINode * s_pCurINode    = NULL;
    volatile smnbINode * s_pChildINode  = NULL;

    s_nDepth = *a_pDepth;

    if ( s_nDepth == -1 )
    {
        s_pCurINode   = (smnbINode*)aIndexHeader->root;
        s_nLstReadPos = -1;
    }
    else
    {
        s_pCurINode   = (smnbINode*)(a_pStack->node);
        s_nLstReadPos = a_pStack->lstReadPos;

        a_pStack--;
        s_nDepth--;
    }

    if ( s_pCurINode != NULL )
    {
        while ( ( s_pCurINode->flag & SMNB_NODE_TYPE_MASK ) == SMNB_NODE_TYPE_INTERNAL )
        {
            s_nLstReadPos++;

            s_nSlotCount = s_pCurINode->mSlotCount;
            IDE_ERROR_RAISE( s_nSlotCount != 0, ERR_CORRUPTED_INDEX );

            findLastSlotInInternal( aIndexHeader,
                                    a_pCallBack,
                                    (smnbINode*)s_pCurINode,
                                    s_nLstReadPos,
                                    s_nSlotCount - 1,
                                    &s_nSlotPos );

            IDE_DASSERT( s_nSlotPos >= 0 );
            s_pChildINode = (smnbINode*)(s_pCurINode->mChildPtrs[s_nSlotPos]);

            s_nLstReadPos = -1;

            IDE_ERROR_RAISE( ( s_nSlotCount != 0 ) ||
                             ( s_nSlotCount != s_nSlotPos ), ERR_CORRUPTED_INDEX ); /* backward scan(findLast)�� Tree ��ü�� Lock ��� ���� */

            a_pStack->node       = (smnbINode*)s_pCurINode;
            a_pStack->lstReadPos = s_nSlotPos;

            s_nDepth++;
            a_pStack++;

            s_pCurINode   = s_pChildINode;
        }
    }
    else
    {
        /* nothing to do */
    }

    *a_pDepth = s_nDepth;
    *a_pLNode = (smnbLNode*)s_pCurINode;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_CORRUPTED_INDEX )
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_INCONSISTENT_INDEX ) );

        ideLog::log( IDE_SM_0, "Index Corrupted Detected : Wrong Index SlotCount" );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*********************************************************************
 * FUNCTION DESCRIPTION : smnbBTree::findLastSlotInInternal          *
 * ------------------------------------------------------------------*
 * INTERNAL NODE ������ ���� �������� ������ row�� ��ġ�� ã�´�.
 * ( ������ key�� �����ΰ�� ���� ������(������)�� slot ��ġ�� ã�� )
 *
 * case1. �Ϲ� index�̰ų�, partial key index�� �ƴѰ��
 *   case1-1. direct key index�� ���
 *            : �Լ� findLastInInternal() ������ direct key ������� �˻�
 *   case1-2. �Ϲ� index�� ���
 *            : �Լ� findLastRowInInternal() ������ row ������� �˻�
 *
 * case2. partial key index�ΰ��
 *        : �Լ� findLastInInternal() ������ direct key ������� �˻���
 *          �Լ� callback() �Ǵ� findLastRowInInternal() ������ ��˻�
 *
 *  => partial key ���� ��˻��� �ʿ��� ����
 *    1. partial key�� �˻��� ��ġ�� ��Ȯ�� ��ġ�� �ƴҼ��ֱ⶧����
 *       full key�� ��˻��� �Ͽ� Ȯ���Ͽ��� �Ѵ�
 *
 * aHeader   - [IN]  INDEX ���
 * aCallBack - [IN]  Range Callback
 * aNode     - [IN]  INTERNAL NODE
 * aMinimum  - [IN]  �˻��� slot���� �ּҰ�
 * aMaximum  - [IN]  �˻��� slot���� �ִ밪
 * aSlot     - [OUT] ã���� slot ��ġ
 *********************************************************************/
inline void smnbBTree::findLastSlotInInternal( smnbHeader          * aHeader,
                                               const smiCallBack   * aCallBack,
                                               const smnbINode     * aNode,
                                               SInt                  aMinimum,
                                               SInt                  aMaximum,
                                               SInt                * aSlot )
{
    idBool  sResult;

    if ( aHeader->mIsPartialKey == ID_FALSE )
    {
        /* full direct key */
        if ( SMNB_IS_DIRECTKEY_IN_NODE( aNode ) == ID_TRUE )
        {
            /* ����Ű�ΰ�� ù�÷���key�� ���÷�����row��ó���Ѵ� */
            findLastKeyInInternal( aHeader,
                                   aCallBack,
                                   aNode,
                                   aMinimum,
                                   aMaximum,
                                   aSlot );
            IDE_CONT( end );
        }
        /* direct key ���� */
        else
        {
            findLastRowInInternal( aCallBack,
                                   aNode,
                                   aMinimum,
                                   aMaximum,
                                   aSlot );
            IDE_CONT( end );
        }
    }
    else
    {
        /* partial key */
        findLastKeyInInternal( aHeader,
                               aCallBack,
                               aNode,
                               aMinimum,
                               aMaximum,
                               aSlot );

        if ( *aSlot < aMinimum )
        {
            /* ������������. */
            IDE_CONT( end );
        }
        else
        {
            if ( aNode->mRowPtrs[*aSlot] == NULL )
            {
                /* ������������. */
                IDE_CONT( end );
            }
            else
            {
                /* nothing to do */
            }
        }

        /* partial key��, �ش� node�� key�� ��� ���Ե� �����, key�� ���Ѵ�  */
        if ( isFullKeyInInternalSlot( aHeader,
                                      aNode,
                                      (SShort)(*aSlot) ) == ID_TRUE )
        {
            aCallBack->callback( &sResult,
                                 aNode->mRowPtrs[*aSlot],
                                 SMNB_GET_KEY_PTR( aNode, *aSlot ),
                                 0,
                                 SC_NULL_GRID,
                                 aCallBack->data );
        }
        else
        {
            /* ���������� indirect key�� ���Ѵ� */
            aCallBack->callback( &sResult,
                                 aNode->mRowPtrs[*aSlot],
                                 NULL,
                                 0,
                                 SC_NULL_GRID,
                                 aCallBack->data );
        }

        if ( sResult == ID_TRUE )
        {
            /* ã�� slot�� �ٷε� slot �� �´°��� ã������ full key�� �ٽ� �˻��Ѵ� */

            aMinimum = *aSlot;

            if ( aMaximum >= ( *aSlot + 1 ) )
            {
                aMaximum = *aSlot + 1;
            }
            else
            {
                /* ã�� slot �ڷ� �ٸ� slot�� ����. ã�� slot�� �´�. */
                IDE_CONT( end );
            }
        }
        else
        {
            aMaximum = *aSlot;
        }

        if ( aMinimum <= aMaximum )
        {
            findLastRowInInternal( aCallBack,
                                   aNode,
                                   aMinimum,
                                   aMaximum,
                                   aSlot );
        }
        else
        {
            /* PROJ-2433
             * node �������� �����ϴ°��̾���. */
            *aSlot = aMinimum - 1;
        }
    }

    IDE_EXCEPTION_CONT( end );

    return;
}

void smnbBTree::findLastRowInInternal( const smiCallBack   * aCallBack,
                                       const smnbINode     * aNode,
                                       SInt                  aMinimum,
                                       SInt                  aMaximum,
                                       SInt                * aSlot )
{
    SInt      sMedium   = 0;
    idBool    sResult;
    SChar   * sRow      = NULL;

    do
    {
        sMedium = (aMinimum + aMaximum) >> 1;
        sRow    = aNode->mRowPtrs[sMedium];

        if ( sRow == NULL )
        {
            sResult = ID_FALSE;
        }
        else
        {
            // BUG-27948 �̹� ������ Row�� ���� �ִ��� Ȯ��
            IDE_DASSERT( !SM_SCN_IS_FREE_ROW(((smpSlotHeader *)sRow)->mCreateSCN) );

            (void)aCallBack->callback( &sResult,
                                       sRow,
                                       NULL,
                                       0,
                                       SC_NULL_GRID,
                                       aCallBack->data );
        }

        if ( sResult == ID_TRUE )
        {
            aMinimum = sMedium + 1;
            *aSlot   = (UInt)aMinimum;
        }
        else
        {
            aMaximum = sMedium - 1;
            *aSlot   = (UInt)sMedium;
        }
    }
    while ( aMinimum <= aMaximum );
}

void smnbBTree::findLastKeyInInternal( smnbHeader          * aHeader,
                                       const smiCallBack   * aCallBack,
                                       const smnbINode     * aNode,
                                       SInt                  aMinimum,
                                       SInt                  aMaximum,
                                       SInt                * aSlot )
{
    SInt      sMedium           = 0;
    idBool    sResult;
    SChar   * sRow              = NULL;
    void    * sKey              = NULL;
    UInt      sPartialKeySize   = ( aHeader->mIsPartialKey == ID_TRUE ) ? (aNode->mKeySize) : 0;

    do
    {
        sMedium = (aMinimum + aMaximum) >> 1;
        sKey    = SMNB_GET_KEY_PTR( aNode, sMedium );
        sRow    = aNode->mRowPtrs[sMedium];
                         
        if ( sRow == NULL )
        {
            sResult = ID_FALSE;
        }
        else
        {
            (void)aCallBack->callback( &sResult,
                                       sRow, /* composite key index�� ù��°���� �÷��� ���ϱ����ؼ�*/
                                       sKey,
                                       sPartialKeySize,
                                       SC_NULL_GRID,
                                       aCallBack->data );
        }

        if ( sResult == ID_TRUE )
        {
            aMinimum = sMedium + 1;
            *aSlot   = (UInt)aMinimum;
        }
        else
        {
            aMaximum = sMedium - 1;
            *aSlot   = (UInt)sMedium;
        }
    }
    while ( aMinimum <= aMaximum );
}

IDE_RC smnbBTree::afterLast( smnbIterator* a_pIterator,
                             const smSeekFunc** )
{
    for( a_pIterator->mKeyRange        = a_pIterator->mKeyRange;
         a_pIterator->mKeyRange->next != NULL;
         a_pIterator->mKeyRange        = a_pIterator->mKeyRange->next ) ;

    IDE_TEST( afterLastInternal( a_pIterator ) != IDE_SUCCESS );

    a_pIterator->flag  = a_pIterator->flag & (~SMI_RETRAVERSE_MASK);
    a_pIterator->flag |= SMI_RETRAVERSE_AFTER;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smnbBTree::afterLastInternal( smnbIterator* a_pIterator )
{
    /* BUG-37643 Node�� array�� ���������� �����ϴµ�
     * compiler ����ȭ�� ���ؼ� ���������� ���ŵ� �� �ִ�.
     * ���� �̷��� ������ volatile�� �����ؾ� �Ѵ�. */
    const smiRange* s_pRange;
    idBool          s_bResult;
    SInt            s_nSlot;
    SInt            i = 0;
    SInt            s_nMin;
    SInt            s_nMax;
    SInt            s_cSlot;
    volatile SChar* s_pRow;
    idBool          s_bNext;
    IDU_LATCH       s_version;
    smnbStack       sStack[SMNB_STACK_DEPTH];
    SInt            sDepth = -1;
    smnbLNode*      s_pCurLNode = NULL;
    smnbLNode*      s_pNxtLNode = NULL;
    smnbLNode*      s_pPrvLNode = NULL;
    void          * sKey        = NULL;

    IDE_ASSERT( lockTree( a_pIterator->index ) == IDE_SUCCESS );

    s_pRange = a_pIterator->mKeyRange;

    if ( a_pIterator->mProperties->mReadRecordCount > 0 )
    {
        IDE_TEST( smnbBTree::findLast( a_pIterator->index,
                                       &(s_pRange->maximum),
                                       &sDepth,
                                       sStack,
                                       &s_nSlot ) != IDE_SUCCESS );

        while( (sDepth < 0) && (s_pRange->prev != NULL) )
        {
            s_pRange = s_pRange->prev;
            IDE_TEST( smnbBTree::findLast( a_pIterator->index,
                                           &(s_pRange->maximum),
                                           &sDepth,
                                           sStack,
                                           &s_nSlot ) != IDE_SUCCESS );
        }
    }

    a_pIterator->mKeyRange = s_pRange;

    if ( ( sDepth >= 0 ) && ( a_pIterator->mProperties->mReadRecordCount >  0 ) )
    {
        s_pCurLNode = (smnbLNode*)(sStack[sDepth].node);
        s_bNext     = ID_TRUE;
        s_version   = getLatchValueOfLNode(s_pCurLNode) & (~SMNB_SCAN_LATCH_BIT);

        IDL_MEM_BARRIER;

        while(1)
        {
            s_cSlot      = s_pCurLNode->mSlotCount;
            s_pNxtLNode  = s_pCurLNode->nextSPtr;
            s_pPrvLNode  = s_pCurLNode->prevSPtr;

            IDE_ERROR_RAISE( s_cSlot > 0, ERR_CORRUPTED_INDEX );

            findLastSlotInLeaf( a_pIterator->index,
                                &s_pRange->maximum,
                                s_pCurLNode,
                                0,
                                s_cSlot - 1,
                                &s_nSlot);

            a_pIterator->version   = s_version;
            a_pIterator->node      = s_pCurLNode;
            a_pIterator->prvNode   = s_pCurLNode->prevSPtr;
            a_pIterator->nxtNode   = s_pCurLNode->nextSPtr;
            a_pIterator->least     = ID_FALSE;
            a_pIterator->highest   = ID_TRUE;

            s_nMin = 0;
            s_nMax = s_nSlot;

            if ( s_nSlot > 0 )
            {
                s_pRow = a_pIterator->node->mRowPtrs[s_nSlot];
                
                if ( s_pRow == NULL )
                {
                    IDL_MEM_BARRIER;
                    s_version = getLatchValueOfLNode(s_pCurLNode) & (~SMNB_SCAN_LATCH_BIT);
                    IDL_MEM_BARRIER;

                    continue;
                }

                /*
                 * PROJ-2433
                 * ���ݰ��� �ּҰ� ���ǵ� �����ϴ��� Ȯ���ϴºκ��̴�.
                 * direct key�� full key���߸� ����ε� �񱳰� �����ϴ�.
                 */ 
                if ( isFullKeyInLeafSlot( a_pIterator->index,
                                          a_pIterator->node,
                                          (SShort)s_nSlot ) == ID_TRUE )
                {
                    sKey = SMNB_GET_KEY_PTR( a_pIterator->node, s_nSlot );
                }
                else
                {
                    sKey = NULL;
                }

                // BUG-27948 �̹� ������ Row�� ���� �ִ��� Ȯ��
                IDE_DASSERT( !SM_SCN_IS_FREE_ROW(((smpSlotHeader *)s_pRow)->mCreateSCN ));

                /* PROJ-2433 */
                s_pRange->minimum.callback( &s_bResult,
                                            (SChar*)s_pRow,
                                            sKey,
                                            0,
                                            SC_NULL_GRID,
                                            s_pRange->minimum.data );
                if ( s_bResult == ID_TRUE )
                {
                    if ( s_nSlot > 1 )
                    {
                        s_pRow = a_pIterator->node->mRowPtrs[s_nSlot - 1];

                        if ( s_pRow == NULL )
                        {
                            IDL_MEM_BARRIER;
                            s_version = getLatchValueOfLNode(s_pCurLNode) & (~SMNB_SCAN_LATCH_BIT);
                            IDL_MEM_BARRIER;

                            continue;
                        }

                        /* PROJ-2433
                         * ���ݰ��� �ּҰ� ���ǵ� �����ϴ��� Ȯ���ϴºκ��̴�.
                         * direct key�� full key���߸� ����ε� �񱳰� �����ϴ�. */
                        if ( isFullKeyInLeafSlot( a_pIterator->index,
                                                  a_pIterator->node,
                                                  (SShort)( s_nSlot - 1 ) ) == ID_TRUE )
                        {
                            sKey = SMNB_GET_KEY_PTR( a_pIterator->node, s_nSlot - 1 );
                        }
                        else
                        {
                            sKey = NULL;
                        }

                        // BUG-27948 �̹� ������ Row�� ���� �ִ��� Ȯ��
                        IDE_DASSERT( !SM_SCN_IS_FREE_ROW(((smpSlotHeader *)s_pRow)->mCreateSCN ));

                        /* PROJ-2433 */
                        s_pRange->minimum.callback( &s_bResult,
                                                    (SChar*)s_pRow,
                                                    sKey,
                                                    0,
                                                    SC_NULL_GRID,
                                                    s_pRange->minimum.data );
                        if ( s_bResult == ID_FALSE )
                        {
                            a_pIterator->least    = ID_TRUE;
                            s_nMin                = s_nSlot;
                        }
                    }
                }
                else
                {
                    a_pIterator->least = ID_TRUE;
                    s_nMin = s_nSlot + 1;
                }
            }

            if ( a_pIterator->least == ID_FALSE )
            {
                s_pRow = a_pIterator->node->mRowPtrs[0];

                if ( s_pRow == NULL )
                {
                    IDL_MEM_BARRIER;
                    s_version = getLatchValueOfLNode(s_pCurLNode) & (~SMNB_SCAN_LATCH_BIT);
                    IDL_MEM_BARRIER;

                    continue;
                }

                /* PROJ-2433
                 * ���ݰ��� �ּҰ� ���ǵ� �����ϴ��� Ȯ���ϴºκ��̴�.
                 * direct key�� full key���߸� ����ε� �񱳰� �����ϴ�. */
                if ( isFullKeyInLeafSlot( a_pIterator->index,
                                          a_pIterator->node,
                                          0 ) == ID_TRUE )
                {
                    sKey = SMNB_GET_KEY_PTR( a_pIterator->node, 0 );
                }
                else
                {
                    sKey = NULL;
                }

                // BUG-27948 �̹� ������ Row�� ���� �ִ��� Ȯ��
                IDE_DASSERT( !SM_SCN_IS_FREE_ROW(((smpSlotHeader *)s_pRow)->mCreateSCN ));

                /* PROJ-2433 */
                s_pRange->minimum.callback( &s_bResult,
                                            (SChar*)s_pRow,
                                            sKey,
                                            0,
                                            SC_NULL_GRID,
                                            s_pRange->minimum.data );
                if ( s_bResult == ID_TRUE )
                {
                    a_pIterator->least    = ID_FALSE;
                    s_nMin                = 0;
                }
                else
                {
                    smnbBTree::findFirstSlotInLeaf( a_pIterator->index,
                                                    &(s_pRange->minimum),
                                                    a_pIterator->node,
                                                    0,
                                                    a_pIterator->node->mSlotCount - 1,
                                                    &s_nSlot );

                    a_pIterator->least  = ID_TRUE;
                    s_nMin              = s_nSlot;
                }
            }

            IDU_FIT_POINT( "1.BUG-31890@smnbBTree::afterLastInternal" );

            /* PROJ-2433 */
            if ( ( s_nMin >= 0 ) &&
                 ( s_nMax - s_nMin >= 0 ) )
            {
                idlOS::memcpy( &(a_pIterator->rows[0]),
                               &(a_pIterator->node->mRowPtrs[s_nMin]),
                               (s_nMax - s_nMin + 1) * ID_SIZEOF(a_pIterator->node->mRowPtrs[0]) );
                i = s_nMax - s_nMin;
            }
            else
            {
                i = -1;
            }

            a_pIterator->lowFence  = a_pIterator->rows;
            a_pIterator->highFence = a_pIterator->rows + i;
            a_pIterator->slot      = a_pIterator->highFence + 1;

            IDL_MEM_BARRIER;

            if ( s_version == getLatchValueOfLNode(s_pCurLNode) )
            {
                if ( i != -1 )
                {
                    break;
                }
                else
                {
                    /* nothing to do */
                }

                IDE_ERROR_RAISE( s_cSlot != 0, ERR_CORRUPTED_INDEX );

                if ( s_nSlot < s_cSlot - 1)
                {
                    s_bNext = ID_FALSE;
                }
                else
                {
                    /* nothing to do */
                }

                if ( ( s_bNext == ID_TRUE ) && ( s_pNxtLNode != NULL ) )
                {
                    s_pCurLNode = s_pNxtLNode;
                }
                else
                {
                    s_bNext = ID_FALSE;
                    s_pCurLNode = s_pPrvLNode;
                }

                if ( ( s_pCurLNode == NULL ) || ( a_pIterator->least == ID_TRUE ) )
                {
                    s_pCurLNode = NULL;
                    break;
                }
                else
                {
                    /* nothing to do */
                }
            }
            else
            {
                /* nothing to do */
            }

            s_nSlot   = 0;

            IDL_MEM_BARRIER;
            s_version = getLatchValueOfLNode(s_pCurLNode) & (~SMNB_SCAN_LATCH_BIT);
            IDL_MEM_BARRIER;
        }//while
    }

    if ( s_pCurLNode == NULL )
    {
        a_pIterator->least     =
        a_pIterator->highest   = ID_TRUE;
        a_pIterator->slot      = (SChar**)&a_pIterator->slot;
        a_pIterator->lowFence  = a_pIterator->slot + 1;
        a_pIterator->highFence = a_pIterator->slot - 1;
    }

    IDE_ASSERT( unlockTree( a_pIterator->index ) == IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_CORRUPTED_INDEX )
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_INCONSISTENT_INDEX ) );

        ideLog::log( IDE_SM_0, "Index Corrupted Detected : Wrong Index Slot Position" );
    }
    IDE_EXCEPTION_END;

    if ( ideGetErrorCode() == smERR_ABORT_INCONSISTENT_INDEX )
    {
        smnManager::setIsConsistentOfIndexHeader( a_pIterator->index->mIndexHeader, ID_FALSE );
    }

    return IDE_FAILURE;
}

IDE_RC smnbBTree::beforeFirstU( smnbIterator*       aIterator,
                                const smSeekFunc** aSeekFunc )
{
    IDE_TEST( smnbBTree::beforeFirst( aIterator, aSeekFunc ) != IDE_SUCCESS );

    *aSeekFunc += 6;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smnbBTree::afterLastU( smnbIterator*       aIterator,
                              const smSeekFunc** aSeekFunc )
{
    IDE_TEST( smnbBTree::afterLast( aIterator, aSeekFunc ) != IDE_SUCCESS );

    *aSeekFunc += 6;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smnbBTree::beforeFirstR( smnbIterator*       aIterator,
                                const smSeekFunc** aSeekFunc )
{
    smnbIterator        *sIterator;
    smiCursorProperties sCursorProperty;
    UInt                sState = 0;
    ULong               sIteratorBuffer[ SMI_ITERATOR_SIZE / ID_SIZEOF(ULong) ];

    sIterator = (smnbIterator *)sIteratorBuffer;
    IDE_TEST( allocIterator( (void**)&sIterator ) != IDE_SUCCESS );
    sState = 1;

    IDE_TEST( smnbBTree::beforeFirst( aIterator, aSeekFunc ) != IDE_SUCCESS );
    sIterator->curRecPtr       = aIterator->curRecPtr;
    sIterator->lstFetchRecPtr  = aIterator->lstFetchRecPtr;
    sIterator->lstReturnRecPtr = aIterator->lstReturnRecPtr;
    sIterator->least           = aIterator->least;
    sIterator->highest         = aIterator->highest;
    sIterator->mKeyRange       = aIterator->mKeyRange;
    sIterator->node            = aIterator->node;
    sIterator->nxtNode         = aIterator->nxtNode;
    sIterator->prvNode         = aIterator->prvNode;
    sIterator->slot           = aIterator->slot;
    sIterator->lowFence       = aIterator->lowFence;
    sIterator->highFence      = aIterator->highFence;
    sIterator->flag           = aIterator->flag;

    /* (BUG-45368) ��ü ���������ʰ� ����Ǵ� ���� ������ ����. */
    sCursorProperty.mReadRecordCount    = aIterator->mProperties->mReadRecordCount;
    sCursorProperty.mFirstReadRecordPos = aIterator->mProperties->mFirstReadRecordPos;

    /* (BUG-45368) node cache ��ü�� �������� �ʰ� ���� ��ŭ�� ������ ����. */
    if ( aIterator->highFence >= aIterator->lowFence )
    {
        idlOS::memcpy( sIterator->rows,
                       aIterator->rows,
                       ( ( (void **)aIterator->highFence - (void **)aIterator->lowFence + 1 ) *
                         ID_SIZEOF(aIterator->rows) ) );
    }
    else
    {
        /* nothing to do */
    }

    IDE_TEST( smnbBTree::fetchNextR( aIterator ) != IDE_SUCCESS );

    aIterator->curRecPtr       = sIterator->curRecPtr;
    aIterator->lstFetchRecPtr  = sIterator->lstFetchRecPtr;
    aIterator->lstReturnRecPtr = sIterator->lstReturnRecPtr;
    aIterator->least           = sIterator->least;
    aIterator->highest         = sIterator->highest;
    aIterator->mKeyRange       = sIterator->mKeyRange;
    aIterator->node            = sIterator->node;
    aIterator->nxtNode         = sIterator->nxtNode;
    aIterator->prvNode         = sIterator->prvNode;
    aIterator->slot           = sIterator->slot;
    aIterator->lowFence       = sIterator->lowFence;
    aIterator->highFence      = sIterator->highFence;
    aIterator->flag           = sIterator->flag;

    aIterator->mProperties->mReadRecordCount    = sCursorProperty.mReadRecordCount;
    aIterator->mProperties->mFirstReadRecordPos = sCursorProperty.mFirstReadRecordPos;

    if ( sIterator->highFence >= sIterator->lowFence )
    {
        idlOS::memcpy( aIterator->rows,
                       sIterator->rows,
                       ( ( (void **)sIterator->highFence - (void **)sIterator->lowFence + 1 ) *
                         ID_SIZEOF(sIterator->rows) ) );
    }
    else
    {
        /* nothing to do */
    }

    *aSeekFunc += 6;

    sState = 0;
    IDE_TEST( freeIterator( sIterator ) != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if ( sState == 1 )
    {
        IDE_ASSERT( freeIterator( sIterator ) == IDE_SUCCESS );
    }

    return IDE_FAILURE;
}

IDE_RC smnbBTree::afterLastR( smnbIterator*       aIterator,
                              const smSeekFunc** aSeekFunc )
{
    IDE_TEST( smnbBTree::beforeFirst( aIterator, aSeekFunc ) != IDE_SUCCESS );

    IDE_TEST( smnbBTree::fetchNextR( aIterator ) != IDE_SUCCESS );

    IDE_TEST( smnbBTree::afterLast( aIterator, aSeekFunc ) != IDE_SUCCESS );

    *aSeekFunc += 6;

    aIterator->flag  = aIterator->flag & (~SMI_RETRAVERSE_MASK);
    aIterator->flag |= SMI_RETRAVERSE_AFTER;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smnbBTree::fetchNext( smnbIterator    * a_pIterator,
                             const void     ** aRow )
{
    idBool sIsRestart;
    idBool sResult;

restart:

    for( a_pIterator->slot++;
         a_pIterator->slot <= a_pIterator->highFence;
         a_pIterator->slot++ )
    {
        a_pIterator->curRecPtr      = *a_pIterator->slot;
        a_pIterator->lstFetchRecPtr = a_pIterator->curRecPtr;

        /* PROJ-2433 
         * NULLüũ checkSCN ��ġ����*/
        if ( a_pIterator->curRecPtr == NULL )
        {
            a_pIterator->highFence = a_pIterator->slot - 1;
            break;
        }
        else
        {
            /* nothing to do */
        }

        if ( smnManager::checkSCN( (smiIterator*)a_pIterator,
                                   a_pIterator->curRecPtr )
            == ID_FALSE )
        {
            continue;
        }
        else
        {
            /* nothing to do */
        }

        // BUG-27948 �̹� ������ Row�� ���� �ִ��� Ȯ��
        IDE_DASSERT( !SM_SCN_IS_FREE_ROW
                     (((smpSlotHeader *)(a_pIterator->curRecPtr))->mCreateSCN) );

        *aRow = a_pIterator->curRecPtr;
        a_pIterator->lstReturnRecPtr = a_pIterator->curRecPtr;

        SC_MAKE_GRID( a_pIterator->mRowGRID,
                      a_pIterator->table->mSpaceID,
                      SMP_SLOT_GET_PID( (smpSlotHeader*)*aRow ),
                      SMP_SLOT_GET_OFFSET( (smpSlotHeader*)*aRow ) );

        IDE_TEST( a_pIterator->mRowFilter->callback( &sResult,
                                                     *aRow,
                                                     NULL,
                                                     0,
                                                     a_pIterator->mRowGRID,
                                                     a_pIterator->mRowFilter->data )
                  != IDE_SUCCESS );

        if ( sResult == ID_TRUE )
        {
            if ( a_pIterator->mProperties->mFirstReadRecordPos == 0 )
            {
                if ( a_pIterator->mProperties->mReadRecordCount != 1 )
                {
                    a_pIterator->mProperties->mReadRecordCount--;

                    return IDE_SUCCESS;
                }
                else
                {
                    a_pIterator->mProperties->mReadRecordCount--;

                    a_pIterator->highFence = a_pIterator->slot;
                    a_pIterator->highest   = ID_TRUE;

                    return IDE_SUCCESS;
                }
            }
            else
            {
                a_pIterator->mProperties->mFirstReadRecordPos--;
            }
        }
    }

    if ( a_pIterator->highest == ID_FALSE )
    {
        sIsRestart = ID_FALSE;

        IDE_TEST( getNextNode( a_pIterator,
                               &sIsRestart ) != IDE_SUCCESS );

        if ( sIsRestart == ID_TRUE )
        {
            goto restart;
        }
        else
        {
            /* nothing to do */
        }
    }
    else
    {
        /* nothing to do */
    }

    a_pIterator->highest = ID_TRUE;

    if ( a_pIterator->mKeyRange->next != NULL )
    {
        a_pIterator->mKeyRange = a_pIterator->mKeyRange->next;
        (void)beforeFirstInternal( a_pIterator );

        IDE_TEST( iduCheckSessionEvent(a_pIterator->mProperties->mStatistics) != IDE_SUCCESS );

        goto restart;
    }
    else
    {
        /* nothing to do */
    }

    a_pIterator->slot            = a_pIterator->highFence + 1;
    a_pIterator->curRecPtr       = NULL;
    a_pIterator->lstFetchRecPtr  = NULL;
    a_pIterator->lstReturnRecPtr = NULL;
    SC_MAKE_NULL_GRID( a_pIterator->mRowGRID );
    *aRow                        = NULL;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smnbBTree::getNextNode( smnbIterator   * aIterator,
                               idBool         * aIsRestart )
{
    idBool        sResult;
    SInt          sSlotCount  = 0;
    SInt          i           = -1;
    IDU_LATCH     sVersion    = IDU_LATCH_UNLOCKED;
    smnbLNode   * sCurLNode   = NULL;
    smnbLNode   * sNxtLNode   = NULL;
    SChar       * sRowPtr     = NULL;
    void        * sKey        = NULL;
    SChar       * sLastReadRow  = NULL;     /* ���� ��忡�� ���������� ���� row pointer */
    idBool        isSetRowCache = ID_FALSE;
    SInt          sMin;
    SInt          sMax;

    IDE_ASSERT( aIterator->highest == ID_FALSE );
    *aIsRestart = ID_FALSE;

    sCurLNode = aIterator->nxtNode;

    sLastReadRow = aIterator->lstReturnRecPtr;

    IDU_FIT_POINT( "smnbBTree::getNextNode::wait" );

    /* BUG-44043 

       Key redistribution�� ��������
       ���� ��忡�� ������ �о��� row ( = aIterator->lstReturnRecPtr ) ��
       ������ �Ǵ� ������忡 �����Ҽ��ִ�.

       aIterator->lstReturnRecPtr ���� ū ù��° row ��ġ�� �߰��Ҷ�����
       next node�� Ž���Ѵ�.
     */ 

    while ( sCurLNode != NULL )
    {
        sVersion   = getLatchValueOfLNode(sCurLNode) & IDU_LATCH_UNMASK;
        IDL_MEM_BARRIER;

        while ( 1 )
        {
            isSetRowCache = ID_FALSE;
            sSlotCount    = sCurLNode->mSlotCount;
            sNxtLNode     = sCurLNode->nextSPtr;

            if ( sSlotCount == 0 )
            {
                IDL_MEM_BARRIER;
                if ( sVersion == getLatchValueOfLNode( sCurLNode ) )
                {
                    sCurLNode = sNxtLNode;
                    break;
                }
                else
                {
                    IDL_MEM_BARRIER;
                    sVersion   = getLatchValueOfLNode( sCurLNode ) & IDU_LATCH_UNMASK;
                    IDL_MEM_BARRIER;

                    continue;
                }
            }
            else
            {
                /* nothing to do */
            }

            findNextSlotInLeaf( &aIterator->index->mAgerStat,
                                aIterator->index->columns,
                                aIterator->index->fence,
                                sCurLNode,
                                sLastReadRow,
                                &sMin );

            if ( sMin >= sSlotCount )
            { 
                /* �����忡�� sLastReadRow�� ã�����Ѱ��,
                   ������忡�� �ٽ� ã���� �Ѵ�. */
                IDL_MEM_BARRIER;
                if ( sVersion == getLatchValueOfLNode( sCurLNode ) )
                {
                    sCurLNode = sNxtLNode;
                    break;
                }
                else
                {
                    IDL_MEM_BARRIER;
                    sVersion   = getLatchValueOfLNode(sCurLNode) & IDU_LATCH_UNMASK;
                    IDL_MEM_BARRIER;

                    continue;
                }
            }
            else
            {
                /* nothing to do */
            }

            i = -1;

            aIterator->highest = ID_FALSE;
            aIterator->least   = ID_FALSE;
            aIterator->node    = sCurLNode;
            aIterator->prvNode = sCurLNode->prevSPtr;
            aIterator->nxtNode = sCurLNode->nextSPtr;

            sRowPtr = sCurLNode->mRowPtrs[sSlotCount - 1];

            if ( sRowPtr == NULL )
            {
                IDL_MEM_BARRIER;
                sVersion   = getLatchValueOfLNode(sCurLNode) & IDU_LATCH_UNMASK;
                IDL_MEM_BARRIER;

                continue;
            }
            else
            {
                /* nothing to do */
            }

            /* PROJ-2433
             * �ִ밪 ������ �����ϴ��� Ȯ���ϴºκ��̴�.
             * direct key�� full key���߸� ����ε� �񱳰� �����ϴ�.
             */
            if ( isFullKeyInLeafSlot( aIterator->index,
                                      sCurLNode,
                                      (SShort)( sSlotCount - 1 ) ) == ID_TRUE )
            {
                sKey = SMNB_GET_KEY_PTR( sCurLNode, sSlotCount - 1 );
            }
            else
            {
                sKey = NULL;
            }

            /* BUG-27948 �̹� ������ Row�� ���� �ִ��� Ȯ�� */
            IDE_DASSERT( !SM_SCN_IS_FREE_ROW(((smpSlotHeader *)sRowPtr)->mCreateSCN) );

            aIterator->mKeyRange->maximum.callback( &sResult,
                                                    sRowPtr,
                                                    sKey,
                                                    0,
                                                    SC_NULL_GRID,
                                                    aIterator->mKeyRange->maximum.data );

            sMax = sSlotCount - 1;

            if ( sResult == ID_FALSE )
            {
                smnbBTree::findLastSlotInLeaf( aIterator->index,
                                               &aIterator->mKeyRange->maximum,
                                               sCurLNode,
                                               sMin,
                                               sSlotCount - 1,
                                               &sMax );
                aIterator->highest   = ID_TRUE;
            }
            else
            {
                /* nothing to do */
            }

            /* PROJ-2433 */
            if ( ( sMin >= 0 ) &&
                 ( sMax - sMin >= 0 ) )
            {
                idlOS::memcpy( &(aIterator->rows[0]),
                               &(sCurLNode->mRowPtrs[sMin]),
                               ( sMax - sMin + 1 ) * ID_SIZEOF(sCurLNode->mRowPtrs[0]) );
                i = sMax - sMin;
            }
            else
            {
                i = -1;
            }

            isSetRowCache = ID_TRUE;

            IDL_MEM_BARRIER;
            if ( sVersion == getLatchValueOfLNode( sCurLNode ) )
            {
                break;
            }

            IDL_MEM_BARRIER;
            sVersion   = getLatchValueOfLNode(sCurLNode) & IDU_LATCH_UNMASK;
            IDL_MEM_BARRIER;
        } /* loop for node latch */

        if ( isSetRowCache == ID_TRUE )
        {
            /* row cache�� ���õǾ�����, ������ iterator���鵵 �����Ѵ�. */

            aIterator->highFence = aIterator->rows + i;
            aIterator->lowFence  = aIterator->rows;
            aIterator->slot      = aIterator->lowFence - 1;

            aIterator->version   = sVersion;
            aIterator->least     = ID_FALSE;

            IDE_TEST( iduCheckSessionEvent( aIterator->mProperties->mStatistics ) != IDE_SUCCESS );

            *aIsRestart = ID_TRUE;

            break;
        }
        else
        {
            /* nothing to do */
        }

    } /* loop for next node */

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smnbBTree::fetchPrev( smnbIterator* aIterator,
                             const void**  aRow )
{
    idBool sIsRestart;
    idBool sResult;

restart:

    for ( aIterator->slot-- ;
          aIterator->slot >= aIterator->lowFence ;
          aIterator->slot-- )
    {
        aIterator->curRecPtr      = *aIterator->slot;
        aIterator->lstFetchRecPtr = aIterator->curRecPtr;

        /* PROJ-2433 */
        if ( aIterator->curRecPtr == NULL )
        {
            continue;
        }
        else
        {
            /* nothing to do */
        }
        if ( smnManager::checkSCN( (smiIterator*)aIterator,
                                   aIterator->curRecPtr )
             == ID_FALSE )
        {
            continue;
        }
        else
        {
            /* nothing to do */
        }

        // BUG-27948 �̹� ������ Row�� ���� �ִ��� Ȯ��
        IDE_DASSERT( !SM_SCN_IS_FREE_ROW
                     (((smpSlotHeader *)(aIterator->curRecPtr))->mCreateSCN) );

        *aRow = aIterator->curRecPtr;

        SC_MAKE_GRID( aIterator->mRowGRID,
                      aIterator->table->mSpaceID,
                      SMP_SLOT_GET_PID( (smpSlotHeader*)*aRow ),
                      SMP_SLOT_GET_OFFSET( (smpSlotHeader*)*aRow ) );

        IDE_TEST( aIterator->mRowFilter->callback( &sResult,
                                                   *aRow,
                                                   NULL,
                                                   0,
                                                   aIterator->mRowGRID,
                                                   aIterator->mRowFilter->data )
                  != IDE_SUCCESS );

        if ( sResult == ID_TRUE )
        {
            if ( aIterator->mProperties->mFirstReadRecordPos == 0 )
            {
                if ( aIterator->mProperties->mReadRecordCount != 1 )
                {
                    aIterator->mProperties->mReadRecordCount--;

                    return IDE_SUCCESS;
                }
                else
                {
                    aIterator->mProperties->mReadRecordCount--;

                    aIterator->lowFence = aIterator->slot;
                    aIterator->least   = ID_TRUE;
                    return IDE_SUCCESS;
                }
            }
            else
            {
                aIterator->mProperties->mFirstReadRecordPos--;
            }
        }
        else
        {
            /* nothing to do */
        }
    }

    if ( aIterator->least == ID_FALSE )
    {
        sIsRestart = ID_FALSE;

        IDE_TEST( getPrevNode( aIterator,
                               &sIsRestart ) != IDE_SUCCESS );

        if ( sIsRestart == ID_TRUE )
        {
            goto restart;
        }
        else
        {
            /* nothing to do */
        }
    }
    else
    {
        /* nothing to do */
    }

    aIterator->least = ID_TRUE;

    if ( aIterator->mKeyRange->prev != NULL )
    {
        aIterator->mKeyRange = aIterator->mKeyRange->prev;
        (void)afterLastInternal( aIterator );

        IDE_TEST(iduCheckSessionEvent(aIterator->mProperties->mStatistics) != IDE_SUCCESS);

        goto restart;
    }
    else
    {
        /* nothing to do */
    }

    aIterator->slot           = aIterator->lowFence - 1;
    aIterator->curRecPtr      = NULL;
    aIterator->lstFetchRecPtr = NULL;
    SC_MAKE_NULL_GRID( aIterator->mRowGRID );
    *aRow                     = NULL;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smnbBTree::getPrevNode( smnbIterator   * aIterator,
                               idBool         * aIsRestart )
{

    idBool        sResult;
    SInt          sSlot      = 0;
    SInt          sLastSlot  = 0;
    SInt          sSlotCount = 0;
    SInt          i          = -1;
    IDU_LATCH     sVersion   = IDU_LATCH_UNLOCKED;
    smnbLNode   * sCurLNode  = NULL;
    SChar       * sRowPtr    = NULL;
    void        * sKey       = NULL;
    SInt          sState     = 0;

    IDE_ASSERT( aIterator->least == ID_FALSE )
    *aIsRestart = ID_FALSE;

    IDE_TEST( lockTree( aIterator->index ) != IDE_SUCCESS );
    sState = 1;

    sCurLNode = aIterator->node->prevSPtr;

    while ( sCurLNode != NULL )
    {
        i = -1;

        IDL_MEM_BARRIER;
        sVersion = getLatchValueOfLNode(sCurLNode) & IDU_LATCH_UNMASK;
        IDL_MEM_BARRIER;

        sSlotCount = sCurLNode->mSlotCount;

        aIterator->version = sVersion;
        aIterator->node    = sCurLNode;
        aIterator->least   = ID_FALSE;
        aIterator->prvNode = sCurLNode->prevSPtr;
        aIterator->nxtNode = sCurLNode->nextSPtr;

        if ( sSlotCount == 0 )
        {
            sCurLNode = sCurLNode->prevSPtr;
            continue;
        }
        else
        {
            /* nothing to do */
        }

        sRowPtr = aIterator->node->mRowPtrs[0];
        if ( sRowPtr == NULL )
        {
            continue;
        }
        else
        {
            /* nothing to do */
        }

        /* PROJ-2433
         * �ּҰ� ������ �����ϴ��� Ȯ���ϴºκ��̴�.
         * direct key�� full key���߸� ����ε� �񱳰� �����ϴ�. */
        if ( isFullKeyInLeafSlot( aIterator->index,
                                  aIterator->node,
                                  0 ) == ID_TRUE )
        {
            sKey = SMNB_GET_KEY_PTR( aIterator->node, 0 );
        }
        else
        {
            sKey = NULL;
        }

        // BUG-27948 �̹� ������ Row�� ���� �ִ��� Ȯ��
        IDE_DASSERT( !SM_SCN_IS_FREE_ROW(((smpSlotHeader *)aIterator->node->mRowPtrs[0])->mCreateSCN) );

        /* PROJ-2433 */
        aIterator->mKeyRange->minimum.callback( &sResult,
                                                sRowPtr,
                                                sKey,
                                                0,
                                                SC_NULL_GRID,
                                                aIterator->mKeyRange->minimum.data );
        sSlot = 0;

        if ( sResult == ID_FALSE )
        {
            smnbBTree::findFirstSlotInLeaf( aIterator->index,
                                            &aIterator->mKeyRange->minimum,
                                            aIterator->node,
                                            0,
                                            sSlotCount - 1,
                                            &sSlot );
            aIterator->least = ID_TRUE;
        }
        else
        {
            /* nothing to do */
        }

        /* PROJ-2433 */
        sLastSlot = sSlotCount - 1;

        if ( ( sSlot >= 0 ) &&
             ( sLastSlot - sSlot >= 0 ) )
        {
            idlOS::memcpy( &(aIterator->rows[0]),
                           &(aIterator->node->mRowPtrs[sSlot]),
                           (sLastSlot - sSlot + 1) * ID_SIZEOF(aIterator->node->mRowPtrs[0]) );
            i = sLastSlot - sSlot;
        }
        else
        {
            i = -1;
        }

        IDL_MEM_BARRIER;
        if ( sVersion == getLatchValueOfLNode( sCurLNode ) )
        {
            break;
        }
        else
        {
            /* nothing to do */
        }
    }

    /*
     * BUG-31558  It's possible to miss some rows when fetching 
     *            with the descending index(back traverse).
     *
     * if we didn't find any available slot, go to the previous node with holding the tree latch.
     */

    /* PROJ-2433
     * ���� �� ���������ʾƵ���
     * �� node�� �ƴϰ�, ������ checkSCN �����ʰ� ��� ���������Ƿ� ���ϳ��̻��ִ� */
    if ( i >= 0 )
    {
        aIterator->highFence = aIterator->rows + i;
        aIterator->lowFence  = aIterator->rows;
        aIterator->slot      = aIterator->highFence + 1;

        aIterator->highest = ID_FALSE;

        IDE_TEST(iduCheckSessionEvent(aIterator->mProperties->mStatistics) != IDE_SUCCESS);

        *aIsRestart = ID_TRUE;
    }
    else
    {
        /* nothing to do */
    }

    sState = 0;
    IDE_TEST( unlockTree( aIterator->index ) != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if ( sState != 0 )
    {
        IDE_PUSH();
        IDE_ASSERT( unlockTree( aIterator->index ) == IDE_SUCCESS );
        IDE_POP();
    }
    else
    {
        /* nothing to do */
    }

    return IDE_FAILURE;
}

IDE_RC smnbBTree::fetchNextU( smnbIterator* aIterator,
                              const void**  aRow )
{
    idBool sIsRestart;
    idBool sResult;

  restart:

    for( aIterator->slot++;
         aIterator->slot <= aIterator->highFence;
         aIterator->slot++ )
    {
        aIterator->curRecPtr      = *aIterator->slot;
        aIterator->lstFetchRecPtr = aIterator->curRecPtr;

        /* PROJ-2433 
         * NULLüũ checkSCN ��ġ����*/
        if ( aIterator->curRecPtr == NULL )
        {
            aIterator->highFence = aIterator->slot - 1;
            break;
        }
        else
        {
            /* nothing to do */
        }
        if ( smnManager::checkSCN( (smiIterator*)aIterator,
                                   aIterator->curRecPtr )
             == ID_FALSE )
        {
            continue;
        }
        else
        {
            /* nothing to do */
        }

        // BUG-27948 �̹� ������ Row�� ���� �ִ��� Ȯ��
        IDE_DASSERT( !SM_SCN_IS_FREE_ROW
                     (((smpSlotHeader *)(aIterator->curRecPtr))->mCreateSCN) );

        *aRow = aIterator->curRecPtr;
        aIterator->lstReturnRecPtr = aIterator->curRecPtr;

        SC_MAKE_GRID( aIterator->mRowGRID,
                      aIterator->table->mSpaceID,
                      SMP_SLOT_GET_PID( (smpSlotHeader*)*aRow ),
                      SMP_SLOT_GET_OFFSET( (smpSlotHeader*)*aRow ) );

        IDE_TEST( aIterator->mRowFilter->callback( &sResult,
                                                   *aRow,
                                                   NULL,
                                                   0,
                                                   aIterator->mRowGRID,
                                                   aIterator->mRowFilter->data )
                  != IDE_SUCCESS );

        if ( sResult == ID_TRUE )
        {
            smnManager::updatedRow( (smiIterator*)aIterator );

            *aRow = aIterator->curRecPtr;
            SC_MAKE_GRID( aIterator->mRowGRID,
                          aIterator->table->mSpaceID,
                          SMP_SLOT_GET_PID( (smpSlotHeader*)*aRow ),
                          SMP_SLOT_GET_OFFSET( (smpSlotHeader*)*aRow ) );


            if ( SM_SCN_IS_NOT_DELETED( ((smpSlotHeader*)aRow)->mCreateSCN ) )
            {
                if ( aIterator->mProperties->mFirstReadRecordPos == 0 )
                {
                    if ( aIterator->mProperties->mReadRecordCount != 1 )
                    {
                        aIterator->mProperties->mReadRecordCount--;

                        return IDE_SUCCESS;
                    }
                    else
                    {
                        aIterator->mProperties->mReadRecordCount--;

                        aIterator->highFence = aIterator->slot;
                        aIterator->highest   = ID_TRUE;
                        return IDE_SUCCESS;
                    }
                }
                else
                {
                    aIterator->mProperties->mFirstReadRecordPos--;
                }

            }
        }
        else
        {
            /* nothing to do */
        }
    }

    if ( aIterator->highest == ID_FALSE )
    {
        sIsRestart = ID_FALSE;

        IDE_TEST( getNextNode( aIterator, &sIsRestart ) != IDE_SUCCESS );

        if ( sIsRestart == ID_TRUE )
        {
            goto restart;
        }
        else
        {
            /* nothing to do */
        }
    }

    aIterator->highest = ID_TRUE;

    if ( aIterator->mKeyRange->next != NULL )
    {
        aIterator->mKeyRange = aIterator->mKeyRange->next;
        (void)beforeFirstInternal( aIterator );

        IDE_TEST(iduCheckSessionEvent(aIterator->mProperties->mStatistics) != IDE_SUCCESS);

        goto restart;
    }

    aIterator->slot            = aIterator->highFence + 1;
    aIterator->curRecPtr       = NULL;
    aIterator->lstFetchRecPtr  = NULL;
    aIterator->lstReturnRecPtr = NULL;
    SC_MAKE_NULL_GRID( aIterator->mRowGRID );
    *aRow                      = NULL;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smnbBTree::fetchPrevU( smnbIterator* aIterator,
                              const void**  aRow )
{
    idBool sIsRestart;
    idBool sResult;

restart:

    for( aIterator->slot--;
         aIterator->slot >= aIterator->lowFence;
         aIterator->slot-- )
    {
        aIterator->curRecPtr = *aIterator->slot;
        aIterator->lstFetchRecPtr = aIterator->curRecPtr;

        /* PROJ-2433 */
        if ( aIterator->curRecPtr == NULL )
        {
            continue;
        }
        else
        {
            /* nothing to do */
        }
        if ( smnManager::checkSCN( (smiIterator*)aIterator,
                                   aIterator->curRecPtr )
             == ID_FALSE )
        {
            continue;
        }
        else
        {
            /* nothing to do */
        }

        // BUG-27948 �̹� ������ Row�� ���� �ִ��� Ȯ��
        IDE_DASSERT( !SM_SCN_IS_FREE_ROW
                     (((smpSlotHeader *)(aIterator->curRecPtr))->mCreateSCN) );

        *aRow = aIterator->curRecPtr;

        SC_MAKE_GRID( aIterator->mRowGRID,
                      aIterator->table->mSpaceID,
                      SMP_SLOT_GET_PID( (smpSlotHeader*)*aRow ),
                      SMP_SLOT_GET_OFFSET( (smpSlotHeader*)*aRow ) );

        IDE_TEST( aIterator->mRowFilter->callback( &sResult,
                                                   *aRow,
                                                   NULL,
                                                   0,
                                                   aIterator->mRowGRID,
                                                   aIterator->mRowFilter->data )
                  != IDE_SUCCESS );

        if ( sResult == ID_TRUE )
        {
            smnManager::updatedRow( (smiIterator*)aIterator );

            *aRow = aIterator->curRecPtr;
            SC_MAKE_GRID( aIterator->mRowGRID,
                          aIterator->table->mSpaceID,
                          SMP_SLOT_GET_PID( (smpSlotHeader*)*aRow ),
                          SMP_SLOT_GET_OFFSET( (smpSlotHeader*)*aRow ) );


            if ( SM_SCN_IS_NOT_DELETED( ((smpSlotHeader*)aRow)->mCreateSCN ) )
            {
                if ( aIterator->mProperties->mFirstReadRecordPos == 0 )
                {
                    if ( aIterator->mProperties->mReadRecordCount != 1 )
                    {
                        aIterator->mProperties->mReadRecordCount--;

                        return IDE_SUCCESS;
                    }
                    else
                    {
                        aIterator->mProperties->mReadRecordCount--;

                        aIterator->lowFence = aIterator->slot;
                        aIterator->least    = ID_TRUE;
                        return IDE_SUCCESS;
                    }
                }
                else
                {
                    aIterator->mProperties->mFirstReadRecordPos--;
                }

            }
        }
        else
        {
            /* nothing to do */
        }
    }

    if ( aIterator->least == ID_FALSE )
    {
        sIsRestart = ID_FALSE;

        IDE_TEST( getPrevNode( aIterator, &sIsRestart ) != IDE_SUCCESS );

        if ( sIsRestart == ID_TRUE )
        {
            goto restart;
        }
        else
        {
            /* nothing to do */
        }
    }

    aIterator->least = ID_TRUE;

    if ( aIterator->mKeyRange->prev != NULL )
    {
        aIterator->mKeyRange = aIterator->mKeyRange->prev;
        (void)afterLastInternal( aIterator );

        IDE_TEST(iduCheckSessionEvent(aIterator->mProperties->mStatistics) != IDE_SUCCESS);

        goto restart;
    }

    aIterator->slot           = aIterator->lowFence - 1;
    aIterator->curRecPtr      = NULL;
    aIterator->lstFetchRecPtr = NULL;
    SC_MAKE_NULL_GRID( aIterator->mRowGRID );
    *aRow                     = NULL;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smnbBTree::fetchNextR( smnbIterator* aIterator )
{
    SChar   * sRow = NULL;
    scGRID    sRowGRID;
    idBool    sIsRestart;
    idBool    sResult;

  restart:

    for( aIterator->slot++;
         aIterator->slot <= aIterator->highFence;
         aIterator->slot++ )
    {
        aIterator->curRecPtr      = *aIterator->slot;
        aIterator->lstFetchRecPtr = aIterator->curRecPtr;

        /* PROJ-2433 
         * NULLüũ checkSCN ��ġ����*/
        if ( aIterator->curRecPtr == NULL )
        {
            aIterator->highFence = aIterator->slot - 1;
            break;
        }
        else
        {
            /* nothing to do */
        }
        if ( smnManager::checkSCN( (smiIterator*)aIterator,
                                   aIterator->curRecPtr )
             == ID_FALSE )
        {
            continue;
        }
        else
        {
            /* nothing to do */
        }

        // BUG-27948 �̹� ������ Row�� ���� �ִ��� Ȯ��
        IDE_DASSERT( !SM_SCN_IS_FREE_ROW
                     (((smpSlotHeader *)(aIterator->curRecPtr))->mCreateSCN) );

        sRow = aIterator->curRecPtr;
        aIterator->lstReturnRecPtr = aIterator->curRecPtr;

        SC_MAKE_GRID( sRowGRID,
                      aIterator->table->mSpaceID,
                      SMP_SLOT_GET_PID( (smpSlotHeader*)sRow ),
                      SMP_SLOT_GET_OFFSET( (smpSlotHeader*)sRow ) );

        IDE_TEST( aIterator->mRowFilter->callback( &sResult,
                                                   sRow,
                                                   NULL,
                                                   0,
                                                   sRowGRID,
                                                   aIterator->mRowFilter->data )
                  != IDE_SUCCESS );

        if ( sResult == ID_TRUE )
        {
            if ( aIterator->mProperties->mFirstReadRecordPos == 0 )
            {
                if ( aIterator->mProperties->mReadRecordCount != 1 )
                {
                    aIterator->mProperties->mReadRecordCount--;

                    IDE_TEST( smnManager::lockRow( (smiIterator*)aIterator)
                              != IDE_SUCCESS );
                }
                else
                {
                    aIterator->mProperties->mReadRecordCount--;

                    aIterator->highFence = aIterator->slot;
                    aIterator->highest   = ID_TRUE;
                    IDE_TEST( smnManager::lockRow( (smiIterator*)aIterator)
                              != IDE_SUCCESS );
                }
            }
            else
            {
                aIterator->mProperties->mFirstReadRecordPos--;
            }
        }
    }

    if ( aIterator->highest == ID_FALSE )
    {
        sIsRestart = ID_FALSE;

        IDE_TEST( getNextNode( aIterator, &sIsRestart ) != IDE_SUCCESS );

        if ( sIsRestart == ID_TRUE )
        {
            goto restart;
        }
        else
        {
            /* nothing to do */
        }
    }

    aIterator->highest = ID_TRUE;

    if ( aIterator->mKeyRange->next != NULL )
    {
        aIterator->mKeyRange = aIterator->mKeyRange->next;
        (void)beforeFirstInternal( aIterator );

        IDE_TEST(iduCheckSessionEvent(aIterator->mProperties->mStatistics) != IDE_SUCCESS);

        goto restart;
    }

    aIterator->slot            = aIterator->highFence + 1;
    aIterator->curRecPtr       = NULL;
    aIterator->lstReturnRecPtr = NULL;
    aIterator->lstFetchRecPtr  = NULL;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smnbBTree::retraverse( idvSQL        * /* aStatistics */,
                              smnbIterator  * aIterator,
                              const void   ** /* aRow */ )
{
    idBool            sResult;
    smnbHeader*       sHeader;
    SChar*            sRowPtr;
    UInt              sFlag;
    SInt              sMin;
    SInt              sMax;
    SInt              i, j;
    IDU_LATCH         sVersion;
    smnbLNode*        sCurLNode;
    smnbLNode*        sNxtLNode;
    SInt              sSlotCount;
    SChar*            sTmpRowPtr;
    void            * sTmpKey = NULL;
    smnbStack         sStack[SMNB_STACK_DEPTH];
    SInt              sDepth;

    sRowPtr  = aIterator->lstFetchRecPtr;
    sFlag    = aIterator->flag & SMI_RETRAVERSE_MASK;

    if ( sRowPtr == NULL )
    {
        IDE_ASSERT(sFlag != SMI_RETRAVERSE_NEXT);

        if ( sFlag == SMI_RETRAVERSE_BEFORE )
        {
            IDE_TEST(beforeFirst(aIterator, NULL) != IDE_SUCCESS);
        }
        else
        {
            if ( sFlag == SMI_RETRAVERSE_AFTER )
            {
                IDE_TEST(afterLast(aIterator, NULL) != IDE_SUCCESS);
            }
            else
            {
                IDE_ASSERT(0);
            }
        }
    }
    else
    {
        while(1)
        {
            if ( (aIterator->flag & SMI_ITERATOR_TYPE_MASK) == SMI_ITERATOR_WRITE )
            {
                if ( sFlag == SMI_RETRAVERSE_BEFORE )
                {
                    if ( aIterator->slot == aIterator->highFence )
                    {
                        break;
                    }

                    sRowPtr = *(aIterator->slot + 1);
                }

                if ( sFlag == SMI_RETRAVERSE_AFTER )
                {
                    if ( aIterator->slot == aIterator->lowFence )
                    {
                        break;
                    }

                    sRowPtr = *(aIterator->slot - 1);
                }
            }

            sHeader  = (smnbHeader*)aIterator->index;

            aIterator->highest = ID_FALSE;
            aIterator->least   = ID_FALSE;

            sDepth = -1;

            IDE_TEST( findPosition( sHeader,
                                    sRowPtr,
                                    &sDepth,
                                    sStack ) != IDE_SUCCESS );

            IDE_ERROR_RAISE( sDepth >= 0, ERR_CORRUPTED_INDEX_NODE_DEPTH );

            sCurLNode  = (smnbLNode*)(sStack[sDepth].node);

            while(1)
            {
                IDL_MEM_BARRIER;
                sVersion   = getLatchValueOfLNode(sCurLNode) & IDU_LATCH_UNMASK;
                IDL_MEM_BARRIER;

                sNxtLNode  = sCurLNode->nextSPtr;
                sSlotCount = sCurLNode->mSlotCount;

                if ( sSlotCount == 0)
                {
                    IDL_MEM_BARRIER;
                    if ( sVersion == getLatchValueOfLNode(sCurLNode))
                    {
                        sCurLNode = sNxtLNode;
                    }

                    IDE_ERROR_RAISE( sCurLNode != NULL, ERR_CORRUPTED_INDEX_SLOTCOUNT );

                    continue;
                }

                aIterator->node = sCurLNode;
                aIterator->prvNode = sCurLNode->prevSPtr;
                aIterator->nxtNode = sCurLNode->nextSPtr;

                sTmpRowPtr = sCurLNode->mRowPtrs[sSlotCount - 1];

                if ( sTmpRowPtr == NULL)
                {
                    continue;
                }

                /* PROJ-2433
                 * ���ݰ��� �ִ밪 ���ǵ� �����ϴ��� Ȯ���ϴºκ��̴�.
                 * direct key�� full key���߸� ����ε� �񱳰� �����ϴ�.  */
                if ( isFullKeyInLeafSlot( aIterator->index,
                                          aIterator->node,
                                          (SShort)( sSlotCount - 1 ) ) == ID_TRUE )
                {
                    sTmpKey = SMNB_GET_KEY_PTR( sCurLNode, sSlotCount - 1 );
                }
                else
                {
                    sTmpKey = NULL;
                }

                // BUG-27948 �̹� ������ Row�� ���� �ִ��� Ȯ��
                IDE_DASSERT( !SM_SCN_IS_FREE_ROW(((smpSlotHeader *)sTmpRowPtr)->mCreateSCN) );

                /* PROJ-2433 */
                aIterator->mKeyRange->maximum.callback( &sResult,
                                                        sTmpRowPtr,
                                                        sTmpKey,
                                                        0,
                                                        SC_NULL_GRID,
                                                        aIterator->mKeyRange->maximum.data );

                sMax = sSlotCount - 1;

                if ( sResult == ID_FALSE )
                {
                    smnbBTree::findLastSlotInLeaf( aIterator->index,
                                                   &aIterator->mKeyRange->maximum,
                                                   sCurLNode,
                                                   0,
                                                   sSlotCount - 1,
                                                   &sMax );

                    aIterator->highest   = ID_TRUE;
                }
                else
                {
                    /* nothing to do */
                }

                sTmpRowPtr = aIterator->node->mRowPtrs[0];

                if ( sTmpRowPtr == NULL)
                {
                    continue;
                }

                /* PROJ-2433
                 * ���ݰ��� �ּҰ� ���ǵ� �����ϴ��� Ȯ���ϴºκ��̴�.
                 * direct key�� full key���߸� ����ε� �񱳰� �����ϴ�.  */
                if ( isFullKeyInLeafSlot( aIterator->index,
                                          aIterator->node,
                                          0 ) == ID_TRUE )
                {
                    sTmpKey = SMNB_GET_KEY_PTR( aIterator->node, 0 );
                }
                else
                {
                    sTmpKey = NULL;
                }

                // BUG-27948 �̹� ������ Row�� ���� �ִ��� Ȯ��
                IDE_DASSERT( !SM_SCN_IS_FREE_ROW(((smpSlotHeader *)sTmpRowPtr)->mCreateSCN));

                /* PROJ-2433 */
                aIterator->mKeyRange->minimum.callback( &sResult,
                                                        sTmpRowPtr,
                                                        sTmpKey,
                                                        0,
                                                        SC_NULL_GRID,
                                                        aIterator->mKeyRange->minimum.data );

                sMin = 0;

                if ( sResult == ID_FALSE )
                {
                    smnbBTree::findFirstSlotInLeaf( aIterator->index,
                                                    &aIterator->mKeyRange->minimum,
                                                    aIterator->node,
                                                    0,
                                                    sSlotCount - 1,
                                                    &sMin );

                    aIterator->least     = ID_TRUE;
                }
                else
                {
                    /* nothing to do */
                }

                aIterator->slot = NULL;

                for(i = sMin, j = -1; i <= sMax; i++)
                {
                    sTmpRowPtr = aIterator->node->mRowPtrs[i];

                    if ( sTmpRowPtr == NULL)
                    {
                        break;
                    }

                    if ( smnManager::checkSCN( (smiIterator*)aIterator, sTmpRowPtr ) == ID_TRUE )
                    {
                        j++;
                        aIterator->rows[j] = sTmpRowPtr;

                        if ( sRowPtr == aIterator->rows[j])
                        {
                            aIterator->slot = aIterator->rows + j;
                        }
                    }
                }

                if ( aIterator->slot != NULL)
                {
                    if ( (aIterator->flag & SMI_ITERATOR_TYPE_MASK) == SMI_ITERATOR_WRITE)
                    {
                        if ( sFlag == SMI_RETRAVERSE_BEFORE)
                        {
                            (aIterator->slot)--;
                        }

                        if ( sFlag == SMI_RETRAVERSE_AFTER)
                        {
                            (aIterator->slot)++;
                        }
                    }

                    aIterator->lowFence  = aIterator->rows;
                    aIterator->highFence = aIterator->rows + j;

                    IDL_MEM_BARRIER;

                    if ( sVersion == getLatchValueOfLNode(sCurLNode))
                    {
                        aIterator->version = sVersion;
                        break;
                    }
                }
                else
                {
                    if ( sVersion == getLatchValueOfLNode(sCurLNode))
                    {
                        sCurLNode = sNxtLNode;
                    }
                }
            }

            break;
        }//while
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_CORRUPTED_INDEX_NODE_DEPTH )
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_INCONSISTENT_INDEX ) );

        ideLog::log( IDE_SM_0, "Index Corrupted Detected : Wrong Index Node Depth" );
    }
    IDE_EXCEPTION( ERR_CORRUPTED_INDEX_SLOTCOUNT )
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_INCONSISTENT_INDEX ) );

        ideLog::log( IDE_SM_0, "Index Corrupted Detected : Wrong Index SlotCount" );
    }
    IDE_EXCEPTION_END;

    if ( ideGetErrorCode() == smERR_ABORT_INCONSISTENT_INDEX )
    {
        smnManager::setIsConsistentOfIndexHeader( aIterator->index->mIndexHeader, ID_FALSE );
    }

    return IDE_FAILURE;
}

IDE_RC smnbBTree::makeDiskImage(smnIndexHeader* a_pIndex,
                                smnIndexFile*   a_pIndexFile)
{
    /* BUG-37643 Node�� array�� ���������� �����ϴµ�
     * compiler ����ȭ�� ���ؼ� ���������� ���ŵ� �� �ִ�.
     * ���� �̷��� ������ volatile�� �����ؾ� �Ѵ�. */
    SInt                 i;
    smnbHeader         * s_pIndexHeader;
    volatile smnbLNode * s_pCurLNode;
    volatile smnbINode * s_pCurINode;

    s_pIndexHeader = (smnbHeader*)(a_pIndex->mHeader);

    s_pCurINode = (smnbINode*)(s_pIndexHeader->root);

    if ( s_pCurINode != NULL)
    {
        while((s_pCurINode->flag & SMNB_NODE_TYPE_MASK) == SMNB_NODE_TYPE_INTERNAL)
        {
            s_pCurINode = (smnbINode*)(s_pCurINode->mChildPtrs[0]);
        }

        s_pCurLNode = (smnbLNode*)s_pCurINode;

        while(s_pCurLNode != NULL)
        {
            for ( i = 0 ; i < s_pCurLNode->mSlotCount ; i++ )
            {
                IDE_TEST( a_pIndexFile->write( (SChar*)(s_pCurLNode->mRowPtrs[i]),
                                               ID_FALSE ) != IDE_SUCCESS );
            }

            s_pCurLNode = s_pCurLNode->nextSPtr;
        }
    }

    IDE_TEST(a_pIndexFile->write(NULL, ID_TRUE) != IDE_SUCCESS);

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smnbBTree::makeNodeListFromDiskImage(smcTableHeader *a_pTable, smnIndexHeader *a_pIndex)
{
    /* BUG-37643 Node�� array�� ���������� �����ϴµ�
     * compiler ����ȭ�� ���ؼ� ���������� ���ŵ� �� �ִ�.
     * ���� �̷��� ������ volatile�� �����ؾ� �Ѵ�. */
    smnIndexFile          s_indexFile;
    smnbHeader          * s_pIndexHeader = NULL;
    volatile smnbLNode  * s_pCurLNode    = NULL;
    SInt                  sState         = 0;
    SInt                  i              = 0;
    UInt                  sReadCnt       = 0;

    s_pIndexHeader = (smnbHeader*)(a_pIndex->mHeader);

    IDE_TEST(s_indexFile.initialize(a_pTable->mSelfOID, a_pIndex->mId)
             != IDE_SUCCESS);
    sState = 1;

    IDE_TEST(s_indexFile.open() != IDE_SUCCESS);
    sState = 2;

    while(1)
    {
        IDE_TEST(s_pIndexHeader->mNodePool.allocateSlots(1,
                                                      (smmSlot**)&(s_pCurLNode))
                 != IDE_SUCCESS);

        initLeafNode( (smnbLNode*)s_pCurLNode,
                      s_pIndexHeader,
                      IDU_LATCH_UNLOCKED );

        IDE_TEST( s_indexFile.read( a_pTable->mSpaceID,
                                    (SChar **)(((smnbLNode *)s_pCurLNode)->mRowPtrs),
                                    (UInt)SMNB_LEAF_SLOT_MAX_COUNT( s_pIndexHeader ),
                                    &sReadCnt )
                  != IDE_SUCCESS );
 
        s_pCurLNode->mSlotCount = (SShort)sReadCnt;

        if ( s_pCurLNode->mSlotCount == 0 )
        {
            //free Node
            IDE_TEST(freeNode((smnbNode*)s_pCurLNode) != IDE_SUCCESS);

            break;
        }
        else
        {
            /* nothing to do */
        }

        if ( SMNB_IS_DIRECTKEY_IN_NODE( s_pCurLNode ) == ID_TRUE )
        {
            /* PROJ-2433
             * �ε��� row�� ���� direct key�� �����Ѵ� */
            for ( i = 0 ; i < s_pCurLNode->mSlotCount ; i++ )
            {
                IDE_TEST( makeKeyFromRow( s_pIndexHeader,
                                          s_pCurLNode->mRowPtrs[i],
                                          SMNB_GET_KEY_PTR( s_pCurLNode, i ) )
                          != IDE_SUCCESS );
            }
        }
        else
        {
            /* nothing to do */
        }

        s_pIndexHeader->nodeCount++;

        if ( s_pIndexHeader->pFstNode == NULL )
        {
            s_pIndexHeader->pFstNode = (smnbLNode*)s_pCurLNode;
            s_pIndexHeader->pLstNode = (smnbLNode*)s_pCurLNode;

            s_pCurLNode->sequence  = 0;
        }
        else
        {
            s_pCurLNode->sequence = s_pIndexHeader->pLstNode->sequence + 1;

            s_pIndexHeader->pLstNode->nextSPtr = (smnbLNode*)s_pCurLNode;
            s_pCurLNode->prevSPtr = s_pIndexHeader->pLstNode;

            s_pIndexHeader->pLstNode = (smnbLNode*)s_pCurLNode;
        }

        if ( s_pCurLNode->mSlotCount != SMNB_LEAF_SLOT_MAX_COUNT( s_pIndexHeader ) )
        {
            break;
        }
        else
        {
            /* nothing to do */
        }
    }//while
    sState = 1;
    IDE_TEST(s_indexFile.close() != IDE_SUCCESS);
    sState = 0;
    IDE_TEST(s_indexFile.destroy() != IDE_SUCCESS);

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    // To fix BUG-20631
    IDE_PUSH();
    switch (sState)
    {
        case 2:
            (void)s_indexFile.close();
        case 1:
            (void)s_indexFile.destroy();
            break;
        case 0:
            break;
    }
    IDE_POP();

    return IDE_FAILURE;
}

IDE_RC smnbBTree::getPosition( smnbIterator *     aIterator,
                               smiCursorPosInfo * aPosInfo )
{
    IDE_ASSERT( (aIterator->slot >= aIterator->lowFence) ||
                (aIterator->slot <= aIterator->highFence) );

    aPosInfo->mCursor.mMRPos.mLeafNode = aIterator->node;

    /* BUG-39983 iterator�� node ������ ������ ���� getPosition�� �߻��ϱ� ���� SMO��
       �߻��� ��쿡�� setPosition���� SMO�� �Ͼ ���� Ȯ�� �� �� �־�� �Ѵ�. */
    aPosInfo->mCursor.mMRPos.mPrevNode = aIterator->prvNode;
    aPosInfo->mCursor.mMRPos.mNextNode = aIterator->nxtNode;
    aPosInfo->mCursor.mMRPos.mVersion  = aIterator->version;
    aPosInfo->mCursor.mMRPos.mRowPtr   = aIterator->curRecPtr;
    aPosInfo->mCursor.mMRPos.mKeyRange = (smiRange *)aIterator->mKeyRange; /* BUG-43913 */

    return IDE_SUCCESS;
}


IDE_RC smnbBTree::setPosition( smnbIterator     * aIterator,
                               smiCursorPosInfo * aPosInfo )
{
    IDU_LATCH    sVersion;
    SInt         sSlotCount;
    idBool       sResult;
    SInt         sLastSlot;
    SInt         sFirstSlot;
    SInt         i;
    SInt         j;
    smnbLNode  * sNode      = (smnbLNode*)aPosInfo->mCursor.mMRPos.mLeafNode;
    SChar      * sRowPtr    = (SChar*)aPosInfo->mCursor.mMRPos.mRowPtr;
    SChar     ** sStartPtr  = NULL;
    idBool       sFound     = ID_FALSE;
    UInt         sFlag      = ( aIterator->flag & SMI_RETRAVERSE_MASK ) ;
    void       * sKey       = NULL;

    aIterator->mKeyRange = aPosInfo->mCursor.mMRPos.mKeyRange; /* BUG-43913 */

    while( 1 )
    {
        sVersion   = getLatchValueOfLNode(sNode) & IDU_LATCH_UNMASK;
        IDL_MEM_BARRIER;

        if ( (sVersion != aPosInfo->mCursor.mMRPos.mVersion) &&
            ( (sNode->prevSPtr != aPosInfo->mCursor.mMRPos.mPrevNode) ||
              (sNode->nextSPtr != aPosInfo->mCursor.mMRPos.mNextNode) ) )
        {
            /* iterator�� node ������ ����� ���� SMO�� �߻����� ���
               ��ǥ slot�� �ش� node�� ���� ���ɼ��� �����Ƿ� getPosition���� ������ ������ ������
               retraverse�� slot�� ��ġ�� �ٽ� ã�´�. */
            break; 
        }

        // set iterator
        sSlotCount = sNode->mSlotCount;
        aIterator->highest = ID_FALSE;
        aIterator->least   = ID_FALSE;
        aIterator->node    = sNode;
        aIterator->prvNode = sNode->prevSPtr;
        aIterator->nxtNode = sNode->nextSPtr;

        // find higher boundary
        sRowPtr = aIterator->node->mRowPtrs[sSlotCount - 1];
        if ( sRowPtr == NULL )
        {
            continue;
        }

        /* PROJ-2433
         * ���ݰ��� �ִ밪 ���ǵ� �����ϴ��� Ȯ���ϴºκ��̴�.
         * direct key�� full key���߸� ����ε� �񱳰� �����ϴ�.  */
        if ( isFullKeyInLeafSlot( aIterator->index,
                                  aIterator->node,
                                  (SShort)( sSlotCount - 1 ) ) == ID_TRUE )
        {
            sKey = SMNB_GET_KEY_PTR( aIterator->node, sSlotCount - 1 );
        }
        else
        {
            sKey = NULL;
        }

        // BUG-27948 �̹� ������ Row�� ���� �ִ��� Ȯ��
        IDE_DASSERT( !SM_SCN_IS_FREE_ROW(((smpSlotHeader *)sRowPtr)->mCreateSCN) );

        aIterator->mKeyRange->maximum.callback( &sResult,
                                                sRowPtr,
                                                sKey,
                                                0,
                                                SC_NULL_GRID,
                                                aIterator->mKeyRange->maximum.data );
        sLastSlot = sSlotCount - 1;
        if ( sResult == ID_FALSE )
        {
            smnbBTree::findLastSlotInLeaf( aIterator->index,
                                           &aIterator->mKeyRange->maximum,
                                           aIterator->node,
                                           0,
                                           sSlotCount - 1,
                                           &sLastSlot );
            aIterator->highest = ID_TRUE;
        }
        else
        {
            /* nothing to do */
        }

        // find lower boundary
        sRowPtr = aIterator->node->mRowPtrs[0];
        if ( sRowPtr == NULL )
        {
            continue;
        }

        /* PROJ-2433
         * ���ݰ��� �ּҰ� ���ǵ� �����ϴ��� Ȯ���ϴºκ��̴�.
         * direct key�� full key���߸� ����ε� �񱳰� �����ϴ�.  */
        if ( isFullKeyInLeafSlot( aIterator->index,
                                  aIterator->node,
                                  0 ) == ID_TRUE )
        {
            sKey = SMNB_GET_KEY_PTR( aIterator->node, 0 );
        }
        else
        {
            sKey = NULL;
        }

        // BUG-27948 �̹� ������ Row�� ���� �ִ��� Ȯ��
        IDE_DASSERT( !SM_SCN_IS_FREE_ROW(((smpSlotHeader *)sRowPtr)->mCreateSCN) );

        /*PROJ-2433 */
        aIterator->mKeyRange->minimum.callback( &sResult,
                                                sRowPtr,
                                                sKey,
                                                0,
                                                SC_NULL_GRID,
                                                aIterator->mKeyRange->minimum.data );
        sFirstSlot = 0;
        if ( sResult == ID_FALSE )
        {
            smnbBTree::findFirstSlotInLeaf( aIterator->index,
                                            &aIterator->mKeyRange->minimum,
                                            aIterator->node,
                                            0,
                                            sSlotCount - 1,
                                            &sFirstSlot );
            aIterator->least = ID_TRUE;
        }
        else
        {
            /* nothing to do */
        }

        // make row cache
        j = -1;
        aIterator->lowFence = &aIterator->rows[0];
   
        /* Ž�� ���� �ۺ��� Ž���� �������� ��´�.
           �� �������� ���� ���������� ���� �о����� üũ�Ҷ� ���ȴ�. */
        sStartPtr = aIterator->rows - 2;
        aIterator->slot = sStartPtr;
        for( i = sFirstSlot; i <= sLastSlot; i++ )
        {
            sRowPtr = aIterator->node->mRowPtrs[i];
            if ( sRowPtr == NULL ) // delete op is running concurrently
            {
                break;
            }

            if ( smnManager::checkSCN( (smiIterator*)aIterator,
                                        sRowPtr )
                == ID_TRUE )
            {
                j++;
                aIterator->rows[j] = sRowPtr;
                if ( sRowPtr == aPosInfo->mCursor.mMRPos.mRowPtr )
                {
                    if ( sFlag == SMI_RETRAVERSE_BEFORE )
                    {
                        aIterator->slot = &aIterator->rows[j-1];
                    }
                    else
                    {
                        aIterator->slot = &aIterator->rows[j+1];
                    }
                    sFound = ID_TRUE;
                }
            }
        }
        aIterator->highFence = &aIterator->rows[j];

        IDL_MEM_BARRIER;
   
        if ( sVersion == getLatchValueOfLNode( sNode ) )
        {
            /* BUG-39983 Ž���� �������� ��� ������ �����Ѵ�.
               Ž���� �������� ��쿡�� retraverse�� ���� ���� ������ �����Ѵ�. */
            break;
        }
        else
        {
            /* BUG-39983 Ž�� �� �ٸ� Tx�� �ش� node�� �������� ���
              ��ġ�� ����� ���ɼ��� �����Ƿ� ��Ž���Ѵ�. */
            sFound = ID_FALSE;
            continue;
        }
    }

    if ( sFound != ID_TRUE )
    {
        /* BUG-39983 lstFetchRecPtr�� NULL�� ������ ��带 ó������ Ž���ϵ��� �Ѵ�.*/
        aIterator->lstFetchRecPtr = NULL;
        IDE_TEST( retraverse( NULL,
                              aIterator,
                              NULL )
                  != IDE_SUCCESS );
    }

    /* Ž�� ����� �������� ��� Ž���� ������ ���̹Ƿ� assert ó���Ѵ�. */
    IDE_ERROR_RAISE( aIterator->slot != sStartPtr, ERR_CORRUPTED_INDEX );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_CORRUPTED_INDEX )
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_INCONSISTENT_INDEX ) );

        ideLog::log( IDE_SM_0, "Index Corrupted Detected : Wrong Index Iterator" );
    }
    IDE_EXCEPTION_END;

    if ( ideGetErrorCode() == smERR_ABORT_INCONSISTENT_INDEX )
    {
        smnManager::setIsConsistentOfIndexHeader( aIterator->index->mIndexHeader, ID_FALSE );
    }

    return IDE_FAILURE;
}


IDE_RC smnbBTree::allocIterator( void ** /* aIteratorMem */ )
{
    return IDE_SUCCESS;
}


IDE_RC smnbBTree::freeIterator( void * /* aIteratorMem */ )
{
    return IDE_SUCCESS;
}

/*******************************************************************
 * Definition
 *
 *    To Fix BUG-15670
 *
 *    Row Pointer �����κ��� Fixed/Variable�� �������
 *    Key Value�� Key Size�� ȹ���Ѵ�.
 *
 * Implementation
 *
 *    Null �� �ƴ��� ����Ǵ� ��Ȳ���� ȣ��Ǿ�� �Ѵ�.
 *
 *******************************************************************/
IDE_RC smnbBTree::getKeyValueAndSize(  SChar         * aRowPtr,
                                       smnbColumn    * aIndexColumn,
                                       void          * aKeyValue,
                                       UShort        * aKeySize )
{
    SChar     * sColumnPtr;
    SChar     * sKeyPtr;
    UShort      sKeySize;
    UShort      sMinMaxKeySize;
    smiColumn * sColumn;
    UInt        sLength = 0;

    //---------------------------------------
    // Parameter Validation
    //---------------------------------------

    IDE_ASSERT( aRowPtr      != NULL );
    IDE_ASSERT( aIndexColumn != NULL );
    IDE_ASSERT( aKeySize     != NULL );

    //---------------------------------------
    // Column Pointer ��ġ ȹ��
    //---------------------------------------

    sColumn    = &aIndexColumn->column;
    sColumnPtr =  aRowPtr + sColumn->offset;

    //---------------------------------------
    // Key Value ȹ��
    //---------------------------------------
    if ( ( sColumn->flag & SMI_COLUMN_COMPRESSION_MASK )
         != SMI_COLUMN_COMPRESSION_TRUE )
    {
        switch( sColumn->flag & SMI_COLUMN_TYPE_MASK )
        {
            case SMI_COLUMN_TYPE_FIXED:
                if ( sColumn->size > MAX_MINMAX_VALUE_SIZE )
                {
                    // CHAR Ȥ�� VARCHAR type column�̸鼭 column�� ���̰�
                    // MAX_MINMAX_VALUE_SIZE���� �� ���,
                    // key value�� �����ϰ� length�� �����ؾ� ��.
    
                    // BUG-24449 Keyũ��� MT �Լ��� ���� ȹ���ؾ� ��.
                    sKeySize = aIndexColumn->actualSize( sColumn,
                                                         sColumnPtr );
                    if ( sKeySize > MAX_MINMAX_VALUE_SIZE )
                    {
                        sMinMaxKeySize = MAX_MINMAX_VALUE_SIZE;
                    }
                    else
                    {
                        sMinMaxKeySize = sKeySize;
                    }
                    sKeySize = sMinMaxKeySize - aIndexColumn->headerSize;
                    aIndexColumn->makeMtdValue( sColumn->size,
                                                aKeyValue,
                                                0, //aCopyOffset
                                                sKeySize,
                                                sColumnPtr + aIndexColumn->headerSize);
                }
                else
                {
                    // column�� type�� �����̵� MAX_MINMAX_VALUE_SIZE����
                    // ���� ��� : key value�� ������ �ʿ� ����
                    sMinMaxKeySize = sColumn->size;
                    idlOS::memcpy( (UChar*)aKeyValue,
                                (UChar*)sColumnPtr,
                                sMinMaxKeySize  );
                }
                break;
            case SMI_COLUMN_TYPE_VARIABLE:
            case SMI_COLUMN_TYPE_VARIABLE_LARGE:
    
                sKeyPtr = sgmManager::getVarColumn( aRowPtr, sColumn , &sLength );
    
                // Null�� �ƴ��� ����Ǿ�� ��.
                IDE_ERROR_RAISE( sLength != 0, ERR_CORRUPTED_INDEX_LENGTH );
    
                //-------------------------------
                // copy from smiGetVarColumn()
                //-------------------------------
    
    
                if ( sLength > MAX_MINMAX_VALUE_SIZE )
                {
                    // VARCHAR type column key�� ���̰� MAX_MINMAX_VALUE_SIZE����
                    // �� ���, key value�� �����ϰ� length�� �����ؾ� ��.
                    sMinMaxKeySize = MAX_MINMAX_VALUE_SIZE;
    
                    sKeySize = sMinMaxKeySize - aIndexColumn->headerSize;
                    aIndexColumn->makeMtdValue( sColumn->size,
                                                aKeyValue,
                                                0, //aCopyOffset
                                                sKeySize,
                                                sKeyPtr + aIndexColumn->headerSize );
                }
                else
                {
                    sMinMaxKeySize = sLength;
                    idlOS::memcpy( (UChar*)aKeyValue,
                                (UChar*)sKeyPtr,
                                sMinMaxKeySize );
                }
    
                break;
            default:
                sMinMaxKeySize = 0;
                IDE_ERROR_RAISE( 0, ERR_CORRUPTED_INDEX_FLAG );
                break;
        }
    }
    else
    {
        // BUG-37489 compress column �� ���� index �� ���鶧 ���� FATAL
        sColumnPtr = (SChar*)smiGetCompressionColumn( aRowPtr,
                                                      sColumn,
                                                      ID_TRUE, // aUseColumnOffset
                                                      &sLength );
        IDE_DASSERT( sColumnPtr != NULL );

        if ( sColumn->size > MAX_MINMAX_VALUE_SIZE )
        {
            // CHAR Ȥ�� VARCHAR type column�̸鼭 column�� ���̰�
            // MAX_MINMAX_VALUE_SIZE���� �� ���,
            // key value�� �����ϰ� length�� �����ؾ� ��.

            sKeySize = aIndexColumn->actualSize( sColumn,
                                                 sColumnPtr );

            if ( sKeySize > MAX_MINMAX_VALUE_SIZE )
            {
                sMinMaxKeySize = MAX_MINMAX_VALUE_SIZE;
            }
            else
            {
                sMinMaxKeySize = sKeySize;
            }
            sKeySize = sMinMaxKeySize - aIndexColumn->headerSize;
            aIndexColumn->makeMtdValue( sColumn->size,
                                        aKeyValue,
                                        0, //aCopyOffset
                                        sKeySize,
                                        sColumnPtr + aIndexColumn->headerSize);
        }
        else
        {
            // column�� type�� �����̵� MAX_MINMAX_VALUE_SIZE����
            // ���� ��� : key value�� ������ �ʿ� ����
            sMinMaxKeySize = sColumn->size;
            idlOS::memcpy( (UChar*)aKeyValue,
                           (UChar*)sColumnPtr,
                           sMinMaxKeySize  );
        }
    }

    *aKeySize = sMinMaxKeySize;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_CORRUPTED_INDEX_LENGTH )
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_INCONSISTENT_INDEX ) );

        ideLog::log( IDE_SM_0, "Index Corrupted Detected : Wrong Row Length" );
    }
    IDE_EXCEPTION( ERR_CORRUPTED_INDEX_FLAG )
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_INCONSISTENT_INDEX ) );

        ideLog::log( IDE_SM_0, "Index Corrupted Detected : Wrong Index Flag" );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smnbBTree::setMinMaxStat( smnIndexHeader * aCommonHeader,
                                 SChar          * aRowPtr,
                                 smnbColumn     * aIndexColumn,
                                 idBool           aIsMinStat )
{
    smnRuntimeHeader    * sRunHeader;
    UShort                sDummyKeySize;
    void                * sTarget;

    sRunHeader = (smnRuntimeHeader*)aCommonHeader->mHeader;

    if ( aIsMinStat == ID_TRUE )
    {
        sTarget = aCommonHeader->mStat.mMinValue;
    }
    else
    {
        sTarget = aCommonHeader->mStat.mMaxValue;
    }

    IDE_ASSERT( sRunHeader->mStatMutex.lock(NULL) == IDE_SUCCESS );
    ID_SERIAL_BEGIN( sRunHeader->mAtomicB++; );

    if ( aRowPtr == NULL )
    {
        ID_SERIAL_EXEC(
            idlOS::memset( sTarget,
                           0x00,
                           MAX_MINMAX_VALUE_SIZE),
            1 );
    }
    else
    {
        ID_SERIAL_EXEC(
            IDE_TEST( getKeyValueAndSize( aRowPtr,
                                          aIndexColumn,
                                          sTarget,
                                          &sDummyKeySize ) != IDE_SUCCESS ),
            1 );
    }
    ID_SERIAL_END( sRunHeader->mAtomicA++; );
    IDE_ASSERT( sRunHeader->mStatMutex.unlock() == IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/* 
 * BUG-35163 - [sm_index] [SM] add some exception properties for __DBMS_STAT_METHOD
 *
 * __DBMS_STAT_METHOD�� MANUAL�϶� �Ʒ� �� ������Ƽ�� Ư�� Ÿ���� DB�� ��� ����
 * ����� ������ �� �ִ�.
 *
 * __DBMS_STAT_METHOD_FOR_MRDB
 * __DBMS_STAT_METHOD_FOR_VRDB
 */
inline idBool smnbBTree::needToUpdateStat( idBool aIsMemTBS )
{
    idBool  sRet;
    SInt    sStatMethod;

    if ( smuProperty::getDBMSStatMethod() == SMU_DBMS_STAT_METHOD_AUTO )
    {
        sRet = ID_TRUE;
    }
    else
    {
        if ( aIsMemTBS == ID_TRUE )
        {
            sStatMethod = smuProperty::getDBMSStatMethod4MRDB();
        }
        else
        {
            /* volatile tablespace */
            sStatMethod = smuProperty::getDBMSStatMethod4VRDB();
        }

        if ( sStatMethod == SMU_DBMS_STAT_METHOD_AUTO )
        {
            sRet = ID_TRUE;
        }
        else
        {
            sRet = ID_FALSE;
        }
    }

    return sRet;
}


IDE_RC smnbBTree::dumpIndexNode( smnIndexHeader * aIndex,
                                 smnbNode       * aNode,
                                 SChar          * aOutBuf,
                                 UInt             aOutSize )
{
    smnbLNode      * sLNode;
    smnbINode      * sINode;
    UInt             sFixedKeySize;
    smOID            sOID;
    SChar          * sValueBuf;
    UInt             sState = 0;
    SInt             i;

    sFixedKeySize = ((smnbHeader*)aIndex->mHeader)->mFixedKeySize;

    IDE_TEST( iduMemMgr::calloc( IDU_MEM_ID, 1,
                                 ID_SIZEOF( SChar ) * IDE_DUMP_DEST_LIMIT,
                                 (void**)&sValueBuf )
              != IDE_SUCCESS );
    sState = 1;


    if ( ( aNode->flag & SMNB_NODE_TYPE_MASK)== SMNB_NODE_TYPE_LEAF )
    {
        /* Leaf Node */
        sLNode = (smnbLNode*)aNode;
        
        idlVA::appendFormat( aOutBuf,
                             aOutSize,
                             "LeafNode:\n"
                             "flag         : %"ID_INT32_FMT"\n"
                             "sequence     : %"ID_UINT32_FMT"\n"
                             "prevSPtr     : %"ID_vULONG_FMT"\n"
                             "nextSPtr     : %"ID_vULONG_FMT"\n"
                             "keySize      : %"ID_UINT32_FMT"\n"
                             "maxSlotCount : %"ID_INT32_FMT"\n"
                             "slotCount    : %"ID_INT32_FMT"\n"
                             "[Num]           RowPtr           RowOID\n",
                             sLNode->flag,
                             sLNode->sequence,
                             sLNode->prevSPtr,
                             sLNode->nextSPtr,
                             sLNode->mKeySize,
                             sLNode->mMaxSlotCount,
                             sLNode->mSlotCount );

        for ( i = 0 ; i < sLNode->mSlotCount ; i++ )
        {
            if ( sLNode->mRowPtrs[ i ] != NULL )
            {
                sOID = SMP_SLOT_GET_OID( sLNode->mRowPtrs[ i ] );
            }
            else
            {
                sOID = SM_NULL_OID;
            }
            idlVA::appendFormat( aOutBuf,
                                 aOutSize,
                                 "[%3"ID_UINT32_FMT"]"
                                 " %16"ID_vULONG_FMT
                                 " %16"ID_vULONG_FMT,
                                 i,
                                 sLNode->mRowPtrs[ i ],
                                 sOID );
            ideLog::ideMemToHexStr( (UChar*)sLNode->mRowPtrs[ i ],
                                    sFixedKeySize + SMP_SLOT_HEADER_SIZE,
                                    IDE_DUMP_FORMAT_PIECE_SINGLE |
                                    IDE_DUMP_FORMAT_ADDR_NONE |
                                    IDE_DUMP_FORMAT_BODY_HEX |
                                    IDE_DUMP_FORMAT_CHAR_ASCII ,
                                    sValueBuf,
                                    IDE_DUMP_DEST_LIMIT );
            idlVA::appendFormat( aOutBuf,
                                 aOutSize,
                                 "   %s\n",
                                 sValueBuf);

        }
    }
    else
    {
        /* Internal Node */
        sINode = (smnbINode*)aNode;
        
        idlVA::appendFormat( aOutBuf,
                             aOutSize,
                             "InternalNode:\n"
                             "flag         : %"ID_INT32_FMT"\n"
                             "sequence     : %"ID_UINT32_FMT"\n"
                             "prevSPtr     : %"ID_vULONG_FMT"\n"
                             "nextSPtr     : %"ID_vULONG_FMT"\n"
                             "keySize      : %"ID_UINT32_FMT"\n"
                             "maxSlotCount : %"ID_INT32_FMT"\n"
                             "slotCount    : %"ID_INT32_FMT"\n"
                             "[Num]           RowPtr           RowOID\n",
                             sINode->flag,
                             sINode->sequence,
                             sINode->prevSPtr,
                             sINode->nextSPtr,
                             sINode->mKeySize,
                             sINode->mMaxSlotCount,
                             sINode->mSlotCount );

        for ( i = 0 ; i < sINode->mSlotCount ; i++ )
        {
            if ( sINode->mRowPtrs[ i ] != NULL )
            {
                sOID = SMP_SLOT_GET_OID( sINode->mRowPtrs[ i ] );
            }
            else
            {
                sOID = SM_NULL_OID;
            }
            idlVA::appendFormat( aOutBuf,
                                 aOutSize,
                                 "[%3"ID_UINT32_FMT"]"
                                 " %16"ID_vULONG_FMT
                                 " %16"ID_vULONG_FMT
                                 " %16"ID_vULONG_FMT"\n",
                                 i,
                                 sINode->mRowPtrs[ i ],
                                 sOID,
                                 sINode->mChildPtrs[ i ] );
            ideLog::ideMemToHexStr( (UChar*)sINode->mRowPtrs[ i ],
                                    sFixedKeySize,
                                    IDE_DUMP_FORMAT_NORMAL,
                                    sValueBuf,
                                    IDE_DUMP_DEST_LIMIT );
            idlVA::appendFormat( aOutBuf,
                                 aOutSize,
                                 sValueBuf);
        }
    }

    sState = 0;
    IDE_TEST( iduMemMgr::free( sValueBuf ) != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    switch(sState)
    {
    case 1:
        IDE_ASSERT( iduMemMgr::free( sValueBuf ) == IDE_SUCCESS );
    default:
        break;
    }

    return IDE_FAILURE;
}

void smnbBTree::logIndexNode( smnIndexHeader * aIndex,
                              smnbNode       * aNode )
{
    SChar           * sOutBuffer4Dump;

    if ( iduMemMgr::calloc( IDU_MEM_SM_SMN,
                            1,
                            ID_SIZEOF( SChar ) * IDE_DUMP_DEST_LIMIT,
                            (void**)&sOutBuffer4Dump )
        == IDE_SUCCESS )
    {
        if ( dumpIndexNode( aIndex, 
                            aNode, 
                            sOutBuffer4Dump,
                            IDE_DUMP_DEST_LIMIT )
            == IDE_SUCCESS )
        {
            ideLog::log( IDE_SM_0, "%s\n", sOutBuffer4Dump );
        }
        else
        {
            /* dump fail */
        }

        (void) iduMemMgr::free( sOutBuffer4Dump );
    }
    else
    {
        /* alloc fail */
    }
}

/*********************************************************************
 * FUNCTION DESCRIPTION : smnbBTree::makeKeyFromRow                  *
 * ------------------------------------------------------------------*
 * PROJ-2433 Direct Key Index
 * DIRECT KEY INDEX���� ����� DIRECT KEY�� row���� �����ϴ��Լ�.
 *
 * - COMPOSITE KEY INDEX�� ���� ù��°�÷��� �����Ѵ�.
 * - partial key�� ������ �ڷ����̸� aIndex->mKeySize ��ŭ partial �Ǿ����.
 *
 * aIndex    - [IN]  INDEX ���
 * aRow      - [IN]  row pointer
 * aKey      - [OUT] ����� Direct Key
 *********************************************************************/
IDE_RC smnbBTree::makeKeyFromRow( smnbHeader   * aIndex,
                                  SChar        * aRow,
                                  void         * aKey )
{
    UInt          sLength       = 0;
    SChar       * sVarValuePtr  = NULL;
    smnbColumn  * sFirstCol     = &(aIndex->columns[0]);
    void        * sKeyBuf       = NULL;

    if ( ( sFirstCol->column.flag & SMI_COLUMN_COMPRESSION_MASK )
          == SMI_COLUMN_COMPRESSION_TRUE )
    {
        /* PROJ-2433
         * compression table�� �����Ǵ� index��
         * �Ϲ� index�� ����Ѵ�. */
        ideLog::log( IDE_SERVER_0,
                     "ERROR! cannot make Direct Key Index in compression table [Index name:%s]",
                     aIndex->mIndexHeader->mName );
        IDE_ERROR_RAISE( 0, ERR_CORRUPTED_INDEX );
    }
    else
    {
        switch ( sFirstCol->column.flag & SMI_COLUMN_TYPE_MASK )
        {
            case SMI_COLUMN_TYPE_FIXED:

                if ( sFirstCol->column.size <= aIndex->mKeySize )
                {
                    idlOS::memcpy( aKey,
                                   aRow + sFirstCol->column.offset,
                                   sFirstCol->column.size );
                }
                else
                {
                    idlOS::memcpy( aKey,
                                   aRow + sFirstCol->column.offset,
                                   aIndex->mKeySize );
                }
                break;

            case SMI_COLUMN_TYPE_VARIABLE:
            case SMI_COLUMN_TYPE_VARIABLE_LARGE:
               
                /* PROJ-2419 */
                if ( aIndex->mIsPartialKey == ID_TRUE )
                {
                    /* partial key �ΰ�� buff�� ���� key�� ������,
                     * partial ���̸�ŭ�߶� ��ȯ�Ѵ� */
                    IDE_TEST( iduMemMgr::calloc( IDU_MEM_SM_SMN, 1,
                                                 sFirstCol->column.size,
                                                 (void **)&sKeyBuf )
                              != IDE_SUCCESS );

                    sVarValuePtr = sgmManager::getVarColumn( aRow,
                                                             &(sFirstCol->column),
                                                             (SChar *)sKeyBuf );

                    idlOS::memcpy( aKey,
                                   sKeyBuf,
                                   aIndex->mKeySize );
                }
                else
                {
                    sVarValuePtr = sgmManager::getVarColumn( aRow,
                                                             &(sFirstCol->column),
                                                             &sLength);

                    if ( sLength <= aIndex->mKeySize )
                    {
                        idlOS::memcpy( aKey, sVarValuePtr, sLength );
                    }
                    else
                    {
                        idlOS::memcpy( aKey, sVarValuePtr, aIndex->mKeySize );
                    }
                }

                /* variable column�� value�� NULL�� ���, NULL ���� ä�� */
                if ( sVarValuePtr == NULL )
                {
                    /* PROJ-2433 
                     * null ���������Ѵ�
                     * mtd����� setNull�� �����Ѵ�. */
                    sFirstCol->null( &(sFirstCol->column), aKey );
                }
                else
                {
                    /* nothing to do */
                }

                break;
            default:
                ideLog::log( IDE_SM_0,
                             "Invalid first column flag : %"ID_UINT32_FMT" [Index name:%s]",
                             sFirstCol->column.flag,
                             aIndex->mIndexHeader->mName );
                break;
        }
    }

    if ( sKeyBuf != NULL )
    {
        IDE_ASSERT( iduMemMgr::free( sKeyBuf ) == IDE_SUCCESS );
    }
    else
    {
        /* nothing to do */
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_CORRUPTED_INDEX )
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_INCONSISTENT_INDEX ) );

        ideLog::log( IDE_SM_0, "Index Corrupted Detected : Wrong Index Flag" );
    }
    IDE_EXCEPTION_END;

    if ( sKeyBuf != NULL )
    {
        IDE_ASSERT( iduMemMgr::free( sKeyBuf ) == IDE_SUCCESS );
    }
    else
    {
        /* nothing to do */
    }

    return IDE_FAILURE;
}

/*********************************************************************
 * FUNCTION DESCRIPTION : smnbBTree::setDirectKeyIndex               *
 * ------------------------------------------------------------------*
 * PROJ-2433 Direct Key Index
 * BTREE INDEX ��������� direct key index ���õ� ������ �����Ѵ�.
 *
 * - composite index�� ù��° �÷��� direct key�� ����ȴ�.
 * - CHAR,VARCHAR,NCHAR,NVARCHAR ���������� partial key index�� ����Ѵ�.
 *
 * aHeader   - [IN]  BTREE INDEX �������
 * aIndex    - [IN]  INDEX ���
 * aColumn   - [IN]  �÷�����
 * aMtdAlign - [IN]  ���������� MTD����ü Align
 *********************************************************************/
IDE_RC smnbBTree::setDirectKeyIndex( smnbHeader         * aHeader,
                                     smnIndexHeader     * aIndex,
                                     const smiColumn    * aColumn,
                                     UInt                 aMtdAlign )
{
    UInt sColumnSize = 0;

    if ( ( aIndex->mFlag & SMI_INDEX_DIRECTKEY_MASK ) == SMI_INDEX_DIRECTKEY_TRUE ) 
    {
        /* �÷�ũ�⸦ ����ü align���� ���� */
        sColumnSize = idlOS::align( aColumn->size, aMtdAlign );

        if ( gSmiGlobalCallBackList.isUsablePartialDirectKey( (void *)aColumn ) )
        {
            if ( aIndex->mMaxKeySize >= sColumnSize )
            {
                /* full-key */
                aHeader->mIsPartialKey  = ID_FALSE;
                aHeader->mKeySize       = sColumnSize;
            }
            else
            {
                /* partial-key */
                aHeader->mIsPartialKey  = ID_TRUE;
                /* mMaxKeySize���� ũ������ ���߿� align �´� ���� ū�� ã�� */
                aHeader->mKeySize       = idlOS::align( aIndex->mMaxKeySize - aMtdAlign + 1, aMtdAlign );
            }
        }
        /* ��Ÿ�ڷ��� */
        else
        {
            /* ������ �̿��� �ڷ����� ���ؼ��� partial key�� ���������ʴ´�. */
            IDE_TEST( aIndex->mMaxKeySize < sColumnSize );

            /* full-key */
            aHeader->mIsPartialKey  = ID_FALSE;
            aHeader->mKeySize       = sColumnSize;
        }

        /* DIRECT KEY INDEX�� INTERNAL NODE ���� : smnbINode + child_pointers + row_pointers + direct_keys */
        aHeader->mINodeMaxSlotCount = ( ( mNodeSize - ID_SIZEOF(smnbINode) + ID_SIZEOF(smnbNode *) ) /
                                        ( ID_SIZEOF(smnbNode *) + ID_SIZEOF(SChar *) + aHeader->mKeySize ) );
        /* DIRECT KEY INDEX�� LEAF NODE ���� : smnbLNode + row_pointers + direct_keys */
        aHeader->mLNodeMaxSlotCount = ( ( mNodeSize - ID_SIZEOF(smnbLNode) + ID_SIZEOF(smnbNode *) ) /
                                        ( ID_SIZEOF(SChar *) + aHeader->mKeySize ) );
    }
    else /* normal index (=no direct key index) */
    {
        aHeader->mIsPartialKey      = ID_FALSE;
        aHeader->mKeySize           = 0;
        aHeader->mINodeMaxSlotCount = ( ( mNodeSize - ID_SIZEOF(smnbINode) + ID_SIZEOF(smnbNode *) ) /
                                        ( ID_SIZEOF(smnbNode *) + ID_SIZEOF(SChar *) ) );
        aHeader->mLNodeMaxSlotCount = ( ( mNodeSize - ID_SIZEOF(smnbLNode) + ID_SIZEOF(smnbNode *) ) /
                                        ( ID_SIZEOF(SChar *) ) );
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}


/*********************************************************************
 * FUNCTION DESCRIPTION : smnbBTree::isFullKeyInLeafSlot             *
 * ------------------------------------------------------------------*
 * PROJ-2433 Direct Key Index
 * LEAF NODE�� aIdx��° slot�� direct key�� full key �������θ� �����Ѵ�.
 *
 * aIndex    - [IN]  INDEX ���
 * aNode     - [IN]  LEAF NODE
 * aIdx      - [IN]  slot index
 *********************************************************************/
inline idBool smnbBTree::isFullKeyInLeafSlot( smnbHeader      * aIndex,
                                              const smnbLNode * aNode,
                                              SShort            aIdx )
{
    if ( SMNB_IS_DIRECTKEY_IN_NODE( aNode ) == ID_TRUE )
    {
        if ( aIndex->mIsPartialKey == ID_FALSE )
        {
            /* direct key index �̰�, partial key index�� �ƴ� */
            return ID_TRUE;
        }
        else
        {
            if ( aIndex->columns[0].actualSize( &(aIndex->columns[0].column),
                                                SMNB_GET_KEY_PTR( aNode, aIdx ) ) <= aNode->mKeySize )
            {
                /* partial key index������,
                   �� slot�� ��� direct key�� �����ϰ����� */
                return ID_TRUE;
            }
            else
            {
                /* nothing to do */
            }
        }
    }
    else
    {
        /* nothing to do */
    }

    return ID_FALSE;
}

/*********************************************************************
 * FUNCTION DESCRIPTION : smnbBTree::isFullKeyInInternalSlot         *
 * ------------------------------------------------------------------*
 * PROJ-2433 Direct Key Index
 * INTERNAL NODE�� aIdx��° slot�� direct key�� full key �������θ� �����Ѵ�.
 *
 * aIndex    - [IN]  INDEX ���
 * aNode     - [IN]  LEAF NODE
 * aIdx      - [IN]  slot index
 *********************************************************************/
inline idBool smnbBTree::isFullKeyInInternalSlot( smnbHeader      * aIndex,
                                                  const smnbINode * aNode,
                                                  SShort            aIdx )
{
    if ( SMNB_IS_DIRECTKEY_IN_NODE( aNode ) == ID_TRUE )
    {
        if ( aIndex->mIsPartialKey == ID_FALSE )
        {
            /* direct key index �̰�, partial key index�� �ƴ� */
            return ID_TRUE;
        }
        else
        {
            if ( aIndex->columns[0].actualSize( &(aIndex->columns[0].column),
                                                SMNB_GET_KEY_PTR( aNode, aIdx ) ) <= aNode->mKeySize )
            {
                /* partial key index������,
                   �� slot�� ��� direct key�� �����ϰ����� */
                return ID_TRUE;
            }
            else
            {
                /* nothing to do */
            }
        }
    }
    else
    {
        /* nothing to do */
    }

    return ID_FALSE;
}

/*********************************************************************
 * FUNCTION DESCRIPTION : smnbBTree::keyRedistribute                 *
 * ------------------------------------------------------------------*
 * PROJ-2613 Key redistribution in MRDB index
 * Ű ��й踦 �����Ѵ�.
 * �Է� ���� ���� ��ŭ�� slot�� ���� ��忡�� ���� ���� �̵���Ų��.
 *
 * aIndex                 - [IN]  INDEX ���
 * aCurNode               - [IN]  Src LEAF NODE
 * aNxtNode               - [IN]  Dest LEAF NODE
 * aKeyRedistributeCount  - [IN]  Ű ��й�� �̵���ų slot�� ��
 *********************************************************************/
IDE_RC smnbBTree::keyRedistribute( smnbHeader * aIndex,
                                   smnbLNode  * aCurNode,
                                   smnbLNode  * aNxtNode,
                                   SInt         aKeyRedistributeCount )
{
    SInt i = 0;

    IDE_ERROR( aKeyRedistributeCount > 0 );

    /* ��й� �Ǵ� Ű�� �ִ� slot ���� 1/2�� ���� �� ����. */
    IDE_ERROR( aKeyRedistributeCount <= ( SMNB_LEAF_SLOT_MAX_COUNT( aIndex ) / 2 ) );

    /* �̿� ����� slot���� Ȯ���� �ش� ����� slot���� 0�� ���
     * �̿� ��尡 ���� �Ǿ��� �� �����Ƿ� Ű ��й踦 �����Ѵ�. */
    IDE_TEST( aNxtNode->mSlotCount == 0 );

    /* ���� ��尡 �ٸ� Tx�� ���� split�Ǿ��� �� �����Ƿ� Ű ��й踦 �����Ѵ�. */
    IDE_TEST( aCurNode->mSlotCount != SMNB_LEAF_SLOT_MAX_COUNT( aIndex ) );

    /* Ű�� ��й� �ϱ��� �� ������忡�� SCAN LATCH�� �Ǵ�. */
    SMNB_SCAN_LATCH( aCurNode );
    SMNB_SCAN_LATCH( aNxtNode );


    /* �̿� ����� slot�� �ڷ� �̵����� ��й�� Ű�� �� �ڸ��� �����. */
    shiftLeafSlots( aNxtNode,
                    0,
                    (SShort)( aNxtNode->mSlotCount - 1 ),
                    (SShort)( aKeyRedistributeCount ) ); 

    /* ���� ����� slot�� �̿� ��忡 �����Ͽ� ��й踦 �����Ѵ�. */
    copyLeafSlots( aNxtNode,
                   0,
                   aCurNode,
                   (SShort)( SMNB_LEAF_SLOT_MAX_COUNT( aIndex ) - aKeyRedistributeCount ),
                   (SShort)( aCurNode->mSlotCount - 1 ) );

    /* ���� ��忡 �����ִ� slot�� �����Ѵ�. */
    for ( i = 0; i < aKeyRedistributeCount; i++ )
    {
        setLeafSlot( aCurNode, 
                     (SShort)( ( aCurNode->mSlotCount - 1 ) - i ), 
                     NULL, 
                     NULL );
    }
    
    /* ���� ���� �̿� ����� slotcount�� �����Ѵ�.*/
    aNxtNode->mSlotCount = aNxtNode->mSlotCount + aKeyRedistributeCount;
    aCurNode->mSlotCount = aCurNode->mSlotCount - aKeyRedistributeCount;

    /* �� ������忡 �ɸ� SCAN LATCH�� Ǭ��. */
    SMNB_SCAN_UNLATCH( aNxtNode );
    SMNB_SCAN_UNLATCH( aCurNode );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

/*********************************************************************
 * FUNCTION DESCRIPTION : smnbBTree::keyRedistributionPropagate      *
 * ------------------------------------------------------------------*
 * PROJ-2613 Key redistribution in MRDB index
 * ��������� Ű ��й谡 ����� �� �׿� ���� �θ� ��嵵 �����Ѵ�.
 *
 * aIndex      - [IN]  INDEX ���
 * aINode      - [IN]  ������ �θ� ���
 * aLNode      - [IN]  Ű ��й谡 ����� ���� ���
 * aOldRowPtr  - [IN]  Ű ��й谡 ����Ǳ��� ���� ����� ��ǥ ��
 *********************************************************************/
IDE_RC smnbBTree::keyRedistributionPropagate( smnbHeader * aIndex,
                                              smnbINode  * aINode,
                                              smnbLNode  * aLNode,
                                              SChar      * aOldRowPtr )
{
    SInt    s_pOldPos = 0;
    void    * sNewKey = NULL;
    void    * sNewRowPtr = NULL;
#ifdef DEBUG
    void    * sOldKeyInInternal = NULL;
    void    * sOldKeyRowPtrInInternal = NULL;
#endif

    IDE_ERROR( aOldRowPtr != NULL );

    /* ������忡�� ���ͳ� ���� ������ ���� �����´�. */
    getLeafSlot( (SChar **)&sNewRowPtr,
                 &sNewKey,
                 aLNode,
                 (SShort)( aLNode->mSlotCount - 1 ) );

    /* ���ͳ� ��峻 ������ ��ġ�� �����´�. */
    IDE_TEST( findSlotInInternal( aIndex, 
                                  aINode, 
                                  aOldRowPtr, 
                                  &s_pOldPos ) != IDE_SUCCESS );

#ifdef DEBUG 
    /* ���ͳ� ��忡�� ������ Ű ���� ���������� Ȯ���ϴ� �κ�.
       �Ź� slot�� Ž���ϴ� ���� ����� ũ�⶧���� debug mode������ �۵��ϵ��� �Ѵ�. */
    IDE_ERROR( aLNode == (smnbLNode*)(aINode->mChildPtrs[s_pOldPos]) );

    getInternalSlot( NULL,
                     (SChar **)&sOldKeyInInternal,
                     &sOldKeyRowPtrInInternal,
                     aINode,
                     (SShort)s_pOldPos );

    IDE_ERROR( sOldKeyInInternal == (void*)aOldRowPtr );
#endif

    SMNB_SCAN_LATCH( aINode );

    /* ���ͳ� ��带 �����Ѵ�. */
    setInternalSlot( aINode,
                     (SShort)s_pOldPos,
                     (smnbNode *)aLNode,
                     (SChar *)sNewRowPtr,
                     sNewKey );

    SMNB_SCAN_UNLATCH( aINode );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*********************************************************************
 * FUNCTION DESCRIPTION : smnbBTree::keyReorganization               *
 * ------------------------------------------------------------------*
 * PROJ-2614 Memory Index Reorganization
 * ������带 ��ȸ�ϸ鼭 �̿� ���� ������ �����ϴٸ� �����Ѵ�.
 *
 * aIndex      - [IN]  INDEX ���
 *********************************************************************/
IDE_RC smnbBTree::keyReorganization( smnbHeader    * aIndex )
{
    smnbLNode   * sCurLNode = NULL;
    smnbLNode   * sNxtLNode = NULL;
    smnbINode   * sCurINode = NULL;
    smnbNode    * sFreeNodeList = NULL;
    UInt          sDeleteNodeCount = 0;
    UInt          sLockState = 0;
    UInt          sNodeState = 0;
    void        * sOldKeyRow = NULL;
    void        * sNewKeyRow = NULL;
    void        * sNewKey = NULL;
    SInt          sInternalSlotPos = 0;
    SInt          sNodeOldCount = 0;
    SInt          sNodeNewCount = 0;
    SInt          i = 0;

    sCurINode = ( smnbINode * )aIndex->root;

    /* 1. ù��° leaf node�� �� �θ� �Ǵ� internal node�� ã�´�. */

    /* �ش� �ε��� �ȿ� ��尡 ���ų� 1���� �������� ��쿡��
     * ������ �����Ҽ� �����Ƿ� reorganization�� �����Ѵ�. */
    IDE_TEST_CONT( ( sCurINode == NULL ) || 
                   ( ( sCurINode->flag & SMNB_NODE_TYPE_MASK ) == SMNB_NODE_TYPE_LEAF ), 
                   SKIP_KEY_REORGANIZATION );

    IDU_FIT_POINT( "smnbBTree::keyReorganization::findFirstLeafNode" );

    IDE_TEST( lockTree( aIndex ) != IDE_SUCCESS );
    sLockState = 1;

    while ( ( sCurINode->mChildPtrs[0]->flag & SMNB_NODE_TYPE_MASK ) == SMNB_NODE_TYPE_INTERNAL )
    {
        sCurINode = (smnbINode*)sCurINode->mChildPtrs[0];
    }
    sCurLNode = (smnbLNode*)sCurINode->mChildPtrs[0];

    sLockState = 0;
    IDE_TEST( unlockTree( aIndex ) != IDE_SUCCESS);

    while ( sCurLNode != NULL )
    {
        /* ���� �۾��� lockTable �۾��� �������� �ʰ� ����Ǳ� ������
         * ���� �۾� ���� �� �ش� �ε����� ���ŵ� �� �ִ�.
         * ���� �� �ε��� ���Ű� Ȯ�ε� ��� ���� �۾��� �����Ѵ�. */
        IDE_TEST_CONT( aIndex->mIndexHeader->mDropFlag == SMN_INDEX_DROP_TRUE,
                       SKIP_KEY_REORGANIZATION );

        /* 2. ��� ���� ��尡 ���� ���� ������ �������� Ȯ���Ѵ�. */
        if ( checkEnableReorgInNode( sCurLNode,
                                     sCurINode,
                                     SMNB_LEAF_SLOT_MAX_COUNT( aIndex ) ) == ID_FALSE )
        {
            /* ������ �Ұ����� ��� ���� ���� �̵��Ѵ�. */
            sCurLNode = sCurLNode->nextSPtr;
            if ( sCurLNode == NULL )
            {
                /* ��������� ������ ���� ��� */
                continue;
            }
            else
            {
                if ( sCurLNode->mSlotCount == 0 )
                {
                    /* �����͸� Ÿ�� ���� ������尡 ���ŵǾ��� ��� */
                    continue;
                }
                else
                {
                    /* do nothing... */
                }
            }

            sOldKeyRow = sCurLNode->mRowPtrs[( sCurLNode->mSlotCount ) - 1];

            IDE_TEST( findSlotInInternal( aIndex, 
                                          sCurINode, 
                                          sOldKeyRow, 
                                          &sInternalSlotPos ) != IDE_SUCCESS );

            /* �̿� ���� �̵��ϸ鼭 �θ� ��尡 ����� ��� */
            if ( sCurINode->mSlotCount <= sInternalSlotPos )
            {
                /* ���� ���ͳ� ��尡 ���� ��� */
                IDE_TEST( sCurINode->nextSPtr == NULL );

                sCurINode = sCurINode->nextSPtr;

                /* ���� ��忡�� ���� ��� ���ͳ� ��忡 ������ �߻��� ���̹Ƿ� ������ ���� */
                IDE_TEST( findSlotInInternal( aIndex, 
                                              sCurINode, 
                                              sOldKeyRow,
                                              &sInternalSlotPos ) != IDE_SUCCESS );

                IDE_TEST( sCurLNode != ( smnbLNode* )( sCurINode->mChildPtrs[sInternalSlotPos] ) );
            }
            else
            {
                /* do nothing... */
            }
            
            continue;    
        }
        else
        {
            sNodeState = 0;

            sNxtLNode = sCurLNode->nextSPtr;
            IDU_FIT_POINT( "smnbBTree::keyReorganization::beforeLock" );

            /* 3. lock�� ��´�. */
            IDE_TEST( lockTree( aIndex ) != IDE_SUCCESS );
            sLockState = 1;

            IDU_FIT_POINT( "smnbBTree::keyReorganization::exceptionPoint1" );

            IDE_TEST( lockNode( sCurLNode ) != IDE_SUCCESS );
            sLockState = 2;

            IDU_FIT_POINT( "smnbBTree::keyReorganization::exceptionPoint2" );

            IDE_TEST( lockNode( sNxtLNode ) != IDE_SUCCESS );
            sLockState = 3;

            IDU_FIT_POINT( "smnbBTree::keyReorganization::afterLock" );

            /* 4. 2�� �۾� �� treelock�� ��� �� �̿� ��尡 ����� �� ������ �ٽ� Ȯ���Ѵ�. */
            if ( checkEnableReorgInNode( sCurLNode,
                                         sCurINode,
                                         SMNB_LEAF_SLOT_MAX_COUNT( aIndex ) ) == ID_FALSE )       
            {
                sLockState = 2;
                IDE_TEST( unlockNode( sNxtLNode ) != IDE_SUCCESS );

                sLockState = 1;
                IDE_TEST( unlockNode( sCurLNode ) != IDE_SUCCESS );

                sLockState = 0;
                IDE_TEST( unlockTree( aIndex ) != IDE_SUCCESS);

                continue;
            }
            else
            {
                /* do nothing... */
            }

            /* 5. ���� ����� ���� �����صд�.
             *    �� ���� ���ͳ� ��带 �����ϱ� ���� ��ġ�� ã�µ� ���ȴ�. */

            sOldKeyRow = sCurLNode->mRowPtrs[( sCurLNode->mSlotCount ) - 1];

            IDE_TEST( findSlotInInternal( aIndex, 
                                          sCurINode, 
                                          sOldKeyRow, 
                                          &sInternalSlotPos ) != IDE_SUCCESS );

            /* 6. ���� ���� ���� ��尣 ���� �۾��� �����Ѵ�. */
            SMNB_SCAN_LATCH( sCurLNode );
            SMNB_SCAN_LATCH( sNxtLNode );

            sLockState = 4;

            IDU_FIT_POINT( "smnbBTree::keyReorganization::exceptionPoint3" );

            sNodeOldCount = sCurLNode->mSlotCount;

            IDE_ERROR( sCurLNode->mSlotCount + sNxtLNode->mSlotCount 
                       <= SMNB_LEAF_SLOT_MAX_COUNT( aIndex ) );

            /* 6.1 ���� ����� slot���� ���� ��忡 �����Ѵ�. */
            copyLeafSlots( sCurLNode, 
                           sCurLNode->mSlotCount,
                           sNxtLNode,
                           0, /* aSrcStartIdx */
                           sNxtLNode->mSlotCount );

            sNodeState = 1;

            IDU_FIT_POINT( "smnbBTree::keyReorganization::exceptionPoint4" );

            /* 6.2 ���� ����� ��Ÿ ������ �����Ѵ�. */

            /* Ʈ���� ������ ��忡 ���ؼ��� �������� �����Ƿ�
             * �׻� ���� ����� next����� prev pointer�� �����ؾ� �Ѵ�. */
            IDE_ERROR( sNxtLNode->nextSPtr != NULL );

            IDE_TEST( lockNode( sNxtLNode->nextSPtr ) != IDE_SUCCESS );
            sLockState = 5;

            IDU_FIT_POINT( "smnbBTree::keyReorganization::exceptionPoint10" );

            SMNB_SCAN_LATCH( sNxtLNode->nextSPtr );
            sLockState = 6;

            IDU_FIT_POINT( "smnbBTree::keyReorganization::exceptionPoint5" );

            sNxtLNode->nextSPtr->prevSPtr = sNxtLNode->prevSPtr;

            SMNB_SCAN_UNLATCH( sNxtLNode->nextSPtr );
            sLockState = 5;

            IDE_TEST( unlockNode( sNxtLNode->nextSPtr ) != IDE_SUCCESS );
            sLockState = 4;

            sCurLNode->mSlotCount += sNxtLNode->mSlotCount;
            sNodeNewCount = sCurLNode->mSlotCount;
            sCurLNode->nextSPtr = sNxtLNode->nextSPtr;

            sNxtLNode->flag |= SMNB_NODE_INVALID;

            sNodeState = 2;

            IDU_FIT_POINT( "smnbBTree::keyReorganization::exceptionPoint6" );

            SMNB_SCAN_UNLATCH( sNxtLNode );
            SMNB_SCAN_UNLATCH( sCurLNode );

            sLockState = 3;

            /* 7. ���ŵ� �̿� ��带 free node list�� �����Ѵ�. */
            sNxtLNode->freeNodeList = sFreeNodeList;
            sFreeNodeList = (smnbNode*)sNxtLNode;

            sNodeState = 3;

            IDU_FIT_POINT( "smnbBTree::keyReorganization::exceptionPoint7" );

            sDeleteNodeCount++;

            /* 8. ������ ����� Ű ���� �����´�. */
            sNewKeyRow = NULL;
            sNewKey = NULL;

            getLeafSlot( (SChar **)&sNewKeyRow,
                         &sNewKey,
                         sCurLNode,
                         (SShort)( sCurLNode->mSlotCount - 1 ) );

            /* 9. ���� ����� lock�� �����Ѵ�. */
            sLockState = 2;
            IDE_TEST( unlockNode( sNxtLNode ) != IDE_SUCCESS );

            sLockState = 1;
            IDE_TEST( unlockNode( sCurLNode ) != IDE_SUCCESS );

            /* 10. ���ͳ� ��带 �����Ѵ�. */
            SMNB_SCAN_LATCH( sCurINode );

            shiftInternalSlots( sCurINode, 
                                ( SShort )( sInternalSlotPos + 1 ), 
                                ( SShort )( sCurINode->mSlotCount - 1 ),
                                ( SShort )(-1) );
            
            setInternalSlot( sCurINode,
                             ( SShort )sInternalSlotPos,
                             (smnbNode *)sCurLNode,
                             (SChar *)sNewKeyRow,
                             sNewKey );

            setInternalSlot( sCurINode,
                             ( SShort )( sCurINode->mSlotCount - 1 ),
                             NULL,
                             NULL,
                             NULL );

            sNodeState = 4;

            IDU_FIT_POINT( "smnbBTree::keyReorganization::exceptionPoint8" );

            sCurINode->mSlotCount--;

            sNodeState = 5;

            IDU_FIT_POINT( "smnbBTree::keyReorganization::exceptionPoint9" );

            SMNB_SCAN_UNLATCH( sCurINode );

            /* 11. tree latch�� �����Ѵ�. */
            sLockState = 0;
            IDE_TEST( unlockTree( aIndex ) != IDE_SUCCESS );
        }
    }

    /* �۾��� �Ϸ� �� �� free list�� ������ �����Ѵ�. */

    if ( sFreeNodeList != NULL )
    {
        aIndex->nodeCount -= sDeleteNodeCount;

        smLayerCallback::addNodes2LogicalAger( mSmnbFreeNodeList,
                                               ( smnNode* )sFreeNodeList );
    }
    else
    {
        /* do nothing... */
    }

    IDE_EXCEPTION_CONT( SKIP_KEY_REORGANIZATION );

    IDE_ASSERT( sLockState == 0 );

    return IDE_SUCCESS;
    
    IDE_EXCEPTION_END;

    switch ( sNodeState )
    {
        case 5:     /* ���ͳ� ����� slotcounter ���� ���� ���� */
            sCurINode->mSlotCount++;
        case 4:     /* ���ͳ� ����� slot ���� ���� */
            /* ���ͳ� ����� slot shift ���� */
            shiftInternalSlots( sCurINode,
                                ( SShort )( sInternalSlotPos + 1 ),
                                ( SShort )( sCurINode->mSlotCount - 1 ),
                                ( SShort )(1) );

            /* �������� ����� ������ ������ ���� */
            getLeafSlot( (SChar **)&sNewKeyRow,
                         &sNewKey,
                         sNxtLNode,
                         (SShort)( sNxtLNode->mSlotCount - 1 ) );
            setInternalSlot( sCurINode,
                             ( SShort )( sInternalSlotPos + 1 ),
                             (smnbNode *)sNxtLNode,
                             (SChar *)sNewKeyRow,
                             sNewKey );

            /* ���մ���� ��������� Ű ���� ���� */
            getLeafSlot( (SChar **)&sNewKeyRow,
                         &sNewKey,
                         sCurLNode,
                         (SShort)( sNodeOldCount - 1 ) ); 
            setInternalSlot( sCurINode,
                             ( SShort )sInternalSlotPos,
                             (smnbNode *)sCurLNode,
                             (SChar *)sNewKeyRow,
                             sNewKey );
            SMNB_SCAN_UNLATCH( sCurINode );

            IDE_ASSERT( lockNode( sCurLNode ) == IDE_SUCCESS );
            sLockState = 2;
            IDE_ASSERT( lockNode( sNxtLNode ) == IDE_SUCCESS );
            sLockState = 3;

            sDeleteNodeCount--;
        case 3:     /* ���� ���� ��带 freelist�� ������ ������ ���� */
            sFreeNodeList = (smnbNode*)sNxtLNode->freeNodeList;
            sNxtLNode->freeNodeList = NULL;

            SMNB_SCAN_LATCH( sCurLNode );
            SMNB_SCAN_LATCH( sNxtLNode );
            sLockState = 4;
        case 2:     /* ������ ����� ����� ��Ÿ���� ������ ���� */
            sNxtLNode->flag |= SMNB_NODE_VALID;

            SMNB_SCAN_LATCH( sNxtLNode->nextSPtr );
            sNxtLNode->nextSPtr->prevSPtr = sNxtLNode;
            SMNB_SCAN_UNLATCH( sNxtLNode->nextSPtr );

            sCurLNode->mSlotCount = sNodeOldCount;
            sCurLNode->nextSPtr = sNxtLNode;

        case 1:     /* ���� ���� ����� slot�� ���� ������忡 ������ ������ ���� */
            for ( i = 0; i < sNodeNewCount - sNodeOldCount; i++ )
            {
                setLeafSlot( sCurLNode,
                             (SShort)( sNodeNewCount - 1 - i ),
                             NULL,
                             NULL );
            }
        case 0:
        default:
            break;
    }

    switch ( sLockState )
    {
        case 6:
            SMNB_SCAN_UNLATCH( sNxtLNode->nextSPtr );
        case 5:
            IDE_ASSERT( unlockNode( sNxtLNode->nextSPtr ) == IDE_SUCCESS );
        case 4:
            SMNB_SCAN_UNLATCH( sNxtLNode );
            SMNB_SCAN_UNLATCH( sCurLNode );
        case 3:
            IDE_ASSERT( unlockNode( sNxtLNode ) == IDE_SUCCESS );
        case 2:
            IDE_ASSERT( unlockNode( sCurLNode ) == IDE_SUCCESS );
        case 1:
            IDE_ASSERT( unlockTree( aIndex ) == IDE_SUCCESS );
        case 0:
        default:
            break;
    }

    /* �۾��� �Ϸ� �� �� free list�� ������ �����Ѵ�. */
    if ( sFreeNodeList != NULL )
    {
        aIndex->nodeCount -= sDeleteNodeCount;

        smLayerCallback::addNodes2LogicalAger( mSmnbFreeNodeList,
                                               ( smnNode* )sFreeNodeList );
    }

    if ( ideGetErrorCode() == smERR_ABORT_INCONSISTENT_INDEX )
    {
        smnManager::setIsConsistentOfIndexHeader( aIndex->mIndexHeader, ID_FALSE );
    }

    IDE_SET( ideSetErrorCode( smERR_ABORT_ReorgFail ) );

    return IDE_FAILURE;
}

/*********************************************************************
 * FUNCTION DESCRIPTION : smnbBTree::findNextSlotInLeaf              *
 * ------------------------------------------------------------------*
 * BUG-44043
 *
 * ������忡�� aSearchRow ������ ū ù��° ���� ��ġ�� ã�Ƽ�
 * aSlot ���� �����Ѵ�.
 * ( ��ġ�� ã�� ���ϸ� ����� mSlotCount ���� �����Ѵ�. )
 *
 * aIndexStat - [IN]  INDEX ����
 * aColumns   - [IN]  INDEX �÷�����
 * aFence     - [IN]  INDEX �÷����� fence
 * aNode      - [IN]  Ž���� �������
 * aSearchRow - [IN]  Ž���� Ű
 * aSlot      - [OUT] Ž���� ���� ��ġ
 *********************************************************************/
inline void smnbBTree::findNextSlotInLeaf( smnbStatistic    * aIndexStat,
                                           const smnbColumn * aColumns,
                                           const smnbColumn * aFence,
                                           const smnbLNode  * aNode,
                                           SChar            * aSearchRow,
                                           SInt             * aSlot )
{
    IDE_DASSERT( aNode != NULL );
    IDE_DASSERT( aNode->mSlotCount > 0 );

    SInt    sMinimum = 0;
    SInt    sMaximum = ( aNode->mSlotCount - 1 );
    SInt    sMedium  = 0;
    SChar * sRow     = NULL;

    do
    {
        sMedium = ( sMinimum + sMaximum ) >> 1;
        sRow    = aNode->mRowPtrs[sMedium];

        if ( smnbBTree::compareRowsAndPtr( aIndexStat,
                                           aColumns,
                                           aFence,
                                           sRow,
                                           aSearchRow ) > 0 )
        {
            sMaximum = sMedium - 1;
            *aSlot   = sMedium;
        }
        else
        {
            sMinimum = sMedium + 1;
            *aSlot   = sMinimum;
        }
    }
    while ( sMinimum <= sMaximum );

    return;
}

