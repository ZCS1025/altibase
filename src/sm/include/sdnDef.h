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
 * $Id: sdnDef.h 29386 2008-11-18 06:52:08Z upinel9 $
 **********************************************************************/

#ifndef _O_SDN_DEF_H_
# define _O_SDN_DEF_H_ 1

# include <idl.h>
# include <idpULong.h>
# include <idpUInt.h>
# include <smDef.h>
# include <smnDef.h>

#define SDN_CTS_MAX_KEY_CACHE  (4)
#define SDN_CTS_KEY_CACHE_NULL (0)


#define SDN_RUNTIME_PARAMETERS                                           \
    SMN_RUNTIME_PARAMETERS                                               \
                                                                         \
    iduLatch       mLatch; /* FOR SMO Operation */                       \
                                                                         \
    scSpaceID      mTableTSID;                                           \
    scSpaceID      mIndexTSID;                                           \
    sdRID          mMetaRID;                                             \
    smOID          mTableOID;                                            \
    UInt           mIndexID;                                             \
                                                                         \
    /*  PROJ-1671 Bitmap-based Tablespace And Segment Space Management*/ \
    /* Segment Handle : Segment RID �� Semgnet Cache */                  \
    sdpSegmentDesc mSegmentDesc;                                         \
                                                                         \
    ULong          mSmoNo; /* �ý��� startup�ÿ� 1�� ����(0 �ƴ�)  */    \
                           /* 0���� ���� startup�ÿ� node�� ��ϵ� */    \
                           /* SmoNo�� reset�ϴµ� ����           */    \
                                                                         \
    idBool         mIsUnique;                                            \
    idBool         mIsNotNull; /*PK�� NULL�� ������ ����(BUG-17762).*/   \
    idBool         mLogging;                                             \
    /* BUG-17957 */                                                      \
    /* index run-time header�� creation option(logging, force) �߰�*/    \
    idBool              mIsCreatedWithLogging;                           \
    idBool              mIsCreatedWithForce;                             \
                                                                         \
    smLSN               mCompletionLSN;

 /* BUG-25279     Btree for spatial�� Disk Btree�� �ڷᱸ�� �� �α� �и� 
  * Btree For Spatial�� �Ϲ� Disk Btree�� ���屸���� �и��ȴ�. ������ Disk Index�ν�
  * ���������� ������ �ϴ� ��Ÿ�� ��� �׸��� �����Ѵ�. �̸� ������ ���� �����Ѵ�. */
typedef struct sdnRuntimeHeader
{
    SDN_RUNTIME_PARAMETERS
} sdnRuntimeHeader;

typedef struct sdnCTS
{
    smSCN       mCommitSCN;     // Commit SCN or Begin SCN
    smSCN       mNxtCommitSCN;  // Commit SCN of the chained CTS
    scPageID    mTSSlotPID;     // TSS page id
    scSlotNum   mTSSlotNum;     // TSS slotnum 
    UChar       mState;         // CTS State
    UChar       mDummy;         // BUG-44757 mChaining�� �� �̻� ������� �����Ƿ� �����ؾ� ������
                                //           ���� DB�� ȣȯ���� ���Ͽ� dummy�� ������ ����Ӵϴ�.
    scPageID    mUndoPID;       /* undo page id of the chained CTS (�̰��� Ȯ���Ͽ� chaining ���θ� Ȯ����) */
    scSlotNum   mUndoSlotNum;   /* undo slotnum of the chained CTS (�̰��� Ȯ���Ͽ� chaining ���θ� Ȯ����) */
    UShort      mRefCnt;        /* �� CTS�� �����ϴ� key�� ���� */
    UShort      mRefKey[ SDN_CTS_MAX_KEY_CACHE ];
} sdnCTS;

typedef struct sdnCTLayerHdr
{
    UChar       mCount;
    UChar       mUsedCount;
    UChar       mDummy[6];
} sdnCTLayerHdr;

typedef struct sdnCTL
{
    UChar       mCount;
    UChar       mUsedCount;
    UChar       mDummy[6];
    sdnCTS      mArrCTS[1];
} sdnCTL;

#define SDN_CTS_NONE          (0)  // 'N': �ʱ� ����
#define SDN_CTS_UNCOMMITTED   (1)  // 'U': �������� ���� ���� ����(Ŀ�� ��Ȯ��)
#define SDN_CTS_STAMPED       (2)  // 'S': ���������� ����(Ŀ�� ����)
#define SDN_CTS_DEAD          (3)  // 'D': ���̻� ������ �ʴ� ����

#define SDN_CTS_INFINITE     ((UChar)0x1F)
#define SDN_CTS_IN_KEY       ((UChar)0x1E)

#define SDN_CHAINED_NO       (0)
#define SDN_CHAINED_YES      (1)

#define SDN_IS_VALID_CTS( aCTS ) ( (aCTS != SDN_CTS_INFINITE) && (aCTS != SDN_CTS_IN_KEY) )

/* BUG-24091
 * [SD-����߰�] vrow column ���鶧 ���ϴ� ũ�⸸ŭ�� �����ϴ� ��� �߰� */
#define SDN_FETCH_SIZE_UNLIMITED    ID_UINT_MAX

typedef IDE_RC (*sdnSoftKeyStamping)( sdrMtx        * aMtx,
                                      sdpPhyPageHdr * aPage,
                                      UChar           aCTSlotNum,
                                      UChar         * aContxt);

typedef IDE_RC (*sdnHardKeyStamping)( idvSQL        * aStatistics,
                                      sdrMtx        * aMtx,
                                      sdpPhyPageHdr * aPage,
                                      UChar           aCTSlotNum,
                                      UChar         * aContxt,
                                      idBool        * aSuccess );

typedef IDE_RC (*sdnLogAndMakeChainedKeys)( sdrMtx        * aMtx,
                                          sdpPhyPageHdr * aPage,
                                          UChar           aCTSlotNum,
                                          UChar         * aContext,
                                          UChar         * aKeyList,
                                          UShort        * aKeyListSize,
                                          UShort        * aChainedKeyCount );

typedef IDE_RC (*sdnWriteChainedKeysLog)( sdrMtx        * aMtx,
                                          sdpPhyPageHdr * aPage,
                                          UChar           aCTSlotNum );

typedef IDE_RC (*sdnMakeChainedKeys)( sdpPhyPageHdr * aPage,
                                      UChar           aCTSlotNum,
                                      UChar         * aContext,
                                      UChar         * aKeyList,
                                      UShort        * aKeyListSize,
                                      UShort        * aChainedKeyCount );

typedef idBool (*sdnFindChainedKey)( idvSQL* aStatistis,
                                     sdnCTS* sCTS,
                                     UChar * aChainedKeyList,
                                     UShort  aKeyCount,
                                     UChar * aChainedCCTS,
                                     UChar * aChainedLCTS,
                                     UChar * aFindContext );

typedef IDE_RC (*sdnLogAndMakeUnchainedKeys)( idvSQL        * aStatistics,
                                              sdrMtx        * aMtx,
                                              sdpPhyPageHdr * aPage,
                                              sdnCTS        * aCTS,
                                              UChar           aCTSlotNum,
                                              UChar         * aChainedKeyList,
                                              UShort          aChainedKeySize,
                                              UShort        * aUnchainedKeyCount,  
                                              UChar         * aFindContext );

typedef IDE_RC (*sdnWriteUnchainedKeysLog)( sdrMtx        * aMtx,
                                            sdpPhyPageHdr * aPage,
                                            UShort          aUnchainedKeyCount,
                                            UChar         * aUnchainedKey,
                                            UInt            aUnchainedKeySize );

typedef IDE_RC (*sdnMakeUnchainedKeys)( idvSQL        * aStatistics,
                                        sdpPhyPageHdr * aPage,
                                        sdnCTS        * aCTS,
                                        UChar           aCTSlotNum,
                                        UChar         * aChainedKeyList,
                                        UShort          aChainedKeySize,
                                        UChar         * aFindContext,
                                        UChar         * aUnchainedKey,
                                        UInt          * aUnchainedKeySize,
                                        UShort        * aUnchainedKeyCount );

/* BUG-32976 [SM] In situation where there is insufficient on undo tablespace
 *           when making a chained CTS in disk index, mini-transaction rollback
 *           makes the server shutdown abnormally.
 *
 * mMakeChainedKeys, mMakeUnchainedKeys �������̽��� �и��Ѵ�.
 *
 * mLogAndMakeXXX  : �α׸� ����� chained/unchained keys�� ����� �������̽�.
 * mWriteXXX       : chained/unchained keys�� ���� �α׸� ����� �������̽�.
 * mMakeXXX        : chained/unchained keys�� ����� �������̽�. */
typedef struct sdnCallbackFuncs
{
    sdnSoftKeyStamping          mSoftKeyStamping;
    sdnHardKeyStamping          mHardKeyStamping;
//    sdnLogAndMakeChainedKeys    mLogAndMakeChainedKeys;
    sdnWriteChainedKeysLog      mWriteChainedKeysLog;
    sdnMakeChainedKeys          mMakeChainedKeys;
    sdnFindChainedKey           mFindChainedKey;
    sdnLogAndMakeUnchainedKeys  mLogAndMakeUnchainedKeys;
//    sdnWriteUnchainedKeysLog    mWriteUnchainedKeysLog;
    sdnMakeUnchainedKeys        mMakeUnchainedKeys;
} sdnCallbackFuncs;

#endif /* _O_SDN_DEF_H_ */
