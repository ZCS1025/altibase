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
 * $Id: sdtDef.h 79989 2017-05-15 09:58:46Z et16 $
 *
 * Description :
 *
 * - Dirty Page Buffer �ڷᱸ��
 *
 **********************************************************************/

#ifndef _O_SDT_DEF_H_
#define _O_SDT_DEF_H_ 1

#include <smDef.h>
#include <iduLatch.h>
#include <smuUtility.h>
#include <iduMutex.h>


/****************************************************************************
 * PROJ-2201 Innovation in sorting and hashing(temp)
 ****************************************************************************/
#define SDT_WAGROUPID_MAX        (4)    /* �ִ� �׷� ���� */

/*******************************
 * Group
 *****************************/
/* Common */
/* sdtTempRow::fetch�� SDT_WAGROUPID_NONE�� ����Ѵٴ� ����,
 * BufferMiss�� �ش� WAGroup�� ReadPage�Ͽ� �ø��� �ʰڴٴ� �� */
#define SDT_WAGROUPID_NONE     ID_UINT_MAX
#define SDT_WAGROUPID_INIT     (0)
/* Sort */
#define SDT_WAGROUPID_SORT     (1)
#define SDT_WAGROUPID_FLUSH    (2)
#define SDT_WAGROUPID_SUBFLUSH (3)
/* Sort IndexScan */
#define SDT_WAGROUPID_INODE    (1)
#define SDT_WAGROUPID_LNODE    (2)
/* Hash */
#define SDT_WAGROUPID_HASH     (1)
#define SDT_WAGROUPID_SUB      (2)
#define SDT_WAGROUPID_PARTMAP  (3)
/* Scan*/
#define SDT_WAGROUPID_SCAN     (1)

typedef UInt sdtWAGroupID;

/* INMEMORY -> DiscardPage�� ������� �ʴ´�. ���� ���ʺ��� �������� �Ҵ�
 *             ���ش�. WAMap���� ���ռ��� �ø��� �����̴�.
 * FIFO -> ���ʺ��� ������� �Ҵ����ش�. ���� ����ϸ�, ���ʺ��� �����Ѵ�.
 * LRU  -> ������� ������ �������� �Ҵ��Ѵ�. getWAPage�� �ϸ� Top���� �ø���.
 * */
typedef enum
{
    SDT_WA_REUSE_NONE,
    SDT_WA_REUSE_INMEMORY, /* REUSE�� ������� ������,�ڿ������� ������ �Ҵ�*/
    SDT_WA_REUSE_FIFO,     /* ���������� �������� ����� */
    SDT_WA_REUSE_LRU       /* LRU�� ���� �������� �Ҵ����� */
} sdtWAReusePolicy;

typedef enum
{
    SDT_WA_PAGESTATE_NONE,          /* Frame�� �ʱ�ȭ�Ǿ����� ����. */
    SDT_WA_PAGESTATE_INIT,          /* PageFrame�� �ʱ�ȭ�� ��. */
    SDT_WA_PAGESTATE_CLEAN,         /* Assign�ž����� ������ ������ */
    SDT_WA_PAGESTATE_DIRTY,         /* ������ ����Ǿ�����, Flush�õ��� ����*/
    SDT_WA_PAGESTATE_IN_FLUSHQUEUE, /* Flusher���� �۾��� �Ѱ��� */
    SDT_WA_PAGESTATE_WRITING        /* Flusher�� ������� */
} sdtWAPageState;

typedef struct sdtWAGroup
{
    sdtWAReusePolicy mPolicy;

    /* WAGroup�� ����.
     * ���⼭ End�� ������ ��밡���� PageID + 1 �̴�. */
    scPageID         mBeginWPID;
    scPageID         mEndWPID;
    /* Fifo �Ǵ� LRU ��å�� ���� ������ ��Ȱ���� ���� ����.
     * �������� �Ҵ��� WPID�� ������.
     *�� �� ������ ���������� �ѹ��� ��Ȱ����� ���� �������̱� ������,
     * �� �������� ��Ȱ���ϸ� �ȴ�. */
    scPageID         mReuseWPIDSeq;

    /* LRU�϶� LRU List�� ���ȴ� */
    scPageID         mReuseWPID1; /* �ֱٿ� ����� ������ */
    scPageID         mReuseWPID2; /* ������� ������ ��Ȱ�� ��� */

    /* ������ ������ �õ��� ������.
     * WPID�� ��Ī�Ǹ�, ���� Fix�Ǿ� �����ȴ�. unassign�Ǿ� �ٸ� ������
     * �� ��ü�Ǹ�, �����ϴ� ������ �߰��� ����� �����̴�. */
    scPageID         mHintWPID;

    /* WAGroup�� Map�� ������, �װ��� Group��ü�� �ϳ��� ū �������� ����
     * InMemory������ �����Ѵٴ� �ǹ��̴�. */
    void           * mMapHdr;
} sdtWAGroup;

/* WAMap�� Slot�� Ŀ���� 16Byte */
#define SDT_WAMAP_SLOT_MAX_SIZE  (16)

typedef enum
{
    SDT_WM_TYPE_UINT,
    SDT_WM_TYPE_GRID,
    SDT_WM_TYPE_EXTDESC,
    SDT_WM_TYPE_RUNINFO,
    SDT_WM_TYPE_POINTER,
    SDT_WM_TYPE_MAX
} sdtWMType;

typedef struct sdtWMTypeDesc
{
    sdtWMType     mWMType;
    const SChar * mName;
    UInt          mSlotSize;
    smuDumpFunc   mDumpFunc;
} sdtWMTypeDesc;

typedef struct sdtWAMapHdr
{
    void         * mWASegment;
    sdtWAGroupID   mWAGID;          /* Map�� ���� Group */
    scPageID       mBeginWPID;      /* WAMap�� ���۵Ǵ� PID */
    UInt           mSlotCount;      /* Slot ���� */
    UInt           mSlotSize;       /* Slot �ϳ��� ũ�� */
    UInt           mVersionCount;   /* Slot�� Versioning ���� */
    UInt           mVersionIdx;     /* ������ Version Index */
    sdtWMType      mWMType;         /* Map Slot�� ����(Pointer, GRID �� )*/
} sdtWAMapHdr;

typedef struct sdtWCB
{
    sdtWAPageState mWPState;
    SInt           mFix;
    idBool         mBookedFree; /* Free�� ����� */

    /* �� WAPage�� ����� NPage : ��ũ�� �����Ѵ�.  */
    scSpaceID      mNSpaceID;
    scPageID       mNPageID;

    /* WorkArea�󿡼��� PageID */
    scPageID       mWPageID;

    /* LRUList������ */
    scPageID       mLRUPrevPID;
    scPageID       mLRUNextPID;

    /* ������ Ž���� Hash�� ���� LinkPID */
    sdtWCB       * mNextWCB4Hash;

    /* �ڽ��� Page ������ */
    UChar        * mWAPagePtr;
} sdtWCB;

/* Flush�Ϸ��� Page�� ��� Queue */
typedef struct sdtWAFlushQueue
{
    SInt                 mFQBegin;  /* Queue�� Begin ���� */
    SInt                 mFQEnd;    /* Queue�� Begin ���� */
    idBool               mFQDone;   /* Flush ������ ����Ǿ�� �ϴ°�? */
    smiTempTableStats ** mStatsPtr;
    idvSQL             * mStatistics;

    sdtWAFlushQueue    * mNextTarget;   /* Flusher�� Queue ���� */
    UInt                 mWAFlusherIdx; /* ����� Flusher�� ID */
    void               * mWASegment;
    scPageID             mSlotPtr[ 1 ]; /* Flush�� �۾� Slot*/
} sdtWAFlushQueue;

typedef struct sdtWAFlusher
{
    UInt              mIdx;           /* Flusher�� ID */
    idBool            mRun;
    sdtWAFlushQueue * mTargetHead;    /* Flush�� ����List�� Head */
    iduMutex          mMutex;
} sdtWAFlusher;

/* Extent �ϳ��� Page���� 64���� ���� */
#define SDT_WAEXTENT_PAGECOUNT      (64)
#define SDT_WAEXTENT_PAGECOUNT_MASK (SDT_WAEXTENT_PAGECOUNT-1)

/* WGRID, NGRID ���� �����ϱ� ����, SpaceID�� ����� */
#define SDT_SPACEID_WORKAREA  ( ID_USHORT_MAX )     /*InMemory,WGRID */
#define SDT_SPACEID_WAMAP     ( ID_USHORT_MAX - 1 ) /*InMemory,WAMap Slot*/
/* Slot�� ���� ������� ���� ���� */
#define SDT_WASLOT_UNUSED     ( ID_UINT_MAX )

#define SDT_WAEXTENT_SIZE (((( ID_SIZEOF(sdtWCB) + SD_PAGE_SIZE ) * SDT_WAEXTENT_PAGECOUNT) + SD_PAGE_SIZE - 1 ) & \
                           ~( SD_PAGE_SIZE - 1 ) )

/* WorkArea ��ü�� ���� */
typedef enum
{
    SDT_WASTATE_SHUTDOWN,     /* ����Ǿ����� */
    SDT_WASTATE_INITIALIZING, /* ������ �غ��� */
    SDT_WASTATE_RUNNING,      /* ������ */
    SDT_WASTATE_FINALIZING    /* ������ �غ��� */
} sdtWAState;



#define SDT_TEMP_FREEOFFSET_BITSIZE (13)
#define SDT_TEMP_FREEOFFSET_BITMASK (0x1FFF)
#define SDT_TEMP_TYPE_SHIFT         (SDT_TEMP_FREEOFFSET_BITSIZE)

#define SDT_TEMP_SLOT_UNUSED      (ID_USHORT_MAX)

typedef enum
{
    SDT_TEMP_PAGETYPE_INIT,
    SDT_TEMP_PAGETYPE_INMEMORYGROUP,
    SDT_TEMP_PAGETYPE_SORTEDRUN,
    SDT_TEMP_PAGETYPE_LNODE,
    SDT_TEMP_PAGETYPE_INODE,
    SDT_TEMP_PAGETYPE_INDEX_EXTRA,
    SDT_TEMP_PAGETYPE_HASHPARTITION,
    SDT_TEMP_PAGETYPE_UNIQUEROWS,
    SDT_TEMP_PAGETYPE_ROWPAGE,
    SDT_TEMP_PAGETYPE_MAX
} sdtTempPageType;

typedef struct sdtTempPageHdr
{
    /* ���� 13bit�� FreeOffset, ���� 3bit�� Type���� ����� */
    ULong    mTypeAndFreeOffset;
    scPageID mPrevPID;
    scPageID mSelfPID;
    scPageID mNextPID;
    UShort   mSlotCount;
} sdtTempPageHdr;


/*****************************************************************************
 * PROJ-2201 Innovation in sorting and hashing(temp)
 *****************************************************************************/

#define SDT_TRFLAG_NULL              (0x00)
#define SDT_TRFLAG_HEAD              (0x01) /*Head RowPiece���� ����.*/
#define SDT_TRFLAG_NEXTGRID          (0x02) /*NextGRID�� ����ϴ°�?*/
#define SDT_TRFLAG_CHILDGRID         (0x04) /*ChildGRID�� ����ϴ°�?*/
#define SDT_TRFLAG_UNSPLIT           (0x10) /*�ɰ����� �ʵ��� ������*/
/* GRID�� �����Ǿ� �ִ°�? */
#define SDT_TRFLAG_GRID     ( SDT_TRFLAG_NEXTGRID | SDT_TRFLAG_CHILDGRID )


/**************************************************************************
 * TempRowPiece�� ������ ���� �����ȴ�.
 *
 * +-------------------------------+.+---------+--------+.-------------------------+
 * + RowPieceHeader                |.|GRID HEADER       |. ColumnValues.(mtdValues)|
 * +----+-----+------+---------+---+.+---------+--------+.-------------------------+
 * |flag|dummy|ValLen|HashValue|hit|.|ChildGRID|NextGRID|.ColumnValue|...
 * +----+-----+------+---------+---+.+---------+--------+.-------------------------+
 * <----------   BASE    ----------> <------Option------>
 *
 * Base�� ��� rowPiece�� ������.
 * NextGRID�� ChildGRID�� �ʿ信 ���� ���� �� �ִ�.
 * (������ ������ ������, ChildGRID�� FirstRowPiece�� �����Ѵ�. *
 **************************************************************************/

/* TRPInfo(TempRowPieceInfo)�� Runtime����ü�� Runtime��Ȳ���� ���Ǹ�
 * ���� Page�󿡴� sdtTRPHeader(TempRowPiece)�� Value�� ��ϵȴ�.  */
typedef struct sdtTRPHeader
{
    /* Row�� ���� �������ִ� TR(TempRow)Flag.
     * (HashValue��� ����, ������ ���Խ� ���� ����, Column�ɰ��� ���� ) */
    UChar       mTRFlag;
    UChar       mDummy;
    /* RowHeader�κ��� ������. Value�κ��� ���� */
    UShort      mValueLength;

    /* hashValue */
    UInt        mHashValue;

    /* hitSequence�� */
    ULong       mHitSequence;

    /******************************* Optional ****************************/
    /* IndexInternalNode�� UniqueTempHash������ �ش� Slot�� ChildRID��
     * ����ȴ�. */
    scGRID      mChildGRID;

    /* �������� ���Ե� RowPiece�� RID�̴�. �׷��� ������ �������� ����Ǳ�
     * ������ ���������δ� ������ RowPiece�� RID�̴�.
     * �� FirstRowPiece �� ���� RowPiece���� �� NextRID�� ����Ǿ��ִ�. */
    scGRID      mNextGRID;
} sdtTRPHeader;

typedef struct sdtTRPInfo4Insert
{
    sdtTRPHeader    mTRPHeader;

    UInt            mColumnCount;  /* Column���� */
    smiTempColumn * mColumns;      /* Column���� */
    UInt            mValueLength;  /* Value�� ���� */
    smiValue      * mValueList;    /* Value�� */
} sdtTRPInfo4Insert;

typedef struct sdtTRInsertResult
{
    scGRID   mHeadRowpieceGRID; /* �Ӹ� Rowpiece�� GRID */
    UChar  * mHeadRowpiecePtr;  /* �Ӹ� Rowpiece�� Pointer */
    scGRID   mTailRowpieceGRID; /* ���� Rowpiece�� Pointer */
    UInt     mRowPageCount;     /* Row�����ϴµ� ����� page����*/
    idBool   mAllocNewPage;     /* �� Page�� �Ҵ��Ͽ��°�? */
    idBool   mComplete;         /* ���� �����Ͽ��°� */
} sdtTRInsertResult;

typedef struct sdtTRPInfo4Select
{
    sdtTRPHeader  * mSrcRowPtr;
    sdtTRPHeader  * mTRPHeader;
    /* Chainig Row�ϰ��, HeadRow�� unfix�� �� �ֱ⿡ �����ص� */
    sdtTRPHeader    mTRPHeaderBuffer;

    UInt            mValueLength; /* Value�� ���� */
    UChar        *  mValuePtr;    /* Value�� ù ��ġ*/
} sdtTRPInfo4Select;


/* �ݵ�� ��ϵǾ� �ϴ� �׸��. ( sdtTRPHeader ���� )
 * mTRFlag, mValueLength, mHashValue, mHitSequence,
 * (1) + (1) + (2) + (4) + (8) = 16 */
#define SDT_TR_HEADER_SIZE_BASE ( 16 )

/* �߰��� �ɼų��� �׸�� ( sdtTRPHeader ���� )
 * Base + mNextGRID + mChildGRID
 * (16) + (8 + 8) */
#define SDT_TR_HEADER_SIZE_FULL ( SDT_TR_HEADER_SIZE_BASE + 16 )

/* RID�� ������ 32Byte */
#define SDT_TR_HEADER_SIZE(aFlag)  ( ( aFlag & SDT_TRFLAG_GRID )  ?                             \
                                     SDT_TR_HEADER_SIZE_FULL : SDT_TR_HEADER_SIZE_BASE )



#endif  // _O_SDT_DEF_H_
