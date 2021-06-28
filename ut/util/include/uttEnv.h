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
 
#ifndef _O_UTTENV_H_
#define _O_UTTENV_H_ 1

#include <acp.h>
#include <idl.h>
#include <ideErrorMgr.h>

/* BUG-47652 Set file permission */
/* Refactoring duplicated definition */
#define ENV_PRODUCT_PREFIX          ""
#define EXPORT_PRODUCT_LNAME        "aexport"
#define EXPORT_PRODUCT_UNAME        "AEXPORT"
#define ENV_ALTIBASE_PORT_NO        "ALTIBASE_PORT_NO"
#define ENV_ALTIBASE_IPCDA_PORT_NO  "ALTIBASE_IPCDA_PORT_NO"
#define PROPERTY_PORT_NO            "PORT_NO"
#define PROPERTY_IPCDA_PORT_NO      "IPCDA_PORT_NO"
#define DEFAULT_PORT_NO             20300

#define ISQL_PRODUCT_NAME           PRODUCT_PREFIX"isql"

#define ENV_ISQL_PREFIX             ENV_PRODUCT_PREFIX"ISQL_"
#define ENV_ILO_PREFIX              ENV_PRODUCT_PREFIX"ILO_"
#define ENV_AEXPORT_PREFIX          ENV_PRODUCT_PREFIX"AEXPORT_"

/* BUG-40407 SSL */
#define ENV_ISQL_CONNECTION         ENV_ISQL_PREFIX"CONNECTION"
/* BUG-41406 */
/* BUG-41281 SSL */
#define ENV_ALTIBASE_SSL_PORT_NO    ALTIBASE_ENV_PREFIX"SSL_PORT_NO"


/* BUG-47652 Set file permission */
/* Theoretically almost no limit env. variable name length in Linux */
#define ENV_NAME_LEN                    1024
#define DEFAULT_FILE_PERM               438 /* rwrwrw */

#define ENV_FILE_PERMISSION             "FILE_PERMISSION"
#define ENV_ALTIBASE_UT_FILE_PERMISSION ALTIBASE_ENV_PREFIX"UT_"ENV_FILE_PERMISSION
#define ENV_ISQL_FILE_PERMISSION        ENV_ISQL_PREFIX     ENV_FILE_PERMISSION
#define ENV_AEXPORT_FILE_PERMISSION     ENV_AEXPORT_PREFIX  ENV_FILE_PERMISSION
#define ENV_ILO_FILE_PERMISSION         ENV_ILO_PREFIX      ENV_FILE_PERMISSION

/* BUG-47652 Set file permission */
class uttEnv
{
    public:
        static inline IDE_RC setFilePermission( const SChar *aFilePermStr, 
                                                UInt        *aFilePerm,
                                                idBool      *abExistFilePerm )
        {
            /***********************************************************************
             *
             * Description :
             *    사용자 입력 File Permission 환경 변수값이 있으면 
             *    fchmod에서 사용할 값으로 변경해 aFilePerm에 할당.
             *    e.g. 600 -> 384, 606 -> 390, 666 -> 438
             *
             * Implementation :
             *    inline을 사용한 이유는 iloader_api에서 uttEnv dependency를 없애기 위한 
             *    것(uttEnv 라이브러리없이 컴파일 가능)이 목적.
             *
             ***********************************************************************/
            UInt sFilePermInput;
            UInt sUsr;
            UInt sGrp;
            UInt sOth;

            if ( aFilePermStr != NULL && ( idlOS::strlen(aFilePermStr) > 0) )
            {
                sFilePermInput = (UInt) idlOS::atoi( aFilePermStr );

                sOth = sFilePermInput % 10;
                sGrp = ( sFilePermInput / 10 ) % 10;
                sUsr = ( sFilePermInput / 100 ) % 10;

                IDE_TEST( sOth >= 8 );
                IDE_TEST( sGrp >= 8 );
                IDE_TEST( sUsr >= 8 );

                *aFilePerm = (sUsr << 6) | (sGrp << 3) | sOth;
                
                if ( abExistFilePerm != NULL )
                {
                    *abExistFilePerm = ID_TRUE;
                }
            }

            return IDE_SUCCESS;

            IDE_EXCEPTION_END;

            return IDE_FAILURE;
        };
};

#endif // _O_UTTENV_H_

