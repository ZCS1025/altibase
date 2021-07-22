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

#include <idl.h>
#include <idp.h>
#include <ideErrorMgr.h>
#include <idErrorCode.h>
#include <mmuProperty.h>
#include <iduLatch.h>
#include <mmcTask.h>

#ifndef _O_MMU_ACCESS_LIST_H_
#define _O_MMU_ACCESS_LIST_H_ 1

/* PROJ-2624 [��ɼ�] MM - ������ access_list ������� ���� : 1024�� �ø� */
#define MM_IP_ACL_MAX_COUNT      (1024)
#define MM_IP_ACL_MAX_ADDR_STR   (40)
#define MM_IP_ACL_MAX_MASK_STR   (16)

/* BUG-48515 
 * �ε��� access_list ���� ������ ������ struct
 */
typedef struct mmuIPACLInfo
{
    idBool          mPermit;        /* ID_TRUE: Permit, ID_FALSE: Deny */
    struct in6_addr mAddr;
    SChar           mAddrStr[MM_IP_ACL_MAX_ADDR_STR];
    UInt            mAddrFamily;
    UInt            mMask;
    UInt            mLimitCount;     /* BUG-48515 ���� remote ip �ִ� ���� ������ */
    UInt            mCurConnCount;   /* BUG-48515 ���� remote ip ���� ���� ���� */
} mmuIPACLInfo;

typedef struct mmuAccessListInfo
{
    UInt     mNumber;       // �Է� ����; �ܼ� ������, 1������ ����
    UInt     mOp;           // 0: DENY, 1: PERMIT
    SChar    mAddress[MM_IP_ACL_MAX_ADDR_STR];  // ipv6 �ּ� �ִ� 39��
    SChar    mMask[MM_IP_ACL_MAX_MASK_STR];     // ipv4 mask �ִ� 15��
    UInt     mLimitCount;
    UInt     mCurConnCount;
} mmuAccessListInfo;

class mmuAccessList
{
private:
    static iduLatch  mLatch;
    static idBool    mInitialized;

    static mmuIPACLInfo mIPACLInfo[MM_IP_ACL_MAX_COUNT];
    static mmuIPACLInfo mIPACLInfoWithLimit[MM_IP_ACL_MAX_COUNT];

    static UInt   mIPACLCount;
    static UInt   mIPACLCountWithLimit;

public:
    static IDE_RC initialize();
    static IDE_RC finalize();

    static void   lock();
    static void   lockRead();
    static void   unlock();

    static void   clear();
    static IDE_RC add( idBool            aIPACLPermit,
                       struct in6_addr * aIPACLAddr,
                       SChar           * aIPACLAddrStr,
                       UInt              aIPACLAddrFamily,
                       UInt              aIPACLMask,
                       UInt              sIPACLLimitSize );

    static UInt   getIPACLCount();

    static IDE_RC checkIPACL( struct sockaddr_storage  * aAddr,
                              SChar                    * aAddrStr,
                              idBool                   * aAllowed );

    static IDE_RC checkIPACLWithSessDecre( struct sockaddr_storage  * aAddr );

    static IDE_RC equalsIPACL( struct sockaddr_storage *aSessionAddr,
                               struct in6_addr         *aIPACLAddr,
                               UInt                     aIPACLFamily,
                               UInt                     aIPACLMask );

    static IDE_RC disconnect(mmcTask *aTask);

    static IDE_RC loadAccessList();

    static IDE_RC buildAccessListRecordAdd( void                * aHeader,
                                            iduFixedTableMemory * aMemory, 
                                            mmuIPACLInfo        * aIPAClInfo,
                                            mmuAccessListInfo   * aACLInfo );

    static IDE_RC buildAccessListRecord( idvSQL              * aStatistics,
                                         void                * aHeader,
                                         void                * aDumpObj,
                                         iduFixedTableMemory * aMemory );

    static UInt   convertToChar( void   * aBaseObj,
                                 void   * aMember,
                                 UChar  * aBuf,
                                 UInt     aBufSize );
};

inline void mmuAccessList::lock()
{
    if ( mInitialized == ID_TRUE )
    {
        IDE_ASSERT( mLatch.lockWrite( NULL,/* idvSQL* */
                                      NULL ) /* sWeArgs*/
                    == IDE_SUCCESS );
    }
    else
    {
        /* Nothing To Do */
    }
}

inline void mmuAccessList::lockRead()
{
    if ( mInitialized == ID_TRUE )
    {
        IDE_ASSERT( mLatch.lockRead( NULL,/* idvSQL* */
                                     NULL ) /* sWeArgs*/
                    == IDE_SUCCESS );
    }
    else
    {
        /* Nothing To Do */
    }
}

inline void mmuAccessList::unlock()
{
    if ( mInitialized == ID_TRUE )
    {
        IDE_ASSERT( mLatch.unlock() == IDE_SUCCESS );
    }
    else
    {
        /* Nothing To Do */
    }
}

inline UInt mmuAccessList::getIPACLCount()
{
    return mIPACLCount + mIPACLCountWithLimit;
}

#endif
