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

#ifndef _O_SDT_WA_SEGMENT_H_
#define _O_SDT_WA_SEGMENT_H_ 1

#include <idl.h>

#include <smDef.h>
#include <smiMisc.h>
#include <smiDef.h>
#include <sdtDef.h>
#include <sdtWorkArea.h>
#include <sctTableSpaceMgr.h>

class sdtWorkArea;
class sdtWASegment
{
public:
    /* ���������� ������ WAPage. Hint �� Dump�ϱ� ���� ���� */
    /* getNPageToWCB, getWAPagePtr, getWCB �� Hint */
    sdtWCB             * mHintWCBPtr;

    sdtWAMapHdr          mNExtentMap;     /*NormalExtent���� �����ϴ� Map*/
    UInt                 mNExtentCount;   /*NextentArray�� ���� Extent�� ����*/
    UInt                 mNPageCount;     /*�Ҵ��� NPage�� ���� */
    UInt                 mWAExtentCount;
    sdtWAMapHdr          mSortHashMapHdr; /*KeyMap, Heap, PIDList���� Map */

    scSpaceID            mSpaceID;
    UInt                 mPageSeqInLFE;   /*LFE(LastFreeExtent)������ �Ҵ��ؼ�
                                            �ǳ��������� ��ȣ */
    void               * mLastFreeExtent; /*NExtentArray�� �������� Extent*/
    sdtWAGroup           mGroup[ SDT_WAGROUPID_MAX ];  /*WAGroup�� ���� ����*/
    iduStackMgr          mFreeNPageStack; /*FreenPage�� ��Ȱ��� */
    sdtWAFlushQueue    * mFlushQueue;     /*FlushQueue */
    idBool               mDiscardPage;    /*�Ѱܳ� Page�� �ִ°�?*/

    idvSQL             * mStatistics;
    smiTempTableStats ** mStatsPtr;
    sdtWASegment       * mNext;
    sdtWASegment       * mPrev;

    /* ������ Ž���� Hash*/
    sdtWCB            ** mNPageHashPtr;
    UInt                 mNPageHashBucketCnt;

    /******************************************************************
     * Segment
     ******************************************************************/
    static IDE_RC createWASegment(idvSQL             * aStatistics,
                                  smiTempTableStats ** aStatsPtr,
                                  idBool               aLogging,
                                  scSpaceID            aSpaceID,
                                  ULong                aSize,
                                  sdtWASegment      ** aWASegment );

    static UInt   getWASegmentPageCount(sdtWASegment * aWASegment );

    static IDE_RC dropWASegment(sdtWASegment * aWASegment,
                                idBool         aWait4Flush);

    /******************************************************************
     * Group
     ******************************************************************/
    static IDE_RC createWAGroup( sdtWASegment     * aWASegment,
                                 sdtWAGroupID       aWAGroupID,
                                 UInt               aPageCount,
                                 sdtWAReusePolicy   aPolicy );
    static IDE_RC resetWAGroup( sdtWASegment      * aWASegment,
                                sdtWAGroupID        aWAGroupID,
                                idBool              aWait4Flush );
    static IDE_RC clearWAGroup( sdtWASegment      * aWASegment,
                                sdtWAGroupID        aWAGroupID );

    /************ Group �ʱ�ȭ **************************/
    static void initInMemoryGroup( sdtWAGroup     * aGrpInfo );
    static void initFIFOGroup( sdtWAGroup     * aGrpInfo );
    static void initLRUGroup( sdtWASegment   * aWASegment,
                              sdtWAGroup     * aGrpInfo );

    static void initWCB( sdtWASegment * aWASegment,
                         sdtWCB       * aWCBPtr,
                         scPageID       aWPID );
    static void bindWCB( sdtWASegment * aWASegment,
                         sdtWCB       * aWCBPtr,
                         scPageID       aWPID );

    /************ Hint ó�� ***************/
    inline static scPageID getHintWPID( sdtWASegment * aWASegment,
                                        sdtWAGroupID   aWAGroupID );
    inline static IDE_RC setHintWPID( sdtWASegment * aWASegment,
                                      sdtWAGroupID   aWAGroupID,
                                      scPageID       aHintWPID );
    inline static sdtWAGroup *getWAGroupInfo( sdtWASegment * aWASegment,
                                              sdtWAGroupID   aWAGroupID );

    inline static idBool isInMemoryGroup( sdtWASegment * aWASegment,
                                          sdtWAGroupID   aWAGroupID );
    inline static idBool isInMemoryGroupByPID( sdtWASegment * aWASegment,
                                               scPageID       aWPID );

    static UInt   getWAGroupPageCount( sdtWASegment * aWASegment,
                                       sdtWAGroupID   aWAGroupID );
    static UInt   getAllocableWAGroupPageCount( sdtWASegment * aWASegment,
                                                sdtWAGroupID   aWAGroupID );

    static scPageID getFirstWPageInWAGroup( sdtWASegment * aWASegment,
                                            sdtWAGroupID   aWAGroupID );
    static scPageID getLastWPageInWAGroup( sdtWASegment * aWASegment,
                                           sdtWAGroupID   aWAGroupID );

    static IDE_RC getFreeWAPage( sdtWASegment * aWASegment,
                                 sdtWAGroupID   aWAGroupID,
                                 scPageID     * aRetPID );
    static IDE_RC mergeWAGroup(sdtWASegment     * aWASegment,
                               sdtWAGroupID       aWAGroupID1,
                               sdtWAGroupID       aWAGroupID2,
                               sdtWAReusePolicy   aPolicy );
    static IDE_RC splitWAGroup(sdtWASegment     * aWASegment,
                               sdtWAGroupID       aWAGroupIDSrc,
                               sdtWAGroupID       aWAGroupIDDst,
                               sdtWAReusePolicy   aPolicy );

    static IDE_RC dropAllWAGroup(sdtWASegment * aWASegment,
                                 idBool         aWait4Flush);
    static IDE_RC dropWAGroup(sdtWASegment * aWASegment,
                              sdtWAGroupID   aWAGroupID,
                              idBool         aWait4Flush);



    /******************************************************************
     * Page �ּ� ���� Operation
     ******************************************************************/
    inline static void convertFromWGRIDToNGRID( sdtWASegment * aWASegment,
                                                scGRID         aWGRID,
                                                scGRID       * aNGRID );
    inline static IDE_RC getWCBByNPID( sdtWASegment * aWASegment,
                                       sdtWAGroupID   aWAGroupID,
                                       scSpaceID      aNSpaceID,
                                       scPageID       aNPID,
                                       sdtWCB      ** aRetWCB );
    inline static IDE_RC getWPIDFromGRID(sdtWASegment * aWASegment,
                                         sdtWAGroupID   aWAGroupID,
                                         scGRID         aGRID,
                                         scPageID     * aRetPID );

    /******************************************************************
     * Page ���� ���� Operation
     ******************************************************************/
    /*************************  DirtyPage  ****************************/
    inline static IDE_RC setDirtyWAPage(sdtWASegment * aWASegment,
                                        scPageID       aWPID);
    static IDE_RC writeDirtyWAPages(sdtWASegment * aWASegment,
                                    sdtWAGroupID   aWAGroupID );
    static IDE_RC makeInitPage( sdtWASegment * aWASegment,
                                scPageID       aWPID,
                                idBool         aWait4Flush);
    inline static void bookedFree(sdtWASegment * aWASegment,
                                  scPageID       aWPID);

    /*************************  Fix Page  ****************************/
    inline static IDE_RC fixPage(sdtWASegment * aWASegment,
                                 scPageID       aWPID);
    inline static IDE_RC unfixPage(sdtWASegment * aWASegment,
                                   scPageID       aWPID);

    /*************************    State    ****************************/
    inline static idBool isFreeWAPage(sdtWASegment * aWASegment,
                                      scPageID       aWPID);
    inline static sdtWAPageState getWAPageState(sdtWASegment   * aWASegment,
                                                scPageID         aWPID );
    inline static sdtWAPageState getWAPageState(sdtWCB * aWCBPtr );
    inline static IDE_RC setWAPageState(sdtWASegment * aWASegment,
                                        scPageID       aWPID,
                                        sdtWAPageState aState );
    inline static void setWAPageState( sdtWCB         * aWCBPtr,
                                       sdtWAPageState   aState );
    inline static IDE_RC getSiblingWCBPtr( sdtWCB  * aWCBPtr,
                                           UInt      aWPID,
                                           sdtWCB ** aTargetWCBPtr );
    inline static void checkAndSetWAPageState( sdtWCB         * aWCBPtr,
                                               sdtWAPageState   aChkState,
                                               sdtWAPageState   aAftState,
                                               sdtWAPageState * aBefState);
    static idBool isFixedPage(sdtWASegment * aWASegment,
                              scPageID       aWPID );

    /******************************************************************
     * Page�������� Operation
     ******************************************************************/
    inline static IDE_RC getPagePtrByGRID(sdtWASegment  * aWASegment,
                                          sdtWAGroupID    aGrpID,
                                          scGRID          aGRID,
                                          UChar        ** aNSlotPtr,
                                          idBool        * aIsValidSlot );

    inline static IDE_RC getPageWithFix( sdtWASegment  * aWASegment,
                                         sdtWAGroupID    aGrpID,
                                         scGRID          aGRID,
                                         UInt          * aWPID,
                                         UChar        ** aWAPagePtr,
                                         UInt          * aSlotCount );
    inline static IDE_RC getPage(sdtWASegment  * aWASegment,
                                 sdtWAGroupID    aGrpID,
                                 scGRID          aGRID,
                                 UInt          * aWPID,
                                 UChar        ** aWAPagePtr,
                                 UInt          * aSlotCount );
    inline static void getSlot( UChar         * aWAPagePtr,
                                UInt            aSlotCount,
                                UInt            aSlotNo,
                                UChar        ** aNSlotPtr,
                                idBool        * aIsValid );


    inline static sdtWCB * getWCB(sdtWASegment  * aWASegment,
                                  scPageID        aWPID );

    inline static UChar  * getWAPagePtr( sdtWASegment  * aWASegment,
                                         sdtWAGroupID    aGroupID,
                                         scPageID        aWPID );
    inline static sdtWCB * getWCBInternal(sdtWASegment  * aWASegment,
                                          scPageID        aWPID );
    inline static sdtWCB* getWCBInExtent( UChar * aExtentPtr,
                                          UInt    aIdx );
    inline static UChar* getFrameInExtent( UChar * aExtentPtr,
                                           UInt    aIdx );
    static scPageID       getNPID( sdtWASegment * aWASegment,
                                   scPageID       aWPID );

    /*************************************************
     * NormalPage Operation
     *************************************************/
    static IDE_RC assignNPage(sdtWASegment * aWASegment,
                              scPageID       aWPID,
                              scSpaceID      aNSpaceID,
                              scPageID       aNPID);
    static IDE_RC unassignNPage(sdtWASegment * aWASegment,
                                scPageID       aWPID );
    static IDE_RC movePage( sdtWASegment * aWASegment,
                            sdtWAGroupID   aSrcGroupID,
                            scPageID       aSrcWPID,
                            sdtWAGroupID   aDstGroupID,
                            scPageID       aDstWPID );

    inline static UInt getNPageHashValue( sdtWASegment * aWASegment,
                                          scSpaceID      aNSpaceID,
                                          scPageID       aNPID);
    inline static sdtWCB * findWCB( sdtWASegment * aWASegment,
                                    scSpaceID      aNSpaceID,
                                    scPageID       aNPID);
    static IDE_RC readNPage(sdtWASegment * aWASegment,
                            sdtWAGroupID   aWAGroupID,
                            scPageID       aWPID,
                            scSpaceID      aNSpaceID,
                            scPageID       aNPID);
    static IDE_RC writeNPage(sdtWASegment * aWASegment,
                             scPageID       aWPID );
    static IDE_RC pushFreePage(sdtWASegment * aWASegment,
                               scPageID       aNPID);
    static IDE_RC allocAndAssignNPage(sdtWASegment * aWASegment,
                                      scPageID       aTargetPID );
    static IDE_RC allocFreeNExtent( sdtWASegment * aWASegment );
    static IDE_RC freeAllNPage( sdtWASegment * aWASegment );

    static UInt   getNExtentCount( sdtWASegment * aWASegment )
    {
        return aWASegment->mNExtentCount;
    }

    /***********************************************************
     * FullScan�� NPage ��ȸ �Լ�
     ***********************************************************/
    static UInt getNPageCount( sdtWASegment * aWASegment );
    static IDE_RC getNPIDBySeq( sdtWASegment * aWASegment,
                                UInt           aIdx,
                                scPageID     * aPID );

    static idBool   isWorkAreaTbsID( scSpaceID aSpaceID )
    {
        return ( aSpaceID == SDT_SPACEID_WORKAREA ) ? ID_TRUE : ID_FALSE ;
    }
public:


    /******************************************************************
     * WAExtent
     * TempTableHeader�� ��ġ�� �� �� �ٷ� �����ʿ� WAExtent����
     * Pointer �� ��ġ�ȴ�.
     ******************************************************************/
    inline static UChar * getWAExtentPtr( sdtWASegment  * aWASegment,
                                          UInt            aIdx );


    static IDE_RC addWAExtentPtr(sdtWASegment * aWASegment,
                                 UChar        * aWAExtentPtr );
    static IDE_RC setWAExtentPtr(sdtWASegment  * aWASegment,
                                 UInt            aIdx,
                                 UChar         * aWAExtentPtr );
public:
    static UInt getExtentIdx( scPageID  aWPID )
    {
        return aWPID / SDT_WAEXTENT_PAGECOUNT ;
    }
    static UInt getPageSeqInExtent( scPageID  aWPID )
    {
        return aWPID & SDT_WAEXTENT_PAGECOUNT_MASK ;
    }

private:
    static IDE_RC moveLRUListToTop( sdtWASegment * aWASegment,
                                    sdtWAGroupID   aWAGroupID,
                                    scPageID       aPID );
    static IDE_RC validateLRUList(sdtWASegment * aWASegment,
                                  sdtWAGroupID   aGroupID );
    static sdtWAGroupID findGroup( sdtWASegment * aWASegment,
                                   scPageID       aPID );


public:
    /***********************************************************
     * Dump�� �Լ���
     ***********************************************************/
    static void exportWASegmentToFile( sdtWASegment * aWASegment );
    static void importWASegmentFromFile( SChar         * aFileName,
                                         sdtWASegment ** aWASegment,
                                         UChar        ** aPtr,
                                         UChar        ** aAlignedPtr );
    static void dumpWASegment( void           * aWASegment,
                               SChar          * aOutBuf,
                               UInt             aOutSize );
    static void dumpWAGroup( sdtWASegment   * aWASegment,
                             sdtWAGroupID     aWAGroupID ,
                             SChar          * aOutBuf,
                             UInt             aOutSize );
    static void dumpFlushQueue( void           * aWAFQ,
                                SChar          * aOutBuf,
                                UInt             aOutSize );
    static void dumpWCBs( void           * aWASegment,
                          SChar          * aOutBuf,
                          UInt             aOutSize );
    static void dumpWCB( void           * aWCB,
                         SChar          * aOutBuf,
                         UInt             aOutSize );
    static void dumpWAPageHeaders( void           * aWASegment,
                                   SChar          * aOutBuf,
                                   UInt             aOutSize );
} ;


/**************************************************************************
 * Description :
 *      HintPage������
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWAGroupID     - ��� Group ID
 ***************************************************************************/
scPageID sdtWASegment::getHintWPID( sdtWASegment * aWASegment,
                                    sdtWAGroupID   aWAGroupID )
{
    return sdtWASegment::getWAGroupInfo( aWASegment,aWAGroupID )->mHintWPID;
}

/**************************************************************************
 * Description :
 *      HintPage�� ������.
 *      HintPage�� unfix�Ǹ� �ȵǱ⿡ �������ѵ�
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWAGroupID     - ��� Group ID
 ***************************************************************************/
IDE_RC sdtWASegment::setHintWPID( sdtWASegment * aWASegment,
                                  sdtWAGroupID   aWAGroupID,
                                  scPageID       aHintWPID )
{
    scPageID   * sHintWPIDPtr;

    sHintWPIDPtr =
        &sdtWASegment::getWAGroupInfo( aWASegment,aWAGroupID )->mHintWPID;
    if ( *sHintWPIDPtr != aHintWPID )
    {
        if ( *sHintWPIDPtr != SC_NULL_PID )
        {
            IDE_TEST( unfixPage( aWASegment, *sHintWPIDPtr ) != IDE_SUCCESS );
        }

        *sHintWPIDPtr = aHintWPID;

        if ( *sHintWPIDPtr != SC_NULL_PID )
        {
            IDE_TEST( fixPage( aWASegment, *sHintWPIDPtr ) != IDE_SUCCESS );
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    ideLog::log( IDE_DUMP_0,
                 "PrevHintWPID : %u\n"
                 "NextHintWPID : %u\n",
                 *sHintWPIDPtr,
                 aHintWPID );

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * GroupInfo�� ������ �����´�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWAGroupID     - ��� Group ID
 ***************************************************************************/
sdtWAGroup *sdtWASegment::getWAGroupInfo( sdtWASegment * aWASegment,
                                          sdtWAGroupID   aWAGroupID )
{
    return &aWASegment->mGroup[ aWAGroupID ];
}

/**************************************************************************
 * Description :
 *      InMemoryGroup���� �Ǵ���.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWAGroupID     - ��� Group ID
 ***************************************************************************/
idBool sdtWASegment::isInMemoryGroup( sdtWASegment * aWASegment,
                                      sdtWAGroupID   aWAGroupID )
{
    sdtWAGroup * sWAGrpInfo = sdtWASegment::getWAGroupInfo( aWASegment,
                                                            aWAGroupID );

    if ( sWAGrpInfo->mPolicy == SDT_WA_REUSE_INMEMORY )
    {
        return ID_TRUE;
    }

    return ID_FALSE;
}

/**************************************************************************
 * Description :
 *      WPID�� �������� Group�� ã�� InMemoryGroup���� �Ǵ���.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWPID          - WAPageID
 ***************************************************************************/
idBool sdtWASegment::isInMemoryGroupByPID( sdtWASegment * aWASegment,
                                           scPageID       aWPID )
{
    return isInMemoryGroup( aWASegment,
                            findGroup( aWASegment, aWPID ) );
}


/**************************************************************************
 * Description :
 *      ���߿� unassign�ɶ� Free�ǵ���, ǥ���صд�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWPID          - WAPageID
 ***************************************************************************/
void sdtWASegment::bookedFree(sdtWASegment * aWASegment,
                              scPageID       aWPID)
{
    sdtWCB   * sWCBPtr;

    sWCBPtr = getWCB( aWASegment, aWPID );
    if ( sWCBPtr->mNPageID == SC_NULL_PID )
    {
        /* �Ҵ� �Ǿ����� ������, �����ص� �ȴ�. */
    }
    else
    {
        sWCBPtr->mBookedFree = ID_TRUE;
    }
}

/**************************************************************************
 * Description :
 *      �ش� WAPage�� discard���� �ʵ��� �����Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWPID          - WAPageID
 ***************************************************************************/
IDE_RC sdtWASegment::fixPage(sdtWASegment * aWASegment,
                             scPageID       aWPID)
{
    sdtWCB   * sWCBPtr;

    sWCBPtr = getWCB( aWASegment, aWPID );

    sWCBPtr->mFix ++;
    IDE_ERROR( sWCBPtr->mFix > 0 );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 *      FixPage�� Ǯ�� Discard�� ����Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWPID          - WAPageID
 ***************************************************************************/
IDE_RC sdtWASegment::unfixPage(sdtWASegment * aWASegment,
                               scPageID       aWPID)
{
    sdtWCB   * sWCBPtr;

    sWCBPtr = getWCB( aWASegment, aWPID );

    sWCBPtr->mFix --;
    IDE_ERROR( sWCBPtr->mFix >= 0 );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 *     Free, �� Write�� �ʿ� ���� ��Ȱ�� ������ ���������� Ȯ���Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWPID          - WAPageID
 ***************************************************************************/
idBool sdtWASegment::isFreeWAPage(sdtWASegment * aWASegment,
                                  scPageID       aWPID)
{
    sdtWAPageState   sState = getWAPageState( aWASegment, aWPID );

    if ( ( sState == SDT_WA_PAGESTATE_NONE ) ||
         ( sState == SDT_WA_PAGESTATE_INIT ) ||
         ( sState == SDT_WA_PAGESTATE_CLEAN ) )
    {
        return ID_TRUE;
    }
    else
    {
        /* nothing to do */
    }
    return ID_FALSE;
}

/**************************************************************************
 * Description :
 *     Page�� ���¸� �˾Ƴ���.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWPID          - WAPageID
 ***************************************************************************/
sdtWAPageState sdtWASegment::getWAPageState(sdtWASegment   * aWASegment,
                                            scPageID         aWPID )
{
    return getWAPageState( getWCB( aWASegment, aWPID ) );
}

/**************************************************************************
 * Description :
 *     Page�� ���¸� �˾Ƴ���.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWPID          - WAPageID
 * <OUT>
 * aState         - Page�� ����
 ***************************************************************************/
sdtWAPageState sdtWASegment::getWAPageState( sdtWCB         * aWCBPtr )
{
    if ( ( aWCBPtr->mWPState != SDT_WA_PAGESTATE_IN_FLUSHQUEUE ) &&
         ( aWCBPtr->mWPState != SDT_WA_PAGESTATE_WRITING ) )
    {
        /* ���ü� �̽��� ���� ���´� �׳� �����´�.
         *
         * �ֳ��ϸ�, ���ü� �̽��� �ֵ��� �����ϴ� ���� ServiceThread��
         * writeNPage��. ���� ServiceThread�ڽ��� �� getWAPageState��
         * �ҷ��� ���� ����.
         *
         * Flusher�� checkAndSetWAPageState �� ���¸� Ȯ���ϱ⿡ ���� ����.*/
        return aWCBPtr->mWPState;
    }
    else
    {
        return (sdtWAPageState)idCore::acpAtomicGet32( &aWCBPtr->mWPState );
    }
}

/**************************************************************************
 * Description :
 *     Page�� ���¸� �����Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWPID          - WAPageID
 * aState         - ������ Page�� ����
 ***************************************************************************/
IDE_RC sdtWASegment::setWAPageState(sdtWASegment * aWASegment,
                                    scPageID       aWPID,
                                    sdtWAPageState aState )
{
    setWAPageState( getWCB( aWASegment, aWPID ), aState );

    return IDE_SUCCESS;
}

/**************************************************************************
 * Description :
 *     Page�� ���¸� �����Ѵ�.
 *
 * <IN>
 * aWCBPtr        - ��� WCB
 * aState         - ������ Page�� ����
 ***************************************************************************/
void sdtWASegment::setWAPageState( sdtWCB         * aWCBPtr,
                                   sdtWAPageState   aState )
{
    if ( ( aWCBPtr->mWPState != SDT_WA_PAGESTATE_IN_FLUSHQUEUE ) &&
         ( aWCBPtr->mWPState != SDT_WA_PAGESTATE_WRITING ) )
    {
        /* ���ü� �̽��� ���� ���´� �׳� �����´�.
         *
         * �ֳ��ϸ�, ���ü� �̽��� �ֵ��� �����ϴ� ���� ServiceThread��
         * writeNPage��. ���� ServiceThread�ڽ��� �� getWAPageState��
         * �ҷ��� ���� ����.
         *
         * Flusher�� checkAndSetWAPageState �� ���¸� Ȯ���ϱ⿡ ���� ����.*/
        aWCBPtr->mWPState = aState;
    }
    else
    {
        (void)idCore::acpAtomicSet32( &aWCBPtr->mWPState, aState );
    }
}

/**************************************************************************
 * Description :
 *     �ش� WCB�� �̿� WCB�� ���´�.
 *
 * <IN>
 * aWCBPtr        - ��� WCB
 * aWPID          - �̿� ���������� ������ ����
 * <OUT>
 * aWCBPtr        - ���� WCB
 ***************************************************************************/
IDE_RC sdtWASegment::getSiblingWCBPtr( sdtWCB  * aWCBPtr,
                                       UInt      aWPID,
                                       sdtWCB ** aTargetWCBPtr )
{
    ( *aTargetWCBPtr ) = aWCBPtr - aWPID;

    IDE_ERROR( (*aTargetWCBPtr)->mWPageID == aWCBPtr->mWPageID - aWPID );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 *     Page�� ���¸� Ȯ���ϰ�, �ڽ��� ������ ���°� ������ �����Ѵ�.
 *
 * <IN>
 * aWCBPtr        - ��� WCB
 * aChkState      - ������ �������� ����
 * aAftState      - ������ ���� ��� ������ �������� ����
 * <OUT>
 * aBefState      - �����ϱ� �� �������� ��¥ ����
 ***************************************************************************/
void sdtWASegment::checkAndSetWAPageState( sdtWCB         * aWCBPtr,
                                           sdtWAPageState   aChkState,
                                           sdtWAPageState   aAftState,
                                           sdtWAPageState * aBefState)
{
    *aBefState = (sdtWAPageState)idCore::acpAtomicCas32( &aWCBPtr->mWPState,
                                                         aAftState,
                                                         aChkState );
}

/**************************************************************************
 * Description :
 *     Page �ּҷ� HashValue����
 *
 * <IN>
 * aNSpaceID,aNPID- Page�ּ�
 ***************************************************************************/
UInt sdtWASegment::getNPageHashValue( sdtWASegment * aWASegment,
                                      scSpaceID      aNSpaceID,
                                      scPageID       aNPID )
{
    return ( aNSpaceID + aNPID ) % aWASegment->mNPageHashBucketCnt;
}

/**************************************************************************
 * Description :
 *     NPageHash�� ������ �ش� NPage�� ���� WPage�� ã�Ƴ�
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aNSpaceID,aNPID- Page�ּ�
 ***************************************************************************/
sdtWCB * sdtWASegment::findWCB( sdtWASegment * aWASegment,
                                scSpaceID      aNSpaceID,
                                scPageID       aNPID )
{
    UInt       sHashValue = getNPageHashValue( aWASegment, aNSpaceID, aNPID );
    sdtWCB   * sWCBPtr;

    sWCBPtr = aWASegment->mNPageHashPtr[ sHashValue ];

    while ( sWCBPtr != NULL )
    {
        /* TableSpaceID�� ���ƾ� �ϸ�, HashValue�� �¾ƾ� �� */
        IDE_DASSERT( sWCBPtr->mNSpaceID == aWASegment->mSpaceID );
        IDE_DASSERT( getNPageHashValue( aWASegment, sWCBPtr->mNSpaceID, sWCBPtr->mNPageID )
                     == sHashValue );

        if ( sWCBPtr->mNPageID == aNPID )
        {
            IDE_DASSERT( sWCBPtr->mNSpaceID == aNSpaceID );
            break;
        }
        else
        {
            /* nothing to do */
        }

        sWCBPtr  = sWCBPtr->mNextWCB4Hash;
    }

    return sWCBPtr;
}

/**************************************************************************
 * Description :
 *     WA������ GRID�� NGRID�� ������
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWGRID         - ������ ���� WGRID
 * <OUT>
 * aNGRID         - ���� ��
 ***************************************************************************/
void sdtWASegment::convertFromWGRIDToNGRID( sdtWASegment * aWASegment,
                                            scGRID         aWGRID,
                                            scGRID       * aNGRID )
{
    sdtWCB         * sWCBPtr;
    sdtWAPageState   sWAPageState;

    if ( SC_MAKE_SPACE( aWGRID ) == SDT_SPACEID_WORKAREA )
    {
        /* WGRID���� �ϰ�, InMemoryGroup�ƴϾ�� �� */
        sWCBPtr = getWCB( aWASegment,SC_MAKE_PID( aWGRID ) );
        sWAPageState = getWAPageState( sWCBPtr );

        /* NPID�� Assign �Ǿ�߸� �ǹ� ���� */
        if ( ( sWAPageState != SDT_WA_PAGESTATE_NONE ) &&
             ( sWAPageState != SDT_WA_PAGESTATE_INIT ) )
        {
            SC_MAKE_GRID( *aNGRID,
                          sWCBPtr->mNSpaceID,
                          sWCBPtr->mNPageID,
                          SC_MAKE_OFFSET( aWGRID ) );
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
}

/**************************************************************************
 * Description :
 *     WASegment������ �ش� NPage�� ã�´�.
 *     �������� ������, Group���� VictimPage�� ã�Ƽ� �о �����ش�.
 *     �׷��� ã�Ƽ� WCB�� ��ȯ�Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWAGroupID     - ��� Group ID
 * aNSpaceID,aNPID- Page�ּ�
 * <OUT>
 * aRetWCB        - ���� ��
 ***************************************************************************/
IDE_RC sdtWASegment::getWCBByNPID(sdtWASegment * aWASegment,
                                  sdtWAGroupID   aWAGroupID,
                                  scSpaceID      aNSpaceID,
                                  scPageID       aNPID,
                                  sdtWCB      ** aRetWCB )
{
    scPageID   sWPID=SD_NULL_PID;

    if ( ( aWASegment->mHintWCBPtr->mNPageID == aNPID ) )
    {
        IDE_DASSERT( aWASegment->mHintWCBPtr->mNSpaceID == aNSpaceID );

        (*aRetWCB) = aWASegment->mHintWCBPtr;
    }
    else
    {
        (*aRetWCB) = findWCB( aWASegment, aNSpaceID, aNPID );

        if ( (*aRetWCB) == NULL )
        {
            /* WA�� �������� �������� ���� ���, �� �������� Group�� �Ҵ�޾�
             * Read��*/
            IDE_TEST( getFreeWAPage( aWASegment,
                                     aWAGroupID,
                                     &sWPID )
                      != IDE_SUCCESS );
            IDE_ERROR( sWPID != SD_NULL_PID );

            IDE_TEST( readNPage( aWASegment,
                                 aWAGroupID,
                                 sWPID,
                                 aNSpaceID,
                                 aNPID )
                      != IDE_SUCCESS );
            (*aRetWCB) = getWCB( aWASegment, sWPID ) ;
        }
        else
        {
            /* nothing to do */
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    ideLog::log( IDE_DUMP_0,
                 "aWAGroupID : %u\n"
                 "aNSpaceID  : %u\n"
                 "aNPID      : %u\n"
                 "sWPID      : %u\n",
                 aWAGroupID,
                 aNSpaceID,
                 aNPID,
                 sWPID );

    IDE_DASSERT( 0 );

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * GRID�� �������� WPID�� �����´�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWAGroupID     - ��� Group ID
 * aGRID          - ������ ���� GRID
 * <OUT>
 * aRetPID        - ��� WPID
 ***************************************************************************/
IDE_RC sdtWASegment::getWPIDFromGRID(sdtWASegment * aWASegment,
                                     sdtWAGroupID   aWAGroupID,
                                     scGRID         aGRID,
                                     scPageID     * aRetPID )
{
    sdtWCB   * sWCBPtr;

    if ( SC_MAKE_SPACE( aGRID ) == SDT_SPACEID_WORKAREA )
    {
        (*aRetPID) = SC_MAKE_PID( aGRID );
    }
    else
    {
        IDE_TEST( sdtWASegment::getWCBByNPID( aWASegment,
                                              aWAGroupID,
                                              SC_MAKE_SPACE( aGRID ),
                                              SC_MAKE_PID( aGRID ),
                                              &sWCBPtr )
                  != IDE_SUCCESS );
        (*aRetPID) = sWCBPtr->mWPageID;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 *     DirtyPage�� �����Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWPID          - ��� Page
 ***************************************************************************/
IDE_RC sdtWASegment::setDirtyWAPage( sdtWASegment * aWASegment,
                                     scPageID       aWPID )
{
    sdtWCB         * sWCBPtr;
    sdtWAPageState   sWAPageState;

    sWCBPtr      = getWCB( aWASegment, aWPID );
    sWAPageState = getWAPageState( sWCBPtr );

    if ( ( sWAPageState != SDT_WA_PAGESTATE_NONE ) &&
         ( sWAPageState != SDT_WA_PAGESTATE_INIT ) )
    {
        if ( sWAPageState == SDT_WA_PAGESTATE_CLEAN )
        {
            IDE_TEST( setWAPageState( aWASegment,
                                      aWPID,
                                      SDT_WA_PAGESTATE_DIRTY )
                      != IDE_SUCCESS );

        }
        else
        {
            /* Redirty */
            checkAndSetWAPageState( sWCBPtr,
                                    SDT_WA_PAGESTATE_WRITING,
                                    SDT_WA_PAGESTATE_DIRTY,
                                    &sWAPageState );
        }
    }
    else
    {
        IDE_DASSERT( isInMemoryGroup( aWASegment,
                                      findGroup( aWASegment, aWPID ) )
                     == ID_TRUE );
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 *     WGRID�� �������� �ش� Slot�� �����͸� ��ȯ�Ѵ�.
 *
 * <IN>
 * aWAPagePtr     - ��� WAPage�� ������
 * aGroupID       - ��� GroupID
 * aGRID          - ��� GRID ��
 * <OUT>
 * aNSlotPtr      - ���� ����
 * aIsValid       - Slot��ȣ�� �ùٸ���? ( Page�� �������, overflow����� )
 ***************************************************************************/
IDE_RC sdtWASegment::getPagePtrByGRID( sdtWASegment  * aWASegment,
                                       sdtWAGroupID    aGroupID,
                                       scGRID          aGRID,
                                       UChar        ** aNSlotPtr,
                                       idBool        * aIsValid )

{
    UChar         * sWAPagePtr = NULL;
    UInt            sSlotCount = 0;
    scPageID        sWPID      = SC_NULL_PID;

    IDE_TEST( getPage( aWASegment,
                       aGroupID,
                       aGRID,
                       &sWPID,
                       &sWAPagePtr,
                       &sSlotCount )
              != IDE_SUCCESS );
    getSlot( sWAPagePtr,
             sSlotCount,
             SC_MAKE_OFFSET( aGRID ),
             aNSlotPtr,
             aIsValid );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 *     getPage�� �����ϵ�, �ش� Page�� Fix��Ű�⵵ �Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aGroupID       - ��� GroupID
 * aGRID          - ��� GRID ��
 * <OUT>
 * aWPID          - ���� WPID
 * aWAPagePtr     - ���� WAPage�� ��ġ
 * aSlotCount     - �ش� page�� SlotCount
 ***************************************************************************/
IDE_RC sdtWASegment::getPageWithFix( sdtWASegment  * aWASegment,
                                     sdtWAGroupID    aGroupID,
                                     scGRID          aGRID,
                                     UInt          * aWPID,
                                     UChar        ** aWAPagePtr,
                                     UInt          * aSlotCount )
{
    if ( *aWPID != SC_NULL_PID )
    {
        IDE_DASSERT( *aWAPagePtr != NULL );
        IDE_TEST( unfixPage( aWASegment, *aWPID ) != IDE_SUCCESS );
    }
    else
    {
        /* nothing to do */
    }

    IDE_TEST( getPage( aWASegment,
                       aGroupID,
                       aGRID,
                       aWPID,
                       aWAPagePtr,
                       aSlotCount )
              != IDE_SUCCESS );

    IDE_TEST( fixPage( aWASegment, *aWPID ) != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 *     GRID�� �������� Page�� ���� ������ ��´�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aGroupID       - ��� GroupID
 * aGRID          - ��� GRID ��
 * <OUT>
 * aWPID          - ���� WPID
 * aWAPagePtr     - ���� WAPage�� ��ġ
 * aSlotCount     - �ش� page�� SlotCount
 ***************************************************************************/
IDE_RC sdtWASegment::getPage( sdtWASegment  * aWASegment,
                              sdtWAGroupID    aGroupID,
                              scGRID          aGRID,
                              UInt          * aWPID,
                              UChar        ** aWAPagePtr,
                              UInt          * aSlotCount )
{
    sdtWCB    * sWCBPtr;
    scSpaceID   sSpaceID;
    scPageID    sPageID;

    IDE_DASSERT( SC_GRID_IS_NULL( aGRID ) == ID_FALSE );

    sSpaceID = SC_MAKE_SPACE( aGRID );
    sPageID  = SC_MAKE_PID( aGRID );

    if ( isWorkAreaTbsID( sSpaceID ) == ID_TRUE )
    {
        (*aWPID)      = sPageID;
        (*aWAPagePtr) = getWAPagePtr( aWASegment, aGroupID, *aWPID );
    }
    else
    {
        IDE_TEST_RAISE( sctTableSpaceMgr::isTempTableSpace( sSpaceID ) != ID_TRUE,
                        tablespaceID_error );

        IDE_TEST( getWCBByNPID( aWASegment,
                                aGroupID,
                                sSpaceID,
                                sPageID,
                                &sWCBPtr )
                  != IDE_SUCCESS );
        (*aWPID)      = sWCBPtr->mWPageID;
        (*aWAPagePtr) = sWCBPtr->mWAPagePtr;
    }

    IDE_TEST( sdtTempPage::getSlotCount( *aWAPagePtr,
                                         aSlotCount )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION( tablespaceID_error );
    {
        ideLog::log( IDE_SM_0,
                      "[FAILURE] Fatal error during fetch disk temp table"
                      "(Tablespace ID : %"ID_UINT32_FMT", Page ID : %"ID_UINT32_FMT")",
                      sSpaceID,
                      sPageID );
        IDE_SET( ideSetErrorCode( smERR_ABORT_INTERNAL ) );
    } 
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}
/**************************************************************************
 * Description :
 *     getPage�� ������ ��������  Slot�� ��´�.
 *
 * <IN>
 * aWAPagePtr     - ��� WAPage�� ������
 * aSlotCount     - ��� Page�� Slot ����
 * aSlotNo        - ���� ���� ��ȣ
 * <OUT>
 * aNSlotPtr      - ���� ����
 * aIsValid       - Slot��ȣ�� �ùٸ���? ( Page�� �������, overflow����� )
 ***************************************************************************/
void sdtWASegment::getSlot( UChar         * aWAPagePtr,
                            UInt            aSlotCount,
                            UInt            aSlotNo,
                            UChar        ** aNSlotPtr,
                            idBool        * aIsValid )
{
    if ( aSlotCount > aSlotNo )
    {
        *aNSlotPtr = aWAPagePtr +
            sdtTempPage::getSlotOffset( aWAPagePtr, (scSlotNum)aSlotNo );
        *aIsValid = ID_TRUE;
    }
    else
    {
        /* Slot�� �Ѿ ���.*/
        *aNSlotPtr    = aWAPagePtr;
        *aIsValid     = ID_FALSE;
    }
}

/**************************************************************************
 * Description :
 *     WPID�� �������� WCB�� ��ġ�� ��ȯ�Ѵ�.
 *
 * <Warrning>
 * Flusher������ �ݵ�� getWCB, getWAPagePtr���� Hint�� ����ϴ� �Լ���
 * ����ϸ� �ȵȴ�.
 * �ش� �Լ����� ServiceThread ȥ�� �����Ѵٴ� ������ �ֱ� ������,
 * Flusher�� ����ϸ� �ȵȴ�. getWCBInternal������ ó���ؾ� �Ѵ�.
 * <Warrning>
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWPID          - ��� Page
 ***************************************************************************/
sdtWCB * sdtWASegment::getWCB(sdtWASegment  * aWASegment,
                              scPageID        aWPID )
{
    if ( aWASegment->mHintWCBPtr->mWPageID != aWPID )
    {
        aWASegment->mHintWCBPtr = getWCBInternal( aWASegment, aWPID );
    }
    else
    {
        /* nothing to do */
    }

    return aWASegment->mHintWCBPtr;
}

/**************************************************************************
 * Description :
 *     WPID�� �������� WAPage�� �����͸� ��ȯ�Ѵ�.
 *
 * <Warrning>
 * Flusher������ �ݵ�� getWCB, getWAPagePtr���� Hint�� ����ϴ� �Լ���
 * ����ϸ� �ȵȴ�.
 * �ش� �Լ����� ServiceThread ȥ�� �����Ѵٴ� ������ �ֱ� ������,
 * Flusher�� ����ϸ� �ȵȴ�. getWCBInternal������ ó���ؾ� �Ѵ�.
 * <Warrning>
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aGroupID       - ��� GroupID
 * aWPID          - ��� Page
 ***************************************************************************/
UChar * sdtWASegment::getWAPagePtr(sdtWASegment  * aWASegment,
                                   sdtWAGroupID    aGroupID,
                                   scPageID        aWPID )
{
    if ( aWASegment->mHintWCBPtr->mWPageID != aWPID )
    {
        IDE_ASSERT( moveLRUListToTop( aWASegment,
                                      aGroupID,
                                      aWPID )
                    == IDE_SUCCESS );

        aWASegment->mHintWCBPtr = getWCBInternal( aWASegment, aWPID );
    }
    else
    {
        /* nothing to do */
    }

    return aWASegment->mHintWCBPtr->mWAPagePtr;
}

/**************************************************************************
 * Description :
 *     WPID�� �������� WCB�� ��ġ�� ��ȯ�Ѵ�.
 *     ��, Hint�� �̿����� �ʴ´�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWPID          - ��� Page
 ***************************************************************************/
sdtWCB * sdtWASegment::getWCBInternal( sdtWASegment  * aWASegment,
                                       scPageID        aWPID )
{
    return  getWCBInExtent( getWAExtentPtr( aWASegment,
                                            getExtentIdx(aWPID) ),
                            getPageSeqInExtent(aWPID) );
}

/**************************************************************************
 * Description :
 *     Extent������ WPage�� ��ġ�� ��ȯ��.
 *
 * <IN>
 * aExtentPtr     - Extent�� ��ġ
 * aIdx           - Extent�� Frame�� ��ȣ
 ***************************************************************************/
UChar* sdtWASegment::getFrameInExtent( UChar * aExtentPtr,
                                       UInt    aIdx )
{
    IDE_DASSERT( aIdx <= SDT_WAEXTENT_PAGECOUNT );

    return aExtentPtr + (aIdx * SD_PAGE_SIZE);
}

/**************************************************************************
 * Description :
 *     Extent������ WCB�� ��ġ�� ��ȯ��.
 *
 * <IN>
 * aExtentPtr     - Extent�� ��ġ
 * aIdx           - Extent�� WCB�� ��ȣ
 ***************************************************************************/
sdtWCB* sdtWASegment::getWCBInExtent( UChar * aExtentPtr,
                                      UInt    aIdx )
{
    /* Extent�� ������ Frame �ڿ����� WCB�� ��ġ�� */
    sdtWCB * sWCBStartPtr =
        (sdtWCB*)getFrameInExtent( aExtentPtr, SDT_WAEXTENT_PAGECOUNT );

    IDE_DASSERT( aIdx < SDT_WAEXTENT_PAGECOUNT );

    return &sWCBStartPtr[ aIdx ];
}

/**************************************************************************
 * Description :
 *     WAExtent�� ��ġ�� ��ȯ�Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aIdx           - Extent�� ��ȣ
 * <OUT>
 * aWAExtenntPtr  - ���� Extent�� ������ ��ġ
 ***************************************************************************/
UChar * sdtWASegment::getWAExtentPtr(sdtWASegment  * aWASegment,
                                     UInt            aIdx )
{
    UInt     sOffset;
    UChar ** sSlotPtr;

    IDE_DASSERT( aIdx < aWASegment->mWAExtentCount );

    sOffset = ID_SIZEOF( sdtWASegment ) + (ID_SIZEOF( UChar* ) * aIdx);

    /* 0�� Extent�� ���� �Ѵ�. */
    IDE_DASSERT( getExtentIdx(sOffset / SD_PAGE_SIZE) == 0 );
    IDE_DASSERT( ( sOffset % ID_SIZEOF( UChar *) )  == 0 );

    /* WASegment�� �ݵ�� WAExtent�� ���� ���ʿ� ��ġ�ǰ�,
     * 0�� Extent���� ��� ��ġ�Ǿ� ���Ӽ��� ������ ������
     * Ž���� �����ϴ� */
    sSlotPtr = (UChar**)(((UChar*)aWASegment) + sOffset);

    IDE_DASSERT( *sSlotPtr != NULL );
    IDE_DASSERT( ( (vULong)*sSlotPtr % SDT_WAEXTENT_SIZE ) == 0 );

    return *sSlotPtr;
}


#endif //_O_SDT_WA_SEGMENT_H_
