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
 * $Id: $
 * Description :
 **********************************************************************/

#ifndef _O_SDT_HASH_DEF_H_
#define _O_SDT_HASH_DEF_H_ 1

#include <sdtDef.h>

#define SDT_HASH_UNIQUE      (1)
#define SDT_HASH_NORMAL      (0)

#define SDT_MIN_HASHSLOT_EXTENTCOUNT (2)
#define SDT_MIN_SUBHASH_EXTENTCOUNT  (1)
#define SDT_MIN_HASHROW_EXTENTCOUNT  (3)
#define SDT_MIN_HASH_EXTENTCOUNT     (6)
#define SDT_MIN_HASH_SIZE            (SDT_MIN_HASH_EXTENTCOUNT * SDT_WAEXTENT_SIZE)

typedef struct sdtHashGroup
{
    scPageID   mBeginWPID;
    scPageID   mEndWPID;
    scPageID   mReuseSeqWPID;
} sdtHashGroup;

typedef struct sdtHashSegHdr
{
    UChar                mWorkType;
    UChar                mIsInMemory;    /*쫓겨난 Page가 있는가?*/
    scSpaceID            mSpaceID;

    UInt                 mHashSlotCount; /* Slot 개수 */

    UShort               mUnique;
    UShort               mOpenCursorType;

    scPageID             mEndWPID;

    UInt                 mMaxWAExtentCount;

    UInt                 mNPageHashBucketCnt;
    sdtWCB            ** mNPageHashPtr;
    
    UInt                 mNPageCount;             /*할당한 NPage의 개수 */
    UInt                 mExtPageCount;           /*Extent의 크기(extent내 page count)*/

    sdtNExtFstPIDList    mNExtFstPIDList4Row;     /*NormalExtent들을 보관하는 Map*/
    sdtNExtFstPIDList    mNExtFstPIDList4SubHash; /*NormalExtent들을 보관하는 Map*/

    sdtWCB            *  mUsedWCBPtr;
    /* 마지막으로 접근한 WAPage. Hint 및 Dump하기 위해 사용됨 */
    /* getNPageToWCB, getWAPagePtr, getWCB 용 Hint */
    sdtWCB             * mInsertHintWCBPtr;

    // Hash 0 ~ | Row ~ | Fetch ~ | sub Hash ~ | end

    sdtHashGroup         mInsertGrp;
    sdtHashGroup         mFetchGrp;
    sdtHashGroup         mSubHashGrp;
    sdtWCB             * mSubHashWCBPtr;

    sdtWAExtent        * mNxtFreeWAExtent;
    sdtWAExtent        * mCurFreeWAExtent;
    sdtWAExtentInfo      mWAExtentInfo;  
    UInt                 mCurrFreePageIdx;
    UInt                 mAllocWAPageCount;

    UInt                 mSubHashPageCount;
    UInt                 mSubHashBuildCount;
    UInt                 mHashSlotPageCount;
    UInt                 mHashSlotAllocPageCount;

    idvSQL             * mStatistics;
    smiTempTableStats  * mStatsPtr;

    /* 페이지 탐색용 Hash*/
    sdtWCB               mWCBMap[1];

} sdtHashSegHdr;

#define SDT_HASH_SEGMENT_HEADER_SIZE (ID_SIZEOF(sdtHashSegHdr) - ID_SIZEOF(sdtWCB))

typedef struct sdtHashPageHdr
{
    scPageID mSelfNPID;
    UShort   mType;
    UShort   mFreeOffset;
} sdtHashPageHdr;

/**************************************************************************
 * HashTempRowPiece는 다음과 같이 구성된다.
 *
 * +-------------------------------+.+--------------+.+--------------+.+-------------------------+
 * + RowPieceHeader                +.+ ChildRowInfo +.+ NextRowInfo  +.+ ColumnValues.(mtdValues)
 * +----+-----+------+---------+---+.+--------------+.+--------------+.+-------------------------+
 * |flag|IsRow|ValLen|HashValue|hit|.|PID|Offset|Ptr|.|PID|Offset|Ptr|.|ColumnValue|...         
 * +----+-----+------+---------+---+.+--------------+.+--------------+.+-------------------------+
 *
 * Base는 모든 rowPiece를 가진다.
 **************************************************************************/

#define SDT_HASH_ROW_HEADER (0x00)

/* TRPInfo(TempRowPieceInfo)는 Runtime구조체로 Runtime상황에서 사용되며
 * 실제 Page상에는 sdtHashTRPHdr(TempRowPiece)와 Value가 기록된다.  */
typedef struct sdtHashTRPHdr
{
    /* Row에 대해 설명해주는 TR(TempRow)Flag.
     * (HashValue사용 여부, 이전번 삽입시 성공 여부, Column쪼개짐 여부 ) */
    UChar       mTRFlag;
    /* Row 시그니처. */
    UChar       mIsRow;
    /* RowHeader부분을 제외한. Value부분의 길이 */
    UShort      mValueLength;

    /* hashValue */
    UInt        mHashValue;

    /* hitSequence값 */
    UInt        mHitSequence;

    scPageID    mChildPageID;
    scOffset    mChildOffset;
    scOffset    mNextOffset;
    scPageID    mNextPageID;

    /* InMemory 인 경우는  Row Pointer
     * OutMemory 인 경우는 SubHash Pointer */
    sdtHashTRPHdr * mChildRowPtr;
    /* 이전번에 삽입된 RowPiece의 위치이다. 그런데 삽입은 역순으로 진행되기
     * 때문에 공간적으로는 다음번 RowPiece의 Pointer이다.
     * 즉 FirstRowPiece 후 뒤쪽 RowPiece들이 이 Pointer에 연결되어있다. */
    sdtHashTRPHdr * mNextRowPiecePtr;
} sdtHashTRPHdr;

typedef struct sdtHashInsertInfo
{
    sdtHashTRPHdr   mTRPHeader;

    UInt            mColumnCount;  /* Column개수 */
    smiTempColumn * mColumns;      /* Column정보 */
    UInt            mValueLength;  /* Value총 길이 */
    smiValue      * mValueList;    /* Value들 */
} sdtHashInsertInfo;

typedef struct sdtHashInsertResult
{
    sdtHashTRPHdr* mHeadRowpiecePtr;  /* 머리 Rowpiece의 Pointer */
    scPageID       mHeadPageID;       /* 머리 Rowpiece의 PageID */
    scOffset       mHeadOffset;       /* 머리 Rowpiece의 Offset */
    idBool         mComplete;         /* 삽입 성공하였는가 */
} sdtHashInsertResult;

typedef struct sdtHashScanInfo
{
    sdtHashTRPHdr  * mTRPHeader;

    UInt            mValueLength;    /* Value총 길이 */
    UInt            mFetchEndOffset; /* 여기 까지 fetch */
    UChar         * mValuePtr;       /* Value의 첫 위치*/
} sdtHashScanInfo;


/* sdtHashTRPHdr 참조 */
#define SDT_HASH_TR_HEADER_SIZE_FULL ( 40 )

#endif  // _O_SDT_HASH_DEF_H_
