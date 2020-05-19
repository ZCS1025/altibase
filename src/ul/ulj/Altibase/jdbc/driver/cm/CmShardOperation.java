/*
 * Copyright (c) 1999~2017, Altibase Corp. and/or its affiliates. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License, version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

package Altibase.jdbc.driver.cm;

import Altibase.jdbc.driver.datatype.Column;
import Altibase.jdbc.driver.datatype.ColumnFactory;
import Altibase.jdbc.driver.ex.Error;
import Altibase.jdbc.driver.ex.ErrorDef;
import Altibase.jdbc.driver.sharding.core.*;
import Altibase.jdbc.driver.sharding.util.Range;

import java.sql.SQLException;
import java.sql.Types;
import java.util.List;

import static Altibase.jdbc.driver.sharding.util.ShardingTraceLogger.shard_log;

public class CmShardOperation extends CmOperationDef
{
    private static final int MAX_SERVER_IP_LEN = 16;
    private CmChannel        mChannel;
    private ColumnFactory    mColumnFactory;
    
    CmShardOperation(CmChannel aChannel)
    {
        mChannel = aChannel;
        // BUG-46513 ColumnFactory객체를 채널로부터 할당 받는다.
        mColumnFactory = aChannel.getColumnFactory();
    }
    
    /**
     * 통신버퍼에 DB ShardNodeGetList(91) 프로토콜을 쓴다.
     */
    void writeGetNodeList() throws SQLException
    {
        mChannel.checkWritable(1);
        mChannel.writeOp(DB_OP_SHARD_NODE_GET_LIST);
    }

    /**
     * 통신버퍼에 DB ShardNodeUpdateList(89) 프로토콜을 쓴다.
     */
    void writeUpdateNodeList() throws SQLException
    {
        mChannel.checkWritable(1);
        mChannel.writeOp(DB_OP_SHARD_NODE_UPDATE_LIST);
    }

    /**
     * 프로토콜을 통해 DB ShardNodeUpdateList(89)의 결과를 저장한다.
     * @param aShardContextConnect 샤드컨텍스트 객체
     */
    void readUpdateNodeListResult(CmProtocolContextShardConnect aShardContextConnect) throws SQLException
    {
        shard_log("(Update Node List Result) Shard Node List Update Result");
        ShardNodeConfig sShardNodeConfig = makeShardNodeConfig(aShardContextConnect,
                                                               DB_OP_SHARD_NODE_UPDATE_LIST_RESULT);
        if (sShardNodeConfig != null) // BUG-46513 getNodeList프로토콜의 결과로 에러가 발생한 경우에는 건너뛴다.
        {
            aShardContextConnect.setShardNodeConfig(sShardNodeConfig);
            aShardContextConnect.setShardMetaNumber(mChannel.readLong());
            if (sShardNodeConfig.getNodeCount() == 0)
            {
                Error.throwSQLException(ErrorDef.SHARD_NO_NODES);
            }
        }
    }

    /**
     * 프로토콜을 통해 DB ShardNodeGetList(91)의 결과를 저장한다.
     * @param aShardContextConnect 샤드컨텍스트 객체
     */
    void readGetNodeListResult(CmProtocolContextShardConnect aShardContextConnect) throws SQLException
    {
        ShardNodeConfig sShardNodeConfig = makeShardNodeConfig(aShardContextConnect,
                                                               DB_OP_SHARD_NODE_GET_LIST_RESULT);
        if (sShardNodeConfig != null) // BUG-46513 getNodeList프로토콜의 결과로 에러가 발생한 경우에는 건너뛴다.
        {
            long sShardPin = mChannel.readLong();
            long sShardMetaNumber = mChannel.readLong();
            aShardContextConnect.setShardNodeConfig(sShardPin, sShardMetaNumber, sShardNodeConfig);
            if (sShardNodeConfig.getNodeCount() == 0)
            {
                Error.throwSQLException(ErrorDef.SHARD_NO_NODES);
            }
        }
        shard_log("(READ GET NODELIST RESULT) {0} ", aShardContextConnect);
    }

    /**
     * 데이터 노드의 정보를 프로토콜을 통해 받아와 구성한다.
     * @param aContext ShardConnect 컨텍스트 객체
     * @param aOp SHARD_NODE_GET_LIST_RESULT 또는 SHARD_NODE_UPDATE_LIST_RESULT op 코드
     * @return ShardNodeConfig객체
     * @throws SQLException 서버와 통신 중 에러가 발생했을 때
     */
    private ShardNodeConfig makeShardNodeConfig(CmProtocolContextShardConnect aContext, byte aOp) throws SQLException
    {
        byte sOp = mChannel.readOp();
        if (sOp == DB_OP_ERROR_RESULT)
        {
            // BUG-46513 getNodeList 프로토콜의 결과로 에러가 리턴되면 에러값을 셋팅하고 null을 리턴한다.
            CmOperation.readErrorResult(mChannel, aContext);
            return null;
        }
        else if (sOp != aOp)
        {
            Error.throwInternalError(ErrorDef.INVALID_OPERATION_PROTOCOL, String.valueOf(sOp));
        }

        ShardNodeConfig sShardNodeConfig = new ShardNodeConfig();
        byte sIsTestEnable = mChannel.readByte();
        sShardNodeConfig.setTestEnable(sIsTestEnable == 1);
        if (sIsTestEnable > 1)
        {
            Error.throwSQLException(ErrorDef.SHARD_INVALID_TEST_MARK, "Invalid test mark.");
        }
        int sNodeCount = mChannel.readUnsignedShort();
        sShardNodeConfig.setNodeCount(sNodeCount);
        for (int i = 0; i < sNodeCount; i++)
        {
            DataNode sDataNode = new DataNode();
            sDataNode.setNodeId(mChannel.readInt());
            short sLength = mChannel.readUnsignedByte();
            sDataNode.setNodeName(mChannel.readDecodedString(sLength, 0));
            sDataNode.setServerIp(mChannel.readString(MAX_SERVER_IP_LEN));
            sDataNode.setPortNo(mChannel.readUnsignedShort());
            sDataNode.setAlternativeServerIp(mChannel.readString(MAX_SERVER_IP_LEN));
            sDataNode.setAlternativePortNo(mChannel.readUnsignedShort());
            sShardNodeConfig.addNode(sDataNode);
        }
        return sShardNodeConfig;
    }

    /**
     * 통신버퍼에 DB ShardAnalyze(87) 프로토콜을 쓴다.
     * @param aSql sql string
     */
    void writeShardAnalyze(String aSql) throws SQLException
    {
        int sBytesLen = mChannel.prepareToWriteString(aSql, WRITE_STRING_MODE_DB);

        mChannel.checkWritable(10);
        mChannel.writeOp(DB_OP_SHARD_ANALYZE);
        mChannel.writeInt(0);             // statement id
        mChannel.writeByte((byte)0);      // mode : dummy(0)
        mChannel.writeInt(sBytesLen);     // statement string length
        mChannel.writePreparedString();   // statement string
    }

    /**
     * 프로토콜 DB ShardAnalyze(87)의 결과를 저장한다.
     * @param aShardContextStmt shard statement context 객체
     */
    void readShardAnalyze(CmProtocolContextShardStmt aShardContextStmt) throws SQLException
    {
        if (mChannel.readOp() != DB_OP_SHARD_ANALYZE_RESULT)
        {
            CmOperation.readErrorResult(mChannel, aShardContextStmt);
            // analyze에 실해하면 serverside flag를 활성화 한다.
            aShardContextStmt.getShardAnalyzeResult().setShardCoordinate(true);
            shard_log("(SHARD ANALYZE FAILED) use coordinator");
            return;
        }
        makeShardPrepareResult(aShardContextStmt.getShardPrepareResult());
        CmShardAnalyzeResult sShardAnalyzeResult = aShardContextStmt.getShardAnalyzeResult();
        makeShardAnalyzeResult(sShardAnalyzeResult);
        makeShardValueMetaInfo(new ShardValueInfoPrimaryStrategy(sShardAnalyzeResult),
                sShardAnalyzeResult.getShardKeyDataType());

        if (sShardAnalyzeResult.isShardSubKeyExists())
        {
            makeShardSubKeyAnalyzeResult(sShardAnalyzeResult);
            makeShardValueMetaInfo(new ShardValueInfoSubKeyStrategy(sShardAnalyzeResult),
                    sShardAnalyzeResult.getShardSubKeyDataType());
        }
        shard_log("(SHARD ANALZE RESULT) {0}", sShardAnalyzeResult);

        ShardNodeConfig sShardNodeConfig = aShardContextStmt.getShardContextConnect().getShardNodeConfig();
        makeShardValueRangeInfo(aShardContextStmt.getShardAnalyzeResult(), sShardNodeConfig);
        makeCoordinateInfo(aShardContextStmt);
    }

    private void makeCoordinateInfo(CmProtocolContextShardStmt aShardContextStmt)
    {
        CmShardAnalyzeResult sShardAnalyzeResult = aShardContextStmt.getShardAnalyzeResult();
        if (!sShardAnalyzeResult.canMerge())
        {
            sShardAnalyzeResult.setShardCoordinate(true);
        }
        else
        {
            // BUG-45640 autocommit on 이고 global transaction 상태인 경우 server side로 수행한다.
            int sShardRangeInfoCnt = sShardAnalyzeResult.getShardRangeInfoCnt();
            if (aShardContextStmt.isAutoCommitMode() &&
                aShardContextStmt.getShardTransactionLevel() == ShardTransactionLevel.GLOBAL &&
                    sShardRangeInfoCnt > 1)
            {
                ShardSplitMethod sShardSplitMethod = sShardAnalyzeResult.getShardSplitMethod();
                if (sShardSplitMethod == ShardSplitMethod.HASH ||
                    sShardSplitMethod == ShardSplitMethod.RANGE ||
                    sShardSplitMethod == ShardSplitMethod.LIST)
                {
                    if (!sShardAnalyzeResult.isShardSubKeyExists())
                    {
                        if (sShardAnalyzeResult.getShardValueCount() != 1)
                        {
                            sShardAnalyzeResult.setShardCoordinate(true);
                        }
                    }
                    else
                    {
                        if (sShardAnalyzeResult.getShardValueCount() != 1 ||
                                sShardAnalyzeResult.getShardSubValueCount() != 1)
                        {
                            sShardAnalyzeResult.setShardCoordinate(true);
                        }
                    }
                }
                else if (sShardSplitMethod == ShardSplitMethod.CLONE)
                {
                    if (sShardAnalyzeResult.getShardValueCount() != 0)
                    {
                        sShardAnalyzeResult.setShardCoordinate(true);
                    }
                }
            }
        }
    }

    /**
     * 샤드키 값에 대한 Range 정보를 받아온다.
     * @param aShardAnalyzeResult shard statement analyze 객체
     * @param aShardNodeConfig 샤드 데이터 노드 구성 정보
     * @throws SQLException 서버와의 통신 중 에러가 발생한 경우
     */
    private void makeShardValueRangeInfo(CmShardAnalyzeResult aShardAnalyzeResult,
                                         ShardNodeConfig aShardNodeConfig) throws SQLException
    {
        int sShardRangeInfoCnt = mChannel.readShort();
        ShardKeyDataType sShardKeyDataType = aShardAnalyzeResult.getShardKeyDataType();
        ShardSplitMethod sShardSplitMethod = aShardAnalyzeResult.getShardSplitMethod();
        ShardKeyDataType sShardSubKeyDateType = aShardAnalyzeResult.getShardSubKeyDataType();
        ShardSplitMethod sShardSubSplitMethod = aShardAnalyzeResult.getShardSubSplitMethod();

        ShardRangeList sShardRangeList = new ShardRangeList();
        for (int i = 0; i < sShardRangeInfoCnt; i++)
        {
            Range sRange = readShardRangeInfo(sShardKeyDataType, sShardRangeList.getCurrRange(), sShardSplitMethod);
            Range sSubRange = null;

            if (aShardAnalyzeResult.isShardSubKeyExists())
            {
                Range aCurrRange = sShardRangeList.getCurrRange();
                // subkey가 true일 경우 primary range의 범위가 변경되었을때 flag를 셋팅해 줘야 한다.
                if (aCurrRange != null && !sRange.equals(aCurrRange))
                {
                    sShardRangeList.setPrimaryRangeChanged(true);
                }

                sSubRange = readShardRangeInfo(sShardSubKeyDateType, sShardRangeList.getCurrSubRange(),
                                               sShardSubSplitMethod);
            }

            int sNodeId = mChannel.readInt();
            DataNode sNode = aShardNodeConfig.getNode(sNodeId);
            sShardRangeList.addRange(new ShardRange(sNode, sRange, sSubRange));
        }
        aShardAnalyzeResult.setShardRangeInfo(sShardRangeList);
        shard_log("(SHARD RANGE INFO) {0}", sShardRangeList);
    }

    @SuppressWarnings("unchecked")
    private Range readShardRangeInfo(ShardKeyDataType aShardKeyDataType, Range aPrevRange,
                                     ShardSplitMethod aShardSplitMethod) throws SQLException
    {
        Range sRange = null;
        // hash인 경우에는 샤드키값을 Integer로 설정한다.
        if (aShardSplitMethod == ShardSplitMethod.HASH)
        {
            if (aPrevRange == null)
            {
                aPrevRange = Range.getNullRange();
            }
            // BUG-46642 HP Unix용 JDK1.5.0.30 에서 컴파일이 안되는 문제가 있어 제너릭메소드 호출시 타입을 명시한다.
            sRange = Range.<Integer>between(aPrevRange, mChannel.readInt());
        }
        // clone이나 solo인 경우에는 shard range 정보가 없기 때문에 Range정보를 생성하지 않는다.
        else if (aShardSplitMethod == ShardSplitMethod.LIST || aShardSplitMethod == ShardSplitMethod.RANGE)
        {
            aPrevRange = (aPrevRange == null) ? Range.getNullRange() : aPrevRange;
            switch (aShardKeyDataType)
            {
                case SMALLINT:
                    sRange = Range.<Short>between(aPrevRange, mChannel.readShort());
                    break;
                case INTEGER:
                    sRange = Range.<Integer>between(aPrevRange, mChannel.readInt());
                    break;
                case BIGINT:
                    sRange = Range.<Long>between(aPrevRange, mChannel.readLong());
                    break;
                case CHAR:
                case VARCHAR:
                    int sLength = mChannel.readShort();
                    String sStr = mChannel.readDecodedString(sLength, 0);
                    sRange = Range.between(aPrevRange, sStr);
                    break;
            }
        }

        return sRange;
    }

    private void makeShardSubKeyAnalyzeResult(CmShardAnalyzeResult aShardAnalyzeResult) throws SQLException
    {
        aShardAnalyzeResult.setShardSubSplitMethod(ShardSplitMethod.get(mChannel.readByte()));
        aShardAnalyzeResult.setShardSubKeyDataType(ShardKeyDataType.get(mChannel.readInt()));
        aShardAnalyzeResult.setShardSubValueCount(mChannel.readShort());
    }

    /**
     * loop를 돌면서 shard value 값의 메타 정보를 구성한다.
     * @param aStrategy shard value info를 저장할 전략객체(primary or subkey)
     * @param aShardKeyDataType 샤드 키 데이터 타입
     * @throws SQLException 서버와 통신 중 오류가 발생한 경우
     */
    private void makeShardValueMetaInfo(ShardValueInfoStrategy aStrategy,
                                        ShardKeyDataType aShardKeyDataType) throws SQLException
    {
        for (int i = 0; i < aStrategy.getShardValueCount(); i++)
        {
            ShardValueInfo sShardValueInfo = new ShardValueInfo();
            byte sType = mChannel.readByte();  // type
            sShardValueInfo.setType(sType);
            if (sType == 0)         // prepared execute
            {
                sShardValueInfo.setBindParamid(mChannel.readShort());
            }
            else if (sType == 1)    // direct execute
            {
                ShardSplitMethod sShardSplitMethod = aStrategy.getShardSplitMethod();
                if (sShardSplitMethod == ShardSplitMethod.HASH  ||
                    sShardSplitMethod == ShardSplitMethod.RANGE ||
                    sShardSplitMethod == ShardSplitMethod.LIST)
                {
                    // direct execute인 경우에는 바로 shard 값을 받아온다.
                    readShardValue(sShardValueInfo, aShardKeyDataType);
                }
            }

            aStrategy.addShardValue(sShardValueInfo);
            shard_log("(SHARD VALUE META INFO) {0} ", sShardValueInfo);
        }
    }

    private void readShardValue(ShardValueInfo sShardValueInfo,
                                ShardKeyDataType aShardKeyDataType) throws SQLException
    {
        Column sColumn = null;

        switch (aShardKeyDataType)
        {
            case SMALLINT:
                sColumn = mColumnFactory.getInstance(Types.SMALLINT);
                sColumn.setValue(mChannel.readShort());
                break;
            case INTEGER:
                sColumn = mColumnFactory.getInstance(Types.INTEGER);
                sColumn.setValue(mChannel.readInt());
                break;
            case BIGINT:
                sColumn = mColumnFactory.getInstance(Types.BIGINT);
                sColumn.setValue(mChannel.readUnsignedLong());
                break;
            case CHAR:
            case VARCHAR:
                sColumn = mColumnFactory.getInstance(Types.VARCHAR);
                int sLength = mChannel.readShort();
                sColumn.setValue(mChannel.readString(sLength));
                break;
        }
        sShardValueInfo.setValue(sColumn);
    }

    /**
     * 샤드 메타 정보를 받아온다.
     * @param aShardAnalyzeResult 샤드 메타 정보를 저장할 객체
     * @throws SQLException 서버와 통신 중 에러가 발생한 경우
     */
    private void makeShardAnalyzeResult(CmShardAnalyzeResult aShardAnalyzeResult) throws SQLException
    {
        aShardAnalyzeResult.setShardSplitMethod(ShardSplitMethod.get(mChannel.readByte()));
        aShardAnalyzeResult.setShardKeyDataType(ShardKeyDataType.get(mChannel.readInt()));
        aShardAnalyzeResult.setShardDefaultNodeID(mChannel.readInt());
        aShardAnalyzeResult.setShardSubKeyExists(mChannel.readByte());
        aShardAnalyzeResult.setShardCanMerge(mChannel.readByte());
        aShardAnalyzeResult.setShardValueCount(mChannel.readShort());
    }

    /**
     * prepare 결과를 받아온다.
     * @param aPrepareResult prepare결과를 저장하는 객체
     * @throws SQLException 서버와 통신 중 에러가 발생한 경우
     */
    private void makeShardPrepareResult(CmPrepareResult aPrepareResult) throws SQLException
    {
        aPrepareResult.setStatementId(mChannel.readInt());
        aPrepareResult.setStatementType(mChannel.readInt());
        aPrepareResult.setParameterCount(mChannel.readShort());
        aPrepareResult.setResultSetCount(mChannel.readShort());
        shard_log("(SHARD PREPARE RESULT) {0} ", aPrepareResult);
    }

    void writeShardTransactionCommitRequest(List<DataNode> aTouchedNodeList) throws SQLException
    {
        short sTouchedNodeCount = (short)aTouchedNodeList.size();
        mChannel.checkWritable(2 + 2 + sTouchedNodeCount * 4);
        mChannel.writeOp(CmOperation.DB_OP_SHARD_TRANSACTION);
        mChannel.writeByte((byte)1);  // commit
        mChannel.writeShort(sTouchedNodeCount);
        for (DataNode sEach : aTouchedNodeList)
        {
            mChannel.writeInt(sEach.getNodeId());
        }
    }

    void readShardTransactionCommitRequest(CmProtocolContextShardConnect aContext) throws SQLException
    {
        byte sOp = mChannel.readOp();
        if (sOp == DB_OP_ERROR_RESULT)
        {
            CmOperation.readErrorResult(mChannel, aContext);
        }
        else if (sOp != DB_OP_SHARD_TRANSACTION_RESULT)
        {
            Error.throwInternalError(ErrorDef.INVALID_OPERATION_PROTOCOL,
                                     String.valueOf(DB_OP_SHARD_TRANSACTION_RESULT));
        }
    }
}
