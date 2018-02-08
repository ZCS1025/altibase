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
 * $$Id:$
 **********************************************************************/

/******************************************************************************
 * Description :
 *    sdbBufferArea ��ü�� sdbBufferPool���� BCB�� �����ϴ� ������ �Ѵ�.
 *    ��� frame, BCB���� sdbBufferArea���� �����Ǹ� �����ȴ�.
 *    frame�� BCB�� �� ���� �ý��� �����߿� �ø��ų� ���� �� �ֵ���
 *    chunk������ �Ҵ��Ѵ�. 
 *
 ******************************************************************************/
#include <sdbBufferArea.h>
#include <sdbReq.h>

/******************************************************************************
 * Description :
 *    BufferArea�� �ʱ�ȭ�Ѵ�. �� Area�� ������ chunk�� page�� ������
 *    �ʱ� chunk�� ����, �׸��� page�� size�� ���ڷ� �Ѱܾ� �Ѵ�.
 *
 * Implementation :
 *    �ΰ��� mutex�� �ʱ�ȭ�Ѵ�. IDE_FAILURE�� �߻��� �� �ִ� ����
 *    mutex�� �ʱ�ȭ ���л��̴�.
 *    
 * �� ���� ũ�ⱸ�ϴ� �� = aChunkPageCount * aChunkCount * aPageSize
 * 
 * aChunkPageCount  - [IN] chunk�� page�� ����
 * aChunkCount      - [IN] �� BufferArea�� �ʱ⿡ ������ chunk�� ����
 * aPageSize        - [IN] ������ �ϳ��� ũ��(����Ʈ����)
 ******************************************************************************/
IDE_RC sdbBufferArea::initialize(UInt aChunkPageCount,
                                 UInt aChunkCount,
                                 UInt aPageSize)
{
    SInt sState = 0;

    SMU_LIST_INIT_BASE(&mUnUsedBCBListBase);
    SMU_LIST_INIT_BASE(&mAllBCBList);

    mChunkPageCount = aChunkPageCount;
    mPageSize       = aPageSize;
    mChunkCount     = 0; // expandArea���� �����ȴ�.
    mBCBCount       = 0;
    initBCBPtrRange();

    IDE_TEST( mBufferAreaLatch.initialize( (SChar*)"BUFFER_AREA_LATCH" )
              != IDE_SUCCESS );
    sState = 1;

    IDE_TEST(mBCBMemPool.initialize(IDU_MEM_SM_SDB,
                                    (SChar*)"SDB_BCB_MEMORY_POOL",
                                    1,                  // Multi Pool Cnt
                                    ID_SIZEOF(sdbBCB),  // Block Size 
                                    mChunkPageCount,    // Block Cnt In Chunk 
                                    ID_UINT_MAX,        // chunk limit 
                                    ID_FALSE,           // use mutex
                                    8,                  // align byte
                                    ID_FALSE,			// ForcePooling
                                    ID_TRUE,			// GarbageCollection
                                    ID_TRUE)			// HWCacheLine
             != IDE_SUCCESS);
    sState = 2;

    IDE_TEST(mFrameMemPool.initialize(IDU_MEM_SM_SDB,
                                      (SChar*)"SDB_FRAME_MEMORY_POOL",
                                      1,               /* Multi Pool Cnt */
                                      mPageSize,       /* Block Size */
                                      mChunkPageCount, /* Block Cnt In Chunk */
                                      0,               /* Cache Size */
                                      mPageSize)       /* Align Size */
             != IDE_SUCCESS);
    sState = 3;

    IDE_TEST(mListMemPool.initialize(IDU_MEM_SM_SDB,
                                     (SChar*)"SDB_BCB_LIST_MEMORY_POOL",
                                     1,                 // Multi Pool Cnt
                                     ID_SIZEOF(smuList),// Block Size 
                                     mChunkPageCount,   // Block Cnt In Chunk 
                                     ID_UINT_MAX,       // chunk limit 
                                     ID_FALSE,          // use mutex
                                     8,                 // align byte
                                     ID_FALSE,			// ForcePooling 
                                     ID_TRUE,			// GarbageCollection
                                     ID_TRUE)			// HWCacheLine
             != IDE_SUCCESS);
    sState = 4;

    // ������ BCB array�� frame chunk�� �Ҵ��Ѵ�.
    IDE_TEST(expandArea(NULL, aChunkCount) != IDE_SUCCESS);

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    switch(sState)
    {
        case 4:
            IDE_ASSERT(mListMemPool.destroy() == IDE_SUCCESS);
        case 3:
            IDE_ASSERT(mFrameMemPool.destroy() == IDE_SUCCESS);
        case 2:
            IDE_ASSERT(mBCBMemPool.destroy() == IDE_SUCCESS);
        case 1:
            IDE_ASSERT(mBufferAreaLatch.destroy() == IDE_SUCCESS );
        default:
            break;
    }

    return IDE_FAILURE;
}

/******************************************************************************
 * Description :
 *    sdbBufferArea�� �����Ѵ�. ���������� �Ҵ��ߴ� ��� frame chunk��
 *    BCB array�� node���� ��� �����ϰ� mutex�� �����Ѵ�.
 *    destroy ȣ�� �� �ٽ� initialize()�� ȣ���Ͽ� ������ �� �ִ�.
 ******************************************************************************/
IDE_RC sdbBufferArea::destroy()
{
    freeAllAllocatedMem();

    IDE_ASSERT(mListMemPool.destroy() == IDE_SUCCESS);
    IDE_ASSERT(mFrameMemPool.destroy() == IDE_SUCCESS);
    IDE_ASSERT(mBCBMemPool.destroy() == IDE_SUCCESS);

    IDE_ASSERT(mBufferAreaLatch.destroy() == IDE_SUCCESS );

    return IDE_SUCCESS;
}

/******************************************************************************
 * Description :
 *  buffer area�ڽ��� ������ ��� BCB, list, Frame���� ���� �� �޸𸮸�
 *  �����մϴ�.
 ******************************************************************************/
void sdbBufferArea::freeAllAllocatedMem()
{
    smuList *sNode;
    smuList *sBase;
    sdbBCB  *sBCB;

    sBase = &mAllBCBList;
    sNode = SMU_LIST_GET_FIRST(sBase);

    while (sNode != sBase)
    {
        SMU_LIST_DELETE(sNode);
        sBCB = (sdbBCB*)sNode->mData;

        //BUG-21053 ���� ����� ���۸Ŵ����� ���ؽ��� ���� �������� �ʽ��ϴ�.
        IDE_ASSERT( sBCB->destroy() == IDE_SUCCESS );

        mFrameMemPool.memFree(sBCB->mFrameMemHandle);
        mBCBMemPool.memfree(sBCB);
        mListMemPool.memfree(sNode);

        sNode = SMU_LIST_GET_FIRST(sBase);
    }
}

/******************************************************************************
 * Description :
 *    aChunkCount ������ŭ�� ���ο� BCB�� buffer Area���� 
 *    �̿� �Բ� BCB array�� �Ҵ��ϰ� free BCB ����Ʈ�� �����Ѵ�.
 *    ���ü� ��� ����Ǿ� �ִ�.
 *    chunk�� page�� ������ page size�� initialize�� �� ������ ���� ������.
 *
 *    + exception:
 *        - malloc���� �޸� �Ҵ翡 �����ϸ� exception�� �߻��� �� ����
 *     
 *  aStatistics - [IN]  �������
 *  aChunkCount - [IN]  Ȯ���Ϸ��� chunk�� ����
 ******************************************************************************/
IDE_RC sdbBufferArea::expandArea(idvSQL *aStatistics, UInt aChunkCount)
{
    UInt     i;
    UChar   *sFrame;
    sdbBCB  *sBCB;
    UInt     sBCBID;
    smuList *sNode;
    void    *sFrameMemHandle;
    UInt     sBCBCnt;

    IDE_ASSERT(aChunkCount > 0);

    lockBufferAreaX(aStatistics);

    sBCBID  = mChunkPageCount * mChunkCount;
    sBCBCnt = mChunkPageCount * aChunkCount;
    for (i = 0; i < sBCBCnt; i++)
    {
        /* sdbBufferArea_expandArea_alloc_Frame.tc */
        IDU_FIT_POINT("sdbBufferArea::expandArea::alloc::Frame");
        IDE_TEST(mFrameMemPool.alloc(&sFrameMemHandle, (void**)&sFrame) != IDE_SUCCESS);

        /* sdbBufferArea_expandArea_alloc_BCB.tc */
        IDU_FIT_POINT("sdbBufferArea::expandArea::alloc::BCB");
        IDE_TEST(mBCBMemPool.alloc((void**)&sBCB) != IDE_SUCCESS);

        /* sdbBufferArea_expandArea_alloc_Node.tc */
        IDU_FIT_POINT("sdbBufferArea::expandArea::alloc::Node");
        IDE_TEST(mListMemPool.alloc((void**)&sNode) != IDE_SUCCESS);

        IDE_TEST(sBCB->initialize(sFrameMemHandle, sFrame, sBCBID) != IDE_SUCCESS);

        sNode->mData = sBCB;

        SMU_LIST_ADD_LAST(&mUnUsedBCBListBase, &sBCB->mBCBListItem);
        SMU_LIST_ADD_LAST(&mAllBCBList, sNode);

        mBCBCount++;
        sBCBID++;

        setBCBPtrRange( sBCB );
    }

    mChunkCount += aChunkCount;

    unlockBufferArea();

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/******************************************************************************
 * Description :
 *    �־��� aChunkCount ������ŭ chunk�� �����Ѵ�.
 *    �� chunk�� ���� ��� BCB���� ���ۿ��� ���ŵȴ�.
 ******************************************************************************/
IDE_RC sdbBufferArea::shrinkArea(idvSQL */*aStatistics*/, UInt /*aChunkCount*/)
{
    // ���� �������� �ʴ´�.
    return IDE_FAILURE;
}

/******************************************************************************
 * Description :
 *    aFirst���� aLast���� ������ BCB list�� BufferArea�� �߰��Ѵ�.
 *    aFirst���� aLast���� aCount������ �´����� ���������� �˻����� �ʱ�
 *    ������ �� �Լ��� ȣ���ϴ� ������ �ùٸ� count ������ å������ �Ѵ�.
 *
 *  aStatistics - [IN]  ���������� mutex�� ȹ���ϱ� ������
 *                      ��� ������ �Ѱܾ� �Ѵ�.
 *  aCount      - [IN]  �߰��� BCB list�� ����
 *  aFirst      - [IN]  �߰��� BCB list�� ó��. �̰��� mPrev�� NULL�̾�� �Ѵ�.
 *  aLast       - [IN]  �߰��� BCB list�� ������. �̰��� mNext�� NULL�̾�� �Ѵ�.
 ******************************************************************************/
void sdbBufferArea::addBCBs(idvSQL *aStatistics,
                            UInt    aCount,
                            sdbBCB *aFirst,
                            sdbBCB *aLast)
{
    IDE_DASSERT(aCount > 0);
    IDE_DASSERT(aFirst != NULL);
    IDE_DASSERT(aLast != NULL);

    lockBufferAreaX(aStatistics);
    SMU_LIST_ADD_LIST_FIRST(&mUnUsedBCBListBase,
                            &aFirst->mBCBListItem,
                            &aLast->mBCBListItem);
    mBCBCount += aCount;
    unlockBufferArea();
}

/******************************************************************************
 * Description :
 *    BufferArea�� ������ �ִ� BCB�� �ϳ� �����´�.
 *    ��ȯ�Ǵ� BCB�� ����Ʈ���� ���ŵǸ� free�����̴�.
 *    BufferArea�� BCB�� �ϳ��� ������ NULL�� ��ȯ�ȴ�.
 *
 *  aStatistics - [IN]  mutex ȹ���� ���� �������
 ******************************************************************************/
sdbBCB* sdbBufferArea::removeLast(idvSQL *aStatistics)
{
    smuList *sNode;
    sdbBCB  *sRet = NULL;

    lockBufferAreaX(aStatistics);

    if (SMU_LIST_IS_EMPTY(&mUnUsedBCBListBase))
    {
        sRet = NULL;
    }
    else
    {
        sNode = SMU_LIST_GET_LAST(&mUnUsedBCBListBase);
        SMU_LIST_DELETE(sNode);
        mBCBCount--;

        sRet = (sdbBCB*)sNode->mData;
        SDB_INIT_BCB_LIST(sRet);
    }

    unlockBufferArea();

    return sRet;
}

/******************************************************************************
 * Description :
 *    �� BufferArea�� ������ �ִ� ��� BCB list�� ��ȯ�ϰ�
 *    BufferArea�� 0���� BCB�� ���� ���°��ȴ�.
 *
 *  aStatistics - [IN]  mutex ȹ���� ���� �������
 *  aFirst      - [OUT] ��ȯ�� BCB list�� ù��° BCB pointer
 *  aLast       - [OUT] ��ȯ�� BCB list�� ������ BCB pointer
 *  aCount      - [OUT] ��ȯ�� BCB list�� BCB ����
 ******************************************************************************/
void sdbBufferArea::getAllBCBs(idvSQL  *aStatistics,
                               sdbBCB **aFirst,
                               sdbBCB **aLast,
                               UInt    *aCount)
{
    lockBufferAreaX(aStatistics);

    if (SMU_LIST_IS_EMPTY(&mUnUsedBCBListBase))
    {
        *aFirst = NULL;
        *aLast  = NULL;
        *aCount = 0;
    }
    else
    {
        *aFirst = (sdbBCB*)SMU_LIST_GET_FIRST(&mUnUsedBCBListBase)->mData;
        *aLast  = (sdbBCB*)SMU_LIST_GET_LAST(&mUnUsedBCBListBase)->mData;
        *aCount = mBCBCount;
        SMU_LIST_CUT_BETWEEN(&mUnUsedBCBListBase, &(*aFirst)->mBCBListItem);
        SMU_LIST_CUT_BETWEEN(&(*aLast)->mBCBListItem, &mUnUsedBCBListBase);
        SMU_LIST_INIT_BASE(&mUnUsedBCBListBase);
        mBCBCount = 0;
    }
    
    unlockBufferArea();
}


/******************************************************************************
 * Description :
 * 
 * �� �Լ��� ���� BCB�� �����ϴ� ����� ������ �ִ�.
 * ���� ū ������ BCB�� Buffer Pool�� ��������� ��ġ�� �� �ִٴ� ���̴�.
 * �̷��� ���� ����� ����������, BCB�� ������ �� �ִ� ����� ���� 2����
 * ���̾���. hash �Ǵ� list�� end�� ���ؼ�...  �׸��� �� 2����� ����
 * ������ ��ġ�� �ʱ� ������ ���ü��� �����ϴ°��� ��������� �����Ͽ���.
 *
 * �׷���, ��� BCB�� �����ϱ� ���ؼ� �� �Լ��� �������.
 * �׷��� ������ ���ü��� �� �������� �� �Լ��� ����ؾ� �Ѵ�. 
 *
 * ���ǻ���!
 *     list(LRU, Prepare, flush, flusher ���� list)�� �����ϴ� Ʈ�������
 *    �ڽ��� BCB�� list���� �����ϱ⸸ �ϸ� �ٸ� Ʈ������� �� BCB�� ������
 *    �������� �ʴ´ٰ� �����Ѵ�.(fix�� touchCnt�� ����.. ) �׷��� ������
 *    �� Ʈ����ǵ��� ����Ʈ���� ���ŵ� BCB�� ���ؼ� dirty read�� ������� 
 *    �ع�����.  �׷��� ������ �̵鿡�� ������ ��ġ�� ������ �� �Լ� ���� ��
 *    �ؼ��� �ȵȴ�. ���� �б⸸ �ϴ°��� ������ ���� ������, ���� �ൿ��
 *    �� ��쿡�� �ٸ� Ʈ����ǰ��� mutex�� �� ���� ���鼭 �����ϰ� �ؾ��Ѵ�.
 *
 *    ���ü� ���õ� �ڼ��� ������ sdbBufferPool.cpp�� 
 *    ** BufferPool�� ���ü� ���� ** �κ��� ����
 *    
 * aFunc    - [IN]  ���� area�� �� BCB�� ������ �Լ�
 * aObj     - [IN]  aFunc�����Ҷ� �ʿ��� ����
 ******************************************************************************/
IDE_RC sdbBufferArea::applyFuncToEachBCBs(
    idvSQL                *aStatistics,
    sdbBufferAreaActFunc   aFunc,
    void                  *aObj)
{
    smuList *sNode;
    sdbBCB  *sBCB;

    lockBufferAreaS( aStatistics );
    sNode = SMU_LIST_GET_FIRST(&mAllBCBList);
    while (sNode != &mAllBCBList)
    {
        sBCB = (sdbBCB*)sNode->mData;
        IDE_TEST(aFunc(sBCB, aObj) != IDE_SUCCESS);
        sNode = SMU_LIST_GET_NEXT(sNode);
    }
    unlockBufferArea();
    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}
