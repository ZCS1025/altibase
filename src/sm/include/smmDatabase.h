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
 * $Id:$
 **********************************************************************/

#ifndef _O_SMM_DATABSE_H_
#define _O_SMM_DATABSE_H_ 1

#include <smmDef.h>


class smmDatabase
{
private :
    smmDatabase(){};
    ~smmDatabase(){};

    // 데이터베이스의 메타정보
    // membase of DICTIONARY TBS
    static smmMemBase         *mDicMemBase;
    static smmMemBase          mMemBaseBackup; // for BUG-7592

    // SCN갱신시 잡는 Mutex
    static iduMutex            mMtxSCN;
    
public :

    static smSCN               mLstSystemSCN;

    static IDE_RC initialize();
    static IDE_RC destroy();
    
     // membase를 초기화한다.
    static IDE_RC initializeMembase( smmTBSNode * aTBSNode,
                                     SChar *      aDBName,
                                     vULong       aDbFilePageCount,
                                     vULong       aChunkPageCount,
                                     SChar *      aDBCharSet,
                                     SChar *      aNationalCharSet );

    /*-----------------------------
       Basic Interfaces for Membase
    -------------------------------*/
    // mDBName
    static inline SChar* getDBName(smmMemBase * aMemBase);
    
    // mDBFilePageCount
    static inline vULong getDBFilePageCount(smmMemBase * aMemBase);
    
    // mTxTBLSize
    static inline void setTxTBLSize(smmMemBase * aMemBase,
                                    UInt         aTxTBLSize);

    // mDBFileCount
    static inline UInt getDBFileCount(smmMemBase * aMemBase, UInt aWhichDB);
   
    // mAllocPersPageCount
    static inline vULong getAllocPersPageCount(smmMemBase * aMemBase);

    // mExpandChunkPageCnt
    static inline vULong getExpandChunkPageCnt(smmMemBase * aMemBase);
    
    // mmCurrentExpandChunkCnt
    static inline vULong getCurrentExpandChunkCnt(smmMemBase * aMemBase);
    
    // mFreePageLists
    static inline smmDBFreePageList getFreePageList(smmMemBase * aMemBase,
                                                    UInt         aFPListIdx);
    static inline void setFreePageList(smmMemBase * aMemBase,
                                       UInt         aFreePageListIdx,
                                       scPageID     aFirstFreePageID,
                                       vULong       aFreePageCount);

    // mSystemSCN
    static inline smSCN* getSystemSCN();
    static inline IDE_RC setSystemSCN( smSCN * aSystemSCN );
    

    /*-----------------------------
       Runtime Interfaces 
    -------------------------------*/
    static inline smSCN getLstSystemSCN();
    static inline smSCN inaccurateGetLstSystemSCN();
    static IDE_RC setLstSystemSCN( smSCN * aLstSystemSCNPtr, smSCN * aNewLstSystemSCNPtr );

    static  void   getViewSCN(smSCN *a_pSCN);
    static  IDE_RC getCommitSCN( void     * aTrans,
                                 idBool     aIsLegacyTrans,
                                 void     * aStatus );

    inline static  IDE_RC lockSCNMtx()
           { return mMtxSCN.lock( NULL ); }
    inline static  IDE_RC unlockSCNMtx()
           { return mMtxSCN.unlock(); }
    
    // System CommitSCN이 Valid한지 조사한다.
    static void validateCommitSCN();
    
    static  smmMemBase* getDicMemBase(){return mDicMemBase;};
    static  smmMemBase* setDicMemBase(smmMemBase * aDicMemBase)
    {
        return mDicMemBase = aDicMemBase;
    };
    static SChar* getDBName() 
    {
        IDE_DASSERT( mDicMemBase != NULL );
        return mDicMemBase->mDBname;
    };

    // PROJ-1579 NCHAR
    static SChar* getDBCharSet();
    static SChar* getNationalCharSet();
    
   
    /*-----------------------------
       Interfaces for Validation 
    -------------------------------*/
    static inline void makeMembaseBackup();
    static IDE_RC checkMembaseIsValid();
    static IDE_RC checkExpandChunkProps(smmMemBase * aMemBase);
    static IDE_RC checkVersion(smmMemBase *aMemBase);
    static void   dumpMembase();

    /*-----------------------------
       BUG-31862 resize transaction table without db migration
       Interfaces for Update mTxTblSize in mDicMemBase
    -------------------------------*/
#ifdef DEBUG
    static IDE_RC checkTransTblSize(smmMemBase * aMemBase);
#endif
    static IDE_RC refineTransTblSize(); 
};



                     
// mDBFilePageCount
inline vULong smmDatabase::getDBFilePageCount(smmMemBase * aMemBase)
{
    IDE_DASSERT( aMemBase != NULL );
    
    return aMemBase->mDBFilePageCount;
}

// mTxTBLSize
inline void smmDatabase::setTxTBLSize(smmMemBase * aMemBase,
                                      UInt         aTxTBLSize)
{
    IDE_DASSERT( aMemBase != NULL );
    
    aMemBase->mTxTBLSize = aTxTBLSize;
}

// mDBFileCount
inline UInt smmDatabase::getDBFileCount(smmMemBase * aMemBase, UInt aWhichDB)
{
    return aMemBase->mDBFileCount[aWhichDB];
}
    
// mAllocPersPageCount
inline vULong smmDatabase::getAllocPersPageCount(smmMemBase * aMemBase)
{
    IDE_DASSERT( aMemBase != NULL );
    
    return aMemBase->mAllocPersPageCount;
}

// mExpandChunkPageCnt
inline vULong smmDatabase::getExpandChunkPageCnt(smmMemBase * aMemBase)
{
    IDE_DASSERT( aMemBase != NULL );
    
    return aMemBase->mExpandChunkPageCnt;
}

// mCurrentExpandChunkCnt
inline vULong smmDatabase::getCurrentExpandChunkCnt(smmMemBase * aMemBase)
{
    IDE_DASSERT( aMemBase != NULL );
    
    return aMemBase->mCurrentExpandChunkCnt;
}

// mFreePageLists
inline smmDBFreePageList smmDatabase::getFreePageList(smmMemBase * aMemBase,
                                                      UInt         aFPListIdx)
{
    IDE_DASSERT( aMemBase != NULL );
    
    return aMemBase->mFreePageLists[aFPListIdx];
}
inline void smmDatabase::setFreePageList(smmMemBase * aMemBase,
                                         UInt         aFreePageListIdx,
                                         scPageID     aFirstFreePageID,
                                         vULong       aFreePageCount)
{
    IDE_DASSERT( aMemBase != NULL );
    
    aMemBase->mFreePageLists[aFreePageListIdx].mFirstFreePageID = aFirstFreePageID;
    aMemBase->mFreePageLists[aFreePageListIdx].mFreePageCount = aFreePageCount;
}

// mSystemSCN
inline smSCN* smmDatabase::getSystemSCN()
{
    IDE_DASSERT( mDicMemBase != NULL );
    return &(mDicMemBase->mSystemSCN); 
}

/* Service Phase로 상태전이 실시.
 * 이 함수가 불린 다음부터는
 * fillPCHEntry함수에서 PCH 가 NULL이 아니어도 서버를 죽이지 않는다.
 * Service Phase에서는 PCH가 NULL 이 아닌데도 fillPCHEntry가 불릴 수 있기 때문.
 */
inline smSCN smmDatabase::getLstSystemSCN()
{
    return acpAtomicGet64( &mLstSystemSCN );
}

inline smSCN smmDatabase::inaccurateGetLstSystemSCN()
{
    /* PROJ-2733 정확도 보다는 성능 하락 방지용 */
    return mLstSystemSCN;
}

inline void smmDatabase::makeMembaseBackup()
{
    IDE_DASSERT( mDicMemBase != NULL );
    idlOS::memcpy(&mMemBaseBackup, mDicMemBase, ID_SIZEOF(smmMemBase));
}


#endif //_O_SMM_DATABSE_H_
