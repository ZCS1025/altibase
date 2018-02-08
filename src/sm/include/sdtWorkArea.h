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
 * $Id
 *
 * Sort �� Hash�� �ϴ� ��������, �ִ��� ���ü� ó�� ���� �����ϵ��� �����ȴ�.
 * ���� SortArea, HashArea�� �ƿ츣�� �ǹ̷� ���ȴ�. ��
 * SortArea, HashArea��WorkArea�� ���Ѵ�.
 *
 * ���� PrivateWorkArea�� ����������, ù��° ������ P�� ���������� ���Ǳ�
 * ������(��-Page); WorkArea�� �����Ͽ���.
 * �Ϲ������� ���� �� Prefix�� WA�� ���������, ��� ���ܰ� �ִ�.
 * WCB - BCB�� ������ ���� ������, �ǹ̻� WACB���� WCB�� �����Ͽ���.
 * WPID - WAPID�� �� �� WPID�� �ٿ���.
 *
 **********************************************************************/

#ifndef _O_SDT_WORKAREA_H_
#define _O_SDT_WORKAREA_H_ 1

#include <idl.h>
#include <smDef.h>
#include <smiDef.h>
#include <sdtDef.h>
#include <iduLatch.h>
#include <smuWorkerThread.h>
#include <sdtTempPage.h>
#include <sdtWAGroup.h>
#include <sdtWASegment.h>

class sdtWorkArea
{
public:
    /******************************************************************
     * WorkArea
     ******************************************************************/
    static IDE_RC initializeStatic();
    static IDE_RC createWA();
    static IDE_RC resetWA();
    static IDE_RC destroyStatic();
    static IDE_RC dropWA();

    static sdtWAState getWAState() { return mWAState; };
    static UInt getWAFlusherCount() { return mWAFlusherCount; };
    static sdtWAFlusher* getWAFlusher(UInt aWAFlusherIdx ) { return &mWAFlusher[aWAFlusherIdx]; };

    static UInt getGlobalWAFlusherIdx()
    {
        mGlobalWAFlusherIdx = ( mGlobalWAFlusherIdx + 1 ) % sdtWorkArea::getWAFlusherCount();
        return mGlobalWAFlusherIdx;
    }

    static IDE_RC allocWAFlushQueue( sdtWAFlushQueue ** aWAFlushQueue )
    { return  mFlushQueuePool.alloc( (void**)aWAFlushQueue ); };

    static IDE_RC freeWAFlushQueue( sdtWAFlushQueue * aWAFlushQueue )
    {  return  mFlushQueuePool.memfree( (void*)aWAFlushQueue ); };

    static UInt getFreeWAExtentCount() { return mFreePool.getTotItemCnt(); };

    /******************************************************************
     * Segment
     ******************************************************************/
    static IDE_RC allocWASegmentAndExtent( idvSQL             * aStatistics,
                                           sdtWASegment      ** aWASegment,
                                           smiTempTableStats ** aStatsPtr,
                                           UInt                 aExtentCount );
    static IDE_RC freeWASegmentAndExtent( sdtWASegment   * aWASegment );


    static void lock()
    {
        IDE_ASSERT( mMutex.lock( NULL ) == IDE_SUCCESS );
    }
    static void unlock()
    {
        IDE_ASSERT( mMutex.unlock() == IDE_SUCCESS );
    }

    /******************************************************************
     * Flusher
     ******************************************************************/
    static IDE_RC   assignWAFlusher( sdtWASegment       * aWASegment );
    static IDE_RC   releaseWAFlusher( sdtWAFlushQueue   * aWAFlushQueuePtr );
    static void     flusherRun( void * aParam );
    static IDE_RC   flushTempPages( sdtWAFlushQueue * aWAFlushQueue );

    static IDE_RC   flushTempPagesInternal( idvSQL            * aStatistics,
                                            smiTempTableStats * aStats,
                                            sdtWASegment      * aWASegment,
                                            sdtWCB            * aWCBPtr,
                                            UInt                aPageCount );

    static idBool   isSiblingPage( sdtWCB * aPrevWCBPtr,
                                   sdtWCB * aNextWCBPtr );

    static void     incQueueSeq( SInt * aSeq );
    static idBool   isEmptyQueue( sdtWAFlushQueue * aWAFlushQueue );
    static IDE_RC   pushJob( sdtWASegment * aWASegment,
                             scPageID       aWPID );
    static IDE_RC   popJob( sdtWAFlushQueue  * aWAFlushQueue,
                            scPageID         * aWPID,
                            sdtWCB          ** aWCBPtr);


public:
    /***********************************************************
     * FixedTable�� �Լ��� ( for X$TEMPINFO )
     ***********************************************************/
    static IDE_RC buildTempInfoRecord( void                * aHeader,
                                       iduFixedTableMemory * aMemory );

private:
    static UChar               * mArea;        /*WorkArea�� �ִ� �� */
    static UChar               * mAlignedArea; /*8k Align WorkArea�� �ִ� �� */
    static ULong                 mWASize;      /*WorkArea�� Byte ũ�� */
    static iduMutex              mMutex;
    static iduStackMgr           mFreePool;    /*�� Extent�� �����ϴ� �� */
    static sdtWASegment        * mWASegListHead;
    static iduMemPool            mFlushQueuePool;

    static smuWorkerThreadMgr    mFlusherMgr;
    static UInt                  mGlobalWAFlusherIdx;
    static UInt                  mWAFlusherCount;
    static UInt                  mWAFlushQueueSize;
    static sdtWAFlusher        * mWAFlusher;
    static sdtWAState            mWAState;

};

#endif //_O_SDT_WORK_AREA
