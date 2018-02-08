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


/******************************************************************
 * Segment
 ******************************************************************/
/**************************************************************************
 * Description :
 * Segment�� Sizeũ���� WExtent�� ����ü �����Ѵ�
 *
 * <IN>
 * aStatistics - �������
 * aStatsPtr   - TempTable�� �������
 * aLogging    - Logging���� (����� ��ȿ��)
 * aSpaceID    - Extent�� ������ TablespaceID
 * aSize       - ������ WA�� ũ��
 * <OUT>
 * aWASegment  - ������ WASegment
 ***************************************************************************/
IDE_RC sdtWASegment::createWASegment(idvSQL             * aStatistics,
                                     smiTempTableStats ** aStatsPtr,
                                     idBool               aLogging,
                                     scSpaceID            aSpaceID,
                                     ULong                aSize,
                                     sdtWASegment      ** aWASegment )
{
    sdtWASegment   * sWASeg = NULL;
    sdtWCB         * sWCBPtr;
    UInt             sExtentCount;
    ULong            sExtentSize;
    sdtWAGroup     * sInitGrpInfo;
    UChar          * sExtent;
    UInt             sState = 0;
    UInt             i;
    UInt             sHashBucketDensity;
    UInt             sHashBucketCnt;

    /* ���� Logging ��� ��� ���� */
    IDE_ASSERT( aLogging == ID_FALSE );

    /* �ø� �Ͽ� �Ҵ���� */
    sExtentSize  = ( SD_PAGE_SIZE * SDT_WAEXTENT_PAGECOUNT );
    sExtentCount = ( aSize + sExtentSize - 1 ) / sExtentSize;
    sHashBucketDensity = smuProperty::getTempHashBucketDensity();

    IDE_TEST( sdtWorkArea::allocWASegmentAndExtent( aStatistics,
                                                    &sWASeg,
                                                    aStatsPtr,
                                                    sExtentCount)
              != IDE_SUCCESS);
    sState = 1;
    IDE_TEST( sdtWorkArea::assignWAFlusher( sWASeg ) != IDE_SUCCESS );
    sState = 2;

    /***************************** initialize ******************************/
    sWASeg->mSpaceID                 = aSpaceID;
    sWASeg->mFlushQueue->mStatsPtr   = aStatsPtr;
    sWASeg->mFlushQueue->mStatistics = aStatistics;
    sWASeg->mStatsPtr                = aStatsPtr;
    sWASeg->mNExtentCount            = 0;
    sWASeg->mNPageCount              = 0;
    sWASeg->mLastFreeExtent          = NULL;
    sWASeg->mPageSeqInLFE            = 0;
    sWASeg->mDiscardPage             = ID_FALSE;
    sWASeg->mStatistics              = aStatistics;
    sWASeg->mNPageHashPtr            = NULL;
    sWASeg->mNPageHashBucketCnt      = 0;
    /*************************** init first wcb ****************************/
    sExtent = getWAExtentPtr( sWASeg, 0 );
    sWASeg->mHintWCBPtr      = getWCBInExtent( sExtent, 0 );
    initWCB( sWASeg, sWASeg->mHintWCBPtr, 0 );

    /***************************** InitGroup *******************************/
    for( i = 0 ; i< SDT_WAGROUPID_MAX ; i ++ )
    {
        sWASeg->mGroup[ i ].mPolicy = SDT_WA_REUSE_NONE;
    }

    /* InitGroup�� �����Ѵ�. */
    sInitGrpInfo = &sWASeg->mGroup[ 0 ];
    /* Segment ���� WAExtentPtr�� ��ġ�� �� Range�� �����̴�. */
    sInitGrpInfo->mBeginWPID =
        ( ( ID_SIZEOF( sdtWASegment ) +
            ID_SIZEOF(UChar*) * sWASeg->mWAExtentCount )
          / SD_PAGE_SIZE ) + 1;
    sInitGrpInfo->mEndWPID = sExtentCount * SDT_WAEXTENT_PAGECOUNT;
    IDE_ERROR( sInitGrpInfo->mBeginWPID < sInitGrpInfo->mEndWPID );

    for( i = 0 ; i< sExtentCount * SDT_WAEXTENT_PAGECOUNT ; i ++ )
    {
        sWCBPtr = getWCBInternal( sWASeg, i );
        initWCB( sWASeg, sWCBPtr, i );
    }

    sInitGrpInfo->mPolicy     = SDT_WA_REUSE_INMEMORY;
    sInitGrpInfo->mMapHdr     = NULL;
    sInitGrpInfo->mReuseWPID1 = 0;
    sInitGrpInfo->mReuseWPID2 = 0;

    /***************************** NPageMgr *********************************/
    IDE_TEST( sdtWAMap::create( sWASeg,
                                SDT_WAGROUPID_INIT,
                                SDT_WM_TYPE_EXTDESC,
                                smuProperty::getTempMaxPageCount()
                                / SDT_WAEXTENT_PAGECOUNT,
                                1, /* aVersionCount */
                                &sWASeg->mNExtentMap )
              != IDE_SUCCESS );
    IDE_TEST( sWASeg->mFreeNPageStack.initialize( IDU_MEM_SM_TEMP,
                                                  ID_SIZEOF( scPageID ) )
              != IDE_SUCCESS );
    sState = 3;
    /***************************** Hash Bucket ******************************/
    /* BUG-37741-�����̽�
     * NPage�� ã�� HashTable�� ũ�⸦ ������Ƽ�� ������*/
    sWASeg->mNPageHashBucketCnt =
        (sExtentCount * SDT_WAEXTENT_PAGECOUNT)/sHashBucketDensity;

    /* BUG-40608
     * mNPageHashBucketCnt�� �ּ� 1 �̻��� �ǵ��� ���� */
    if( sWASeg->mNPageHashBucketCnt < 1 )
    {
        sWASeg->mNPageHashBucketCnt = 1;
    }
    else
    {
        /* do nothing */
    }

    sHashBucketCnt = sWASeg->mNPageHashBucketCnt;

    /* sdtWASegment_createWASegment_malloc_NPageHashPtr.tc */
    IDU_FIT_POINT_RAISE("sdtWASegment::createWASegment::malloc::NPageHashPtr", insufficient_memory);
    IDE_TEST_RAISE( iduMemMgr::malloc(
                        IDU_MEM_SM_TEMP,
                        (ULong)ID_SIZEOF(sdtWCB*)
                        * sHashBucketCnt,
                        (void**)&(sWASeg->mNPageHashPtr))
                    != IDE_SUCCESS,
                    insufficient_memory );
    sState = 4;
    idlOS::memset( sWASeg->mNPageHashPtr,
                   0x00,
                   (size_t)ID_SIZEOF( sdtWCB* ) * sHashBucketCnt );

    (*aWASegment) = sWASeg;

    return IDE_SUCCESS;

    IDE_EXCEPTION( insufficient_memory );
    {
        IDE_SET(ideSetErrorCode(idERR_ABORT_InsufficientMemory));
    }
    IDE_EXCEPTION_END;

    switch( sState )
    {
        case 4:
            IDE_ASSERT( iduMemMgr::free( sWASeg->mNPageHashPtr ) == IDE_SUCCESS );
        case 3:
            sWASeg->mFreeNPageStack.destroy();
        case 2:
            sdtWorkArea::releaseWAFlusher( sWASeg->mFlushQueue );
        case 1:
            sdtWorkArea::freeWASegmentAndExtent( sWASeg );
            break;
        default:
            break;
    }

    (*aWASegment) = NULL;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * Segment�� ũ�⸦ �޴´�.
 ***************************************************************************/
UInt sdtWASegment::getWASegmentPageCount(sdtWASegment * aWASegment )
{
    return aWASegment->mWAExtentCount * SDT_WAEXTENT_PAGECOUNT;
}

/**************************************************************************
 * Description :
 * Segment�� Drop�ϰ� ���� Extent�� ��ȯ�Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWait4Flush    - Dirty�� Page���� Flush�ɶ����� ��ٸ� ���ΰ�.
 ***************************************************************************/
IDE_RC sdtWASegment::dropWASegment(sdtWASegment * aWASegment,
                                   idBool         aWait4Flush)
{
    if( aWASegment != NULL )
    {
        if( aWASegment->mNPageHashPtr != NULL )
        {
            IDE_TEST( iduMemMgr::free( aWASegment->mNPageHashPtr ) != IDE_SUCCESS );
        }
        IDE_TEST( dropAllWAGroup( aWASegment, aWait4Flush ) != IDE_SUCCESS );

        /* FlushQueue���� ���� �������� �˸�.
         * ���� sdtWASegment::flusherRun���� ��������. */
        aWASegment->mFlushQueue->mFQDone = ID_TRUE;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * WAExtent���� Segment�� �߰��Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWAExtentPtr   - �߰��� ExtentPtr
 ***************************************************************************/
IDE_RC sdtWASegment::addWAExtentPtr(sdtWASegment * aWASegment,
                                    UChar        * aWAExtentPtr )
{
    IDE_TEST( setWAExtentPtr( aWASegment,
                              aWASegment->mWAExtentCount,
                              aWAExtentPtr )
              != IDE_SUCCESS );

    aWASegment->mWAExtentCount ++;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * aIdx��° WAExtent�� ��ġ(Pointer)�� �����Ѵ�.
 * Dump��������, Dump�� File�� �ҷ������� WAExtentPtr�� �ҷ��� �޸𸮿� �°�
 * Pointer �ּҸ� �������ϱ� �����̴�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aIdx           - Extent ID
 * aWAExtentPtr   - �߰��� ExtentPtr
 ***************************************************************************/
IDE_RC sdtWASegment::setWAExtentPtr(sdtWASegment  * aWASegment,
                                    UInt            aIdx,
                                    UChar         * aWAExtentPtr )
{
    UInt     sOffset;
    UChar ** sSlotPtr;

    sOffset = ID_SIZEOF( sdtWASegment ) + ID_SIZEOF( UChar* ) * aIdx;

    /* 0�� Extent�� ���� �Ѵ�. */
    IDE_ERROR( getExtentIdx(sOffset / SD_PAGE_SIZE) == 0 );
    IDE_ERROR( ( sOffset % ID_SIZEOF( UChar *) )  == 0 );

    /* WASegment�� �ݵ�� WAExtent�� ���� ���ʿ� ��ġ�ǰ�,
     * 0�� Extent���� ��� ��ġ�Ǿ� ���Ӽ��� ������ ������
     * Ž���� �����ϴ� */
    sSlotPtr  = (UChar**)(((UChar*)aWASegment) + sOffset);
    *sSlotPtr = aWAExtentPtr;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/******************************************************************
 * Group
 ******************************************************************/
/**************************************************************************
 * Description :
 * Segment�� Group�� �����Ѵ�. �̶� InitGroup���κ��� Extent�� �����´�.
 * Size�� 0�̸�, InitGroup��üũ��� �����Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWAGroupID     - ������ Group ID
 * aPageCount     - ��� Group�� ���� Page ����
 * aPolicy        - ��� Group�� ����� FreePage ��Ȱ�� ��å
 ***************************************************************************/
IDE_RC sdtWASegment::createWAGroup( sdtWASegment     * aWASegment,
                                    sdtWAGroupID       aWAGroupID,
                                    UInt               aPageCount,
                                    sdtWAReusePolicy   aPolicy )
{
    sdtWAGroup     *sInitGrpInfo=getWAGroupInfo(aWASegment,SDT_WAGROUPID_INIT);
    sdtWAGroup     *sGrpInfo    =getWAGroupInfo(aWASegment,aWAGroupID);

    IDE_ERROR( sGrpInfo->mPolicy == SDT_WA_REUSE_NONE );

    /* ũ�⸦ �������� �ʾ�����, ���� �뷮�� ���� �ο��� */
    if( aPageCount == 0 )
    {
        aPageCount = getAllocableWAGroupPageCount( aWASegment,
                                                   SDT_WAGROUPID_INIT );
    }

    /* ������ ���� InitGroup���� ������
     * <---InitGroup---------------------->
     * <---InitGroup---><----CurGroup----->*/
    IDE_ERROR( sInitGrpInfo->mEndWPID >= aPageCount );  /* ����üũ */
    sGrpInfo->mBeginWPID = sInitGrpInfo->mEndWPID - aPageCount;
    sGrpInfo->mEndWPID   = sInitGrpInfo->mEndWPID;
    sInitGrpInfo->mEndWPID -= aPageCount;
    IDE_ERROR( sInitGrpInfo->mBeginWPID <= sInitGrpInfo->mEndWPID );

    sGrpInfo->mHintWPID       = SC_NULL_PID;
    sGrpInfo->mPolicy         = aPolicy;

    IDE_TEST( clearWAGroup( aWASegment, aWAGroupID )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * WAGroup�� ��Ȱ�� �� �� �ֵ��� �����Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWAGroupID     - ��� Group ID
 * aWait4Flush    - Dirty�� Page���� Flush�ɶ����� ��ٸ� ���ΰ�.
 ***************************************************************************/
IDE_RC sdtWASegment::resetWAGroup( sdtWASegment      * aWASegment,
                                   sdtWAGroupID        aWAGroupID,
                                   idBool              aWait4Flush )
{
    sdtWAGroup     * sGrpInfo = getWAGroupInfo(aWASegment,aWAGroupID);
    UInt             i;

    if( sGrpInfo->mPolicy != SDT_WA_REUSE_NONE )
    {
        /* ���⼭ HintPage�� �ʱ�ȭ�Ͽ� Unfix���� ������,
         * ���� makeInit�ܰ迡�� hintPage�� unassign�� �� ����
         * ������ �߻��Ѵ�. */
        IDE_TEST( setHintWPID( aWASegment, aWAGroupID, SC_NULL_PID )
                  != IDE_SUCCESS );

        if( aWait4Flush == ID_TRUE )
        {
            IDE_TEST( writeDirtyWAPages( aWASegment,
                                         aWAGroupID )
                      != IDE_SUCCESS );
            for( i = sGrpInfo->mBeginWPID ; i < sGrpInfo->mEndWPID ; i ++ )
            {
                /* ���� Init���·� �����. */
                IDE_TEST( makeInitPage( aWASegment,
                                        i,
                                        aWait4Flush )
                          != IDE_SUCCESS );
            }
        }
        IDE_TEST( clearWAGroup( aWASegment, aWAGroupID ) != IDE_SUCCESS );
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * WAGroup�� ������ �ʱ�ȭ�Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWAGroupID     - ��� Group ID
 ***************************************************************************/
IDE_RC sdtWASegment::clearWAGroup( sdtWASegment      * aWASegment,
                                   sdtWAGroupID        aWAGroupID )
{
    sdtWAGroup     * sGrpInfo = getWAGroupInfo(aWASegment,aWAGroupID);

    sGrpInfo->mMapHdr         = NULL;
    IDE_TEST( setHintWPID( aWASegment, aWAGroupID, SC_NULL_PID )
              != IDE_SUCCESS );

    /* ��Ȱ�� ��å�� ���� �ʱⰪ ������. */
    switch( sGrpInfo->mPolicy  )
    {
        case SDT_WA_REUSE_INMEMORY:
            initInMemoryGroup( sGrpInfo );
            break;
        case SDT_WA_REUSE_FIFO:
            initFIFOGroup( sGrpInfo );
            break;
        case SDT_WA_REUSE_LRU:
            initLRUGroup( aWASegment, sGrpInfo );
            break;
        case SDT_WA_REUSE_NONE:
        default:
            IDE_ERROR( 0 );
            break;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * InMemoryGroup�� ���� �ʱ�ȭ. ������ ��Ȱ�� �ڷᱸ���� �����Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aGrpInfo       - ��� Group
 ***************************************************************************/
void sdtWASegment::initInMemoryGroup( sdtWAGroup     * aGrpInfo )
{
    /* �ڿ��� ������ ���� ������ Ȱ����.
     * �ֳ��ϸ� InMemoryGroup�� WAMap�� ������ �� ������,
     * WAMap�� �տ��� �������� Ȯ���ϸ鼭 �ε��� �� �ֱ� ������ */
    aGrpInfo->mReuseWPID1 = aGrpInfo->mEndWPID - 1;
    aGrpInfo->mReuseWPID2 = SD_NULL_PID;
}

/**************************************************************************
 * Description :
 * FIFOGroup�� ���� �ʱ�ȭ. ������ ��Ȱ�� �ڷᱸ���� �����Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aGrpInfo       - ��� Group
 ***************************************************************************/
void sdtWASegment::initFIFOGroup( sdtWAGroup     * aGrpInfo )
{
    /* �տ��� �ڷ� ���� ������ Ȱ����. MultiBlockWrite�� �����ϱ�����. */
    aGrpInfo->mReuseWPID1 = aGrpInfo->mBeginWPID;
    aGrpInfo->mReuseWPID2 = SD_NULL_PID;
}

/**************************************************************************
 * Description :
 * LRUGroup�� ���� �ʱ�ȭ. ������ ��Ȱ�� �ڷᱸ���� �����Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aGrpInfo       - ��� Group
 ***************************************************************************/
void sdtWASegment::initLRUGroup( sdtWASegment   * aWASegment,
                                 sdtWAGroup      * aGrpInfo )
{
    sdtWCB         * sWCBPtr;
    UInt             i;

    aGrpInfo->mReuseWPID1 = aGrpInfo->mEndWPID - 1;
    aGrpInfo->mReuseWPID2 = aGrpInfo->mBeginWPID;

    /* MultiBlockWrite�� �����ϱ����� �տ��� �ڷ� ���� ������ Ȱ���ϵ���
     * �ϴ� ����Ʈ�� �����ص�*/
    for( i = aGrpInfo->mBeginWPID ;
         i < aGrpInfo->mEndWPID ;
         i ++ )
    {
        sWCBPtr = getWCB( aWASegment, i );

        sWCBPtr->mLRUNextPID = i - 1;
        sWCBPtr->mLRUPrevPID = i + 1;
    }
    /* ù/�� page�� Link�� �������� */
    sWCBPtr              = getWCB( aWASegment, aGrpInfo->mBeginWPID );
    sWCBPtr->mLRUNextPID = SC_NULL_PID;
    sWCBPtr              = getWCB( aWASegment, aGrpInfo->mEndWPID - 1);
    sWCBPtr->mLRUPrevPID = SC_NULL_PID;
}

/**************************************************************************
 * Description :
 * WCB�� �ʱ�ȭ�Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWCBPtr        - ��� WCBPtr
 * aWPID          - �ش� WCB�� ���� WPAGEID
 ***************************************************************************/
void sdtWASegment::initWCB( sdtWASegment * aWASegment,
                            sdtWCB       * aWCBPtr,
                            scPageID       aWPID )
{
    (void)idCore::acpAtomicSet32( &aWCBPtr->mWPState, SDT_WA_PAGESTATE_NONE );
    aWCBPtr->mNSpaceID     = SC_NULL_SPACEID;
    aWCBPtr->mNPageID      = SC_NULL_PID;
    aWCBPtr->mFix          = 0;
    aWCBPtr->mBookedFree   = ID_FALSE;
    aWCBPtr->mWPageID      = aWPID;
    aWCBPtr->mNextWCB4Hash = NULL;

    bindWCB( aWASegment, aWCBPtr, aWPID );
}

/**************************************************************************
 * Description :
 * WCB�� Frame�� ������
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWCBPtr        - ��� WCBPtr
 * aWPID          - �ش� WCB�� ���� WPAGEID
 ***************************************************************************/
void sdtWASegment::bindWCB( sdtWASegment * aWASegment,
                            sdtWCB       * aWCBPtr,
                            scPageID       aWPID )
{
    UChar        * sWAExtentPtr;

    sWAExtentPtr = getWAExtentPtr( aWASegment, getExtentIdx(aWPID) );

    aWCBPtr->mWAPagePtr =
        getFrameInExtent( sWAExtentPtr, getPageSeqInExtent(aWPID) );

    /* �����غ� */
    IDE_DASSERT( getWCBInExtent( sWAExtentPtr, getPageSeqInExtent(aWPID) )
                 == aWCBPtr );
}

/**************************************************************************
 * Description :
 * Group�� ũ�⸦ Page������ �޴´�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWAGroupID     - ��� Group ID
 ***************************************************************************/
UInt sdtWASegment::getWAGroupPageCount( sdtWASegment * aWASegment,
                                        sdtWAGroupID   aWAGroupID )
{
    sdtWAGroup     * sGrpInfo = getWAGroupInfo( aWASegment, aWAGroupID );

    return sGrpInfo->mEndWPID - sGrpInfo->mBeginWPID;
}

/**************************************************************************
 * Description :
 * Map�� �����ϴ� ��ġ�� ��, ��� ������ Group�� ũ�⸦ Page������ �޴´�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWAGroupID     - ��� Group ID
 ***************************************************************************/
UInt sdtWASegment::getAllocableWAGroupPageCount( sdtWASegment * aWASegment,
                                                 sdtWAGroupID   aWAGroupID )
{
    sdtWAGroup     * sGrpInfo = getWAGroupInfo( aWASegment, aWAGroupID );

    return sGrpInfo->mEndWPID - sGrpInfo->mBeginWPID
        - sdtWAMap::getWPageCount( sGrpInfo->mMapHdr );
}


/**************************************************************************
 * Description :
 * Group�� ù PageID
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWAGroupID     - ��� Group ID
 ***************************************************************************/
scPageID sdtWASegment::getFirstWPageInWAGroup( sdtWASegment * aWASegment,
                                               sdtWAGroupID   aWAGroupID )
{
    return aWASegment->mGroup[ aWAGroupID ].mBeginWPID;
}
/**************************************************************************
 * Description :
 * Group�� ������ PageID
 * ���⼭ Last�� '��¥'������ ������ +1 �̴�.
 * (10�� ���, 0,1,2,3,4,5,6,7,8,9�� �ش� Group�� �����ߴٴ� �ǹ� )
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWAGroupID     - ��� Group ID
 ***************************************************************************/
scPageID sdtWASegment::getLastWPageInWAGroup( sdtWASegment * aWASegment,
                                              sdtWAGroupID   aWAGroupID )
{
    return aWASegment->mGroup[ aWAGroupID ].mEndWPID;
}

/**************************************************************************
 * Description :
 * �� Group�� �ϳ��� (aWAGroupID1 ������) ��ģ��.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWAGroupID1    - ��� GroupID ( �������� ������ )
 * aWAGroupID2    - ��� GroupID ( �Ҹ�� )
 * aPolicy        - ������ group�� ������ ��å
 ***************************************************************************/
IDE_RC sdtWASegment::mergeWAGroup(sdtWASegment     * aWASegment,
                                  sdtWAGroupID       aWAGroupID1,
                                  sdtWAGroupID       aWAGroupID2,
                                  sdtWAReusePolicy   aPolicy )
{
    sdtWAGroup     * sGrpInfo1 = getWAGroupInfo( aWASegment, aWAGroupID1 );
    sdtWAGroup     * sGrpInfo2 = getWAGroupInfo( aWASegment, aWAGroupID2 );

    IDE_ERROR( sGrpInfo1->mPolicy != SDT_WA_REUSE_NONE );
    IDE_ERROR( sGrpInfo2->mPolicy != SDT_WA_REUSE_NONE );

    /* ������ ���� ���¿��� ��.
     * <----Group2-----><---Group1--->*/
    IDE_ERROR( sGrpInfo2->mEndWPID == sGrpInfo1->mBeginWPID );

    /* <----Group2----->
     *                  <---Group1--->
     *
     * ���� ������ ���� ������.
     *
     * <----Group2----->
     * <--------------------Group1---> */
    sGrpInfo1->mBeginWPID = sGrpInfo2->mBeginWPID;

    /* Policy �缳�� */
    sGrpInfo1->mPolicy = aPolicy;
    /* �ι�° Group�� �ʱ�ȭ��Ŵ */
    sGrpInfo2->mPolicy = SDT_WA_REUSE_NONE;

    IDE_TEST( clearWAGroup( aWASegment, aWAGroupID1) != IDE_SUCCESS );
    IDE_DASSERT( validateLRUList( aWASegment, aWAGroupID1 ) == IDE_SUCCESS);

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * �� Group�� �� Group���� ������. ( ũ��� ���� )
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aSrcWAGroupID  - �ɰ� ���� GroupID
 * aDstWAGroupID  - �ɰ����� ������� Group
 * aPolicy        - �� group�� ������ ��å
 ***************************************************************************/
IDE_RC sdtWASegment::splitWAGroup(sdtWASegment     * aWASegment,
                                  sdtWAGroupID       aSrcWAGroupID,
                                  sdtWAGroupID       aDstWAGroupID,
                                  sdtWAReusePolicy   aPolicy )
{
    sdtWAGroup     * sSrcGrpInfo= getWAGroupInfo( aWASegment, aSrcWAGroupID );
    sdtWAGroup     * sDstGrpInfo= getWAGroupInfo( aWASegment, aDstWAGroupID );
    UInt             sPageCount;

    IDE_ERROR( sDstGrpInfo->mPolicy == SDT_WA_REUSE_NONE );

    sPageCount = getAllocableWAGroupPageCount( aWASegment, aSrcWAGroupID )/2;

    IDE_ERROR( sPageCount >= 1 );


    /* ������ ���� Split��
     * <------------SrcGroup------------>
     * <---DstGroup----><----SrcGroup--->*/
    IDE_ERROR( sSrcGrpInfo->mEndWPID >= sPageCount );
    sDstGrpInfo->mBeginWPID = sSrcGrpInfo->mBeginWPID;
    sDstGrpInfo->mEndWPID   = sSrcGrpInfo->mBeginWPID + sPageCount;
    sSrcGrpInfo->mBeginWPID+= sPageCount;

    IDE_ERROR( sSrcGrpInfo->mBeginWPID < sSrcGrpInfo->mEndWPID );
    IDE_ERROR( sDstGrpInfo->mBeginWPID < sDstGrpInfo->mEndWPID );

    /* �ι�° Group�� ���� ������ */
    sDstGrpInfo->mHintWPID       = SC_NULL_PID;
    sDstGrpInfo->mPolicy         = aPolicy;

    IDE_TEST( clearWAGroup( aWASegment, aSrcWAGroupID) != IDE_SUCCESS );
    IDE_TEST( clearWAGroup( aWASegment, aDstWAGroupID) != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * ��� Group�� Drop�Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWait4Flush    - Dirty�� Page���� Flush�ɶ����� ��ٸ� ���ΰ�.
 ***************************************************************************/
IDE_RC sdtWASegment::dropAllWAGroup( sdtWASegment * aWASegment,
                                     idBool         aWait4Flush )
{
    SInt             i;

    for( i = SDT_WAGROUPID_MAX - 1 ; i >  SDT_WAGROUPID_INIT ; i -- )
    {
        if( aWASegment->mGroup[ i ].mPolicy != SDT_WA_REUSE_NONE )
        {
            IDE_TEST( dropWAGroup( aWASegment, i, aWait4Flush )
                      != IDE_SUCCESS );
        }
        aWASegment->mGroup[ i ].mPolicy = SDT_WA_REUSE_NONE;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * Group�� Drop�ϰ� InitGroup�� Extent�� ��ȯ�Ѵ�. ������
 * dropWASegment�ҰŶ�� ���� �� �ʿ� ����.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWAGroupID     - ��� Group ID
 * aWait4Flush    - Dirty�� Page���� Flush�ɶ����� ��ٸ� ���ΰ�.
 ***************************************************************************/
IDE_RC sdtWASegment::dropWAGroup(sdtWASegment * aWASegment,
                                 sdtWAGroupID   aWAGroupID,
                                 idBool         aWait4Flush)
{
    sdtWAGroup     * sInitGrpInfo =
        getWAGroupInfo( aWASegment, SDT_WAGROUPID_INIT );
    sdtWAGroup     * sGrpInfo = getWAGroupInfo( aWASegment, aWAGroupID );

    IDE_TEST( resetWAGroup( aWASegment, aWAGroupID, aWait4Flush )
              != IDE_SUCCESS );

    /* ���� �ֱٿ� �Ҵ��� Group�̾����.
     * �� ������ ���� ���¿��� ��.
     * <---InitGroup---><----CurGroup----->*/
    IDE_ERROR( sInitGrpInfo->mEndWPID == sGrpInfo->mBeginWPID );

    /* �Ʒ� ������ ���� ������ ���� ����. ������ ������ Group��
     * ���õ� ���̱⿡ ��� ����.
     *                  <----CurGroup----->
     * <----------------------InitGroup---> */
    sInitGrpInfo->mEndWPID = sGrpInfo->mEndWPID;
    sGrpInfo->mPolicy      = SDT_WA_REUSE_NONE;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * Group�� ��å�� ���� ������ �� �ִ� WAPage�� ��ȯ�Ѵ�. Reuse�� True��
 * ��Ȱ�뵵 ���ȴ�. �����ϸ� aRetPID�� 0�� Return�ȴ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWAGroupID     - ��� Group ID
 * <OUT>
 * aRetPID        - Ž���� FreeWAPID
 ***************************************************************************/
IDE_RC sdtWASegment::getFreeWAPage( sdtWASegment * aWASegment,
                                    sdtWAGroupID   aWAGroupID,
                                    scPageID     * aRetPID )
{
    sdtWAGroup     * sGrpInfo;
    scPageID         sRetPID;
    UInt             sRetryCount = 0;
    UInt             sRetryLimit;

    IDE_ERROR( aWAGroupID != SDT_WAGROUPID_NONE );

    sGrpInfo    = getWAGroupInfo( aWASegment, aWAGroupID );
    sRetryLimit = getAllocableWAGroupPageCount( aWASegment, aWAGroupID );

    while( 1 )
    {
        switch( sGrpInfo->mPolicy )
        {
            case SDT_WA_REUSE_INMEMORY:
                IDE_ERROR( sGrpInfo->mMapHdr != NULL );

                sRetPID = sGrpInfo->mReuseWPID1;
                sGrpInfo->mReuseWPID1--;

                /* Map�� �ִ� ������ ������, �� ��ٴ� �̾߱� */
                if( sdtWAMap::getEndWPID( sGrpInfo->mMapHdr ) >= sRetPID )
                {
                    /* InMemoryGroup�� victimReplace�� �ȵȴ�.
                     * �ʱ�ȭ �ϰ�, NULL�� ��ȯ�Ѵ�.*/
                    sGrpInfo->mReuseWPID1 = sGrpInfo->mEndWPID - 1;
                    sRetPID = SD_NULL_PID;
                }
                break;

            case SDT_WA_REUSE_FIFO:
                IDE_ERROR( sGrpInfo->mMapHdr == NULL );

                sRetPID = sGrpInfo->mReuseWPID1;
                sGrpInfo->mReuseWPID1++;

                /* Group�� �ѹ� ���� ��ȸ�� */
                if( sGrpInfo->mReuseWPID1 == sGrpInfo->mEndWPID )
                {
                    sGrpInfo->mReuseWPID1 = sGrpInfo->mBeginWPID;
                }
                break;
            case SDT_WA_REUSE_LRU:
                IDE_ERROR( sGrpInfo->mMapHdr == NULL );

                sRetPID = sGrpInfo->mReuseWPID2;
                IDE_TEST( moveLRUListToTop( aWASegment,
                                            aWAGroupID,
                                            sRetPID )
                          != IDE_SUCCESS );
                break;
            default:
                IDE_ERROR( 0 );
                break;
        }

        if( ( sRetPID != SD_NULL_PID ) &&
            ( isFixedPage( aWASegment, sRetPID ) == ID_FALSE ) )
        {
            /* ����� ����� */
            break;
        }
        else
        {
            /* ����� �������� ����*/
            if( sGrpInfo->mPolicy == SDT_WA_REUSE_INMEMORY )
            {
                /* InMemory�� �� ���, ������ ȣ���ڰ� ó���������.
                 * �������� ����̴� ��ȯ�� */
                break;
            }
        }

        IDE_ERROR( sRetryCount < sRetryLimit );
        sRetryCount ++;
    }

    /* �Ҵ���� �������� �� �������� ���� */
    if( sRetPID != SD_NULL_PID )
    {
        IDE_TEST( makeInitPage( aWASegment,
                                sRetPID,
                                ID_TRUE ) /*wait 4 flush */
                  != IDE_SUCCESS );
    }
    (*aRetPID) = sRetPID;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * Segment�� DirtyFlag�� ������ WAPage���� FlushQueue�� ����Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWAGroupID     - ��� Group ID
 ***************************************************************************/
IDE_RC sdtWASegment::writeDirtyWAPages(sdtWASegment * aWASegment,
                                       sdtWAGroupID   aWAGroupID )
{
    sdtWAGroup     * sGrpInfo = getWAGroupInfo( aWASegment, aWAGroupID );
    UInt             i;

    for( i = sGrpInfo->mBeginWPID ; i < sGrpInfo->mEndWPID ; i ++ )
    {
        if( getWAPageState( aWASegment, i ) == SDT_WA_PAGESTATE_DIRTY )
        {
            IDE_TEST( writeNPage( aWASegment, i ) != IDE_SUCCESS );
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * ��� Page�� �ʱ�ȭ�Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWPID          - ��� WPID
 * aWait4Flush    - Dirty�� Page���� Flush�ɶ����� ��ٸ� ���ΰ�.
 ***************************************************************************/
IDE_RC sdtWASegment::makeInitPage( sdtWASegment * aWASegment,
                                   scPageID       aWPID,
                                   idBool         aWait4Flush)
{
    sdtWAPageState   sWAPageState;
    sdtWAFlusher   * sWAFlusher;
    sdtWCB         * sWCBPtr;

    /* Page�� �Ҵ�޾�����, �ش� Page�� ���� Write���� �ʾ��� �� �ִ�.
     * �׷��� ������ Write�� �õ��Ѵ�. */
    while( isFreeWAPage( aWASegment, aWPID ) == ID_FALSE )
    {
        sWAPageState = getWAPageState( aWASegment, aWPID );
        if( ( sWAPageState == SDT_WA_PAGESTATE_IN_FLUSHQUEUE ) &&
            ( sdtWorkArea::isEmptyQueue( aWASegment->mFlushQueue ) == ID_TRUE ) )
        {
            /* retry */
            sWAPageState = getWAPageState( aWASegment, aWPID );
            if( sWAPageState == SDT_WA_PAGESTATE_IN_FLUSHQUEUE )
            {
                ideLog::log( IDE_SM_0,
                             "WPID : %u\n",
                             aWPID );

                smuUtility::dumpFuncWithBuffer( IDE_SM_0,
                                                dumpFlushQueue,
                                                (void*)aWASegment->mFlushQueue );
                IDE_ERROR( 0 );
            }
        }

        if( aWait4Flush == ID_TRUE )
        {
            switch( sWAPageState )
            {
                case SDT_WA_PAGESTATE_DIRTY:
                    /* Dirty �����̸�, ���� Flusher�� ������� ������
                     * ������ write�ص� ���� ����. */
                    IDE_TEST( writeNPage( aWASegment, aWPID )
                              != IDE_SUCCESS );
                    break;
                case SDT_WA_PAGESTATE_IN_FLUSHQUEUE:
                case SDT_WA_PAGESTATE_WRITING:
                    /* Flusher�� Write�Ϸ� �ϴ� ��Ȳ�̱⿡ ����ϸ� �ȴ�. */
                    break;
                default:
                    /* ServiceThrea�� ���¸� ������ ���� Flusher�� Flush�Ͽ�
                     * Clean���� ����� �������� ���� ��.
                     * �ƹ��͵� ���ϰ� ���� Loop�� ���� �� */
                    break;
            }
        }
        else
        {
            switch( sWAPageState )
            {
                case SDT_WA_PAGESTATE_DIRTY:
                    /* Dirty �����̸�, ���� Flusher�� ������� ������
                     * ��� ���¸� �����ϸ� �ȴ�. */
                    IDE_TEST( setWAPageState( aWASegment,
                                              aWPID,
                                              SDT_WA_PAGESTATE_CLEAN )
                              != IDE_SUCCESS );
                    break;
                case SDT_WA_PAGESTATE_IN_FLUSHQUEUE:
                case SDT_WA_PAGESTATE_WRITING:
                    /* Flusher�� ���ü� �̽��� �ִ� ����.
                     * FlushQueue������ ���, Clean���� ������ ��
                     * ���� loop���� �ٽ� üũ��*/
                    sWCBPtr = getWCB( aWASegment, aWPID );
                    checkAndSetWAPageState( sWCBPtr,
                                            SDT_WA_PAGESTATE_IN_FLUSHQUEUE,
                                            SDT_WA_PAGESTATE_CLEAN,
                                            &sWAPageState );
                    break;
                default:
                    /* ServiceThrea�� ���¸� ������ ���� Flusher�� Flush�Ͽ�
                     * Clean���� ����� �������� ���� ��.
                     * �ƹ��͵� ���ϰ� ���� Loop�� ���� �� */
                    break;
            }
        }

        /* sleep�ߴٰ� �ٽ� �õ��� */
        (*aWASegment->mStatsPtr)->mWriteWaitCount ++;
        idlOS::thr_yield();

        /* Flusher�� �������̾�� �� */
        sWAFlusher = sdtWorkArea::getWAFlusher( aWASegment->mFlushQueue->mWAFlusherIdx );
        IDE_ERROR( sWAFlusher->mRun == ID_TRUE );
    }

    /* Assign�Ǿ������� Assign �� �������� �ʱ�ȭ�����ش�. */
    if( getWAPageState( aWASegment, aWPID ) == SDT_WA_PAGESTATE_CLEAN )
    {
        aWASegment->mDiscardPage = ID_TRUE;
        IDE_TEST( unassignNPage( aWASegment,
                                 aWPID )
                  != IDE_SUCCESS );
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * WCB�� �������� Page�� Fix���θ� ��ȯ�Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWPID          - ��� WPID
 ***************************************************************************/
idBool sdtWASegment::isFixedPage(sdtWASegment * aWASegment,
                                 scPageID       aWPID )
{
    sdtWCB         * sWCBPtr;

    sWCBPtr = getWCB( aWASegment, aWPID );

    return (sWCBPtr->mFix > 0) ? ID_TRUE : ID_FALSE;
}

/**************************************************************************
 * Description :
 * WCB�� �������� Page�� NPID�� ��ȯ�Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWPID          - ��� WPID
 ***************************************************************************/
scPageID       sdtWASegment::getNPID( sdtWASegment * aWASegment,
                                      scPageID       aWPID )
{
    sdtWCB         * sWCBPtr;

    sWCBPtr = getWCB( aWASegment, aWPID );

    return sWCBPtr->mNPageID;
}

/**************************************************************************
 * Description :
 * WCB�� �ش� SpaceID �� PageID�� �����Ѵ�. �� Page�� �д� ���� �ƴ϶�
 * ������ �� ������, Disk�� �ִ� �Ϲ�Page�� ������ '����'�ȴ�.

 * <IN>
 * aWASegment     - ��� WASegment
 * aWPID          - ��� WPID
 * aNSpaceID,aNPID- ��� NPage�� �ּ�
 ***************************************************************************/
IDE_RC sdtWASegment::assignNPage(sdtWASegment * aWASegment,
                                 scPageID       aWPID,
                                 scSpaceID      aNSpaceID,
                                 scPageID       aNPageID)
{
    UInt             sHashValue;
    sdtWCB         * sWCBPtr;

    /* ����. �̹� Hash�� �Ŵ޸� ���� �ƴϾ�� �� */
    IDE_DASSERT( findWCB( aWASegment, aNSpaceID, aNPageID )
                 == NULL );
    /* ����ε� WPID���� �� */
    IDE_DASSERT( aWPID < getWASegmentPageCount( aWASegment ) );

    sWCBPtr = getWCB( aWASegment, aWPID );
    /* NPID �� ������ */
    IDE_ERROR( sWCBPtr->mNSpaceID == SC_NULL_SPACEID );
    IDE_ERROR( sWCBPtr->mNPageID  == SC_NULL_PID     );
    sWCBPtr->mNSpaceID = aNSpaceID;
    sWCBPtr->mNPageID  = aNPageID;
    IDE_TEST( setWAPageState( aWASegment, aWPID, SDT_WA_PAGESTATE_CLEAN )
              != IDE_SUCCESS );

    /* Hash�� �����ϱ� */
    sHashValue = getNPageHashValue( aWASegment, aNSpaceID, aNPageID );
    sWCBPtr->mNextWCB4Hash = aWASegment->mNPageHashPtr[sHashValue];
    aWASegment->mNPageHashPtr[sHashValue] = sWCBPtr;

    /* Hash�� �Ŵ޷���, Ž���Ǿ�� �� */
    IDE_DASSERT( findWCB( aWASegment, aNSpaceID, aNPageID )
                 == sWCBPtr );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * NPage�� ����. �� WPage�� �÷��� �ִ� ���� ������.
 * �ϴ� ���� ���� �ʱ�ȭ�ϰ� Hash���� �����ϴ� �� �ۿ� ���Ѵ�.

 * <IN>
 * aWASegment     - ��� WASegment
 * aWPID          - ��� WPID
 ***************************************************************************/
IDE_RC sdtWASegment::unassignNPage( sdtWASegment * aWASegment,
                                    scPageID       aWPID )
{
    UInt             sHashValue = 0;
    scSpaceID        sTargetTBSID;
    scPageID         sTargetNPID;
    sdtWCB        ** sWCBPtrPtr;
    sdtWCB         * sWCBPtr;

    sWCBPtr = getWCB( aWASegment, aWPID );

    /* Fix�Ǿ����� �ʾƾ� �� */
    IDE_ERROR( sWCBPtr->mFix == 0 );
    IDE_ERROR( ( sWCBPtr->mNSpaceID != SC_NULL_SPACEID ) &&
               ( sWCBPtr->mNPageID  != SC_NULL_PID ) );

    /* CleanPage���� �� */
    IDE_ERROR( getWAPageState( aWASegment, aWPID ) == SDT_WA_PAGESTATE_CLEAN );

    /************************** Hash���� ����� *********************/
    sTargetTBSID = sWCBPtr->mNSpaceID;
    sTargetNPID  = sWCBPtr->mNPageID;
    /* �Ŵ޷� �־�� �ϸ�, ���� Ž���� �� �־�� �� */
    IDE_DASSERT( findWCB( aWASegment, sTargetTBSID, sTargetNPID ) == sWCBPtr );
    sHashValue   = getNPageHashValue( aWASegment, sTargetTBSID, sTargetNPID );

    sWCBPtrPtr   = &aWASegment->mNPageHashPtr[ sHashValue ];

    sWCBPtr      = *sWCBPtrPtr;
    while( sWCBPtr != NULL )
    {
        if( sWCBPtr->mNPageID == sTargetNPID )
        {
            IDE_ERROR( sWCBPtr->mNSpaceID == sTargetTBSID );
            /* ã�Ҵ�, �ڽ��� �ڸ��� ���� ���� �Ŵ� */
            (*sWCBPtrPtr) = sWCBPtr->mNextWCB4Hash;
            break;
        }

        sWCBPtrPtr = &sWCBPtr->mNextWCB4Hash;
        sWCBPtr    = *sWCBPtrPtr;
    }

    /* ��������� �� */
    IDE_DASSERT( findWCB( aWASegment, sTargetTBSID, sTargetNPID ) == NULL );

    /***** Free�� �����Ǿ�����, unassign���� �������� ������ Free���ش�. ****/
    if( sWCBPtr->mBookedFree == ID_TRUE )
    {
        IDE_TEST( pushFreePage( aWASegment,
                                sWCBPtr->mNPageID )
                  != IDE_SUCCESS );
    }

    initWCB( aWASegment, sWCBPtr, aWPID );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    ideLog::log( IDE_DUMP_0,
                 "HASHVALUE : %u\n"
                 "WPID      : %u\n",
                 sHashValue,
                 aWPID );

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * WPage�� �ٸ� ������ �ű��. npage, npagehash���� ����ؾ� �Ѵ�.

 * <IN>
 * aWASegment     - ��� WASegment
 * aSrcWAGroupID  - ���� Group ID
 * aSrcWPID       - ���� WPID
 * aDstWAGroupID  - ��� Group ID
 * aDstWPID       - ��� WPID
 ***************************************************************************/
IDE_RC sdtWASegment::movePage( sdtWASegment * aWASegment,
                               sdtWAGroupID   aSrcGroupID,
                               scPageID       aSrcWPID,
                               sdtWAGroupID   aDstGroupID,
                               scPageID       aDstWPID )
{
    UChar        * sSrcWAPagePtr;
    UChar        * sDstWAPagePtr;
    sdtWCB       * sWCBPtr;
    scSpaceID      sNSpaceID;
    scPageID       sNPageID;

    if( aSrcGroupID != aDstGroupID )
    {
        sSrcWAPagePtr = getWAPagePtr( aWASegment, aSrcGroupID, aSrcWPID );
        sDstWAPagePtr = getWAPagePtr( aWASegment, aDstGroupID, aDstWPID );

        /* PageCopy */
        idlOS::memcpy( sDstWAPagePtr,sSrcWAPagePtr,SD_PAGE_SIZE );

        sWCBPtr = getWCB( aWASegment, aSrcWPID );
        sNSpaceID = sWCBPtr->mNSpaceID;
        sNPageID  = sWCBPtr->mNPageID;

        /* ���� �������� �� �������� �����. */
        IDE_TEST( makeInitPage( aWASegment,
                                aSrcWPID,
                                ID_TRUE ) /* wait4flush */
                  != IDE_SUCCESS );

        if( ( sNSpaceID != SC_NULL_SPACEID ) && ( sNPageID != SC_NULL_PID ) )
        {
            /* Assign�� ������������, Assign ��Ų��. */
            IDE_TEST( assignNPage( aWASegment, aDstWPID, sNSpaceID, sNPageID )
                      != IDE_SUCCESS );
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

/**************************************************************************
 * Description :
 * bindPage�ϰ� Disk���� �Ϲ�Page�κ��� ������ �о� �ø���.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWAGroupID     - ��� Group ID
 * aWPID          - �о�帱 WPID
 * aNSpaceID,aNPID- ��� NPage�� �ּ�
 ***************************************************************************/
IDE_RC sdtWASegment::readNPage(sdtWASegment * aWASegment,
                               sdtWAGroupID   aWAGroupID,
                               scPageID       aWPID,
                               scSpaceID      aNSpaceID,
                               scPageID       aNPageID)
{
    UChar          * sWAPagePtr;
    smLSN            sOnlineTBSLSN4Idx;
    UInt             sDummy;
    sdtWCB         * sOldWCB;
    sdtWAGroupID     sOldWAGroupID;

    sOldWCB = findWCB( aWASegment, aNSpaceID, aNPageID );
    if( sOldWCB == NULL )
    {
        /* ������ ���� �������� Read�ؼ� �ø� */
        IDE_TEST( assignNPage( aWASegment,
                               aWPID,
                               aNSpaceID,
                               aNPageID )
                  != IDE_SUCCESS );

        sWAPagePtr = getWAPagePtr( aWASegment, aWAGroupID, aWPID );

        SM_LSN_INIT( sOnlineTBSLSN4Idx );
        IDE_TEST( sddDiskMgr::read( aWASegment->mStatistics,
                                    aNSpaceID,
                                    aNPageID,
                                    sWAPagePtr,
                                    &sDummy, /* datafile id */
                                    &sOnlineTBSLSN4Idx )
                  != IDE_SUCCESS );
        IDU_FIT_POINT( "BUG-45263@sdtWASegment::readNPage::iduFileopen" );
        (*aWASegment->mStatsPtr)->mReadCount ++;
    }
    else
    {
        /* ������ �ִ� ��������, movePage�� �ű��. */
        sOldWAGroupID = findGroup( aWASegment, sOldWCB->mWPageID );
        IDE_TEST( movePage( aWASegment,
                            sOldWAGroupID,
                            sOldWCB->mWPageID,
                            aWAGroupID,
                            aWPID )
                  != IDE_SUCCESS );
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * �ش� WAPage�� FlushQueue�� ����ϰų� ���� Write�Ѵ�.
 * �ݵ�� bindPage,readPage������ SpaceID,PageID�� ������ WAPage���� �Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWPID          - �о�帱 WPID
 ***************************************************************************/
IDE_RC sdtWASegment::writeNPage(sdtWASegment * aWASegment,
                                scPageID       aWPID )
{
    // FlushPageQueue�� �о�ִ´�.
    IDE_TEST( setWAPageState( aWASegment,
                              aWPID,
                              SDT_WA_PAGESTATE_IN_FLUSHQUEUE )
              != IDE_SUCCESS );
    IDE_TEST( sdtWorkArea::pushJob( aWASegment, aWPID ) != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * �� Page�� Stack�� �����Ѵ�. ���߿� allocAndAssignNPage ���� ��Ȱ���Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWPID          - �о�帱 WPID
 ***************************************************************************/
IDE_RC sdtWASegment::pushFreePage(sdtWASegment * aWASegment,
                                  scPageID       aNPID)
{
    IDE_TEST( aWASegment->mFreeNPageStack.push(ID_FALSE, /* lock */
                                               (void*) &aNPID )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * NPage�� �Ҵ�ް� �����Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aTargetPID     - ������ WPID
 ***************************************************************************/
IDE_RC sdtWASegment::allocAndAssignNPage(sdtWASegment * aWASegment,
                                         scPageID       aTargetPID )
{
    idBool         sIsEmpty;
    scPageID       sFreeNPID;

    IDE_TEST( aWASegment->mFreeNPageStack.pop( ID_FALSE, /* lock */
                                               (void*) &sFreeNPID,
                                               &sIsEmpty )
              != IDE_SUCCESS );
    if( sIsEmpty == ID_TRUE )
    {
        if( aWASegment->mLastFreeExtent == NULL )
        {
            /* ���� Alloc �õ� */
            IDE_TEST( allocFreeNExtent( aWASegment ) != IDE_SUCCESS );
        }
        else
        {
            if( aWASegment->mPageSeqInLFE ==
                ((sdpExtDesc*)aWASegment->mLastFreeExtent)->mLength )
            {
                /* ������ NExtent�� �ٽ��� */
                IDE_TEST( allocFreeNExtent( aWASegment ) != IDE_SUCCESS );
            }
            else
            {
                /* ������ �Ҵ��ص� Extent���� ������ */
            }
        }

/* BUG-37503
   allocFreeNExtent()����  slot �����͸� ������ ������ IDE_TEST���� �����ϸ�
   mLastFreeExten�� �߸������ɼ� �ִ�. */
        IDE_TEST_RAISE( aWASegment->mPageSeqInLFE >
                        ((sdpExtDesc*)aWASegment->mLastFreeExtent)->mLength,
                        ERROR_INVALID_EXTENT );

        sFreeNPID = ((sdpExtDesc*)aWASegment->mLastFreeExtent)->mExtFstPID
            + aWASegment->mPageSeqInLFE;
        aWASegment->mPageSeqInLFE ++;
        aWASegment->mNPageCount ++;
    }
    else
    {
        /* Stack�� ���� ��Ȱ������ */
    }

    IDE_TEST( assignNPage( aWASegment,
                           aTargetPID,
                           aWASegment->mSpaceID,
                           sFreeNPID )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERROR_INVALID_EXTENT );
    {
        ideLog::log( IDE_SM_0,
                     "LFE->mLength : %d \n"
                     "LFE->mExtFstPID : %d \n"
                     "WASegment->mPageSeqInLastFreeExtent : %d \n"
                     "WASegment->mNExtentCount : %d \n"
                     "WASegment->mNPageCount : %d \n",
                     ((sdpExtDesc*)aWASegment->mLastFreeExtent)->mLength,
                     ((sdpExtDesc*)aWASegment->mLastFreeExtent)->mExtFstPID,
                     aWASegment->mPageSeqInLFE,
                     aWASegment->mNExtentCount,
                     aWASegment->mNPageCount );
    }

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * �ش� SpaceID�κ��� �� NormalExtent�� �����´�. (sdptbExtent::allocExts
 * ��� )
 *
 * <IN>
 * aWASegment     - ��� WASegment
 ***************************************************************************/
IDE_RC sdtWASegment::allocFreeNExtent( sdtWASegment * aWASegment )
{
    sdpExtDesc      * sSlotPtr;
    sdrMtxStartInfo   sStartInfo;
    sdrMtx            sMtx;
    UInt              sState = 0;

    IDE_TEST_RAISE( sdtWAMap::getSlotCount( &aWASegment->mNExtentMap )
                    <= aWASegment->mNExtentCount,
                    ERROR_NOT_ENOUGH_NEXTENTSIZE );

    IDE_TEST( sdtWAMap::getSlotPtr( &aWASegment->mNExtentMap,
                                    aWASegment->mNExtentCount,
                                    sdtWAMap::getSlotSize( &aWASegment->mNExtentMap ),
                                    (void**)&sSlotPtr )
              != IDE_SUCCESS );

    IDE_TEST(sdrMiniTrans::begin(aWASegment->mStatistics,
                                 &sMtx,
                                 NULL,
                                 SDR_MTX_NOLOGGING,
                                 ID_FALSE,/*MtxUndoable(PROJ-2162)*/
                                 SM_DLOG_ATTR_DEFAULT)
             != IDE_SUCCESS);
    sState = 1;
    sdrMiniTrans::makeStartInfo ( &sMtx, &sStartInfo );

    IDE_TEST( sdpTableSpace::allocExts( aWASegment->mStatistics,
                                        &sStartInfo,
                                        aWASegment->mSpaceID,
                                        1, /*need extent count */
                                        (sdpExtDesc*)sSlotPtr )
              != IDE_SUCCESS );

    sState = 0;
    IDE_TEST(sdrMiniTrans::commit(&sMtx) != IDE_SUCCESS);

    /* BUG-37503
       slot �����͸� ������ ������ IDE_TEST���� �����ϸ�
       mLastFreeExtent���� �߸��Ȱ��� ���� �ȴ�. */
    aWASegment->mLastFreeExtent = (void*)sSlotPtr;
    aWASegment->mNExtentCount ++;
    aWASegment->mPageSeqInLFE = 0;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERROR_NOT_ENOUGH_NEXTENTSIZE );
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_NOT_ENOUGH_NEXTENTSIZE ) );
    }
    IDE_EXCEPTION_END;

    if (sState != 0)
    {
        IDE_ASSERT(sdrMiniTrans::rollback(&sMtx) == IDE_SUCCESS);
    }

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * �� WASegment�� ����� ��� WASegment�� Free�Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 ***************************************************************************/
IDE_RC sdtWASegment::freeAllNPage( sdtWASegment * aWASegment )
{
    UInt             sFreeExtCnt;
    sdpExtDesc     * sSlot;
    UInt             sSlotSize;
    sdrMtx           sMtx;
    UInt             sState = 0;
    UInt             i;

    sSlotSize = sdtWAMap::getSlotSize( &aWASegment->mNExtentMap );
    for( i = 0 ; i < aWASegment->mNExtentCount ; i ++ )
    {
        IDE_TEST(sdrMiniTrans::begin(aWASegment->mStatistics,
                                     &sMtx,
                                     NULL,
                                     SDR_MTX_NOLOGGING,
                                     ID_FALSE,/*MtxUndoable(PROJ-2162)*/
                                     SM_DLOG_ATTR_DEFAULT)
                 != IDE_SUCCESS);
        sState = 1;

        IDE_TEST( sdtWAMap::getSlotPtr( &aWASegment->mNExtentMap,
                                        i,
                                        sSlotSize,
                                        (void**)&sSlot )
                  != IDE_SUCCESS );
        IDE_TEST( sdpTableSpace::freeExt( aWASegment->mStatistics,
                                          &sMtx,
                                          aWASegment->mSpaceID,
                                          sSlot->mExtFstPID,
                                          &sFreeExtCnt )
                  != IDE_SUCCESS );

        sState = 0;
        IDE_TEST(sdrMiniTrans::commit(&sMtx) != IDE_SUCCESS);
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if (sState != 0)
    {
        IDE_ASSERT(sdrMiniTrans::rollback(&sMtx) == IDE_SUCCESS);
    }

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * �� WASegment�� NPage ������ ��ȯ�Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 ***************************************************************************/
UInt   sdtWASegment::getNPageCount( sdtWASegment * aWASegment )
{
    return aWASegment->mNPageCount;
}

/**************************************************************************
 * Description :
 * Seq��°�� NPID�� ��ȯ�Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aIdx           - Seq
 * <OUT>
 * aPID           - N��° NPID
 ***************************************************************************/
IDE_RC sdtWASegment::getNPIDBySeq( sdtWASegment * aWASegment,
                                   UInt           aIdx,
                                   scPageID     * aPID )
{
    sdpExtDesc      * sExtDescPtr;
    UInt              sPageCntInExt;
    UInt              sExtentIdx;
    UInt              sSeq;

    if( getNPageCount( aWASegment ) <= aIdx )
    {
        /*���������� �� ã���� */
        *aPID = SC_NULL_PID;
    }
    else
    {
        sPageCntInExt = sddDiskMgr::getPageCntInExt( aWASegment->mSpaceID );
        sExtentIdx = aIdx / sPageCntInExt;
        sSeq       = aIdx % sPageCntInExt;

        IDE_TEST( sdtWAMap::getSlotPtr( &aWASegment->mNExtentMap,
                                        sExtentIdx,
                                        sdtWAMap::getSlotSize(
                                            &aWASegment->mNExtentMap ),
                                        (void**)&sExtDescPtr )
                  != IDE_SUCCESS );
        *aPID = sExtDescPtr->mExtFstPID + sSeq;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * LRU ��Ȱ�� ��å ����, ����� �ش� Page�� Top���� �̵����� Drop�� �����.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWAGroupID     - ��� Group ID
 * aPID           - ��� WPID
 ***************************************************************************/
IDE_RC sdtWASegment::moveLRUListToTop( sdtWASegment * aWASegment,
                                       sdtWAGroupID   aWAGroupID,
                                       scPageID       aPID )
{
    sdtWAGroup     * sGrpInfo;
    sdtWCB         * sWCBPtr;
    scPageID         sPrevPID;
    scPageID         sNextPID;
    sdtWCB         * sPrevWCBPtr;
    sdtWCB         * sNextWCBPtr;
    sdtWCB         * sOldTopWCBPtr;

    if( ( aWAGroupID != SDT_WAGROUPID_NONE ) &&
        ( aWAGroupID != SDT_WAGROUPID_INIT ) &&
        ( aWAGroupID != SDT_WAGROUPID_MAX ) )
    {
        sGrpInfo = &aWASegment->mGroup[ aWAGroupID ];
        if( ( sGrpInfo->mPolicy     != SDT_WA_REUSE_LRU ) ||
            ( sGrpInfo->mReuseWPID1 == aPID ) )

        {
            /* �̹� Top�̰ų� LRU�� �ƴ϶� ������ �ʿ䰡 ���� */
        }
        else
        {
            IDE_DASSERT( validateLRUList( aWASegment, aWAGroupID )
                         == IDE_SUCCESS);
            sWCBPtr       = getWCB( aWASegment, aPID );
            sOldTopWCBPtr = getWCB( aWASegment, sGrpInfo->mReuseWPID1 );

            IDE_ERROR( sWCBPtr       != NULL );
            IDE_ERROR( sOldTopWCBPtr != NULL );

            sPrevPID = sWCBPtr->mLRUPrevPID;
            sNextPID = sWCBPtr->mLRUNextPID;

            if( sPrevPID == SC_NULL_PID )
            {
                /* Prev�� ���ٴ� ���� Top�̶�� �̾߱�. top���� �ٽ� ���� �ʿ�
                 * ���� */
            }
            else
            {
                /* PrevPage�κ��� ���� Page�� ���� ��ũ�� ��� */
                sPrevWCBPtr = getWCB( aWASegment, sPrevPID );
                if( sNextPID == SD_NULL_PID )
                {
                    /* �ڽ��� ������ Page���� ��� */
                    IDE_ERROR( sGrpInfo->mReuseWPID2 == aPID );
                    sGrpInfo->mReuseWPID2 = sPrevPID;
                }
                else
                {
                    sNextWCBPtr = getWCB( aWASegment, sNextPID );
                    sNextWCBPtr->mLRUPrevPID = sPrevPID;
                }
                sPrevWCBPtr->mLRUNextPID = sNextPID;

                /* ���ο� �Ŵ� */
                sWCBPtr->mLRUPrevPID  = SD_NULL_PID;
                sWCBPtr->mLRUNextPID  = sGrpInfo->mReuseWPID1;
                sOldTopWCBPtr->mLRUPrevPID = aPID;
                sGrpInfo->mReuseWPID1 = aPID;
            }

            IDE_DASSERT( validateLRUList( aWASegment,
                                          aWAGroupID ) == IDE_SUCCESS);
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    ideLog::log( IDE_DUMP_0,
                 "GroupID : %u\n"
                 "PID     : %u",
                 aWAGroupID,
                 aPID );

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * LRU����Ʈ�� �����Ѵ�. Debugging ��� ��
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWAGroupID     - ��� Group ID
 ***************************************************************************/
IDE_RC sdtWASegment::validateLRUList( sdtWASegment * aWASegment,
                                      sdtWAGroupID   aWAGroupID )
{
    sdtWAGroup     * sGrpInfo;
    sdtWCB         * sWCBPtr;
    UInt             sPageCount = 0;
    scPageID         sPrevWPID;
    scPageID         sNextWPID;

    IDE_ERROR( aWAGroupID != SDT_WAGROUPID_MAX );

    sGrpInfo = &aWASegment->mGroup[ aWAGroupID ];
    IDE_ERROR( sGrpInfo->mPolicy == SDT_WA_REUSE_LRU );

    sPrevWPID = SC_NULL_PID;
    sNextWPID = sGrpInfo->mReuseWPID1;
    while( sNextWPID != SC_NULL_PID )
    {
        sWCBPtr = getWCB( aWASegment, sNextWPID );

        IDE_ASSERT( sWCBPtr != NULL );
        IDE_ASSERT( sWCBPtr->mLRUPrevPID == sPrevWPID );
        IDE_ASSERT( sWCBPtr->mWPageID    == sNextWPID );

        sNextWPID = sWCBPtr->mLRUNextPID;
        sPrevWPID = sWCBPtr->mWPageID;

        sPageCount ++;
    }

    IDE_ASSERT( sPrevWPID == sGrpInfo->mReuseWPID2 );
    IDE_ASSERT( sNextWPID == SC_NULL_PID );
    IDE_ASSERT( sPageCount == getWAGroupPageCount( aWASegment, aWAGroupID ) );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

/**************************************************************************
 * Description :
 * PID�� �������� �� PID�� ������ Group�� ã�´�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aPID           - ��� PID
 ***************************************************************************/
sdtWAGroupID sdtWASegment::findGroup( sdtWASegment * aWASegment,
                                      scPageID       aPID )
{
    UInt i;

    for( i = 0 ; i < SDT_WAGROUPID_MAX ; i ++ )
    {
        if( ( aWASegment->mGroup[ i ].mPolicy != SDT_WA_REUSE_NONE ) &&
            ( aWASegment->mGroup[ i ].mBeginWPID <= aPID ) &&
            ( aWASegment->mGroup[ i ].mEndWPID > aPID ) )
        {
            return i;
        }
    }

    /* ã�� ���� */
    return SDT_WAGROUPID_MAX;
}

/**************************************************************************
 * Description :
 * WASegemnt ������ File�� Dump�Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 ***************************************************************************/
void sdtWASegment::exportWASegmentToFile( sdtWASegment * aWASegment )
{
    SChar          sFileName[ SM_MAX_FILE_NAME ];
    SChar          sDateString[ SMI_TT_STR_SIZE ];
    iduFile        sFile;
    UInt           sState = 0;
    UChar        * sWAExtentPtr;
    ULong          sOffset = 0;
    UInt           i;

    IDE_ASSERT( aWASegment != NULL );

    IDE_TEST( sFile.initialize( IDU_MEM_SM_TEMP,
                                1, /* Max Open FD Count */
                                IDU_FIO_STAT_OFF,
                                IDV_WAIT_INDEX_NULL )
              != IDE_SUCCESS);
    sState = 1;

    smuUtility::getTimeString( smiGetCurrTime(),
                               SMI_TT_STR_SIZE,
                               sDateString );

    idlOS::snprintf( sFileName,
                     SM_MAX_FILE_NAME,
                     "%s%c%s.td",
                     smuProperty::getTempDumpDirectory(),
                     IDL_FILE_SEPARATOR,
                     sDateString );

    IDE_TEST( sFile.setFileName( sFileName ) != IDE_SUCCESS);
    IDE_TEST( sFile.createUntilSuccess( smiSetEmergency )
              != IDE_SUCCESS);
    IDE_TEST( sFile.open( ID_FALSE ) != IDE_SUCCESS ); //DIRECT_IO
    sState = 2;

    for( i = 0 ; i < aWASegment->mWAExtentCount ; i ++ )
    {
        sWAExtentPtr = getWAExtentPtr( aWASegment, i );
        IDE_TEST( sFile.write( aWASegment->mStatistics,
                               sOffset,
                               sWAExtentPtr,
                               SDT_WAEXTENT_SIZE )
                  != IDE_SUCCESS );
        sOffset += SDT_WAEXTENT_SIZE;
    }

    IDE_TEST( sFile.sync() != IDE_SUCCESS);

    sState = 1;
    IDE_TEST( sFile.close() != IDE_SUCCESS);

    sState = 0;
    IDE_TEST( sFile.destroy() != IDE_SUCCESS);

    ideLog::log( IDE_DUMP_0, "TEMP_DUMP_FILE : %s", sFileName );

    return;

    IDE_EXCEPTION_END;

    switch( sState )
    {
        case 2:
            (void) sFile.close() ;
        case 1:
            (void) sFile.destroy();
            break;
        default:
            break;
    }

    return;
}

/**************************************************************************
 * Description :
 * File�κ��� WASegment�� �о���δ�.
 *
 * <IN>
 * aFileName      - ��� File
 * <OUT>
 * aWASegment     - �о�帰 WASegment�� ��ġ
 * aPtr           - WA���� ������ ���� ��ġ
 * aAlignedPtr    - �� ���۸� Align�� ���� �������� ��ġ
 ***************************************************************************/
void sdtWASegment::importWASegmentFromFile( SChar         * aFileName,
                                            sdtWASegment ** aWASegment,
                                            UChar        ** aPtr,
                                            UChar        ** aAlignedPtr )
{
    iduFile         sFile;
    UInt            i;
    UInt            sState          = 0;
    UInt            sMemAllocState  = 0;
    ULong           sSize = 0;
    sdtWASegment  * sWASegment = NULL;
    sdtWCB        * sWCBPtr;
    UChar         * sPtr;
    UChar         * sAlignedPtr;

    IDE_TEST( sFile.initialize(IDU_MEM_SM_SMU,
                               1, /* Max Open FD Count */
                               IDU_FIO_STAT_OFF,
                               IDV_WAIT_INDEX_NULL )
              != IDE_SUCCESS );
    sState = 1;

    IDE_TEST( sFile.setFileName( aFileName ) != IDE_SUCCESS );
    IDE_TEST( sFile.open() != IDE_SUCCESS );
    sState = 2;

    IDE_TEST( sFile.getFileSize( &sSize ) != IDE_SUCCESS );
    IDE_TEST( iduMemMgr::malloc( IDU_MEM_SM_TEMP,
                                 sSize + SDT_WAEXTENT_SIZE,
                                 (void**)&sPtr )
              != IDE_SUCCESS );
    sMemAllocState = 1;

    sAlignedPtr = (UChar*)idlOS::align( sPtr, SDT_WAEXTENT_SIZE );

    IDE_TEST( sFile.read( NULL,  /* aStatistics */
                          0,     /* aWhere */
                          sAlignedPtr,
                          sSize,
                          NULL ) /* aEmergencyFunc */
              != IDE_SUCCESS );

    /* Pointer ������ ���� �޸� ��ġ�� �߸��Ǿ� ������, �������Ѵ�. */
    sWASegment              =(sdtWASegment*)getFrameInExtent( sAlignedPtr, 0 );
    sWASegment->mHintWCBPtr =getWCBInExtent( sAlignedPtr, 0 );

    /* Extent��ġ ������ */
    for( i = 0 ; i < sWASegment->mWAExtentCount ; i ++ )
    {
        IDE_TEST( setWAExtentPtr( sWASegment,
                                  i,
                                  sAlignedPtr + i * SDT_WAEXTENT_SIZE )
                  != IDE_SUCCESS );
    }

    /* WCB ��ġ ������ */
    for( i = 0 ;
         i < sWASegment->mWAExtentCount * SDT_WAEXTENT_PAGECOUNT ;
         i ++ )
    {
        sWCBPtr = getWCB( sWASegment, i );
        bindWCB( sWASegment, sWCBPtr, i );
    }

    /* Map�� Segment��ġ �� Group���� Pointer ������ */
    IDE_TEST( sdtWAMap::resetPtrAddr( (void*)&sWASegment->mNExtentMap,
                                      sWASegment )
              != IDE_SUCCESS );
    IDE_TEST( sdtWAMap::resetPtrAddr( (void*)&sWASegment->mSortHashMapHdr,
                                      sWASegment )
              != IDE_SUCCESS );

    sState = 1;
    IDE_TEST( sFile.close() != IDE_SUCCESS );
    sState = 0;
    IDE_TEST( sFile.destroy() != IDE_SUCCESS );

    *aPtr        = sPtr;
    *aAlignedPtr = sAlignedPtr;
    *aWASegment  = sWASegment;

    return;

    IDE_EXCEPTION_END;

    switch( sMemAllocState )
    {
        case 1:
            IDE_ASSERT( iduMemMgr::free( sPtr ) == IDE_SUCCESS );
            sPtr = NULL;
        default:
            break;
    }

    switch(sState)
    {
        case 2:
            IDE_ASSERT( sFile.close() == IDE_SUCCESS );
        case 1:
            IDE_ASSERT( sFile.destroy() == IDE_SUCCESS );
        default:
            break;
    }


    return;
}

void sdtWASegment::dumpWASegment( void           * aWASegment,
                                  SChar          * aOutBuf,
                                  UInt             aOutSize )
{
    sdtWASegment   * sWASegment = (sdtWASegment*)aWASegment;
    SInt             i;

    idlVA::appendFormat(
        aOutBuf,
        aOutSize,
        "DUMP WASEGMENT:\n"
        "\tWASegPtr        : 0x%"ID_xINT64_FMT"\n"
        "\tNExtentCount    : %"ID_UINT32_FMT"\n"
        "\tWAExtentCount   : %"ID_UINT32_FMT"\n"
        "\tSpaceID         : %"ID_UINT32_FMT"\n"
        "\tPageSeqInLFE    : %"ID_UINT32_FMT"\n"
        "\tLinkPtr         : 0x%"ID_xINT64_FMT", 0x%"ID_xINT64_FMT"\n",
        sWASegment,
        sWASegment->mNExtentCount,
        sWASegment->mWAExtentCount,
        sWASegment->mSpaceID,
        sWASegment->mPageSeqInLFE,
        sWASegment->mNext, sWASegment->mPrev );

    idlVA::appendFormat( aOutBuf, aOutSize, "\n" );

    for( i = 0 ; i < SDT_WAGROUPID_MAX ; i ++ )
    {
        dumpWAGroup( sWASegment, i, aOutBuf, aOutSize );
    }

    return;
}

void sdtWASegment::dumpFlushQueue( void           * aWAFQ,
                                   SChar          * aOutBuf,
                                   UInt             aOutSize )
{
    sdtWAFlushQueue * sWAFQ = (sdtWAFlushQueue*)aWAFQ;

    idlVA::appendFormat( aOutBuf,
                         aOutSize,
                         "FLUSH QUEUE:\n"
                         "Begin      : %"ID_UINT32_FMT"\n"
                         "End        : %"ID_UINT32_FMT"\n"
                         "Index      : %"ID_UINT32_FMT"\n",
                         sWAFQ->mFQBegin,
                         sWAFQ->mFQEnd,
                         sWAFQ->mWAFlusherIdx );
    idlVA::appendFormat( aOutBuf, aOutSize, "\n" );

    return;

}

void sdtWASegment::dumpWCBs( void           * aWASegment,
                             SChar          * aOutBuf,
                             UInt             aOutSize )
{
    sdtWASegment   * sWASegment = (sdtWASegment*)aWASegment;
    sdtWCB         * sWCBPtr;
    SInt             i;

    idlVA::appendFormat( aOutBuf,
                         aOutSize,
                         "WCB:\n%5s %5s %4s %4s %8s %8s %8s %8s %8s\n",
                         "WPID",
                         "STATE",
                         "FIX",
                         "FREE",
                         "SPACE_ID",
                         "PAGE_ID",
                         "LRU_PREV",
                         "LRU_NEXT",
                         "HASHNEXT" );

    for( i = 0 ;
         i < (SInt)getWASegmentPageCount( sWASegment ) ;
         i ++ )
    {
        sWCBPtr = getWCB( sWASegment, i );
        dumpWCB( (void*)sWCBPtr,aOutBuf,aOutSize );
    }

    idlVA::appendFormat( aOutBuf, aOutSize, "\n" );

    return;
}

void sdtWASegment::dumpWCB( void           * aWCB,
                            SChar          * aOutBuf,
                            UInt             aOutSize )
{
    sdtWCB   * sWCBPtr;

    sWCBPtr = (sdtWCB*) aWCB;

    idlVA::appendFormat( aOutBuf,
                         aOutSize,
                         "%5"ID_UINT32_FMT
                         " %5"ID_UINT32_FMT
                         " %4"ID_UINT32_FMT
                         " %4"ID_UINT32_FMT
                         " %8"ID_UINT32_FMT
                         " %8"ID_UINT32_FMT
                         " %8"ID_UINT32_FMT
                         " %8"ID_UINT32_FMT"\n",
                         sWCBPtr->mWPageID,
                         sWCBPtr->mWPState,
                         sWCBPtr->mFix,
                         sWCBPtr->mBookedFree,
                         sWCBPtr->mNSpaceID,
                         sWCBPtr->mNPageID,
                         sWCBPtr->mLRUPrevPID,
                         sWCBPtr->mLRUNextPID );
}

void sdtWASegment::dumpWAPageHeaders( void           * aWASegment,
                                      SChar          * aOutBuf,
                                      UInt             aOutSize )
{
    sdtWASegment   * sWASegment = (sdtWASegment*)aWASegment;
    SChar          * sTypeNamePtr;
    SChar            sInvalidName[] = "INVALID";
    sdtTempPageHdr * sPagePtr;
    sdtTempPageType  sType;
    idBool           sSkip = ID_TRUE;
    UInt             i;

    idlVA::appendFormat( aOutBuf,
                         aOutSize,
                         "TEMP PAGE HEADERS:\n"
                         "%8s %16s %16s %16s %16s %8s %8s %8s\n",
                         "WPID",
                         "TAFO", /*TypeAndFreeOffset*/
                         "TYPE",
                         "TYPENAME",
                         "FREEOFF",
                         "PREVPID",
                         "SELFPID",
                         "NEXTPID",
                         "SLOTCNT" );
    for( i = 0 ;
         i < getWASegmentPageCount( sWASegment ) ;
         i ++ )
    {
        sPagePtr = (sdtTempPageHdr*) getWAPagePtr( sWASegment,
                                                   SDT_WAGROUPID_NONE,
                                                   i );

        if( ( sPagePtr->mTypeAndFreeOffset == 0 ) &&
            ( sPagePtr->mPrevPID == 0 ) &&
            ( sPagePtr->mNextPID == 0 ) &&
            ( sPagePtr->mSlotCount  == 0 ) )
        {
            sSkip = ID_TRUE;
        }
        else
        {
            sSkip = ID_FALSE;
        }

        if( sSkip == ID_FALSE)
        {
            sType = (sdtTempPageType)sdtTempPage::getType( (UChar*)sPagePtr );
            if( ( SDT_TEMP_PAGETYPE_INIT <= sType ) &&
                ( sType < SDT_TEMP_PAGETYPE_MAX ) )
            {
                sTypeNamePtr = sdtTempPage::mPageName[ sType ];
            }
            else
            {
                sTypeNamePtr = sInvalidName;
            }


            idlVA::appendFormat( aOutBuf,
                                 aOutSize,
                                 "%8"ID_UINT32_FMT
                                 " %16"ID_UINT64_FMT
                                 " %16"ID_UINT32_FMT
                                 " %16s"
                                 " %16"ID_UINT32_FMT
                                 " %8"ID_UINT32_FMT
                                 " %8"ID_UINT32_FMT
                                 " %8"ID_UINT32_FMT
                                 " %8"ID_UINT32_FMT"\n",
                                 i,
                                 sPagePtr->mTypeAndFreeOffset,
                                 sdtTempPage::getType( (UChar*)sPagePtr ),
                                 sTypeNamePtr,
                                 sdtTempPage::getFreeOffset( (UChar*)sPagePtr ),
                                 sPagePtr->mPrevPID,
                                 sPagePtr->mSelfPID,
                                 sPagePtr->mNextPID,
                                 sPagePtr->mSlotCount );
        }
    }

    idlVA::appendFormat( aOutBuf, aOutSize, "\n" );

    return;
}

void sdtWASegment::dumpWAGroup( sdtWASegment   * aWASegment,
                                sdtWAGroupID     aWAGroupID,
                                SChar          * aOutBuf,
                                UInt             aOutSize )
{
    sdtWAGroup     * sGrpInfo = getWAGroupInfo( aWASegment, aWAGroupID );

    if( sGrpInfo->mPolicy != SDT_WA_REUSE_NONE )
    {
        idlVA::appendFormat(
            aOutBuf,
            aOutSize,
            "DUMP WAGROUP:\n"
            "\tID     : %"ID_UINT32_FMT"\n"
            "\tPolicy : %-4"ID_UINT32_FMT
            "(0:None,1:InMemory,2:FIFO,3:LRU)\n"
            "\tRange  : %"ID_UINT32_FMT" <-> %"ID_UINT32_FMT"\n"
            "\tReuseP : %"ID_UINT32_FMT", %"ID_UINT32_FMT"\n"
            "\tHint   : %"ID_UINT32_FMT"\n"
            "\tMapPtr : 0x%"ID_xINT64_FMT"\n\n",
            aWAGroupID,
            sGrpInfo->mPolicy,
            sGrpInfo->mBeginWPID,
            sGrpInfo->mEndWPID,
            sGrpInfo->mReuseWPID1,
            sGrpInfo->mReuseWPID2,
            sGrpInfo->mHintWPID,
            sGrpInfo->mMapHdr );
    }

    return;
}
