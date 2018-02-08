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
 * $Id: smiTrans.h 82186 2018-02-05 05:17:56Z lswhh $
 **********************************************************************/

#ifndef _O_SMI_TRANS_H_
# define _O_SMI_TRANS_H_ 1

# include <idv.h>
# include <smiDef.h>
# include <smiStatement.h>

# define SMI_TRANS_SLOT_ID_NULL ID_UINT_MAX

typedef enum smiTransInitOpt
{
    SMI_TRANS_INIT,
    SMI_TRANS_ATTACH
} smiTransInitOpt;

class smiTrans
{
    friend class smiTable;
    friend class smiObject;
    friend class smiStatement;
    friend class smiTableCursor;

 public:  /* POD class type should make non-static data members as public */
    void*            mTrans;
    UInt             mFlag;
    smiStatement   * mStmtListHead;
    smiTableCursor   mCursorListHead;

    //PROJ-1608
    smLSN             mBeginLSN;
    smLSN             mCommitLSN;

 public:
    IDE_RC initialize();

    IDE_RC initializeInternal( void );

    IDE_RC destroy( idvSQL * aStatistics );
 
    // For Local & Global Transaction
    IDE_RC begin( smiStatement** aStatement,
                  idvSQL        *aStatistics,
                  UInt           aFlag = ( SMI_ISOLATION_CONSISTENT     |
                                           SMI_TRANSACTION_NORMAL       |
                                           SMI_TRANSACTION_REPL_DEFAULT |
                                           SMI_COMMIT_WRITE_NOWAIT),
                  UInt           aReplID       = SMX_NOT_REPL_TX_ID,
                  idBool         aIgnoreRetry  = ID_FALSE);

    IDE_RC rollback( const SChar* aSavePoint = NULL, UInt aTransReleasePolicy = SMI_RELEASE_TRANSACTION );
    /*
      PROJ-1677 DEQ
                                    (sm-commit& mm-commit )
                                     |
                                     V                         
      dequeue statement begin  ----------------- > execute .... 
                                          ^
                                          |
                                        get queue stamp (session�� queue stamp�� ����)
                                        
      dequeue statement begin�� execute �ٷ� ������ queue item�� commit�Ǿ��ٸ�,
      dequeue execute�ÿ�  �ش� queue item�� MVCC������ ���� ��� ��� ���·� ����.
      �׸��� session�� queue timestamp�� queue timestamp�� ���Ƽ�  ���� enqueue
      event�� �߻��Ҷ� ����  queue�� ����Ÿ�� �������� �ұ��ϰ� dequeue�� �Ҽ� ���� .
      �̹����� �ذ� �ϱ� ���Ͽ�  commitSCN�� �ξ���. */
    
    IDE_RC commit(smSCN* aCommitSCN, UInt aTransReleasePolicy = SMI_RELEASE_TRANSACTION);

    // For Global Transaction 
    /* BUG-18981 */
    IDE_RC prepare(ID_XID *aXID );
    IDE_RC forget(ID_XID *aXID);
    IDE_RC isReadOnly(idBool *aIsReadOnly);
    IDE_RC attach( SInt aSlotID );
    void   showOIDList();
    
    IDE_RC savepoint(const SChar* aSavePoint,
                     smiStatement *aStatement = NULL );

    // For BUG-12512
    void   reservePsmSvp( );
    void   clearPsmSvp( );
    IDE_RC abortToPsmSvp( );
    
    smTID  getTransID();
    inline smiStatement* getStatement( void );
    inline void* getTrans( void );
    inline UInt  getIsolationLevel();
    inline UInt  getReplicationMode();
    inline UInt  getTransactionMode();

    UInt   getFirstUpdateTime();

    // QP���� Meta�� ���ٵ� ��� �� �Լ��� ȣ���Ͽ�
    // Transaction�� Meta���� ���θ� �����Ѵ�
    IDE_RC setMetaTableModified();
    smSN getBeginSN();
    smSN getCommitSN();

    // DDL Transaction�� ǥ���ϴ� Log Record�� ����Ѵ�.
    IDE_RC writeDDLLog();

    void  setStatistics( idvSQL * aStatistics );
    idvSQL * getStatistics( void );
    IDE_RC setReplTransLockTimeout( UInt aReplTransLockTimeout );
    UInt getReplTransLockTimeout( );

    idBool isBegin();
};

inline void* smiTrans::getTrans( void )
{
    return mTrans;
}

inline smiStatement* smiTrans::getStatement( void )
{
    return mStmtListHead;
}

inline UInt smiTrans::getIsolationLevel( void )
{
    return mFlag & SMI_ISOLATION_MASK;
}

inline UInt smiTrans::getReplicationMode()
{
    return mFlag & SMI_TRANSACTION_REPL_MASK;
}

inline UInt smiTrans::getTransactionMode( void )
{
    return mFlag & SMI_TRANSACTION_MASK;
}

#endif /* _O_SMI_TRANS_H_ */
