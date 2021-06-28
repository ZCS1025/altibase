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
 * $Id: utmDbmsMeta.h 86068 2019-08-27 06:13:27Z bethy $
 **********************************************************************/

#ifndef _O_UTMDBMSMETA_H_
#define _O_UTMDBMSMETA_H_ 1

#include <idl.h>
#include <sqlcli.h>
#include <ute.h>
#include <utm.h>
#include <utmDef.h>

enum utmDdlType
{
    DDL = 1,
    USER_DDL,
    DEPENDENT_DDL,
    STATS_DDL
};

class utmDbmsMeta
{
public:
    utmDbmsMeta();
    ~utmDbmsMeta();

    IDE_RC initialize(SQLHDBC aDbc);
    IDE_RC finalize();

    IDE_RC getDdl(utmDdlType   aDdlType,
                  const SChar *aObjType,
                  SChar       *aObjName,
                  SChar       *aSchema);

    IDE_RC getUserDdl(
                  SChar      *aUserName,
                  SChar      *aUserPasswd);

    inline SChar* getDdlStr();

private:
    SQLHSTMT     mStmt;
    SQLHSTMT     mStmt4User;
    SQLHSTMT     mStmt4Dep;
    SQLHSTMT     mStmt4Stats;
    SChar        mObjType[20];
    SChar        mBindParam1[UTM_NAME_LEN];
    SChar        mBindParam2[UTM_NAME_LEN];
    SQLLEN       mBindParam2Ind;
    SQLLEN       mDdlInd;
    SChar        mDdl[QUERY_LEN * QUERY_LEN];

    IDE_RC existsDbmsMetadata(SQLHDBC aDbc);
    IDE_RC init4Ddl(SQLHDBC aDbc);
    IDE_RC init4UserDdl(SQLHDBC aDbc);
    IDE_RC init4DepDdl(SQLHDBC aDbc);
    IDE_RC init4StatsDdl(SQLHDBC aDbc);
};

inline SChar* utmDbmsMeta::getDdlStr()
{
    return mDdl;
}

#endif // _O_UTMDBMSMETA_H_

