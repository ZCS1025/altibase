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

#ifndef _O_MMC_TRANS_H_
#define _O_MMC_TRANS_H_ 1


#include <idl.h>
#include <ide.h>
#include <idu.h>
#include <smi.h>
#include <sdi.h>
class mmcSession;

/* PROJ-2701 online data rebuild & sharding n-transaction sharing */
typedef struct mmcTransShareInfo
{
    UInt        mAllocRefCnt;
    iduMutex    mTransMutex;
    idBool      mIsGlobalTx;
    UInt        mTransRefCnt;
    mmcSession *mDelegatedSessions;
    SChar       mNodeName[SDI_NODE_NAME_MAX_SIZE + 1];
} mmcTransShareInfo;

typedef struct mmcTransObj
{
    smiTrans           mSmiTrans;
    mmcTransShareInfo *mShareInfo;
} mmcTransObj;

//PROJ-1436 SQL-Plan Cache.
typedef IDE_RC (*mmcTransCommt4PrepareFunc)( mmcTransObj *aTrans,
                                             mmcSession  *aSession,
                                             UInt         aTransReleasePolicy );
class mmcTrans
{
private:
    static iduMemPool mPool;
    static iduMemPool mSharePool;

public:
    static IDE_RC initialize();
    static IDE_RC finalize();

    /* only transaction free */
    static IDE_RC freeRaw( mmcTransObj * aTrans );
    /* destroy and free and process for session
     * if aSession is null then this function just do destroy and free.
     */
    static IDE_RC free(mmcSession * aSession, mmcTransObj * aTrans);

    /* only transaction alloc */
    static IDE_RC allocRaw( mmcTransObj ** aTrans );

    /* alloc and transaction initialize and process for session 
     * if aSession is null then this function just do alloc and initialize.
     */
    static IDE_RC alloc(mmcSession * aSession, mmcTransObj **aTrans);
    static void   beginXA( mmcTransObj * aTrans,
                           idvSQL      * aStatistics,
                           UInt          aFlag );
    static void   begin( mmcTransObj * aTrans,
                         idvSQL      * aStatistics,
                         UInt          aFlag,
                         mmcSession  * aSession );
    static IDE_RC endPending( ID_XID * aXID,
                              idBool   aIsCommit );
    static IDE_RC prepareForShard( mmcTransObj * aTrans,
                                   mmcSession  * aSession,
                                   ID_XID      * aXID,
                                   idBool      * aReadOnly );
    static IDE_RC prepareXA( mmcTransObj * aTrans, 
                             ID_XID      * aXID , 
                             mmcSession  *aSession );

    static IDE_RC commitXA( mmcTransObj * aTrans,
                            mmcSession  * aSession,
                            UInt aTransReleasePolicy );
    
    static IDE_RC commit( mmcTransObj* aTrans,
                          mmcSession * aSession,
                          UInt         aTransReleasePolicy = SMI_RELEASE_TRANSACTION,
                          idBool       aIsSqlPrepare = ID_FALSE );
    static IDE_RC commit4Prepare( mmcTransObj * aTrans, 
                                  mmcSession  * aSession, 
                                  UInt aTransReleasePolicy = SMI_RELEASE_TRANSACTION );
    static IDE_RC commit4PrepareWithFree( mmcTransObj * aTrans, 
                                          mmcSession  * aSession, 
                                          UInt aTransReleasePolicy = SMI_RELEASE_TRANSACTION );
    static IDE_RC commitForceDatabaseLink( mmcTransObj * aTrans,
                                           mmcSession  * aSession,
                                           UInt aTransReleasePolicy = SMI_RELEASE_TRANSACTION,
                                           idBool aIsSqlPrepare = ID_FALSE );

    //PROJ-1436 SQL-Plan Cache.
    static IDE_RC commit4Null( mmcTransObj* aTrans,
                               mmcSession * aSession,
                               UInt         aTransReleasePolicy );

    static IDE_RC rollbackXA( mmcTransObj * aTrans,
                              mmcSession  * aSession,
                              UInt aTransReleasePolicy );

    static IDE_RC rollback( mmcTransObj * aTrans,
                            mmcSession  * aSession,
                            const SChar * aSavePoint,
                            idBool        aIsSqlPrepare = ID_FALSE,
                            UInt          aTransReleasePolicy = SMI_RELEASE_TRANSACTION );
    static IDE_RC rollbackForceDatabaseLink( mmcTransObj * aTrans,
                                             mmcSession * aSession,
                                             UInt aTransReleasePolicy = SMI_RELEASE_TRANSACTION );
    static IDE_RC savepoint( mmcTransObj * aTrans,
                             mmcSession  * aSession,
                             const SChar * aSavePoint );

    /* BUG-46785 Shard statement partial rollback */
    static IDE_RC shardStmtPartialRollback( mmcTransObj * aTrans );

    static IDE_RC rollbackRaw( mmcTransObj * aTrans,
                               ULong * aEventFlag,
                               UInt aTransReleasePolicy );

    static IDE_RC commitRaw( mmcTransObj * aTrans,
                             ULong * aEventFlag,
                             UInt    aTransReleasePolicy,
                             smSCN * aCommitSCN );

    static void   beginRaw( mmcTransObj * aTrans,
                            idvSQL * aStatistics,
                            UInt     aFlag,
                            ULong  * aSessionEventFlag );

    static void clearAndSetSessionInfoAfterBegin( mmcSession  * aSession,
                                                  mmcTransObj * aTrans );

    static inline smiTrans*     getSmiTrans(mmcTransObj * aObj);
    static inline smTID         getTransID(mmcTransObj * aObj);
    static inline UInt          getFirstUpdateTime(mmcTransObj * aObj);
    static inline smiStatement* getSmiStatement(mmcTransObj * aObj);

    static inline IDE_RC    isReadOnly(mmcTransObj * aObj, idBool * aIsReadOnly);
    static inline void      reservePsmSvp(mmcTransObj * aObj);
    static inline void      clearPsmSvp(mmcTransObj * aObj);
    static inline IDE_RC    abortToPsmSvp(mmcTransObj * aObj);
    static inline idBool    isReusableRollback(mmcTransObj * aObj);
    static inline void      attachXA(mmcTransObj * aTrans, UInt aSlotID);

    /* PROJ-2701 online data rebuild & sharding n-transaction sharing */
    static inline SChar*    getShardNodeName(mmcTransObj * aTrans);
    static inline void      setTransShardNodeName(mmcTransObj * aTrans, SChar * aNodeName);
    static inline void      unsetTransShardNodeName(mmcTransObj * aTrans);

    static inline void lock(mmcTransObj *aTrans)
    {
        IDE_ASSERT(aTrans->mShareInfo->mTransMutex.lock(NULL) == IDE_SUCCESS);
    }
    static inline void unlock(mmcTransObj *aTrans)
    {
        IDE_ASSERT(aTrans->mShareInfo->mTransMutex.unlock() == IDE_SUCCESS);
    }
    static inline void lockRecursive(mmcTransObj *aTrans)
    {
        IDE_ASSERT(aTrans->mShareInfo->mTransMutex.lockRecursive(NULL) == IDE_SUCCESS);
    }
    static inline void unlockRecursive(mmcTransObj *aTrans)
    {
        IDE_ASSERT(aTrans->mShareInfo->mTransMutex.unlockRecursive() == IDE_SUCCESS);
    }
    static inline idBool isShareableTrans(mmcTransObj *aTrans)
    {
        return ( aTrans->mShareInfo != NULL ) ? ID_TRUE : ID_FALSE;
    }

private:

    /* PROJ-2701 online data rebuild & sharding n-transaction sharing */
    static void removeDelegatedSession(mmcTransShareInfo * aShareInfo, mmcSession * aSession);
    static void addDelegatedSession(mmcTransShareInfo * aShareInfo, mmcSession * aSession);
    static IDE_RC commitSessions(mmcTransShareInfo * aShareInfo);
    static IDE_RC commitShareableTrans( mmcTransObj * aTrans,
                                        mmcSession  * aSession,
                                        UInt          aTransReleasePolicy,
                                        smSCN       * aCommitSCN );

    static IDE_RC commitLocal(mmcTransObj * aTrans,
                              mmcSession * aSession,
                              UInt aTransReleasePolicy = SMI_RELEASE_TRANSACTION,
                              idBool aIsSqlPrepare = ID_FALSE);

    static IDE_RC rollbackLocal(mmcTransObj * aTrans,
                                mmcSession * aSession,
                                const SChar * aSavePoint,
                                UInt aTransReleasePolicy = SMI_RELEASE_TRANSACTION,
                                idBool aIsSqlPrepare = ID_FALSE);

    static IDE_RC findPreparedTrans(ID_XID * aXID,
                                    idBool * aFound,
                                    SInt * aSlotID = NULL);

    static IDE_RC doAfterCommit( mmcSession * aSession,
                                 UInt         aTransReleasePolicy,
                                 idBool       aIsSqlPrepare,
                                 smSCN      * aCommitSCN );
};

/* PROJ-2701 online data rebuild */
inline smiTrans* mmcTrans::getSmiTrans(mmcTransObj * aObj)
{
    return &(aObj->mSmiTrans);
}

inline smTID mmcTrans::getTransID(mmcTransObj * aObj)
{
    return aObj->mSmiTrans.getTransID();
}

inline UInt mmcTrans::getFirstUpdateTime(mmcTransObj * aObj)
{
    return aObj->mSmiTrans.getFirstUpdateTime();
}

inline smiStatement* mmcTrans::getSmiStatement(mmcTransObj * aObj)
{
    return aObj->mSmiTrans.getStatement();
}

inline IDE_RC mmcTrans::isReadOnly(mmcTransObj * aObj, idBool * aIsReadOnly)
{
    return aObj->mSmiTrans.isReadOnly(aIsReadOnly);
}

inline void mmcTrans::reservePsmSvp(mmcTransObj * aObj)
{
    aObj->mSmiTrans.reservePsmSvp();
}

inline void mmcTrans::clearPsmSvp(mmcTransObj * aObj)
{
    aObj->mSmiTrans.clearPsmSvp();
}

inline IDE_RC mmcTrans::abortToPsmSvp(mmcTransObj * aObj)
{
    return aObj->mSmiTrans.abortToPsmSvp();
}

inline idBool mmcTrans::isReusableRollback(mmcTransObj * aObj)
{
    return aObj->mSmiTrans.isReusableRollback();
}

inline void mmcTrans::attachXA(mmcTransObj * aTrans, UInt aSlotID)
{
    IDE_ASSERT(mmcTrans::getSmiTrans(aTrans)->attach(aSlotID) == IDE_SUCCESS);
}

inline SChar* mmcTrans::getShardNodeName(mmcTransObj * aTrans)
{
    IDE_DASSERT(aTrans->mShareInfo != NULL);
    return aTrans->mShareInfo->mNodeName;
}

inline void mmcTrans::setTransShardNodeName(mmcTransObj * aTrans, SChar * aNodeName)
{
    IDE_DASSERT(aTrans->mShareInfo != NULL);
    idlOS::strncpy(aTrans->mShareInfo->mNodeName, aNodeName, SDI_NODE_NAME_MAX_SIZE);
    aTrans->mShareInfo->mNodeName[SDI_NODE_NAME_MAX_SIZE] = '\0';
}

inline void mmcTrans::unsetTransShardNodeName(mmcTransObj * aTrans)
{
    IDE_DASSERT(aTrans->mShareInfo != NULL);
    aTrans->mShareInfo->mNodeName[SDI_NODE_NAME_MAX_SIZE] = '\0';
}

#endif
