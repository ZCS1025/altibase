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
 * $Id: smmSlotList.h 82075 2018-01-17 06:39:52Z jina.kim $
 ***********************************************************************/ 

#ifndef _O_SMM_SLOT_LIST_H_
# define _O_SMM_SLOT_LIST_H_ 1

#include <smc.h>
#include <smu.h>

#include <smmDef.h>

class smmSlotList
{
 public:
    //fix BUG-23007
    IDE_RC initialize( UInt aSlotSize,
                       UInt         aMaximum = SMM_SLOT_LIST_MAXIMUM_DEFAULT,
                       UInt         aCache   = SMM_SLOT_LIST_CACHE_DEFAULT,
                       smmSlotList* aParent  = NULL );
    
    IDE_RC destroy( void );
    
    IDE_RC allocateSlots( UInt         aNumber,
                          smmSlot**    aSlots,
                          UInt         aFlag = SMM_SLOT_LIST_MUTEX_ACQUIRE );
    
    IDE_RC releaseSlots( UInt     aNumber,
                         smmSlot* aSlots,
                         UInt     aFlag = SMM_SLOT_LIST_MUTEX_ACQUIRE );
    
    IDE_RC release( );

    UInt testGetCount( void ) { return mNumber; }

    // BUG-18122 : MEM_BTREE_NODEPOOL performance view �߰�
    UInt getPageCount( void )      { return mPageCount;      }
    UInt getSlotPerPage( void )    { return mSlotPerPage;    }
    UInt getFreeSlotCount( void )  { return mNumber;         }
    UInt getSlotSize( void )       { return mSlotSize;       }
    ULong getTotalAllocReq( void ) { return mTotalAllocReq;  }
    ULong getTotalFreeReq( void )  { return mTotalFreeReq;   }

    inline IDE_RC  lock() { return mMutex.lock( NULL ); }
    inline IDE_RC  unlock() { return mMutex.unlock(); }
    
private:
    iduMutex               mMutex;
    smmSlotList*           mParent;
    UInt                   mMaximum;
    UInt                   mCache;
    UInt                   mNumber;
    struct __smmTempPage__ mPagesBody;
    smmTempPage*           mPages;
    smmSlot                mSlots;
    UInt                   mSlotSize;
    UInt                   mSlotPerPage;
    UInt                   mPageCount;

    // BUG-18122 : MEM_BTREE_NODEPOOL performance view �߰�
    ULong                  mTotalAllocReq;
    ULong                  mTotalFreeReq;
};

#endif /* _O_SMM_SLOT_LIST_H_ */
