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
 * $Id $
 **********************************************************************/

#include <sdtWorkArea.h>
#include <sdtWAMap.h>
#include <smuProperty.h>
#include <sdpDef.h>
#include <sdtTempPage.h>
#include <sdpTableSpace.h>
#include <smiMisc.h>

UChar               * sdtWorkArea::mArea;
UChar               * sdtWorkArea::mAlignedArea;
ULong                 sdtWorkArea::mWASize;
iduMutex              sdtWorkArea::mMutex;
iduStackMgr           sdtWorkArea::mFreePool;
sdtWASegment        * sdtWorkArea::mWASegListHead;
iduMemPool            sdtWorkArea::mFlushQueuePool;

smuWorkerThreadMgr    sdtWorkArea::mFlusherMgr;
UInt                  sdtWorkArea::mGlobalWAFlusherIdx = 0;
UInt                  sdtWorkArea::mWAFlusherCount;
UInt                  sdtWorkArea::mWAFlushQueueSize;
sdtWAFlusher        * sdtWorkArea::mWAFlusher;
sdtWAState            sdtWorkArea::mWAState = SDT_WASTATE_SHUTDOWN;

/******************************************************************
 * WorkArea
 ******************************************************************/
/**************************************************************************
 * Description :
 * ó�� ���� �����Ҷ� ȣ��ȴ�. �������� �ʱ�ȭ�ϰ� Mutex�� �����
 * createWA�� �θ���.
 ***************************************************************************/
IDE_RC sdtWorkArea::initializeStatic()
{
    IDE_TEST( mMutex.initialize( (SChar*)"SDT_WORK_AREA_MUTEX",
                                 IDU_MUTEX_KIND_NATIVE,
                                 IDV_WAIT_INDEX_NULL )
              != IDE_SUCCESS);

    IDE_TEST( createWA() != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * memory�� �Ҵ��ϰ� ������ �����.
 ***************************************************************************/
IDE_RC sdtWorkArea::createWA()
{
    SChar     sMutexName[ IDU_MUTEX_NAME_LEN ];
    UInt      sWAExtentCount;
    vULong    sValue;
    UInt      sLockState = 0;
    UInt      sState = 0;
    UInt      i;

    /* WASegment �ڷᱸ���� ���� ����*/
    /* WASegment�� Page�� ��ġ�Ǳ� ������ Pageũ�⺸�� �۾ƾ� �Ѵ�. */
    IDE_DASSERT( ID_SIZEOF( sdtWASegment ) < SD_PAGE_SIZE );
    /* WASegment + Pointer*N �� Page�� ����ؾ� �ϸ�, �׳ɵ� Page��
     * ����ؾ� �� */
    IDE_DASSERT( ( SD_PAGE_SIZE - ID_SIZEOF( sdtWASegment ) )
                 % ( ID_SIZEOF( UChar * ) ) == 0 );
    IDE_DASSERT( ( SD_PAGE_SIZE % ID_SIZEOF( UChar * ) ) == 0 );

    lock();
    sLockState = 1;

    mWAState = SDT_WASTATE_INITIALIZING;

    sWAExtentCount = smuProperty::getTotalWASize() / SDT_WAEXTENT_SIZE + 1;
    /************************* WorkArea ***************************/
    mWASize = (ULong)sWAExtentCount *  SDT_WAEXTENT_SIZE;
    mWASegListHead = NULL;

    IDU_FIT_POINT_RAISE( "sdtWorkArea::createWA::calloc1", memory_allocate_failed );
    IDE_TEST( iduMemMgr::calloc( IDU_MEM_SM_TEMP,
                                 1,
                                 mWASize + SDT_WAEXTENT_SIZE,
                                 (void**)&mArea )
              != IDE_SUCCESS );
    sState = 1;

    IDE_TEST( mFreePool.initialize( IDU_MEM_SM_TEMP,
                                    ID_SIZEOF( UChar* ) )
              != IDE_SUCCESS );
    sState = 2;

    mAlignedArea = (UChar*)idlOS::align( mArea, SDT_WAEXTENT_SIZE );

    sValue = (vULong)mAlignedArea;
    for( i = 0 ; i < sWAExtentCount ; i ++ )
    {
        IDE_DASSERT( ( sValue % SDT_WAEXTENT_SIZE ) == 0 );

        IDE_TEST( mFreePool.push( ID_FALSE, /* lock */
                                  (void*) &sValue )
                  != IDE_SUCCESS );
        sValue += SDT_WAEXTENT_SIZE;
    }
    sState = 2;
    /****************************** Flusher *****************************/
    mWAFlusherCount = smuProperty::getTempFlusherCount();
    IDU_FIT_POINT_RAISE( "sdtWorkArea::createWA::calloc2", memory_allocate_failed );
    IDE_TEST( iduMemMgr::calloc( IDU_MEM_SM_TEMP,
                                 getWAFlusherCount(),
                                 ID_SIZEOF( sdtWAFlusher ),
                                 (void**)&mWAFlusher )
              != IDE_SUCCESS );
    sState = 3;

    for( i = 0 ; i < getWAFlusherCount() ; i ++ )
    {
        mWAFlusher[ i ].mIdx           = i;
        mWAFlusher[ i ].mTargetHead    = NULL;
        mWAFlusher[ i ].mRun           = ID_FALSE;
        idlOS::snprintf( sMutexName,
                         IDU_MUTEX_NAME_LEN,
                         "SDT_WORKAREA_FLUSHER_%"ID_UINT32_FMT,
                         i );
        IDE_TEST( mWAFlusher[ i ].mMutex.initialize( sMutexName,
                                                     IDU_MUTEX_KIND_NATIVE,
                                                     IDV_WAIT_INDEX_NULL )
                  != IDE_SUCCESS);
    }
    sState = 4;

    IDE_TEST( smuWorkerThread::initialize( flusherRun,
                                           getWAFlusherCount(),
                                           getWAFlusherCount()*4,
                                           &mFlusherMgr )
              != IDE_SUCCESS );

    /* Flusher �⵿��Ű�� ���� Done�� ���������, Flusher���� �����ڸ���
     * ������ ������ ����*/

    for( i = 0 ; i < getWAFlusherCount() ; i ++ )
    {
        IDE_TEST( smuWorkerThread::addJob( &mFlusherMgr,
                                           (void*)&mWAFlusher[ i ] )
                  != IDE_SUCCESS );
        mWAFlusher[ i ].mRun           = ID_TRUE;
    }
    sState = 5;

    mWAFlushQueueSize = smuProperty::getTempFlushQueueSize();
    IDE_TEST( mFlushQueuePool.initialize(
                  IDU_MEM_SM_TEMP,
                  (SChar*)"SDT_FLUSH_QUEUE_POOL",
                  ID_SCALABILITY_SYS,
                  ID_SIZEOF( sdtWAFlushQueue ) +
                  ID_SIZEOF( scPageID ) * mWAFlushQueueSize,
                  64,
                  IDU_AUTOFREE_CHUNK_LIMIT,			/* ChunkLimit */
                  ID_TRUE,							/* UseMutex */
                  IDU_MEM_POOL_DEFAULT_ALIGN_SIZE,	/* AlignByte */
                  ID_FALSE,							/* ForcePooling */
                  ID_TRUE,							/* GarbageCollection */
                  ID_TRUE )							/* HWCacheLine */
              != IDE_SUCCESS);
    sState = 6;

    mWAState = SDT_WASTATE_RUNNING;
    sLockState = 0;
    unlock();

    return IDE_SUCCESS;
#ifdef ALTIBASE_FIT_CHECK
    IDE_EXCEPTION( memory_allocate_failed );
    {
        IDE_SET(ideSetErrorCode(idERR_ABORT_InsufficientMemory));
    }
#endif
    IDE_EXCEPTION_END;

    switch( sState )
    {
        case 6:
            mFlushQueuePool.destroy();
        case 5:
            smuWorkerThread::finalize( &mFlusherMgr );
        case 4:
            while( i-- )
            {
                mWAFlusher[ i ].mMutex.destroy();
            }
        case 3:
            (void) iduMemMgr::free( mWAFlusher );
        case 2:
            mFreePool.destroy();
        case 1:
            (void) iduMemMgr::free( mArea );
        default:
            break;
    }

    mWAFlusherCount = 0;
    mWASize         = 0;
    mWAState        = SDT_WASTATE_SHUTDOWN;

    switch( sLockState )
    {
        case 1:
            unlock();
            break;
        default:
            break;
    }

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * WA�� �籸���Ѵ�.
 ***************************************************************************/
IDE_RC sdtWorkArea::resetWA()
{
    IDE_TEST( dropWA() != IDE_SUCCESS );
    IDE_TEST( createWA() != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * ���� ������ ȣ��ȴ�. dropWA�� ȣ���ϰ� Mutex�� free �Ѵ�.
 ***************************************************************************/
IDE_RC sdtWorkArea::destroyStatic()
{
    IDE_ASSERT( dropWA() == IDE_SUCCESS );
    IDE_ASSERT( mMutex.destroy() == IDE_SUCCESS);

    return IDE_SUCCESS;
}

/**************************************************************************
 * Description :
 * WA�� ���õ� ��ü���� �����ϰ� �޸𸮸� �����Ѵ�.
 ***************************************************************************/
IDE_RC sdtWorkArea::dropWA()
{
    UInt              sFreeCount;
    UInt              sWAExtentCount = mWASize / SDT_WAEXTENT_SIZE;
    PDL_Time_Value    sTV;
    UInt              i;

    sTV.set(0, smuProperty::getTempSleepInterval() );

    lock();
    /* �ٸ� �༮�� ���� Shutdown ���̸� ������ */
    IDE_TEST_CONT( getWAState() != SDT_WASTATE_RUNNING, SKIP );
    mWAState = SDT_WASTATE_FINALIZING;
    unlock();

    sFreeCount = mFreePool.getTotItemCnt();

    /* ��� Transaction���� Extent�� ���� ��� �Ϸ��Ҷ����� ����� */
    while( sFreeCount < sWAExtentCount )
    {
        idlOS::sleep( sTV );

        lock();
        sFreeCount = mFreePool.getTotItemCnt();
        unlock();
    }

    lock();
    /****************************** Flusher *****************************/
    IDE_ASSERT( smuWorkerThread::finalize( &mFlusherMgr ) == IDE_SUCCESS );

    for( i = 0 ; i < getWAFlusherCount() ; i ++ )
    {
        if( mWAFlusher[ i ].mRun == ID_TRUE )
        {
            /* ��� �ִٸ�, Target���� ��� �����Ǿ� �־�� ��. */
            IDE_ASSERT( mWAFlusher[ i ].mTargetHead == NULL );
        }
        IDE_ASSERT( mWAFlusher[ i ].mMutex.destroy() == IDE_SUCCESS);
    }
    IDE_ASSERT( iduMemMgr::free( mWAFlusher ) == IDE_SUCCESS );
    IDE_ASSERT( mFreePool.destroy() == IDE_SUCCESS );
    IDE_ASSERT( mFlushQueuePool.destroy() == IDE_SUCCESS );
    IDE_ASSERT( iduMemMgr::free( mArea ) == IDE_SUCCESS );

    mWAState = SDT_WASTATE_SHUTDOWN;

    IDE_EXCEPTION_CONT( SKIP );
    unlock();

    return IDE_SUCCESS;
}

/**************************************************************************
 * Description :
 * WAExtent���� �Ҵ��Ѵ�. �׸��� WAExtent�� ���� ���ʿ� WASegment�� ��ġ�ϰ�
 * �̸� �ʱ�ȭ���ش�.
 ***************************************************************************/
IDE_RC sdtWorkArea::allocWASegmentAndExtent( idvSQL             *aStatistics,
                                             sdtWASegment      **aWASegment,
                                             smiTempTableStats **aStatsPtr,
                                             UInt                aExtentCount )
{
    sdtWASegment   * sWASeg = NULL;
    UChar          * sExtent;
    idBool           sIsEmpty;
    UInt             sTryCount4AllocWExtent = 0;
    PDL_Time_Value   sTV;
    UInt             sLockState = 0;
    UInt             i;

    sTV.set(0, smuProperty::getTempSleepInterval() );

    IDE_ERROR( aExtentCount >= 1 );
    /* �ƿ� �Ҵ��ϱ⿡ WA�� ������ ��Ȳ */
    IDE_TEST_RAISE( aExtentCount > (mWASize / SDT_WAEXTENT_SIZE),
                    ERROR_NOT_ENOUGH_WORKAREA );

    lock();
    sLockState = 1;

    /***************************WAExtent �Ҵ�****************************/
    while( 1)
    {
        if( getWAState() == SDT_WASTATE_RUNNING ) /* WA�� ��ȿ�ϰ� */
        {
            if( getFreeWAExtentCount() >= aExtentCount ) /* Extent�� �ְ�*/
            {
                break; /* �׷��� Ok */
            }
        }

        sLockState = 0;
        unlock();

        idlOS::sleep( sTV );

        (*aStatsPtr)->mAllocWaitCount ++;
        sTryCount4AllocWExtent ++;

        IDE_TEST_RAISE( sTryCount4AllocWExtent >
                        smuProperty::getTempAllocTryCount(),
                        ERROR_NOT_ENOUGH_WORKAREA );
        IDE_TEST( iduCheckSessionEvent( aStatistics ) != IDE_SUCCESS);

        lock();
        sLockState = 1;
    }

    /* WAExtent�� ��������, �ű⼭ ���� ���ʿ� WASegment����ü�� ��ġ�� */
    for( i = 0 ; i < aExtentCount ; i ++ )
    {
        IDE_TEST( mFreePool.pop( ID_FALSE, /* lock */
                                 (void*) &sExtent,
                                 &sIsEmpty )
                  != IDE_SUCCESS );
        IDE_ERROR( sIsEmpty == ID_FALSE ); /* �ݵ�� �����ؾ��� */

        if( i == 0 )
        {
            /* ù Page���� WASegment�� ��ġ��Ŵ*/
            sWASeg = (sdtWASegment*)sdtWASegment::getFrameInExtent( sExtent, 0 );
            idlOS::memset( sWASeg, 0, ID_SIZEOF( sdtWASegment ) );
        }

        IDE_TEST( sdtWASegment::addWAExtentPtr( sWASeg, sExtent ) != IDE_SUCCESS );
    }
    IDE_ERROR( sWASeg->mWAExtentCount == aExtentCount );

    /*************************** WAList����  ****************************/
    sWASeg->mPrev = NULL;
    if( mWASegListHead != NULL )
    {
        mWASegListHead->mPrev = sWASeg;
    }
    sWASeg->mNext = mWASegListHead;

    mWASegListHead = sWASeg;

    sLockState = 0;
    unlock();

    *aWASegment = sWASeg;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERROR_NOT_ENOUGH_WORKAREA );
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_NOT_ENOUGH_WORKAREA ) );
    }
    IDE_EXCEPTION_END;

    switch( sLockState )
    {
        case 1:
            unlock();
            break;
        default:
            break;
    }

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * WAExtent���� ��ȯ�Ѵ�.
 ***************************************************************************/
IDE_RC sdtWorkArea::freeWASegmentAndExtent( sdtWASegment   * aWASegment )
{
    UChar          * sWAExtent;
    UInt             sLockState = 0;
    UInt             i;

    lock();
    sLockState = 1;

    for( i = 0 ; i < aWASegment->mWAExtentCount ; i ++ )
    {
        sWAExtent = sdtWASegment::getWAExtentPtr( aWASegment, i );
        IDE_TEST( mFreePool.push( ID_FALSE, /* lock */
                                  (void*) &sWAExtent )
                  != IDE_SUCCESS );
    }

    if( aWASegment->mNext != NULL )
    {
        aWASegment->mNext->mPrev = aWASegment->mPrev;
    }
    if( aWASegment->mPrev != NULL )
    {
        aWASegment->mPrev->mNext = aWASegment->mNext;
    }
    else
    {
        mWASegListHead = aWASegment->mNext;
    }

    sLockState = 0;
    unlock();

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    switch( sLockState )
    {
        case 1:
            unlock();
            break;
        default:
            break;
    }

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * ������ WASegment�� ����� Flusher�� �����Ѵ�.
 ***************************************************************************/
IDE_RC sdtWorkArea::assignWAFlusher( sdtWASegment * aWASegment )
{
    sdtWAFlusher    * sWAFlusher = NULL;
    sdtWAFlushQueue * sWAFlushQueue;
    UInt              sLockState = 0;
    UInt              sState = 0;
    UInt              sWAFlusherIdx;
    UInt              i;

    for( i = 0 ; i < getWAFlusherCount() ; i ++ )
    {
        /* AtomicOperation���� ���� ���ص� LoadBalancing��
         * �ұ��������� �������� */
        sWAFlusherIdx = getGlobalWAFlusherIdx();
        sWAFlusher    = getWAFlusher( sWAFlusherIdx );

        IDE_TEST( sWAFlusher->mMutex.lock( NULL ) != IDE_SUCCESS );
        sLockState = 1;

        if( sWAFlusher->mRun == ID_TRUE ) /* �������� Flusher�� ã���� */
        {
            /* sdtWASegment_assignWAFlusher_alloc_FlushQueue.tc */
            IDU_FIT_POINT("sdtWorkArea::assignWAFlusher::alloc::FlushQueue");
            IDE_TEST( allocWAFlushQueue( &aWASegment->mFlushQueue )
                      != IDE_SUCCESS );
            sState = 1;
            sWAFlushQueue = aWASegment->mFlushQueue;

            sWAFlushQueue->mFQBegin      = 0;
            sWAFlushQueue->mFQEnd        = 0;
            sWAFlushQueue->mFQDone       = ID_FALSE;
            sWAFlushQueue->mWAFlusherIdx = sWAFlusherIdx;
            sWAFlushQueue->mWASegment    = aWASegment;

            sWAFlushQueue->mNextTarget   = sWAFlusher->mTargetHead;
            sWAFlusher->mTargetHead      = sWAFlushQueue;

            sLockState = 0;
            IDE_TEST( sWAFlusher->mMutex.unlock() != IDE_SUCCESS );
            break;
        }
        else
        {
            /* �ƴϸ�, ���� Flusher�� ã�� �ٽ� �õ��� */
            sLockState = 0;
            IDE_TEST( sWAFlusher->mMutex.unlock() != IDE_SUCCESS );
        }
    }

    /* �������� Flusher�� �ϳ��� ������, ������ �� �� ���� */
    IDE_ERROR( sWAFlusher != NULL );
    IDE_ERROR( sWAFlusher->mRun == ID_TRUE );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    switch( sLockState )
    {
        case 1:
            sWAFlusher->mMutex.unlock();
            break;
        default:
            break;
    }

    switch( sState )
    {
        case 1:
            freeWAFlushQueue( aWASegment->mFlushQueue );
            break;
        default:
            break;
    }

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * ������ WASegment�� ����� Flusher�� ������´�.
 ***************************************************************************/
IDE_RC sdtWorkArea::releaseWAFlusher( sdtWAFlushQueue   * aWAFlushQueue )
{
    sdtWAFlusher     * sWAFlusher = getWAFlusher( aWAFlushQueue->mWAFlusherIdx );
    sdtWAFlushQueue ** sWAFlushQueuePtr;
    sdtWAFlushQueue  * sWAFlushQueue;
    UInt               sLockState = 0;

    IDE_TEST( sWAFlusher->mMutex.lock( NULL ) != IDE_SUCCESS );
    sLockState = 1;

    sWAFlushQueuePtr = &sWAFlusher->mTargetHead;
    sWAFlushQueue    = sWAFlusher->mTargetHead;

    while( sWAFlushQueue != NULL )
    {
        if( sWAFlushQueue == aWAFlushQueue )
        {
            break;
        }

        sWAFlushQueuePtr = &sWAFlushQueue->mNextTarget;
        sWAFlushQueue    = sWAFlushQueue->mNextTarget;
    }

    IDE_ERROR( sWAFlushQueue != NULL );

    /* �ش� Queue�� ��� �����.  Link���� �����ϰ� Free�� */
    *sWAFlushQueuePtr = sWAFlushQueue->mNextTarget;
    IDE_TEST( freeWAFlushQueue( sWAFlushQueue )
              != IDE_SUCCESS );

    sLockState = 0;
    IDE_TEST( sWAFlusher->mMutex.unlock() != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    switch( sLockState )
    {
        case 1:
            sWAFlusher->mMutex.unlock();
            break;
        default:
            break;
    }

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 *     Flusher�� �����ϴ� ���� �Լ�
 *
 * <Warrning>
 * Flusher������ �ݵ�� getWCB, getWAPagePtr���� Hint�� ����ϴ� �Լ���
 * ����ϸ� �ȵȴ�.
 * �ش� �Լ����� ServiceThread ȥ�� �����Ѵٴ� ������ �ֱ� ������,
 * Flusher�� ����ϸ� �ȵȴ�. getWCBInternal������ ó���ؾ� �Ѵ�.
 * <Warrning>
 *
 * <IN>
 * aParam         - �� Flusher�� ���� ����
 ***************************************************************************/
void sdtWorkArea::flusherRun( void * aParam )
{
    sdtWAFlusher     * sWAFlusher;
    sdtWAFlushQueue  * sWAFlushQueue = NULL;
    sdtWASegment     * sWASegment    = NULL;
    idBool             sDoFlush;
    UInt               sLockState = 0;
    UInt               sDoneState = 0;
    PDL_Time_Value     sTV;

    sTV.set(0, smuProperty::getTempSleepInterval() );

    sWAFlusher = (sdtWAFlusher*)aParam;
    while( getWAState() == SDT_WASTATE_INITIALIZING )
    {
        idlOS::sleep( sTV );
    }

    while( 1 )
    {
        IDE_TEST( sWAFlusher->mMutex.lock( NULL ) != IDE_SUCCESS );
        sLockState = 1;

        if( ( sWAFlusher->mTargetHead == NULL ) &&
            ( getWAState() != SDT_WASTATE_RUNNING ) )
        {
            /*���� ��������, �����ؾ� �ϴ� ��Ȳ */
            sWAFlusher->mRun = ID_FALSE;

            sLockState = 0;
            IDE_TEST( sWAFlusher->mMutex.unlock() != IDE_SUCCESS );
            break;
        }

        sDoFlush         = ID_FALSE;
        sWAFlushQueue    = sWAFlusher->mTargetHead;
        while( sWAFlushQueue != NULL )
        {
            if( sWAFlushQueue->mFQDone == ID_TRUE )
            {
                sWASegment = (sdtWASegment*)sWAFlushQueue->mWASegment;
                sDoneState = 1;
                IDE_TEST( sdtWASegment::freeAllNPage( sWASegment )
                          != IDE_SUCCESS );
                sDoneState = 2;
                IDE_TEST( sWASegment->mFreeNPageStack.destroy()
                          != IDE_SUCCESS );
                sDoneState = 3;
                IDE_TEST( freeWASegmentAndExtent( sWASegment )
                          != IDE_SUCCESS );
                sDoneState = 4;
                sLockState = 0;
                IDE_TEST( sWAFlusher->mMutex.unlock() != IDE_SUCCESS );

                IDE_TEST( releaseWAFlusher( sWAFlushQueue ) != IDE_SUCCESS );

                IDE_TEST( sWAFlusher->mMutex.lock( NULL ) != IDE_SUCCESS );
                sLockState = 1;

                sWAFlushQueue    = sWAFlusher->mTargetHead;
                continue;
            }

            /* Flush�� �۾��� �ִٸ� */
            if( isEmptyQueue( sWAFlushQueue ) == ID_FALSE )
            {
                sLockState = 0;
                IDE_TEST( sWAFlusher->mMutex.unlock() != IDE_SUCCESS );

                IDE_TEST( flushTempPages( sWAFlushQueue ) != IDE_SUCCESS );
                sDoFlush = ID_TRUE;

                IDE_TEST( sWAFlusher->mMutex.lock( NULL ) != IDE_SUCCESS );
                sLockState = 1;
            }

            sWAFlushQueue    = sWAFlushQueue->mNextTarget;
        }

        sLockState = 0;
        IDE_TEST( sWAFlusher->mMutex.unlock() != IDE_SUCCESS );

        if( sDoFlush == ID_FALSE )
        {
            /* �Ұ� ���� */
            idlOS::sleep( sTV );
        }
    }

    IDE_ERROR( sWAFlusher->mRun == ID_FALSE );

    return;

    IDE_EXCEPTION_END;

    switch( sLockState )
    {
        case 1:
            IDE_ASSERT( sWAFlusher->mMutex.unlock() == IDE_SUCCESS );
            break;
        default:
            break;
    }
    switch ( sDoneState )
    {
        /* BUG-42751 ���� �ܰ迡�� ���ܰ� �߻��ϸ� �ش� �ڿ��� �����Ǳ⸦ ��ٸ��鼭
           server stop �� HANG�� �߻��Ҽ� �����Ƿ� ��������� ��. */
        case 1:
            IDE_ASSERT( sWASegment->mFreeNPageStack.destroy() == IDE_SUCCESS );
        case 2:
            IDE_ASSERT( freeWASegmentAndExtent( sWASegment ) == IDE_SUCCESS );
        case 3:
            IDE_ASSERT( releaseWAFlusher( sWAFlushQueue ) == IDE_SUCCESS );
            break;
        default:
            break;
    }
    ideLog::log( IDE_SM_0,
                 "STOP WAFlusher %u",
                 sWAFlusher->mIdx );
    if( sWAFlushQueue != NULL )
    {
        smuUtility::dumpFuncWithBuffer( IDE_DUMP_0,
                                        sdtWASegment::dumpFlushQueue,
                                        (void*)sWAFlushQueue );
    }

    sWAFlusher->mRun = ID_FALSE;
    IDE_DASSERT( 0 );

    return;
}

/**************************************************************************
 * Description :
 * Flusher�� FlushQueue���� Flush�� �������� ������ Write�Ѵ�.
 *
 * <Warrning>
 * Flusher������ �ݵ�� getWCB, getWAPagePtr���� Hint�� ����ϴ� �Լ���
 * ����ϸ� �ȵȴ�.
 * �ش� �Լ����� ServiceThread ȥ�� �����Ѵٴ� ������ �ֱ� ������,
 * Flusher�� ����ϸ� �ȵȴ�. getWCBInternal������ ó���ؾ� �Ѵ�.
 * <Warrning>
 *
 * <IN>
 * aWAFlushQueue  - ��� FlushQueue
 ***************************************************************************/
IDE_RC sdtWorkArea::flushTempPages( sdtWAFlushQueue * aWAFlushQueue )
{
    UInt             sSiblingPageCount = 0;
    sdtWCB         * sPrevWCBPtr       = NULL;
    sdtWCB         * sCurWCBPtr        = NULL;
    scPageID         sWPID;
    UInt             i;

    for( i = 0 ; i < smuProperty::getTempFlushPageCount() ; i ++ )
    {
        IDE_TEST( popJob( aWAFlushQueue, &sWPID, &sCurWCBPtr )
                  != IDE_SUCCESS );
        if( sCurWCBPtr == NULL )
        {
            break;
        }

        if( aWAFlushQueue->mFQDone == ID_TRUE )
        {
            /* ���� Flusher �۾��� ������.
             * ���� �۾����� Flush�� �ʿ䵵 ���� */
            break;
        }

        /* WPID�� ����� �����Ǿ� �ִ��� �˻� */
        IDE_ERROR( sCurWCBPtr->mWPageID == sWPID );

        if( ( sSiblingPageCount > 0 ) &&
            ( isSiblingPage( sPrevWCBPtr, sCurWCBPtr ) == ID_FALSE ) )
        {
            // ���������� ���� �������� ������ �Ǹ�,
            // ������ ���ӵ� ���������� Flush��
            IDE_TEST( flushTempPagesInternal( aWAFlushQueue->mStatistics,
                                              *aWAFlushQueue->mStatsPtr,
                                              (sdtWASegment*)
                                              aWAFlushQueue->mWASegment,
                                              sPrevWCBPtr,
                                              sSiblingPageCount )
                      != IDE_SUCCESS );

            sSiblingPageCount = 0;
        }
        sPrevWCBPtr = sCurWCBPtr;

        sSiblingPageCount ++;

    }

    if( ( sSiblingPageCount > 0 ) && ( aWAFlushQueue->mFQDone == ID_FALSE ) )
    {
        IDE_TEST( flushTempPagesInternal( aWAFlushQueue->mStatistics,
                                          *aWAFlushQueue->mStatsPtr,
                                          (sdtWASegment*)
                                          aWAFlushQueue->mWASegment,
                                          sPrevWCBPtr,
                                          sSiblingPageCount )
                  != IDE_SUCCESS );
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * Page�� MultiBlockWrite�� ����Ѵ�.
 *
 * <Warrning>
 * Flusher������ �ݵ�� getWCB, getWAPagePtr���� Hint�� ����ϴ� �Լ���
 * ����ϸ� �ȵȴ�.
 * �ش� �Լ����� ServiceThread ȥ�� �����Ѵٴ� ������ �ֱ� ������,
 * Flusher�� ����ϸ� �ȵȴ�. getWCBInternal������ ó���ؾ� �Ѵ�.
 * <Warrning>
 *
 * <IN>
 * aStatistics    - �������
 * aStats         - TempTable�� �������
 * aWASegment     - ��� WASegment
 * aWCBPtr        - ��� WCBPtr
 * aPageCount     - �� WCB�κ���, N���� Page�� �ѹ��� Flush�Ѵ�.
 *                  �� WCB�� PID�� 10, N�� 4�̸�, 7,8,9,10 �̴�.
 ***************************************************************************/
IDE_RC sdtWorkArea::flushTempPagesInternal( idvSQL            * aStatistics,
                                            smiTempTableStats * aStats,
                                            sdtWASegment      * aWASegment,
                                            sdtWCB            * aWCBPtr,
                                            UInt                aPageCount )
{
    UChar          * sWAPagePtr;
    sdtWCB         * sTargetWCBPtr;
    sdtWAPageState   sState;
    scSpaceID        sSpaceID;
    scPageID         sPageID;
    UInt             i;

    aStats->mWriteCount ++;
    aStats->mWritePageCount  += aPageCount;

    sWAPagePtr = aWCBPtr->mWAPagePtr - SD_PAGE_SIZE * ( aPageCount - 1 );
#if defined(DEBUG)
    /* �̿��������� ����� �����Դ��� �˻� */
    IDE_TEST( sdtWASegment::getSiblingWCBPtr( aWCBPtr, aPageCount - 1, &sTargetWCBPtr )
              != IDE_SUCCESS );
    IDE_ASSERT( sWAPagePtr == sTargetWCBPtr->mWAPagePtr );
#endif

    sPageID  = aWCBPtr->mNPageID - aPageCount + 1;
    sSpaceID = aWCBPtr->mNSpaceID;
    IDE_ERROR( sSpaceID == aWASegment->mSpaceID );

    IDE_TEST( sddDiskMgr::write4DPath( aStatistics,
                                       sSpaceID,
                                       sPageID,
                                       aPageCount,
                                       sWAPagePtr )
              != IDE_SUCCESS );

    for( i = 0 ; i < aPageCount ; i ++ )
    {
        /* Writing�� �ٽ� Clean���� ���� */
        IDE_TEST( sdtWASegment::getSiblingWCBPtr( aWCBPtr, i, &sTargetWCBPtr )
                  != IDE_SUCCESS );
        sdtWASegment::checkAndSetWAPageState( sTargetWCBPtr,
                                              SDT_WA_PAGESTATE_WRITING,
                                              SDT_WA_PAGESTATE_CLEAN,
                                              &sState );

        if( sState == SDT_WA_PAGESTATE_IN_FLUSHQUEUE )
        {
            /* ServiceThread�� �ٽñ� Flush�� ��û�Ͽ���. �׷��鼭 Queue��
             * Job�� ����Ͽ��� ���̱� ������, �� ���������� ������ �Ѵ�.
             * �ܼ��� ��� ���� ���̴�. */
            aStats->mRedirtyCount ++;
        }

        /* Writing ���¸��� �ƴϾ�� �Ѵ�.
         * Writing���� �����ϴ� ���� �� Segment�� ����ϴ�
         * �� Flusher Thread ���̱� �����̴�. */
        IDE_DASSERT( sTargetWCBPtr->mWPState != SDT_WA_PAGESTATE_WRITING );
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    ideLog::log( IDE_DUMP_0,
                 "TEMP Flush fail :\n"
                 "PCount  : %u\n",
                 aPageCount );
    smuUtility::dumpFuncWithBuffer( IDE_DUMP_0,
                                    sdtWASegment::dumpWCB,
                                    aWCBPtr );

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * NPID�����ε� WPID�����ε� �̿����� �Ǵ��Ѵ�.
 *
 * <IN>
 * aPrevWCBPtr    - ���� WCBPtr
 * aNextWCBPtr    - ���� WCBPtr
 ***************************************************************************/
idBool sdtWorkArea::isSiblingPage( sdtWCB * aPrevWCBPtr, sdtWCB * aNextWCBPtr )
{
    UInt sPrevExtentIdx = sdtWASegment::getExtentIdx( aPrevWCBPtr->mWPageID );
    UInt sNextExtentIdx = sdtWASegment::getExtentIdx( aNextWCBPtr->mWPageID );

    if( ( aNextWCBPtr->mNSpaceID    == aPrevWCBPtr->mNSpaceID ) &&
        ( aNextWCBPtr->mNPageID - 1 == aPrevWCBPtr->mNPageID  ) &&
        ( aNextWCBPtr->mWPageID - 1 == aPrevWCBPtr->mWPageID  ) &&
        ( sPrevExtentIdx == sNextExtentIdx ) )
    {
        return ID_TRUE;
    }

    return ID_FALSE;
}

/**************************************************************************
 * Description :
 * Queue�� Sequqence���� �ű��.
 * �ϳ��� Thread�� �����ϱ� ������, ABA Problem�� �����Ұ� ����.
 * ( Begin�� Flusher��, End�� ServiceThread�� )
 *
 * <IN>
 * aSeq           - �ø� Seq��
 ***************************************************************************/
void sdtWorkArea::incQueueSeq( SInt * aSeq )
{
    (void)idCore::acpAtomicInc32( aSeq );

    /* Max�� ���������� �ʱ�ȭ */
    if( *aSeq == (SInt)mWAFlushQueueSize )
    {
        (void)idCore::acpAtomicSet32( aSeq, 0 );
    }
}

/**************************************************************************
 * Description :
 * Queue�� ��� �ִ°�?
 *
 * <IN>
 * aWAFQ    - ��� Queue
 ***************************************************************************/
idBool   sdtWorkArea::isEmptyQueue( sdtWAFlushQueue * aWAFQ )
{
    if( ( idCore::acpAtomicGet32( &aWAFQ->mFQBegin )
          == idCore::acpAtomicGet32( &aWAFQ->mFQEnd ) ) ||
        ( aWAFQ->mFQDone == ID_TRUE ) ) /* ����Ǿ��, ������� ���� */
    {
        return ID_TRUE;
    }
    else
    {
        return ID_FALSE;
    }
}

/**************************************************************************
 * Description :
 * ServiceThread�� Flush�� ��� Page�� Queue�� ������
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWPID          - ��� WPID
 ***************************************************************************/
IDE_RC   sdtWorkArea::pushJob( sdtWASegment    * aWASeg,
                               scPageID          aWPID )
{
    sdtWAFlushQueue * sWAFQ = aWASeg->mFlushQueue;
    SInt              sEnd;
    SInt              sNextEnd;
    PDL_Time_Value    sTV;

    sTV.set(0, smuProperty::getTempSleepInterval() );

    sEnd = sNextEnd = idCore::acpAtomicGet32( &sWAFQ->mFQEnd );
    incQueueSeq( &sNextEnd );

    /* Queue�� ��á�°�?*/
    while( sNextEnd == idCore::acpAtomicGet32( &sWAFQ->mFQBegin ) )
    {
        /* Flusher�� ��������, ������ ���� */
        IDE_TEST_RAISE( mWAFlusher[ sWAFQ->mWAFlusherIdx ].mRun == ID_FALSE,
                        ERROR_TEMP_FLUSHER_STOPPED );

        /* sleep�ߴٰ� �ٽ� �õ��� */
        (*aWASeg->mStatsPtr)->mQueueWaitCount ++;
        idlOS::thr_yield();
    }

    sWAFQ->mSlotPtr[ sEnd ] = aWPID;

    (void)idCore::acpAtomicSet32( &sWAFQ->mFQEnd, sNextEnd );
    IDE_ERROR( sWAFQ->mFQEnd < (SInt)mWAFlushQueueSize );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERROR_TEMP_FLUSHER_STOPPED );
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_TEMP_FLUSHER_STOPPED ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * Flusher�� Flush�� ��� Page�� Queue���� ������
 *
 * <Warrning>
 * Flusher������ �ݵ�� getWCB, getWAPagePtr���� Hint�� ����ϴ� �Լ���
 * ����ϸ� �ȵȴ�.
 * �ش� �Լ����� ServiceThread ȥ�� �����Ѵٴ� ������ �ֱ� ������,
 * Flusher�� ����ϸ� �ȵȴ�. getWCBInternal������ ó���ؾ� �Ѵ�.
 * <Warrning>
 *
 * <IN>
 * aWAFQ          - ��� FlushQueue
 * <OUT>
 * aWPID          - ��� WPID
 * aWCBPtr        - ��� WCBPtr
 ***************************************************************************/
IDE_RC     sdtWorkArea::popJob( sdtWAFlushQueue  * aWAFQ,
                                scPageID         * aWPID,
                                sdtWCB          ** aWCBPtr)
{
    sdtWASegment   * sWASeg = (sdtWASegment*)aWAFQ->mWASegment;
    sdtWCB         * sWCBPtr        = NULL;
    sdtWAPageState   sState;
    SInt             sBeginSeq;

    *aWPID   = SC_NULL_PID;
    *aWCBPtr = NULL;

    while( isEmptyQueue( aWAFQ ) == ID_FALSE )
    {
        sBeginSeq = idCore::acpAtomicGet32( &aWAFQ->mFQBegin );
        *aWPID    = aWAFQ->mSlotPtr[ sBeginSeq ];

        sWCBPtr = sdtWASegment::getWCBInternal( sWASeg, *aWPID );

        /* Flusher�� �������� �ǹ̷�, Writing ���·� �����ص� */
        sdtWASegment::checkAndSetWAPageState( sWCBPtr,
                                              SDT_WA_PAGESTATE_IN_FLUSHQUEUE,
                                              SDT_WA_PAGESTATE_WRITING,
                                              &sState );

        incQueueSeq( &sBeginSeq );
        (void)idCore::acpAtomicSet32( &aWAFQ->mFQBegin, sBeginSeq );
        IDE_ERROR( aWAFQ->mFQBegin < (SInt)sdtWorkArea::mWAFlushQueueSize );

        if( sState == SDT_WA_PAGESTATE_IN_FLUSHQUEUE )
        {
            *aWCBPtr = sWCBPtr;
            break;
        }
        else
        {
            /* ServiceThread���� �� WCB�� ���� Flush�� Queue�� ���� ��
             * �����Ͽ��� */
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * X$Tempinfo�� ���� ���ڵ� ����
 ***************************************************************************/
IDE_RC sdtWorkArea::buildTempInfoRecord( void                * aHeader,
                                         iduFixedTableMemory * aMemory )
{
    smiTempInfo4Perf sInfo;
    SChar            sName[ 32 ];
    SChar            sTrueFalse  [ 2 ][ 16 ] = {"FALSE","TRUE"};
    SChar            sWAStateName[ 4 ][ 16 ] = {
        "SHUTDOWN",
        "INITIALIZING",
        "RUNNING",
        "FINALIZING" };
    UInt             sLockState = 0;
    UInt             i;

    lock();
    sLockState = 1;

    SMI_TT_SET_TEMPINFO_STR( "WASTATE",
                             sWAStateName[ mWAState ],
                             " " );
    SMI_TT_SET_TEMPINFO_ULONG( "TOTAL WORK AREA SIZE", mWASize, "BYTES" );
    SMI_TT_SET_TEMPINFO_UINT( "FREE WA COUNT", getFreeWAExtentCount(),
                              "EXTENT" );
    SMI_TT_SET_TEMPINFO_UINT( "EXTENT SIZE", SDT_WAEXTENT_SIZE, "BYTES" );
    SMI_TT_SET_TEMPINFO_UINT( "FLUSHER COUNT", getWAFlusherCount(), "INTEGER" );
    SMI_TT_SET_TEMPINFO_UINT( "FLUSHER SEQ", mGlobalWAFlusherIdx, "INTEGER" );

    for( i = 0 ; i < getWAFlusherCount() ; i ++ )
    {
        idlOS::snprintf( sName, 32, "FLUSHER %"ID_UINT32_FMT" ENABLE", i );
        SMI_TT_SET_TEMPINFO_STR( sName,
                                 sTrueFalse[ mWAFlusher[ i ].mRun ],
                                 "TRUE/FALSE" );
    }

    sLockState = 0;
    unlock();

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    switch( sLockState )
    {
        case 1:
            unlock();
            break;
        default:
            break;
    }

    return IDE_FAILURE;
}
