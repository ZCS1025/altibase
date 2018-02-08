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
 * $Id: qmcHashTempTable.h 82075 2018-01-17 06:39:52Z jina.kim $
 *
 * Description :
 *   [Hash Temp Table]
 *
 *   A4������ Memory �� Disk Table�� ��� �����Ѵ�.
 *   �߰� ����� �����ϴ� Node�� �� Hash�� �̿��� �߰� ����� �����
 *   Materialized Node���� Memory Hash Temp Table �Ǵ� Disk Hash Temp
 *   Table�� ����ϰ� �ȴ�.
 *
 *   �� ��, �� ��尡 Memory/Disk Hash Temp Table�� ���� ���� �ٸ�
 *   ������ ���� �ʵ��� �ϱ� ���Ͽ� ������ ���� Hash Temp Table��
 *   �̿��Ͽ� Transparent�� ���� �� ������ �����ϵ��� �Ѵ�.
 *
 *   �̷��� ������ ��Ÿ�� �׸��� �Ʒ��� ����.
 *
 *                                                     -------------
 *                                                  -->[Memory Hash]
 *     ---------------     -------------------      |  -------------
 *     | Plan Node   |---->| Hash Temp Table |------|  -----------
 *     ---------------     -------------------      -->[Disk Hash]
 *                                                     -----------
 *
 * ��� ���� :
 *
 * ��� :
 *
 **********************************************************************/

#ifndef _O_QMC_HASH_TMEP_TABLE_H_
#define _O_QMC_HASH_TMEP_TABLE_H_ 1

#include <qmcMemHashTempTable.h>
#include <qmcMemPartHashTempTable.h>
#include <qmcDiskHashTempTable.h>

/* qmcdHashTempTable.flag                                  */
#define QMCD_HASH_TMP_INITIALIZE                 (0x00000000)

/* qmcdHashTempTable.flag                                  */
// Hash Temp Table�� ���� ��ü
#define QMCD_HASH_TMP_STORAGE_TYPE               (0x00000001)
#define QMCD_HASH_TMP_STORAGE_MEMORY             (0x00000000)
#define QMCD_HASH_TMP_STORAGE_DISK               (0x00000001)

/* qmcdHashTempTable.flag                                  */
// Distinct�� ����
#define QMCD_HASH_TMP_DISTINCT_MASK              (0x00000002)
#define QMCD_HASH_TMP_DISTINCT_FALSE             (0x00000000)
#define QMCD_HASH_TMP_DISTINCT_TRUE              (0x00000002)

/* qmcdHashTempTable.flag                                  */
// Primary Temp Table�� ����
#define QMCD_HASH_TMP_PRIMARY_MASK               (0x00000004)
#define QMCD_HASH_TMP_PRIMARY_TRUE               (0x00000000)
#define QMCD_HASH_TMP_PRIMARY_FALSE              (0x00000004)

/* qmcdHashTempTable.flag                                  */
// Temp Table�� rid fix ����
#define QMCD_HASH_TMP_FIXED_RID_MASK             (0x00000008)
#define QMCD_HASH_TMP_FIXED_RID_FALSE            (0x00000000)
#define QMCD_HASH_TMP_FIXED_RID_TRUE             (0x00000008)

/* qmcdHashTempTable.flag                                  */
// PROJ-2553 Cache-aware Memory Hash Temp Table
// Hash Temp Table�� ����/Ž�� ��� (Bucket List / Partitioned Array)
#define QMCD_HASH_TMP_HASHING_TYPE               (0x00000010)
#define QMCD_HASH_TMP_HASHING_BUCKET             (0x00000000)
#define QMCD_HASH_TMP_HASHING_PARTITIONED        (0x00000010)

// Hash Temp Table �ڷ� ����
typedef struct qmcdHashTemp
{
    UInt                  flag;

    qcTemplate          * mTemplate;
    qmdMtrNode          * recordNode;       // Record ���� ����
    qmdMtrNode          * hashNode;         // Hash�� Column ����
    qmdMtrNode          * aggrNode;         // Aggregation Node ����

    UInt                  bucketCnt;        // Bucket����
    UInt                  mtrRowSize;       // ���� Record�� ũ��
    UInt                  nullRowSize;      // ���� null Record�� ũ��
    qmcMemory           * memoryMgr;        // ���� Record�� ���� �޸� ������
    iduMemory           * memory;           // Hash Temp�� ���Ǵ� Memory
    UInt                  memoryIdx;

    SChar               * nullRow;          // Memory Temp Table�� ���� Null Row

    UInt                  hashKey;          // Range�˻��� ���� Hash Key

    qmcdMemHashTemp     * memoryTemp;       // Memory Hash Temp Table
    qmcdMemPartHashTemp * memoryPartTemp;   // PROJ-2553 Memory Partitioned Hash Temp Table
    qmcdDiskHashTemp    * diskTemp;         // Disk Hash Temp Table

    idBool                existTempType;    // TEMP_TYPE �÷��� �����ϴ���
    void                * insertRow;        // insert�� ���� �ӽ� ����

} qmcdHashTemp;

class qmcHashTemp
{
public:

    //------------------------------------------------
    // Hash Temp Table�� ����
    //------------------------------------------------

    // Temp Table�� �ʱ�ȭ
    static IDE_RC      init( qmcdHashTemp * aTempTable,
                             qcTemplate   * aTemplate,
                             UInt           aMemoryIdx,
                             qmdMtrNode   * aRecordNode,
                             qmdMtrNode   * aHashNode,
                             qmdMtrNode   * aAggrNode,
                             UInt           aBucketCnt,
                             UInt           aFlag );

    // Temp Table�� Data�� �����Ѵ�.
    static IDE_RC      clear( qmcdHashTemp * aTempTable );

    // Temp Table���� ��� Data�� flag�� �ʱ�ȭ�Ѵ�.
    static IDE_RC      clearHitFlag( qmcdHashTemp * aTempTable );

    //------------------------------------------------
    // Hash Temp Table�� ����
    //------------------------------------------------

    // Data�� ���� Memory �Ҵ�
    static IDE_RC      alloc( qmcdHashTemp * aTempTable,
                              void        ** aRow );

    // Non-Distinction Data ����
    static IDE_RC      addRow( qmcdHashTemp * aTempTable,
                               void         * aRow );

    // Distinction Data ����
    static IDE_RC      addDistRow( qmcdHashTemp  * aTempTable,
                                   void         ** aRow,
                                   idBool        * aResult );

    // insert row ����
    static IDE_RC      makeTempTypeRow( qmcdHashTemp  * aTempTable,
                                        void          * aRow,
                                        void         ** aExtRow );

    // Aggregation Column�� ���� Update�� ����
    static IDE_RC      updateAggr( qmcdHashTemp * aTempTable );

    // To Fix PR-8415
    // Aggregation Column�� Final Update
    static IDE_RC      updateFiniAggr( qmcdHashTemp * aTempTable );

    // To Fix PR-8213
    // Group Aggregation������ ���Ǹ�, ���ο� Group�� ����Ѵ�.
    static IDE_RC      addNewGroup( qmcdHashTemp * aTempTable,
                                    void         * aRow );

    // To Fix PR-8415
    // Disk Temp Table�� ����� ��� Aggregation�� ���� �����
    // �����Ͽ��� �Ѵ�.  �̸� ���� ���������� Group�� �о�
    // ó���� �� �ִ� �������̽��� �����Ѵ�.
    // �Ϲ� ���� �˻��� ������� �ʴ� ������
    // Aggregation ����� Update�ؾ� �ϱ� �����̴�.
    // ù��° group ���� �˻�
    static IDE_RC      getFirstGroup( qmcdHashTemp * aTempTable,
                                      void        ** aRow );

    // ���� group ���� �˻�
    static IDE_RC      getNextGroup( qmcdHashTemp * aTempTable,
                                     void        ** aRow );

    //------------------------------------------------
    // Hash Temp Table�� �˻�
    //------------------------------------------------

    //-------------------------
    // ���� �˻�
    //-------------------------

    static IDE_RC      getFirstSequence( qmcdHashTemp * aTempTable,
                                         void        ** aRow );
    static IDE_RC      getNextSequence( qmcdHashTemp * aTempTable,
                                        void        ** aRow );

    //-------------------------
    // Range �˻�
    //-------------------------

    static IDE_RC      getFirstRange( qmcdHashTemp * aTempTable,
                                      UInt           aHashKey,
                                      qtcNode      * aHashFilter,
                                      void        ** aRow );
    static IDE_RC      getNextRange( qmcdHashTemp * aTempTable,
                                     void        ** aRow );

    //-------------------------
    // Hit �˻�
    //-------------------------

    static IDE_RC      getFirstHit( qmcdHashTemp * aTempTable,
                                    void        ** aRow );
    static IDE_RC      getNextHit( qmcdHashTemp * aTempTable,
                                   void        ** aRow );

    //-------------------------
    // Non-Hit �˻�
    //-------------------------

    static IDE_RC      getFirstNonHit( qmcdHashTemp * aTempTable,
                                       void        ** aRow );
    static IDE_RC      getNextNonHit( qmcdHashTemp * aTempTable,
                                      void        ** aRow );

    //-------------------------
    // Same Row And Non-Hit �˻�
    //-------------------------

    static IDE_RC      getSameRowAndNonHit( qmcdHashTemp * aTempTable,
                                            void         * aRow,
                                            void        ** aResultRow );

    //-------------------------
    // Null Row �˻�
    //-------------------------

    static IDE_RC      getNullRow( qmcdHashTemp * aTempTable,
                                   void        ** aRow );
    //-------------------------
    // To Fix PR-8213
    // Same Group �˻� (Group Aggregation)������ ���
    //-------------------------

    static IDE_RC      getSameGroup( qmcdHashTemp  * aTempTable,
                                     void         ** aRow,
                                     void         ** aResultRow );

    //------------------------------------------------
    // Hash Temp Table�� ��Ÿ �Լ�
    //------------------------------------------------

    // �˻��� Record�� Hit Flag ����
    static IDE_RC      setHitFlag( qmcdHashTemp * aTempTable );

    // �˻��� Record�� Hit Flag�� �����Ǿ� �ִ��� Ȯ��
    static idBool      isHitFlagged( qmcdHashTemp * aTempTable );

    static IDE_RC      getDisplayInfo( qmcdHashTemp * aTempTable,
                                       ULong        * aDiskPage,
                                       SLong        * aRecordCnt,
                                       UInt         * aBucketCnt );

private:

    //------------------------------------------------
    // �ʱ�ȭ�� ���� �Լ�
    //------------------------------------------------

    // Memory Temp Table�� ���� NULL ROW ����
    static IDE_RC makeMemNullRow( qmcdHashTemp * aTempTable );

    //------------------------------------------------
    // �ʱ�ȭ�� ���� �Լ�
    //------------------------------------------------

    static IDE_RC getHashKey( qmcdHashTemp * aTempTable,
                              void         * aRow,
                              UInt         * aHashKey );
};

#endif /* _O_QMC_HASH_TMEP_TABLE_H_ */
