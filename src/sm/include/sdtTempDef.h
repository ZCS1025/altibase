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
 * $Id: sdtTempDef.h 24767 2008-01-08 07:42:33Z newdaily $
 **********************************************************************/

#ifndef _O_SDT_TEMP_DEF_H_
# define _O_SDT_TEMP_DEF_H_ 1

#include <sdtDef.h>

/*****************************************************************************
 * PROJ-2201 Innovation in sorting and hashing(temp)
 *****************************************************************************/

/*********************************************************************
 *
 * - SortTemp�� State Transition Diagram
 *      +------------------------------------------------------+
 *      |                                                      |
 * +------------+                                         +----------+
 * |InsertNSort |----<sort()>--+                          |InsertOnly|
 * +------------+   +----------+-------------------+      +----------+
 *      |           |          |                   |           |
 *      |   +------------+ +-------+ +---------+   |           |
 *   sort() |ExtractNSort|-| Merge |-|MakeIndex| sort()      scan()
 *      |   +------------+ +-------+ +---------+   |           |
 *      |                      |          |        |           |
 * +------------+        +---------+ +---------+   |      +----------+
 * |InMemoryScan|        |MergeScan| |IndexScan|---+      |   Scan   |
 * +------------+        +---------+ +---------+          +----------+
 *
 * �� ���¿� ���� Group���� ���еǸ�, Group�� ���� �Ʒ� Copy ������ �޶�����.
 *
 *
 *
 * 1. InsertNSort
 *  InsertNSort�� �ַη� �����Ͱ� ���ԵǴ� �����̴�. �ٸ� �����ϸ鼭 ���ÿ�
 * ���ĵ� �Ѵ�.
 *  �׸��� �� ���´� Limit,MinMax�� InMemoryOnly �����̳�, Range�̳�, �׿ܳ�
 *  �� ���� Group�� ����� �޶�����.
 *
 * 1) InsertNSort( InMemoryOnly )
 *  Limit, MinMax�� �ݵ�� ������ ������ �Ͼ�� �ʴ´ٴ� ������ ���� ���
 *  InMemoryOnly�� �����Ѵ�.
 * +-------+ +-------+
 * | Sort1 |-| Sort2 |
 * +-------+ +-------+
 *     |         |
 *     +<-SWAP->-+
 * �ΰ��� SortGroup�� �ְ�, �� SortGroup�� ���� �������� Compaction�뵵��
 * SWAP�� �����Ѵ�.
 *
 * 2) InsertNSort ( Normal )
 *  �Ϲ����� ���·� Merge���� �ϴ� �����̴�.
 * +-------+ +-------+
 * | Sort  |-| Flush |
 * +-------+ +-------+
 *     |          |
 *     +-NORMAL->-+
 *
 * 3) ExtractNSort ( RangeScan )
 *  RangeScan�� �ʿ��� Index�� ����� �����̴�. Normal�� ������,
 * Key�� Row�� ���� �����Ѵ�. Row���� ��� Į���� ����ְ�, Key�� IndexColumn
 * �� ����. ���� Merge���� ������ Key�� ������ ó���Ѵ�.
 * +-------+ +-------+ +--------+
 * | Sort  |-| Flush |-|SubFlush|
 * +-------+ +-------+ +--------+
 *     |           |        |
 *     +-MAKE_KEY--+        |
 *     |                    |
 *     +-<-<-<-EXTRACT_ROW--+
 *
 *
 *
 *  2. ExtractNSort
 *  InsertNSort�� 1.3�� ����������, insert�� �����͸� QP�κ��� �޴µ�����
 *  Extract�� 1.3 �������� SubFlush�� �����ص� Row�� �ٽ� ������ �����Ѵٴ�
 *  ���� Ư¡�̴�.
 * +-------+ +-------+ +--------+
 * | Sort  |-| Flush |-|SubFlush|
 * +-------+ +-------+ +--------+
 *     |           |        |
 *     +-MAKE_KEY->+        |
 *     |                    |
 *     +-<-<--<-NORMAL------+
 *
 *
 *
 *  3. Merge
 *  SortGroup�� Heaq�� ������ �� FlushGroup���� �����ϸ�, FlushGroup����
 *  Run�� �����Ѵ�. �� InsertNSort(Normal) 1.2�� �����ϴ�.
 *
 *
 *
 *  4. MakeIndex
 *  Merge�� ����������, ExtraPage��� ������ Row�� �ɰ���. �ֳ��ϸ� index��
 *  �� Node���� 2���� Key�� ���� �������̸�, ���� 2�� �̻��� Key��
 *  �� �� �ֵ���, Key�� 4000Byte�� ������ ���� Row�� 4000Byte�� �ǵ���
 *  4000Byte ������ �͵��� ExtraPage�� �����Ų��.
 *
 *  1) LeafNode ����
 * +-------+ +-------+ +--------+
 * | Sort  |-| Flush |-|SubFlush|
 * +-------+ +-------+ +--------+
 *   |        |              | |
 *   |        +-<-MAKE_LNODE-+ |
 *   |                         |
 *   +-(copyExtraRow())-->->->-+
 *
 *  2) InternalNode ����
 *  LeafNode�� Depth�� �Ʒ����� ���� �ö󰡸� ����� ����̴�. �����
 *  BreadthFirst�� �����Ͽ� �ӵ��� ������ ����.
 * +-------+ +-------+ +--------+
 * | Sort  |-| Flush |-|SubFlush|
 * +-------+ +-------+ +--------+
 *            |              |
 *            +-<-MAKE_INODE-+
 *
 *
 *
 *  5. InMemoryScan
 *  InsertNSort(Normal) 1.2�� �����ϴ�.
 *
 *
 *
 *  6. MergeScan
 *  Merge�� �����ϴ�.
 *
 *
 *
 *  7.IndexScan
 *  Copy�� ����.
 *  +-------+ +-------+
 *  | INode |-| LNode |
 *  +-------+ +-------+
 *
 * 8. InsertOnly
 *  InsertOnly �ַη� �����Ͱ� ���ԵǴ� �����̴�. �ٸ� �����ϸ鼭
 *   ������ ���� �ʴ°��� InsertNSort �� ������.
 *
 * 1) InsertOnly( InMemoryOnly )
 *   �ݵ�� ������ ������ �Ͼ�� �ʴ´ٴ� ������ ���� ���
 *  InMemoryOnly�� �����Ѵ�.
 *
 * 2) InsertOnly ( Normal )
 *  �Ϲ����� ���·� ����� ���� ������ ����.
 */

typedef enum sdtCopyPurpose
{
    SDT_COPY_NORMAL,
    SDT_COPY_SWAP,
    SDT_COPY_MAKE_KEY,
    SDT_COPY_MAKE_LNODE,
    SDT_COPY_MAKE_INODE,
    SDT_COPY_EXTRACT_ROW,
} sdtCopyPurpose;

/* make MergeRun
 * +------+------+------+------+------+
 * | Size |0 PID |0 Slot|1 PID |1 Slot|
 * +------+------+------+------+------+
 * MergeRun�� ���Ͱ��� ����� Array��, MergeRunCount * 2 + 1 ���� �����ȴ�.
 */
typedef struct sdtTempMergeRunInfo
{
    /* Run��ȣ.
     * Run�� Last���� �������� ��ġ�Ǹ�, �ϳ��� Run�� MaxRowPageCount�� ũ�⸦
     * ���� ������, LastWPID - MaxRowPageCnt * No �ϸ� Run�� WPID�� �� ��
     * �ִ�. */
    UInt    mRunNo;
    UInt    mPIDSeq;  /*Run�� PageSequence */
    SShort  mSlotNo;  /*Page�� Slot��ȣ */
} sdtTempMergeRunInfo;

#define SDT_TEMP_RUNINFO_NULL ( ID_UINT_MAX )

#define SDT_TEMP_MERGEPOS_SIZEIDX     ( 0 )
#define SDT_TEMP_MERGEPOS_PIDIDX(a)   ( a * 2 + 1 )
#define SDT_TEMP_MERGEPOS_SLOTIDX(a)  ( a * 2 + 2 )

#define SDT_TEMP_MERGEPOS_SIZE(a)     ( a * 2 + 1 )

typedef scPageID sdtTempMergePos;


/* make ScanRun
 * +------+-----+-------+-------+------+
 * | Size | pin | 0 PID | 1 PID |
 * +------+-----+-------+-------+------+
 *  ScanRund ��  ���� ���� ����� array�� RunCount +2  �� �����ȴ�.
 */
#define SDT_TEMP_SCANPOS_SIZEIDX     ( 0 )
#define SDT_TEMP_SCANPOS_PINIDX      ( 1 )
#define SDT_TEMP_SCANPOS_HEADERIDX   ( 2 )

#define SDT_TEMP_SCANPOS_PIDIDX(a)   ( a + 2 )
#define SDT_TEMP_SCANPOS_SIZE(a)     ( a + 2 )

typedef scPageID sdtTempScanPos;

#endif /* _O_SDT_TEMP_DEF_H_ */
