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
 * $Id$
 **********************************************************************/

#include <idl.h>
#include <ide.h>
#include <mtdTypes.h>
#include <smi.h>
#include <qci.h>
#include <qcg.h>
#include <qmx.h>
#include <qmn.h>
#include <qcuProperty.h>
#include <qcm.h>
#include <qdnForeignKey.h>
#include <smiMisc.h>
#include <smDef.h>

// BUG-43843
#define SIMPLE_STMT_TUPLE( _QcStmt_, _Table_ )    \
    ( ( QC_SHARED_TMPLATE(_QcStmt_) != NULL ) ? (QC_SHARED_TMPLATE(_QcStmt_)->tmplate.rows) + (_Table_) : (QC_PRIVATE_TMPLATE(_QcStmt_)->tmplate.rows) + (_Table_) )

extern mtdModule mtdDouble;
extern mtdModule mtdDate;

/* SQL data type codes */
#define	SQL_UNKNOWN_TYPE       0
#define SQL_CHAR               1
#define SQL_NUMERIC            2
#define SQL_DECIMAL            3
#define SQL_INTEGER            4
#define SQL_SMALLINT           5
#define SQL_FLOAT              6
#define SQL_REAL               7
#define SQL_DOUBLE             8
#define SQL_DATETIME           9
#define SQL_TIME              10
#define SQL_TIMESTAMP         11
#define SQL_VARCHAR           12
#define SQL_BIGINT          (-5)
#define SQL_TYPE_TIMESTAMP    93

#define SQL_SIGNED_OFFSET   (-20)
#define SQL_UNSIGNED_OFFSET (-22)

#define SQL_C_CHAR           SQL_CHAR            /* CHAR, VARCHAR, DECIMAL, NUMERIC */
#define SQL_C_LONG           SQL_INTEGER         /* INTEGER        */
#define SQL_C_SHORT          SQL_SMALLINT        /* SMALLINT       */
#define SQL_C_DOUBLE         SQL_DOUBLE          /* FLOAT, DOUBLE  */
#define SQL_C_FLOAT          SQL_REAL            /* REAL           */
#define SQL_C_TYPE_TIMESTAMP SQL_TYPE_TIMESTAMP  /* TYPE_TIMESTAMP */

#define SQL_C_SSHORT        (SQL_C_SHORT + SQL_SIGNED_OFFSET)    /* SIGNED SMALLINT */
#define SQL_C_SLONG         (SQL_C_LONG + SQL_SIGNED_OFFSET)     /* SIGNED INTEGER  */
#define SQL_C_SBIGINT       (SQL_BIGINT + SQL_SIGNED_OFFSET)     /* SIGNED BIGINT */

#define SQL_NULL_DATA       ((SInt)-1)

typedef struct SQL_TIMESTAMP_STRUCT
{
    SShort  year;
    UShort  month;
    UShort  day;
    UShort  hour;
    UShort  minute;
    UShort  second;
    UInt    fraction;  // nanosecond
} SQL_TIMESTAMP_STRUCT;
    
IDE_RC qmxSimple::getSimpleCBigint( struct qciBindParam  * aParam,
                                    SInt                   aIndicator,
                                    UChar                * aBindBuffer,
                                    mtdBigintType        * aValue )
{
    SChar            sNumericBuf[MTD_NUMERIC_SIZE_MAXIMUM];
    mtdNumericType * sNumeric = (mtdNumericType*) &sNumericBuf;
    mtdCharType    * sCharValue;
    UInt             sLength;
    
    if ( aParam->ctype == SQL_C_CHAR )
    {
        sCharValue = (mtdCharType*)aBindBuffer;
        
        if ( aIndicator <= 0 )
        {
            sLength = sCharValue->length;
        }
        else
        {
            sLength = IDL_MIN( aIndicator, sCharValue->length );
        }
            
        IDE_TEST( mtc::makeNumeric(
                      sNumeric,
                      MTD_FLOAT_MANTISSA_MAXIMUM,
                      (const UChar*)(sCharValue->value),
                      sLength )
                  != IDE_SUCCESS );

        if ( sNumeric->length > 1 )
        {
            IDE_TEST( mtc::numeric2Slong( (SLong*)aValue, sNumeric )
                      != IDE_SUCCESS );
        }
        else
        {
            *aValue = MTD_BIGINT_NULL;
        }
    }
    else if ( aParam->ctype == SQL_C_SBIGINT )
    {
        *aValue = *((mtdBigintType*)aBindBuffer);
    }
    else if ( ( aParam->ctype == SQL_C_LONG ) ||
              ( aParam->ctype == SQL_C_SLONG ) )
    {
        *aValue = (mtdBigintType)*((SInt*)aBindBuffer);
    }
    else if ( ( aParam->ctype == SQL_C_SHORT ) ||
              ( aParam->ctype == SQL_C_SSHORT ) )
    {
        *aValue = (mtdBigintType)*((SShort*)aBindBuffer);
    }
    else if ( aParam->ctype == SQL_C_DOUBLE )
    {
        *aValue = (mtdBigintType)*((SDouble*)aBindBuffer);
    }
    else
    {
        IDE_RAISE( ERR_INVALID_TYPE );
    }

    return IDE_SUCCESS;
    
    IDE_EXCEPTION( ERR_INVALID_TYPE )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::getSimpleCBigint",
                                  "invalid type" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmxSimple::getSimpleCNumeric( struct qciBindParam  * aParam,
                                     SInt                   aIndicator,
                                     UChar                * aBindBuffer,
                                     mtdNumericType       * aNumeric )
{
    mtdCharType  * sCharValue;
    UInt           sLength;
    SLong          sLong;
    SDouble        sDouble;

    if ( aParam->ctype == SQL_C_CHAR )
    {
        sCharValue = (mtdCharType*)aBindBuffer;
        
        if ( aIndicator <= 0 )
        {
            sLength = sCharValue->length;
        }
        else
        {
            sLength = IDL_MIN( aIndicator, sCharValue->length );
        }
        
        IDE_TEST( mtc::makeNumeric(
                      aNumeric,
                      MTD_FLOAT_MANTISSA_MAXIMUM,
                      (const UChar*)(sCharValue->value),
                      sLength )
                  != IDE_SUCCESS );
    }
    else if ( aParam->ctype == SQL_C_SBIGINT )
    {
        sLong = *((SLong*)aBindBuffer);
        mtc::makeNumeric( aNumeric, sLong );
    }
    else if ( ( aParam->ctype == SQL_C_LONG ) ||
              ( aParam->ctype == SQL_C_SLONG ) )
    {
        sLong = (SLong)*((SInt*)aBindBuffer);
        mtc::makeNumeric( aNumeric, sLong );
    }
    else if ( ( aParam->ctype == SQL_C_SHORT ) ||
              ( aParam->ctype == SQL_C_SSHORT ) )
    {
        sLong = (SLong)*((SShort*)aBindBuffer);
        mtc::makeNumeric( aNumeric, sLong );
    }
    else if ( aParam->ctype == SQL_C_DOUBLE )
    {
        sDouble = *((SDouble*)aBindBuffer);
        
        IDE_TEST( mtc::makeNumeric( aNumeric,
                                    sDouble )
                  != IDE_SUCCESS );
    }
    else
    {
        IDE_RAISE( ERR_INVALID_TYPE );
    }

    return IDE_SUCCESS;
    
    IDE_EXCEPTION( ERR_INVALID_TYPE )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::getSimpleCNumeric",
                                  "invalid type" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmxSimple::getSimpleCTimestamp( struct qciBindParam  * aParam,
                                       SInt                   /*aIndicator*/,
                                       UChar                * aBindBuffer,
                                       mtdDateType          * aDate )
{
    SQL_TIMESTAMP_STRUCT * sUserDate;

    if ( aParam->ctype == SQL_C_TYPE_TIMESTAMP )
    {
        sUserDate = (SQL_TIMESTAMP_STRUCT*)(aBindBuffer);
            
        IDE_TEST( mtdDateInterface::makeDate( aDate,
                                              sUserDate->year,
                                              sUserDate->month,
                                              sUserDate->day,
                                              sUserDate->hour,
                                              sUserDate->minute,
                                              sUserDate->second,
                                              sUserDate->fraction / 1000 )
                  != IDE_SUCCESS );
    }
    else
    {
        IDE_RAISE( ERR_INVALID_TYPE );
    }

    return IDE_SUCCESS;
    
    IDE_EXCEPTION( ERR_INVALID_TYPE )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::getSimpleCTimestamp",
                                  "invalid type" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

// ctype value�� aBuffer�� �̿��ؼ� mtdValue�� ��ȯ�Ͽ� ��ȯ�Ѵ�.
IDE_RC qmxSimple::getSimpleCValue( qmnValueInfo             * aValueInfo,
                                   void                    ** aMtdValue,
                                   struct qciBindParamInfo  * aParamInfo,
                                   UChar                    * aBindBuffer,
                                   SChar                   ** aBuffer,
                                   idBool                     aNeedCanonize )
{
    struct qciBindParamInfo  * sParamInfo = aParamInfo;
    struct qciBindParam      * sParam;
    UChar                    * sBindBuffer;
    SChar                      sNumericBuf[MTD_NUMERIC_SIZE_MAXIMUM];
    mtdNumericType           * sNumeric = (mtdNumericType*) &sNumericBuf;
    mtdNumericType           * sNumericValue;
    mtdCharType              * sCharValue;
    mtdCharType              * sParamCharValue;
    mtdBigintType              sBigintValue;
    mtdDateType              * sDateValue;
    SInt                       sIndicator;
    idBool                     sCanonized;
    UInt                       i;

    // bindBuffer���� data offset�� ã�´�.
    sBindBuffer = (UChar*)idlOS::align8( (vULong)aBindBuffer );
    
    for ( i = 0; i < (UInt)aValueInfo->value.id; i++ )
    {
        sIndicator = *(SInt*)sBindBuffer;
        sBindBuffer += 8;
        
        if ( sIndicator != SQL_NULL_DATA )
        {
            if ( sParamInfo->param.ctype == SQL_C_CHAR )
            {
                sCharValue = (mtdCharType*)sBindBuffer;
                sBindBuffer += idlOS::align8( sCharValue->length + 2 );
            }
            else if ( ( sParamInfo->param.ctype == SQL_C_SBIGINT ) ||
                      ( sParamInfo->param.ctype == SQL_C_LONG )    ||
                      ( sParamInfo->param.ctype == SQL_C_SLONG )   ||
                      ( sParamInfo->param.ctype == SQL_C_SHORT )   ||
                      ( sParamInfo->param.ctype == SQL_C_SSHORT )  ||
                      ( sParamInfo->param.ctype == SQL_C_DOUBLE ) )
            {
                // ��� type�� 8byte align
                sBindBuffer += 8;
            }
            else if ( sParamInfo->param.ctype == SQL_C_TYPE_TIMESTAMP )
            {
                sBindBuffer += ID_SIZEOF(SQL_TIMESTAMP_STRUCT);
            }
            else
            {
                IDE_RAISE( ERR_INVALID_TYPE );
            }
        }
        else
        {
            // Nothing to do.
        }

        sParamInfo++;
    }

    sIndicator = *(SInt*)sBindBuffer;
    sBindBuffer += 8;
    sParam = &(sParamInfo->param);
    
    if ( sIndicator == SQL_NULL_DATA )
    {
        // bind���� �ʰų� null�� ���
        *aMtdValue = aValueInfo->column.module->staticNull;
    }
    else
    {
        if ( aValueInfo->column.module->id == MTD_SMALLINT_ID )
        {
            IDE_TEST( getSimpleCBigint( sParam,
                                        sIndicator,
                                        sBindBuffer,
                                        &sBigintValue )
                      != IDE_SUCCESS );
            if ( sBigintValue == MTD_BIGINT_NULL )
            {
                *((mtdSmallintType*) *aBuffer) = MTD_SMALLINT_NULL;
            }
            else
            {
                IDE_TEST_RAISE( ( sBigintValue < MTD_SMALLINT_MINIMUM ) ||
                                ( sBigintValue > MTD_SMALLINT_MAXIMUM ),
                                ERR_VALUE_OVERFLOW );
                
                *((mtdSmallintType*) *aBuffer) = (mtdSmallintType)sBigintValue;
            }
            *aMtdValue = (void*) *aBuffer;
        }
        else if ( aValueInfo->column.module->id == MTD_INTEGER_ID )
        {
            IDE_TEST( getSimpleCBigint( sParam,
                                        sIndicator,
                                        sBindBuffer,
                                        &sBigintValue )
                      != IDE_SUCCESS );
            if ( sBigintValue == MTD_BIGINT_NULL )
            {
                *((mtdIntegerType*) *aBuffer) = MTD_INTEGER_NULL;
            }
            else
            {
                IDE_TEST_RAISE( ( sBigintValue < MTD_INTEGER_MINIMUM ) ||
                                ( sBigintValue > MTD_INTEGER_MAXIMUM ),
                                ERR_VALUE_OVERFLOW );
                
                *((mtdIntegerType*) *aBuffer) = (mtdIntegerType)sBigintValue;
            }
            *aMtdValue = (void*) *aBuffer;
        }
        else if ( aValueInfo->column.module->id == MTD_BIGINT_ID )
        {
            IDE_TEST( getSimpleCBigint( sParam,
                                        sIndicator,
                                        sBindBuffer,
                                        &sBigintValue )
                      != IDE_SUCCESS );
            *((mtdBigintType*) *aBuffer) = sBigintValue;
            *aMtdValue = (void*) *aBuffer;
        }
        else if ( aValueInfo->column.module->id == MTD_CHAR_ID )
        {
            sCharValue = (mtdCharType*) *aBuffer;
            *aMtdValue = (void*)sCharValue;
            
            if ( sParam->ctype == SQL_C_CHAR )
            {
                sParamCharValue = (mtdCharType*) sBindBuffer;
                
                if ( sIndicator <= 0 )
                {
                    sCharValue->length = sParamCharValue->length;
                }
                else
                {
                    sCharValue->length = IDL_MIN( sIndicator, sParamCharValue->length );
                }
            
                IDE_TEST_RAISE( sCharValue->length > aValueInfo->column.precision,
                                ERR_INVALID_LENGTH );
            
                if ( sCharValue->length > 0 )
                {
                    idlOS::memcpy( sCharValue->value,
                                   sParamCharValue->value,
                                   sCharValue->length );
                }
                else
                {
                    // Nothing to do.
                }
            }
            else if ( sParam->ctype == SQL_C_SBIGINT )
            {
                sCharValue->length =
                    idlOS::snprintf( (SChar*)sCharValue->value,
                                     aValueInfo->column.precision,
                                     "%"ID_INT64_FMT,
                                     *((SLong*)sBindBuffer) );
            }
            else if ( ( sParam->ctype == SQL_C_LONG ) ||
                      ( sParam->ctype == SQL_C_SLONG ) )
            {
                sCharValue->length =
                    idlOS::snprintf( (SChar*)sCharValue->value,
                                     aValueInfo->column.precision,
                                     "%"ID_INT32_FMT,
                                     *((SInt*)sBindBuffer) );
            }
            else if ( ( sParam->ctype == SQL_C_SHORT ) ||
                      ( sParam->ctype == SQL_C_SSHORT ) )
            {
                sCharValue->length =
                    idlOS::snprintf( (SChar*)sCharValue->value,
                                     aValueInfo->column.precision,
                                     "%"ID_INT32_FMT,
                                     (SInt)*((SShort*)sBindBuffer) );
            }
            else if ( sParam->ctype == SQL_C_DOUBLE )
            {
                sCharValue->length =
                    idlOS::snprintf( (SChar*)sCharValue->value,
                                     aValueInfo->column.precision,
                                     "%"ID_DOUBLE_G_FMT,
                                     *((SDouble*)sBindBuffer) );
            }
            else
            {
                IDE_RAISE( ERR_INVALID_TYPE );
            }
            
            // space padding
            if ( ( aNeedCanonize == ID_TRUE ) &&
                 ( sCharValue->length < aValueInfo->column.precision ) )
            {
                idlOS::memset( sCharValue->value + sCharValue->length,
                               0x20,
                               aValueInfo->column.precision - sCharValue->length );

                sCharValue->length = aValueInfo->column.precision;
            }
            else
            {
                // Nothing to do.
            }
        }
        else if ( aValueInfo->column.module->id == MTD_VARCHAR_ID )
        {
            sCharValue = (mtdCharType*) *aBuffer;
            *aMtdValue = (void*)sCharValue;

            if ( sParam->ctype == SQL_C_CHAR )
            {
                sParamCharValue = (mtdCharType*) sBindBuffer;
                
                if ( sIndicator <= 0 )
                {
                    sCharValue->length = sParamCharValue->length;
                }
                else
                {
                    sCharValue->length =
                        IDL_MIN( sIndicator, sParamCharValue->length );
                }
            
                IDE_TEST_RAISE( sCharValue->length > aValueInfo->column.precision,
                                ERR_INVALID_LENGTH );

                if ( sCharValue->length > 0 )
                {
                    idlOS::memcpy( sCharValue->value,
                                   sParamCharValue->value,
                                   sCharValue->length );
                }
                else
                {
                    // Nothing to do.
                }
            }
            else if ( sParam->ctype == SQL_C_SBIGINT )
            {
                sCharValue->length =
                    idlOS::snprintf( (SChar*)sCharValue->value,
                                     aValueInfo->column.precision,
                                     "%"ID_INT64_FMT,
                                     *((SLong*)sBindBuffer) );
            }
            else if ( ( sParam->ctype == SQL_C_LONG ) ||
                      ( sParam->ctype == SQL_C_SLONG ) )
            {
                sCharValue->length =
                    idlOS::snprintf( (SChar*)sCharValue->value,
                                     aValueInfo->column.precision,
                                     "%"ID_INT32_FMT"",
                                     *((SInt*)sBindBuffer) );
            }
            else if ( ( sParam->ctype == SQL_C_SHORT ) ||
                      ( sParam->ctype == SQL_C_SSHORT ) )
            {
                sCharValue->length =
                    idlOS::snprintf( (SChar*)sCharValue->value,
                                     aValueInfo->column.precision,
                                     "%"ID_INT32_FMT,
                                     (SInt)*((SShort*)sBindBuffer) );
            }
            else if ( sParam->ctype == SQL_C_DOUBLE )
            {
                sCharValue->length =
                    idlOS::snprintf( (SChar*)sCharValue->value,
                                     aValueInfo->column.precision,
                                     "%"ID_DOUBLE_G_FMT,
                                     *((SDouble*)sBindBuffer) );
            }
            else
            {
                IDE_RAISE( ERR_INVALID_TYPE );
            }
        }
        else if ( ( aValueInfo->column.module->id == MTD_NUMERIC_ID ) ||
                  ( aValueInfo->column.module->id == MTD_FLOAT_ID ) )
        {
            sNumericValue = (mtdNumericType*) *aBuffer;
            *aMtdValue = (void*)sNumericValue;
            
            if ( aNeedCanonize == ID_TRUE )
            {
                IDE_TEST( getSimpleCNumeric( sParam,
                                             sIndicator,
                                             sBindBuffer,
                                             sNumeric )
                          != IDE_SUCCESS );

                sCanonized = ID_TRUE;
                
                if ( ( aValueInfo->column.module->id == MTD_FLOAT_ID ) ||
                     ( ( aValueInfo->column.flag & MTC_COLUMN_ARGUMENT_COUNT_MASK ) == 1 ) )
                {
                    IDE_TEST( mtc::floatCanonize( sNumeric,
                                                  sNumericValue,
                                                  aValueInfo->column.precision,
                                                  &sCanonized )
                              != IDE_SUCCESS );
                }
                else
                {
                    IDE_TEST( mtc::numericCanonize( sNumeric,
                                                    sNumericValue,
                                                    aValueInfo->column.precision,
                                                    aValueInfo->column.scale,
                                                    &sCanonized )
                              != IDE_SUCCESS );
                }
                
                if ( sCanonized == ID_FALSE )
                {
                    idlOS::memcpy( sNumericValue,
                                   sNumeric,
                                   sNumeric->length + 1 );  // actual size
                }
                else
                {
                    // Nothing to do.
                }
            }
            else
            {
                IDE_TEST( getSimpleCNumeric( sParam,
                                             sIndicator,
                                             sBindBuffer,
                                             sNumericValue )
                          != IDE_SUCCESS );
            }
        }
        else if ( aValueInfo->column.module->id == MTD_DATE_ID )
        {
            sDateValue = (mtdDateType*) *aBuffer;
            *aMtdValue = (void*)sDateValue;
            
            IDE_TEST( getSimpleCTimestamp( sParam,
                                           sIndicator,
                                           sBindBuffer,
                                           sDateValue )
                      != IDE_SUCCESS );
        }
        else
        {
            IDE_RAISE( ERR_INVALID_TYPE );
        }
    }

    *aBuffer += idlOS::align8( aValueInfo->column.column.size );

    return IDE_SUCCESS;
    
    IDE_EXCEPTION( ERR_VALUE_OVERFLOW )
    {
        IDE_SET( ideSetErrorCode( mtERR_ABORT_VALUE_OVERFLOW ) );
    }
    IDE_EXCEPTION( ERR_INVALID_LENGTH )
    {
        IDE_SET( ideSetErrorCode( mtERR_ABORT_INVALID_LENGTH ) );
    }
    IDE_EXCEPTION( ERR_INVALID_TYPE )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::getSimpleCValue",
                                  "invalid type" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmxSimple::getSimpleConstMtdValue( qcStatement * aStatement,
                                          mtcColumn   * aColumn,
                                          void       ** aMtdValue,
                                          void        * aValue,
                                          SChar      ** aBuffer,
                                          idBool        aNeedCanonize,
                                          idBool        aIsQueue,
                                          void        * aQueueMsgIDSeq )
{
    mtdNumericType * sNumeric;
    mtdNumericType * sNumericValue;
    mtdCharType    * sCharValue;
    mtdCharType    * sNewValue;
    idBool           sCanonized;
    void           * sValue;
    
    if ( ( aColumn->module->id == MTD_SMALLINT_ID ) ||
         ( aColumn->module->id == MTD_INTEGER_ID ) )
    {
            *aMtdValue = aValue;
    }
    else if ( aColumn->module->id == MTD_BIGINT_ID )
    {
        // BUG-45715 support ENQUEUE
        if ( aIsQueue == ID_TRUE )
        {
            IDE_TEST_RAISE( aQueueMsgIDSeq == NULL, ERR_NOT_EXIST_QUEUE );
            
            // queue�� messageIDĮ���� bigint type�̸�,
            // �ش� Į���� ���� ���� sequence�� �о �����Ѵ�.
            IDE_TEST( aStatement->qmxMem->alloc(
                         ID_SIZEOF(mtdBigintType),
                         &sValue)
                     != IDE_SUCCESS);

            IDE_TEST( smiTable::readSequence( QC_SMI_STMT( aStatement ),
                                              aQueueMsgIDSeq,
                                              SMI_SEQUENCE_NEXT,
                                              (mtdBigintType*)sValue,
                                              NULL )
                      != IDE_SUCCESS);
            
            *aMtdValue = sValue;
        }
        else
        {
            *aMtdValue = aValue;
        }
    }
    else if ( aColumn->module->id == MTD_CHAR_ID )
    {
        sCharValue = (mtdCharType*) aValue;
        IDE_TEST_RAISE( sCharValue->length > aColumn->precision,
                        ERR_INVALID_LENGTH );

        if ( ( sCharValue->length < aColumn->precision ) &&
             ( aNeedCanonize == ID_TRUE ) )
        {
            sNewValue = (mtdCharType*) *aBuffer;

            if ( sCharValue->length > 0 )
            {
                idlOS::memcpy( sNewValue->value,
                               sCharValue->value,
                               sCharValue->length );
                
                idlOS::memset( sNewValue->value + sCharValue->length,
                               0x20,
                               aColumn->precision - sCharValue->length );
                
                sNewValue->length = aColumn->precision;
            }
            else
            {
                sNewValue->length = 0;
            }
            
            *aMtdValue = (void*) sNewValue;
            
        }
        else
        {
            *aMtdValue = aValue;
        }
    }
    else if ( aColumn->module->id == MTD_VARCHAR_ID )
    {
        sCharValue = (mtdCharType*) aValue;
        IDE_TEST_RAISE( sCharValue->length > aColumn->precision,
                        ERR_INVALID_LENGTH );
        
        *aMtdValue = aValue;
    }
    else if ( ( aColumn->module->id == MTD_NUMERIC_ID ) ||
              ( aColumn->module->id == MTD_FLOAT_ID ) )
    {
        if ( aNeedCanonize == ID_TRUE )
        {
            sNumeric = (mtdNumericType*)aValue;
            sNumericValue = (mtdNumericType*) *aBuffer;
            *aMtdValue = (void*)sNumericValue;

            sCanonized = ID_TRUE;
            
            if ( ( aColumn->module->id == MTD_FLOAT_ID ) ||
                 ( ( aColumn->flag & MTC_COLUMN_ARGUMENT_COUNT_MASK ) == 1 ) )
            {
                IDE_TEST( mtc::floatCanonize( sNumeric,
                                              sNumericValue,
                                              aColumn->precision,
                                              &sCanonized )
                          != IDE_SUCCESS );
            }
            else
            {
                IDE_TEST( mtc::numericCanonize( sNumeric,
                                                sNumericValue,
                                                aColumn->precision,
                                                aColumn->scale,
                                                &sCanonized )
                          != IDE_SUCCESS );
            }
                
            if ( sCanonized == ID_FALSE )
            {
                idlOS::memcpy( sNumericValue,
                               sNumeric,
                               sNumeric->length + 1 );  // actual size
            }
            else
            {
                // Nothing to do.
            }
        }
        else
        {
            *aMtdValue = aValue;
        }
    }
    else if ( aColumn->module->id == MTD_DATE_ID )
    {
        *aMtdValue = aValue;
    }
    else
    {
        IDE_RAISE( ERR_INVALID_TYPE );
    }

    *aBuffer += idlOS::align8( aColumn->column.size );
    
    return IDE_SUCCESS;
    
    IDE_EXCEPTION( ERR_INVALID_LENGTH )
    {
        IDE_SET( ideSetErrorCode( mtERR_ABORT_INVALID_LENGTH ) );
    }
    IDE_EXCEPTION( ERR_INVALID_TYPE )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::getCanonizedSimpleConstMtdValue",
                                  "invalid type" ) );
    }
    IDE_EXCEPTION( ERR_NOT_EXIST_QUEUE )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::getSimpleConstMtdValue",
                                  "not exist queue" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmxSimple::getSimpleMtdBigint( qciBindParam  * aParam,
                                      mtdBigintType * aValue )
{
    SChar            sNumericBuf[MTD_NUMERIC_SIZE_MAXIMUM];
    mtdNumericType * sNumeric = (mtdNumericType*) &sNumericBuf;
    mtdNumericType * sNumericValue;
    mtdCharType    * sCharValue;
    
    if ( aParam->type == MTD_SMALLINT_ID )
    {
        if ( *((mtdSmallintType*)aParam->data) == MTD_SMALLINT_NULL )
        {
            *aValue = MTD_BIGINT_NULL;
        }
        else
        {
            *aValue = (mtdBigintType)*((mtdSmallintType*)aParam->data);
        }
    }
    else if ( aParam->type == MTD_INTEGER_ID )
    {
        if ( *((mtdIntegerType*)aParam->data) == MTD_INTEGER_NULL )
        {
            *aValue = MTD_BIGINT_NULL;
        }
        else
        {
            *aValue = (mtdBigintType)*((mtdIntegerType*)aParam->data);
        }
    }
    else if ( aParam->type == MTD_BIGINT_ID )
    {
        *aValue = *((mtdBigintType*)aParam->data);
    }
    else if ( ( aParam->type == MTD_CHAR_ID ) ||
              ( aParam->type == MTD_VARCHAR_ID ) )
    {
        sCharValue = (mtdCharType*) aParam->data;

        if ( sCharValue->length == 0 )
        {
            *aValue = MTD_BIGINT_NULL;
        }
        else
        {
            IDE_TEST( mtc::makeNumeric(
                          sNumeric,
                          MTD_FLOAT_MANTISSA_MAXIMUM,
                          sCharValue->value,
                          sCharValue->length )
                      != IDE_SUCCESS );
        
            if ( sNumeric->length > 1 )
            {
                IDE_TEST( mtc::numeric2Slong( aValue, sNumeric )
                          != IDE_SUCCESS );
            }
            else
            {
                *aValue = MTD_BIGINT_NULL;
            }
        }
    }
    else if ( ( aParam->type == MTD_FLOAT_ID ) ||
              ( aParam->type == MTD_NUMERIC_ID ) )
    {
        sNumericValue = (mtdNumericType*) aParam->data;

        if ( sNumericValue->length == 0 )
        {
            *aValue = MTD_BIGINT_NULL;
        }
        else
        {
            IDE_TEST( mtc::numeric2Slong( aValue,
                                          sNumericValue )
                      != IDE_SUCCESS );
        }
    }
    else
    {
        IDE_RAISE( ERR_INVALID_TYPE );
    }
    
    return IDE_SUCCESS;
    
    IDE_EXCEPTION( ERR_INVALID_TYPE )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::getSimpleMtdBigint",
                                  "invalid type" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}
                               
IDE_RC qmxSimple::getSimpleMtdNumeric( qciBindParam   * aParam,
                                       mtdNumericType * aValue )
{
    mtdNumericType * sNumeric;
    mtdCharType    * sChar;
    mtdDoubleType    sDouble;
    SLong            sLong;
    
    if ( aParam->type == MTD_SMALLINT_ID )
    {
        sLong = (mtdBigintType)*((mtdSmallintType*)aParam->data);
        if ( sLong == MTD_SMALLINT_NULL )
        {
            aValue->length = 0;
        }
        else
        {
            mtc::makeNumeric( aValue, sLong );
        }
    }
    else if ( aParam->type == MTD_INTEGER_ID )
    {
        sLong = (mtdBigintType)*((mtdIntegerType*)aParam->data);
        if ( sLong == MTD_INTEGER_NULL )
        {
            aValue->length = 0;
        }
        else
        {
            mtc::makeNumeric( aValue, sLong );
        }
    }
    else if ( aParam->type == MTD_BIGINT_ID )
    {
        sLong = *((mtdBigintType*)aParam->data);
        if ( sLong == MTD_BIGINT_NULL )
        {
            aValue->length = 0;
        }
        else
        {
            mtc::makeNumeric( aValue, sLong );
        }
    }
    else if ( ( aParam->type == MTD_CHAR_ID ) ||
              ( aParam->type == MTD_VARCHAR_ID ) )
    {
        sChar = (mtdCharType*)aParam->data;
        IDE_TEST( mtc::makeNumeric(
                      aValue,
                      MTD_FLOAT_MANTISSA_MAXIMUM,
                      sChar->value,
                      sChar->length )
                  != IDE_SUCCESS );
    }
    else if ( ( aParam->type == MTD_FLOAT_ID ) ||
              ( aParam->type == MTD_NUMERIC_ID ) )
    {
        sNumeric = (mtdNumericType*)aParam->data;
        idlOS::memcpy( aValue,
                       sNumeric,
                       sNumeric->length + 1 );  // actual size
    }
    else if ( aParam->type == MTD_DOUBLE_ID )
    {
        sDouble = *((mtdDoubleType*)aParam->data);
        if ( mtdDouble.isNull( NULL, &sDouble ) == ID_TRUE )
        {
            aValue->length = 0;
        }
        else
        {
            IDE_TEST( mtc::makeNumeric( aValue, sDouble )
                      != IDE_SUCCESS );
        }
    }
    else
    {
        IDE_RAISE( ERR_INVALID_TYPE );
    }
    
    return IDE_SUCCESS;
    
    IDE_EXCEPTION( ERR_INVALID_TYPE )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::getSimpleMtdNumeric",
                                  "invalid type" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}
                               
IDE_RC qmxSimple::getSimpleMtdValue( qmnValueInfo             * aValueInfo,
                                     void                    ** aMtdValue,
                                     struct qciBindParamInfo  * aParamInfo,
                                     SChar                   ** aBuffer,
                                     idBool                     aNeedCanonize )
{
    mtcColumn      * sColumn = & aValueInfo->column;
    qciBindParam   * sParam = & aParamInfo[aValueInfo->value.id].param;
    SChar            sNumericBuf[MTD_NUMERIC_SIZE_MAXIMUM];
    mtdNumericType * sNumeric = (mtdNumericType*) &sNumericBuf;
    mtdNumericType * sNumericValue;
    mtdCharType    * sCharValue;
    mtdCharType    * sNewValue;
    mtdBigintType    sBigintValue;
    idBool           sCanonized;

    if ( sColumn->module->id == MTD_SMALLINT_ID )
    {
        IDE_TEST( getSimpleMtdBigint( sParam,
                                      & sBigintValue )
                  != IDE_SUCCESS );
        if ( sBigintValue == MTD_BIGINT_NULL )
        {
            *((mtdSmallintType*) *aBuffer) = MTD_SMALLINT_NULL;
        }
        else
        {   
            IDE_TEST_RAISE( ( sBigintValue < MTD_SMALLINT_MINIMUM ) ||
                            ( sBigintValue > MTD_SMALLINT_MAXIMUM ),
                            ERR_VALUE_OVERFLOW );
            
            *((mtdSmallintType*) *aBuffer) = (mtdSmallintType)sBigintValue;
        }
        *aMtdValue = (void*) *aBuffer;
    }
    else if ( sColumn->module->id == MTD_INTEGER_ID )
    {
        IDE_TEST( getSimpleMtdBigint( sParam,
                                      & sBigintValue )
                  != IDE_SUCCESS );
        if ( sBigintValue == MTD_BIGINT_NULL )
        {
            *((mtdIntegerType*) *aBuffer) = MTD_INTEGER_NULL;
        }
        else
        {
            IDE_TEST_RAISE( ( sBigintValue < MTD_INTEGER_MINIMUM ) ||
                            ( sBigintValue > MTD_INTEGER_MAXIMUM ),
                            ERR_VALUE_OVERFLOW );
            
            *((mtdIntegerType*) *aBuffer) = (mtdIntegerType)sBigintValue;
        }
        *aMtdValue = (void*) *aBuffer;
    }
    else if ( sColumn->module->id == MTD_BIGINT_ID )
    {
        IDE_TEST( getSimpleMtdBigint( sParam,
                                      & sBigintValue )
                  != IDE_SUCCESS );
        *((mtdBigintType*) *aBuffer) = (mtdBigintType)sBigintValue;
        *aMtdValue = (void*) *aBuffer;
    }
    else if ( sColumn->module->id == MTD_CHAR_ID )
    {
        if ( ( sParam->type == MTD_CHAR_ID ) ||
             ( sParam->type == MTD_VARCHAR_ID ) )
        {
            sCharValue = (mtdCharType*) sParam->data;
            IDE_TEST_RAISE( sCharValue->length > sColumn->precision,
                            ERR_INVALID_LENGTH );

            // space padding
            if ( ( aNeedCanonize == ID_TRUE ) &&
                 ( sCharValue->length < sColumn->precision ) )
            {
                sNewValue = (mtdCharType*) *aBuffer;

                if ( sCharValue->length > 0 )
                {
                    idlOS::memcpy( sNewValue->value,
                                   sCharValue->value,
                                   sCharValue->length );
                
                    idlOS::memset( sNewValue->value + sCharValue->length,
                                   0x20,
                                   sColumn->precision - sCharValue->length );
                
                    sNewValue->length = sColumn->precision;
                }
                else
                {
                    sNewValue->length = 0;
                }
            
                *aMtdValue = (void*) sNewValue;
            }
            else
            {
                *aMtdValue = sParam->data;
            }
        }
        else
        {
            IDE_RAISE( ERR_INVALID_TYPE );
        }
    }
    else if ( sColumn->module->id == MTD_VARCHAR_ID )
    {
        if ( ( sParam->type == MTD_CHAR_ID ) ||
             ( sParam->type == MTD_VARCHAR_ID ) )
        {
            sCharValue = (mtdCharType*) sParam->data;
            
            IDE_TEST_RAISE( sCharValue->length > sColumn->precision,
                            ERR_INVALID_LENGTH );
            *aMtdValue = sParam->data;
        }
        else
        {
            IDE_RAISE( ERR_INVALID_TYPE );
        }
    }
    else if ( ( sColumn->module->id == MTD_NUMERIC_ID ) ||
              ( sColumn->module->id == MTD_FLOAT_ID ) )
    {
        if ( aNeedCanonize == ID_TRUE )
        {
            IDE_TEST( getSimpleMtdNumeric( sParam,
                                           sNumeric )
                      != IDE_SUCCESS );
        
            sNumericValue = (mtdNumericType*) *aBuffer;
            *aMtdValue = (void*)sNumericValue;

            sCanonized = ID_TRUE;
            
            if ( ( sColumn->module->id == MTD_FLOAT_ID ) ||
                 ( ( sColumn->flag & MTC_COLUMN_ARGUMENT_COUNT_MASK ) == 1 ) )
            {
                IDE_TEST( mtc::floatCanonize( sNumeric,
                                              sNumericValue,
                                              sColumn->precision,
                                              &sCanonized )
                          != IDE_SUCCESS );
            }
            else
            {
                IDE_TEST( mtc::numericCanonize( sNumeric,
                                                sNumericValue,
                                                sColumn->precision,
                                                sColumn->scale,
                                                &sCanonized )
                          != IDE_SUCCESS );
            }
        
            if ( sCanonized == ID_FALSE )
            {
                idlOS::memcpy( sNumericValue,
                               sNumeric,
                               sNumeric->length + 1 );  // actual size
            }
            else
            {
                // Nothing to do.
            }
        }
        else
        {
            sNumericValue = (mtdNumericType*) *aBuffer;
            *aMtdValue = (void*)sNumericValue;
            
            IDE_TEST( getSimpleMtdNumeric( sParam,
                                           sNumericValue )
                      != IDE_SUCCESS );
        }
    }
    else if ( sColumn->module->id == MTD_DATE_ID )
    {
        IDE_TEST_RAISE( sParam->type != MTD_DATE_ID,
                        ERR_INVALID_TYPE );
        
        *aMtdValue = sParam->data;
    }
    else
    {
        IDE_RAISE( ERR_INVALID_TYPE );
    }

    *aBuffer += idlOS::align8( sColumn->column.size );
    
    return IDE_SUCCESS;
    
    IDE_EXCEPTION( ERR_VALUE_OVERFLOW )
    {
        IDE_SET( ideSetErrorCode( mtERR_ABORT_VALUE_OVERFLOW ) );
    }
    IDE_EXCEPTION( ERR_INVALID_LENGTH )
    {
        IDE_SET( ideSetErrorCode( mtERR_ABORT_INVALID_LENGTH ) );
    }
    IDE_EXCEPTION( ERR_INVALID_TYPE )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::getCanonizedSimpleMtdValue",
                                  "invalid type" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmxSimple::getSimpleMtdValueSize( mtcColumn * aColumn,
                                         void      * aMtdValue,
                                         UInt      * aSize )
{
    mtdCharType    * sCharValue;
    mtdNumericType * sNumericValue;
    
    if ( aColumn->module->id == MTD_SMALLINT_ID )
    {
        *aSize = 2;
    }
    else if ( aColumn->module->id == MTD_BIGINT_ID )
    {
        *aSize = 8;
    }
    else if ( aColumn->module->id == MTD_INTEGER_ID )
    {
        *aSize = 4;
    }
    else if ( ( aColumn->module->id == MTD_CHAR_ID ) ||
              ( aColumn->module->id == MTD_VARCHAR_ID ) )
    {
        sCharValue = (mtdCharType*) aMtdValue;
        *aSize = sCharValue->length + 2;
    }
    else if ( ( aColumn->module->id == MTD_NUMERIC_ID ) ||
              ( aColumn->module->id == MTD_FLOAT_ID ) )
    {
        sNumericValue = (mtdNumericType*) aMtdValue;
        *aSize = sNumericValue->length + 1;  // actual size
    }
    else if ( aColumn->module->id == MTD_DATE_ID )
    {
        *aSize = ID_SIZEOF(mtdDateType);
    }
    else
    {
        IDE_RAISE( ERR_INVALID_TYPE );
    }

    return IDE_SUCCESS;
    
    IDE_EXCEPTION( ERR_INVALID_TYPE )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::getSimpleMtdValueSize",
                                  "invalid type" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmxSimple::setSimpleMtdValue( mtcColumn  * aColumn,
                                     SChar      * aResult,
                                     const void * aMtdValue )
{
    mtdCharType    * sCharValue;
    mtdNumericType * sNumericValue;
    
    if ( aColumn->module->id == MTD_SMALLINT_ID )
    {
        *(SShort*)aResult = *(const SShort*)aMtdValue;
    }
    else if ( aColumn->module->id == MTD_BIGINT_ID )
    {
        *(SLong*)aResult = *(const SLong*)aMtdValue;
    }
    else if ( aColumn->module->id == MTD_INTEGER_ID )
    {
        *(SInt*)aResult = *(const SInt*)aMtdValue;
    }
    else if ( ( aColumn->module->id == MTD_CHAR_ID ) ||
              ( aColumn->module->id == MTD_VARCHAR_ID ) )
    {
        sCharValue = (mtdCharType*)aMtdValue;

        idlOS::memcpy( aResult,
                       sCharValue,
                       sCharValue->length + 2 );
    }
    else if ( ( aColumn->module->id == MTD_NUMERIC_ID ) ||
              ( aColumn->module->id == MTD_FLOAT_ID ) )
    {
        sNumericValue = (mtdNumericType*)aMtdValue;

        idlOS::memcpy( aResult,
                       sNumericValue,
                       sNumericValue->length + 1 );  // actual size
    }
    else if ( aColumn->module->id == MTD_DATE_ID )
    {
        *(mtdDateType*)aResult = *(const mtdDateType*)aMtdValue;
    }
    else
    {
        IDE_RAISE( ERR_INVALID_TYPE );
    }
    
    return IDE_SUCCESS;
    
    IDE_EXCEPTION( ERR_INVALID_TYPE )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::setSimpleMtdValue",
                                  "invalid type" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmxSimple::makeSimpleRidRange( qcStatement          * aStatement,
                                      struct qmncSCAN      * aSCAN,
                                      UChar                * aBindBuffer,
                                      qmxFastScanInfo      * aScanInfo,
                                      UInt                   aScanCount,
                                      void                ** aMtdValue,
                                      qtcMetaRangeColumn   * /*aRangeColumn*/,
                                      smiRange             * aRange,
                                      smiRange            ** aKeyRange,
                                      idBool               * aIsNull,
                                      SChar               ** aBuffer )
{
    qmnValueInfo  * sValueInfo;
    idBool          sIsNull = ID_FALSE;
    idBool          sFound;
    const void    * sValue;
    UInt            i = 0;
    UInt            j;

    IDE_DASSERT( aSCAN->simpleValueCount == 1 );

    // initialize rid range
    aRange->prev = NULL;
    aRange->next = NULL;
    aRange->minimum.callback = mtk::rangeCallBack4Rid;
    aRange->maximum.callback = mtk::rangeCallBack4Rid;

    sValueInfo = aSCAN->simpleValues;
                
    switch ( sValueInfo->type )
    {
        case QMN_VALUE_TYPE_CONST_VALUE:
            aMtdValue[i] = sValueInfo->value.constVal;
            break;

        case QMN_VALUE_TYPE_HOST_VALUE:
            if ( aBindBuffer != NULL )
            {
                IDE_TEST( getSimpleCValue(
                              sValueInfo,
                              & aMtdValue[i],
                              aStatement->pBindParam,
                              aBindBuffer,
                              aBuffer,
                              ID_FALSE )  // compare�� ���� canonize�� ���ʿ�
                          != IDE_SUCCESS );
            }
            else
            {
                IDE_TEST( getSimpleMtdValue(
                              sValueInfo,
                              & aMtdValue[i],
                              aStatement->pBindParam,
                              aBuffer,
                              ID_FALSE )  // compare�� ���� canonize�� ���ʿ�
                          != IDE_SUCCESS );
            }
            break;

        case QMN_VALUE_TYPE_COLUMN:
            sFound = ID_FALSE;
            for ( j = 0; j < aScanCount; j++ )
            {
                if ( aScanInfo[j].scan->tupleRowID ==
                     sValueInfo->value.columnVal.table )
                {
                    sValue = mtc::value( & sValueInfo->value.columnVal.column,
                                         aScanInfo[j].row,
                                         MTD_OFFSET_USE );
                    sFound = ID_TRUE;
                    break;
                }
                else
                {
                    // Nothing to do.
                }
            }

            IDE_TEST_RAISE( sFound == ID_FALSE, ERR_INVALID_COLUMN );
                    
            aMtdValue[i] = (void*) sValue;
            break;

        default:
            IDE_RAISE( ERR_INVALID_TYPE );
            break;
    }

    // null value�� �ֳ�?
    if ( sIsNull == ID_FALSE )
    {
        IDE_TEST( isSimpleNullValue( & sValueInfo->column,
                                     aMtdValue[i],
                                     & sIsNull )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    // set range
    aRange->minimum.data = aMtdValue[i];
    aRange->maximum.data = aMtdValue[i];
    
    *aKeyRange = aRange;
    *aIsNull = sIsNull;
    
    return IDE_SUCCESS;
    
    IDE_EXCEPTION( ERR_INVALID_COLUMN )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::makeSimpleRidRange",
                                  "invalid column" ) );
    }
    IDE_EXCEPTION( ERR_INVALID_TYPE )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::makeSimpleRidRange",
                                  "invalid type" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmxSimple::makeSimpleKeyRange( qcStatement          * aStatement,
                                      struct qmncSCAN      * aSCAN,
                                      qcmIndex             * aIndex,
                                      UChar                * aBindBuffer,
                                      qmxFastScanInfo      * aScanInfo,
                                      UInt                   aScanCount,
                                      void                ** aMtdValue,
                                      qtcMetaRangeColumn   * aRangeColumn,
                                      smiRange             * aRange,
                                      smiRange            ** aKeyRange,
                                      idBool               * aIsNull,
                                      SChar               ** aBuffer )
{
    qmnValueInfo  * sValueInfo;
    qmnValueInfo  * sMinValueInfo;  // greater
    qmnValueInfo  * sMaxValueInfo;  // less
    idBool          sIsNull = ID_FALSE;
    idBool          sFound;
    const void    * sValue;
    UInt            i;
    UInt            j;
    UInt            k;

    if ( aSCAN->simpleValueCount > 0 )
    {
        //------------------------------
        // initialize range
        //------------------------------

        aRange->prev = NULL;
        aRange->next = NULL;

        if ( aSCAN->simpleCompareOpCount == 0 )
        {
            aRange->minimum.callback = qtc::rangeMinimumCallBack4Mtd;
            aRange->maximum.callback = qtc::rangeMaximumCallBack4Mtd;
        }
        else if ( aSCAN->simpleCompareOpCount == 1 )
        {
            sValueInfo = &(aSCAN->simpleValues[aSCAN->simpleValueCount - 1]);

            if ( sValueInfo->op == QMN_VALUE_OP_LT )
            {
                aRange->minimum.callback = qtc::rangeMinimumCallBack4GEMtd;
                aRange->maximum.callback = qtc::rangeMaximumCallBack4LTMtd;
            }
            else if ( sValueInfo->op == QMN_VALUE_OP_LE )
            {
                aRange->minimum.callback = qtc::rangeMinimumCallBack4GEMtd;
                aRange->maximum.callback = qtc::rangeMaximumCallBack4LEMtd;
            }
            else if ( sValueInfo->op == QMN_VALUE_OP_GT )
            {
                aRange->minimum.callback = qtc::rangeMinimumCallBack4GTMtd;
                aRange->maximum.callback = qtc::rangeMaximumCallBack4LTMtd;
            }
            else if ( sValueInfo->op == QMN_VALUE_OP_GE )
            {
                aRange->minimum.callback = qtc::rangeMinimumCallBack4GEMtd;
                aRange->maximum.callback = qtc::rangeMaximumCallBack4LTMtd;
            }
            else
            {
                IDE_DASSERT( 0 );
            }
        }
        else if ( aSCAN->simpleCompareOpCount == 2 )
        {
            IDE_DASSERT( aSCAN->simpleValueCount >= 2 );

            sValueInfo = &(aSCAN->simpleValues[aSCAN->simpleValueCount - 1]);
            if ( ( sValueInfo->op == QMN_VALUE_OP_LT ) ||
                 ( sValueInfo->op == QMN_VALUE_OP_LE ) )
            {
                sMinValueInfo = &(aSCAN->simpleValues[aSCAN->simpleValueCount - 2]);
                sMaxValueInfo = sValueInfo;
            }
            else
            {
                sMinValueInfo = sValueInfo;
                sMaxValueInfo = &(aSCAN->simpleValues[aSCAN->simpleValueCount - 2]);
            }

            if ( sMinValueInfo->op == QMN_VALUE_OP_GT )
            {
                aRange->minimum.callback = qtc::rangeMinimumCallBack4GTMtd;
            }
            else if ( sMinValueInfo->op == QMN_VALUE_OP_GE )
            {
                aRange->minimum.callback = qtc::rangeMinimumCallBack4GEMtd;
            }
            else
            {
                IDE_DASSERT( 0 );
            }

            if ( sMaxValueInfo->op == QMN_VALUE_OP_LT )
            {
                aRange->maximum.callback = qtc::rangeMaximumCallBack4LTMtd;
            }
            else if ( sMaxValueInfo->op == QMN_VALUE_OP_LE )
            {
                aRange->maximum.callback = qtc::rangeMaximumCallBack4LEMtd;
            }
            else
            {
                IDE_DASSERT( 0 );
            }
        }
        else
        {
            IDE_DASSERT( 0 );
        }

        //------------------------------
        // add range
        //------------------------------

        for ( i = 0; i < aSCAN->simpleValueCount; i++ )
        {
            sValueInfo = &(aSCAN->simpleValues[i]);

            switch ( sValueInfo->type )
            {
                case QMN_VALUE_TYPE_CONST_VALUE:
                    aMtdValue[i] = sValueInfo->value.constVal;
                    break;

                case QMN_VALUE_TYPE_HOST_VALUE:
                    if ( aBindBuffer != NULL )
                    {
                        IDE_TEST( getSimpleCValue(
                                      sValueInfo,
                                      & aMtdValue[i],
                                      aStatement->pBindParam,
                                      aBindBuffer,
                                      aBuffer,
                                      ID_FALSE )  // compare�� ���� canonize�� ���ʿ�
                                  != IDE_SUCCESS );
                    }
                    else
                    {
                        IDE_TEST( getSimpleMtdValue(
                                      sValueInfo,
                                      & aMtdValue[i],
                                      aStatement->pBindParam,
                                      aBuffer,
                                      ID_FALSE )  // compare�� ���� canonize�� ���ʿ�
                                  != IDE_SUCCESS );
                    }
                    break;

                case QMN_VALUE_TYPE_COLUMN:
                    sFound = ID_FALSE;
                    for ( j = 0; j < aScanCount; j++ )
                    {
                        if ( aScanInfo[j].scan->tupleRowID ==
                             sValueInfo->value.columnVal.table )
                        {
                            sValue = mtc::value( & sValueInfo->value.columnVal.column,
                                                 aScanInfo[j].row,
                                                 MTD_OFFSET_USE );
                            sFound = ID_TRUE;
                            break;
                        }
                        else
                        {
                            // Nothing to do.
                        }
                    }

                    IDE_TEST_RAISE( sFound == ID_FALSE, ERR_INVALID_COLUMN );

                    aMtdValue[i] = (void*) sValue;
                    break;

                default:
                    IDE_RAISE( ERR_INVALID_TYPE );
                    break;
            }

            // null value�� �ֳ�?
            if ( sIsNull == ID_FALSE )
            {
                IDE_TEST( isSimpleNullValue( & sValueInfo->column,
                                             aMtdValue[i],
                                             & sIsNull )
                          != IDE_SUCCESS );
            }
            else
            {
                // Nothing to do.
            }

            if ( aSCAN->simpleCompareOpCount == 0 )
            {
                qtc::setMetaRangeColumn( aRangeColumn + i,
                                         aIndex->keyColumns + i,
                                         aMtdValue[i],
                                         aIndex->keyColsFlag[i] &
                                         SMI_COLUMN_ORDER_MASK,
                                         i );

                // link range column
                if ( i == 0 )
                {
                    aRange->minimum.data = aRangeColumn;
                    aRange->maximum.data = aRangeColumn;
                }
                else
                {
                    aRangeColumn[i - 1].next = aRangeColumn + i;
                }
            }
            else
            {
                // =�� ���ڸ���, <,<=,>,>=�� �ش��ڸ��� ����
                if ( sValueInfo->op == QMN_VALUE_OP_EQUAL )
                {
                    k = i;
                }
                else
                {
                    k = aSCAN->simpleValueCount - aSCAN->simpleCompareOpCount;
                }

                if ( ( sValueInfo->op == QMN_VALUE_OP_EQUAL ) ||
                     ( sValueInfo->op == QMN_VALUE_OP_GT ) ||
                     ( sValueInfo->op == QMN_VALUE_OP_GE ) )
                {
                    // min
                    qtc::setMetaRangeColumn( aRangeColumn + k,
                                             aIndex->keyColumns + k,
                                             aMtdValue[i],
                                             aIndex->keyColsFlag[k] &
                                             SMI_COLUMN_ORDER_MASK,
                                             k );
                }
                else
                {
                    // Nothing to do.
                }

                if ( ( sValueInfo->op == QMN_VALUE_OP_EQUAL ) ||
                     ( sValueInfo->op == QMN_VALUE_OP_LT ) ||
                     ( sValueInfo->op == QMN_VALUE_OP_LE ) )
                {
                    // max
                    qtc::setMetaRangeColumn( aRangeColumn + k + 32,
                                             aIndex->keyColumns + k,
                                             aMtdValue[i],
                                             aIndex->keyColsFlag[k] &
                                             SMI_COLUMN_ORDER_MASK,
                                             k );
                }
                else
                {
                    // Nothing to do.
                }
            
                // <,<=�� �ִ� ��� min�� �߰������Ѵ�.
                if ( ( ( sValueInfo->op == QMN_VALUE_OP_LT ) ||
                       ( sValueInfo->op == QMN_VALUE_OP_LE ) )
                     &&
                     ( aSCAN->simpleCompareOpCount == 1 ) )
                {
                    aRangeColumn[k].compare = qtc::compareMinimumLimit;
                    aRangeColumn[k].next = NULL;
                }
                else
                {
                    // Nothing to do.
                }
            
                // >,>=�� �ִ� ��� max�� �߰� �����Ѵ�.
                if ( ( ( sValueInfo->op == QMN_VALUE_OP_GT ) ||
                       ( sValueInfo->op == QMN_VALUE_OP_GE ) )
                     &&
                     ( aSCAN->simpleCompareOpCount == 1 ) )
                {
                    aRangeColumn[k + 32].compare = qtc::compareMaximumLimit4Mtd;
                    aRangeColumn[k + 32].next = NULL;
                
                    qtc::changeMetaRangeColumn( aRangeColumn + k + 32,
                                                aIndex->keyColumns + k,
                                                k );
                }
                else
                {
                    // Nothing to do.
                }
            
                // link range column
                if ( k == 0 )
                {
                    aRange->minimum.data = aRangeColumn;
                    aRange->maximum.data = aRangeColumn + 32;
                }
                else
                {
                    aRangeColumn[k - 1].next = aRangeColumn + k;
                    aRangeColumn[k - 1 + 32].next = aRangeColumn + k + 32;
                }
            }
        }

        *aKeyRange = aRange;
    }
    else
    {
        *aKeyRange = smiGetDefaultKeyRange();
    }
    
    *aIsNull = sIsNull;
        
    return IDE_SUCCESS;
    
    IDE_EXCEPTION( ERR_INVALID_COLUMN )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::makeSimpleKeyRange",
                                  "invalid column" ) );
    }
    IDE_EXCEPTION( ERR_INVALID_TYPE )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::makeSimpleKeyRange",
                                  "invalid type" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmxSimple::updateSimpleKeyRange( qmncSCAN            * aSCAN,
                                        qmxFastScanInfo     * aScanInfo,
                                        UInt                  aScanCount,
                                        void               ** aMtdValue,
                                        qtcMetaRangeColumn  * aRangeColumn,
                                        idBool              * aIsNull )
{
    qmnValueInfo  * sValueInfo;
    idBool          sIsNull = ID_FALSE;
    idBool          sFound;
    const void    * sValue;
    UInt            i;
    UInt            j;
    
    if ( aSCAN->simpleValueCount > 0 )
    {
        for ( i = 0; i < aSCAN->simpleValueCount; i++ )
        {
            sValueInfo = &(aSCAN->simpleValues[i]);
                
            switch ( sValueInfo->type )
            {
                case QMN_VALUE_TYPE_COLUMN:
                    sFound = ID_FALSE;
                    for ( j = 0; j < aScanCount; j++ )
                    {
                        if ( aScanInfo[j].scan->tupleRowID ==
                             sValueInfo->value.columnVal.table )
                        {
                            sValue = mtc::value( & sValueInfo->value.columnVal.column,
                                                 aScanInfo[j].row,
                                                 MTD_OFFSET_USE );
                            sFound = ID_TRUE;
                            break;
                        }
                        else
                        {
                            // Nothing to do.
                        }
                    }

                    IDE_TEST_RAISE( sFound == ID_FALSE, ERR_INVALID_COLUMN );
                    
                    aMtdValue[i] = (void*) sValue;

                    // null value�� �ֳ�?
                    if ( sIsNull == ID_FALSE )
                    {
                        IDE_TEST( isSimpleNullValue( & sValueInfo->column,
                                                     aMtdValue[i],
                                                     & sIsNull )
                                  != IDE_SUCCESS );
                    }
                    else
                    {
                        // Nothing to do.
                    }

                    // change
                    if ( ( sValueInfo->op == QMN_VALUE_OP_EQUAL ) ||
                         ( sValueInfo->op == QMN_VALUE_OP_GT ) ||
                         ( sValueInfo->op == QMN_VALUE_OP_GE ) )
                    {
                        // min
                        aRangeColumn[i].value = aMtdValue[i];
                    }
                    else
                    {
                        // Nothing to do.
                    }
            
                    if ( ( sValueInfo->op == QMN_VALUE_OP_EQUAL ) ||
                         ( sValueInfo->op == QMN_VALUE_OP_LT ) ||
                         ( sValueInfo->op == QMN_VALUE_OP_LE ) )
                    {
                        // max
                        aRangeColumn[i + 32].value = aMtdValue[i];
                    }
                    else
                    {
                        // Nothing to do.
                    }
                    
                    break;

                default:
                    break;
            }
        }
    }
    else
    {
        // Nothing to do.
    }
    
    *aIsNull = sIsNull;
        
    return IDE_SUCCESS;
    
    IDE_EXCEPTION( ERR_INVALID_COLUMN )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::updateSimpleKeyRange",
                                  "invalid column" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmxSimple::calculateSimpleValues( qcStatement      * aStatement,
                                         struct qmncUPTE  * aUPTE,
                                         UChar            * aBindBuffer,
                                         const void       * aRow,
                                         smiValue         * aSmiValues,
                                         SChar            * aBuffer )
{
    SChar            sNumericBuf[MTD_NUMERIC_SIZE_MAXIMUM];
    mtdNumericType * sNumeric = (mtdNumericType*) &sNumericBuf;
    SChar          * sBuffer = aBuffer;
    qmnValueInfo   * sValueInfo;
    void           * sValue;
    const void     * sColumn;
    idBool           sIsNull;
    idBool           sCanonized;
    UInt             i;

    for ( i = 0; i < aUPTE->updateColumnCount; i++ )
    {
        sValueInfo = &(aUPTE->simpleValues[i]);

        if ( sValueInfo->op == QMN_VALUE_OP_ASSIGN )
        {
            // Nothing to do.
        }
        else
        {
            switch ( sValueInfo->type )
            {
                case QMN_VALUE_TYPE_CONST_VALUE:
                    sValue = sValueInfo->value.constVal; 
                    break;
                
                case QMN_VALUE_TYPE_HOST_VALUE:
                    if ( aBindBuffer != NULL )
                    {
                        IDE_TEST( getSimpleCValue(
                                      sValueInfo,
                                      & sValue,
                                      aStatement->pBindParam,
                                      aBindBuffer,
                                      & sBuffer,
                                      ID_FALSE )  // �ϴ� �׳� ������
                                  != IDE_SUCCESS );
                    }
                    else
                    {
                        IDE_TEST( getSimpleMtdValue(
                                      sValueInfo,
                                      & sValue,
                                      aStatement->pBindParam,
                                      & sBuffer,
                                      ID_FALSE )  // �ϴ� �׳� ������
                                  != IDE_SUCCESS );
                    }
                    break;

                default:
                    IDE_RAISE( ERR_INVALID_TYPE );
                    break;
            }

            sColumn = mtc::value( & sValueInfo->column,
                                  aRow,
                                  MTD_OFFSET_USE );

            IDE_TEST( isSimpleNullValue( & sValueInfo->column,
                                         sColumn,
                                         & sIsNull )
                      != IDE_SUCCESS );

            if ( sIsNull == ID_TRUE )
            {
                // set smiValue
                IDE_TEST( setSimpleSmiValue( & sValueInfo->column,
                                             (void*) sColumn,
                                             &(aSmiValues[i]) )
                          != IDE_SUCCESS );
            }
            else
            {
                if ( sValueInfo->op == QMN_VALUE_OP_ADD )
                {
                    if ( sValueInfo->column.module->id == MTD_SMALLINT_ID )
                    {
                        *(SShort*)sBuffer = *(SShort*)sColumn + *(SShort*)sValue;
                    }
                    else if ( sValueInfo->column.module->id == MTD_BIGINT_ID )
                    {
                        *(SLong*)sBuffer = *(SLong*)sColumn + *(SLong*)sValue;
                    }
                    else if ( sValueInfo->column.module->id == MTD_INTEGER_ID )
                    {
                        *(SInt*)sBuffer = *(SInt*)sColumn + *(SInt*)sValue;
                    }
                    else if ( ( sValueInfo->column.module->id == MTD_NUMERIC_ID ) ||
                              ( sValueInfo->column.module->id == MTD_FLOAT_ID ) )
                    {
                        IDE_TEST( mtc::addFloat( sNumeric,
                                                 MTD_FLOAT_PRECISION_MAXIMUM,
                                                 (mtdNumericType*)sColumn,
                                                 (mtdNumericType*)sValue )
                                  != IDE_SUCCESS );

                        sCanonized = ID_TRUE;
                        
                        if ( ( sValueInfo->column.module->id == MTD_FLOAT_ID ) ||
                             ( ( sValueInfo->column.flag & MTC_COLUMN_ARGUMENT_COUNT_MASK ) == 1 ) )
                        {
                            IDE_TEST( mtc::floatCanonize( sNumeric,
                                                          (mtdNumericType*)sBuffer,
                                                          sValueInfo->column.precision,
                                                          &sCanonized )
                                      != IDE_SUCCESS );
                        }
                        else
                        {
                            IDE_TEST( mtc::numericCanonize( sNumeric,
                                                            (mtdNumericType*)sBuffer,
                                                            sValueInfo->column.precision,
                                                            sValueInfo->column.scale,
                                                            &sCanonized )
                                      != IDE_SUCCESS );
                        }
                
                        if ( sCanonized == ID_FALSE )
                        {
                            idlOS::memcpy( (mtdNumericType*)sBuffer,
                                           sNumeric,
                                           sNumeric->length + 1 );  // actual size
                        }
                        else
                        {
                            // Nothing to do.
                        }
                    }
                    else
                    {
                        IDE_RAISE( ERR_INVALID_TYPE );
                    }
                }
                else if ( sValueInfo->op == QMN_VALUE_OP_SUB )
                {
                    if ( sValueInfo->column.module->id == MTD_SMALLINT_ID )
                    {
                        *(SShort*)sBuffer = *(SShort*)sColumn - *(SShort*)sValue;
                    }
                    else if ( sValueInfo->column.module->id == MTD_BIGINT_ID )
                    {
                        *(SLong*)sBuffer = *(SLong*)sColumn - *(SLong*)sValue;
                    }
                    else if ( sValueInfo->column.module->id == MTD_INTEGER_ID )
                    {
                        *(SInt*)sBuffer = *(SInt*)sColumn - *(SInt*)sValue;
                    }
                    else if ( ( sValueInfo->column.module->id == MTD_NUMERIC_ID ) ||
                              ( sValueInfo->column.module->id == MTD_FLOAT_ID ) )
                    {
                        IDE_TEST( mtc::subtractFloat( sNumeric,
                                                      MTD_FLOAT_PRECISION_MAXIMUM,
                                                      (mtdNumericType*)sColumn,
                                                      (mtdNumericType*)sValue )
                                  != IDE_SUCCESS );

                        sCanonized = ID_TRUE;
                        
                        if ( ( sValueInfo->column.module->id == MTD_FLOAT_ID ) ||
                             ( ( sValueInfo->column.flag & MTC_COLUMN_ARGUMENT_COUNT_MASK ) == 1 ) )
                        {
                            IDE_TEST( mtc::floatCanonize( sNumeric,
                                                          (mtdNumericType*)sBuffer,
                                                          sValueInfo->column.precision,
                                                          &sCanonized )
                                      != IDE_SUCCESS );
                        }
                        else
                        {
                            IDE_TEST( mtc::numericCanonize( sNumeric,
                                                            (mtdNumericType*)sBuffer,
                                                            sValueInfo->column.precision,
                                                            sValueInfo->column.scale,
                                                            &sCanonized )
                                      != IDE_SUCCESS );
                        }
                
                        if ( sCanonized == ID_FALSE )
                        {
                            idlOS::memcpy( (mtdNumericType*)sBuffer,
                                           sNumeric,
                                           sNumeric->length + 1 );  // actual size
                        }
                        else
                        {
                            // Nothing to do.
                        }
                    }
                    else
                    {
                        IDE_RAISE( ERR_INVALID_TYPE );
                    }
                }
                else
                {
                    IDE_RAISE( ERR_INVALID_OP );
                }

                // set smiValue
                IDE_TEST( setSimpleSmiValue( & sValueInfo->column,
                                             (void*) sBuffer,
                                             &(aSmiValues[i]) )
                          != IDE_SUCCESS );
            
                sBuffer += idlOS::align8( sValueInfo->column.column.size );
            }
        }
    }
    
    return IDE_SUCCESS;
    
    IDE_EXCEPTION( ERR_INVALID_OP )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::calculateSimpleValues",
                                  "invalid operator" ) );
    }
    IDE_EXCEPTION( ERR_INVALID_TYPE )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::calculateSimpleValues",
                                  "invalid type" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmxSimple::isSimpleNullValue( mtcColumn  * aColumn,
                                     const void * aValue,
                                     idBool     * aIsNull )
{
    mtdCharType    * sCharValue;
    mtdNumericType * sNumericValue;
    idBool           sIsNull = ID_FALSE;
    
    if ( aColumn->module->id == MTD_SMALLINT_ID )
    {
        if ( *(SShort*)aValue == MTD_SMALLINT_NULL )
        {
            sIsNull = ID_TRUE;
        }
        else
        {
            // Nothing to do.
        }
    }
    else if ( aColumn->module->id == MTD_BIGINT_ID )
    {
        if ( *(SLong*)aValue == MTD_BIGINT_NULL )
        {
            sIsNull = ID_TRUE;
        }
        else
        {
            // Nothing to do.
        }
    }
    else if ( aColumn->module->id == MTD_INTEGER_ID )
    {
        if ( *(SInt*)aValue == MTD_INTEGER_NULL )
        {
            sIsNull = ID_TRUE;
        }
        else
        {
            // Nothing to do.
        }
    }
    else if ( ( aColumn->module->id == MTD_CHAR_ID ) ||
              ( aColumn->module->id == MTD_VARCHAR_ID ) )
    {
        sCharValue = (mtdCharType*) aValue;
        if ( sCharValue->length == 0 )
        {
            sIsNull = ID_TRUE;
        }
        else
        {
            // Nothing to do.
        }
    }
    else if ( ( aColumn->module->id == MTD_NUMERIC_ID ) ||
              ( aColumn->module->id == MTD_FLOAT_ID ) )
    {
        sNumericValue = (mtdNumericType*) aValue;
        if ( sNumericValue->length == 0 )
        {
            sIsNull = ID_TRUE;
        }
        else
        {
            // Nothing to do.
        }
    }
    else if ( aColumn->module->id == MTD_DATE_ID )
    {
        if ( MTD_DATE_IS_NULL( (mtdDateType*)aValue ) == 1 )
        {
            sIsNull = ID_TRUE;
        }
        else
        {
            // Nothing to do.
        }
    }
    else
    {
        IDE_RAISE( ERR_INVALID_TYPE );
    }

    *aIsNull = sIsNull;
    
    return IDE_SUCCESS;
    
    IDE_EXCEPTION( ERR_INVALID_TYPE )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::isSimpleNullValue",
                                  "invalid type" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmxSimple::checkSimpleNullValue( mtcColumn  * aColumn,
                                        const void * aValue )
{
    idBool  sIsNull;
    
    if ( ( aColumn->flag & MTC_COLUMN_NOTNULL_MASK )
         == MTC_COLUMN_NOTNULL_TRUE )
    {
        IDE_TEST( isSimpleNullValue( aColumn,
                                     aValue,
                                     & sIsNull )
                  != IDE_SUCCESS );
        
        IDE_TEST_RAISE( sIsNull == ID_TRUE, ERR_NOT_ALLOW_NULL );
    }
    else
    {
        // Nothing to do.
    }
    
    return IDE_SUCCESS;
    
    IDE_EXCEPTION( ERR_NOT_ALLOW_NULL )
    {
        /* BUG-45680 insert ����� not null column�� ���� �����޽��� ������ column ���� ���. */
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMX_NOT_NULL_CONSTRAINT,
                                  "",
                                  "" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmxSimple::setSimpleSmiValue( mtcColumn  * aColumn,
                                     void       * aValue,
                                     smiValue   * aSmiValue )
{
    idBool  sIsNull;
    UInt    sLength;
    
    if ( ( ( aColumn->column.flag & SMI_COLUMN_TYPE_MASK )
           == SMI_COLUMN_TYPE_VARIABLE ) ||
         ( ( aColumn->column.flag & SMI_COLUMN_TYPE_MASK )
           == SMI_COLUMN_TYPE_VARIABLE_LARGE ) )
    {
        IDE_TEST( isSimpleNullValue( aColumn,
                                     aValue,
                                     & sIsNull )
                  != IDE_SUCCESS );

        if ( sIsNull == ID_TRUE )
        {
            aSmiValue->value = NULL;
            aSmiValue->length = 0;
        }
        else
        {
            IDE_TEST( getSimpleMtdValueSize( aColumn,
                                             aValue,
                                             & sLength )
                      != IDE_SUCCESS );
            
            aSmiValue->value = aValue;
            aSmiValue->length = sLength;
        }
    }
    else
    {
        IDE_TEST( getSimpleMtdValueSize( aColumn,
                                         aValue,
                                         & sLength )
                  != IDE_SUCCESS );
            
        aSmiValue->value = aValue;
        aSmiValue->length = sLength;
    }
    
    return IDE_SUCCESS;
    
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmxSimple::calculateSimpleToChar( qmnValueInfo * aValueInfo,
                                         mtdDateType  * aDateValue,
                                         mtdCharType  * aCharValue )
{
    SInt                  sStringMaxLen;
    mtdFormatInfo       * sFormatInfo;
    mtfTo_charCalcInfo  * sCalcInfo;
    UShort                sFormatCount;
    UShort                sIterator;
    SInt                  sBufferCur;
    
    if ( mtdDate.isNull( NULL, aDateValue ) == ID_TRUE )
    {
        aCharValue->length = 0;
    }
    else
    {
        sStringMaxLen = aValueInfo->column.precision;
        sFormatInfo = (mtdFormatInfo*) aValueInfo->value.columnVal.info;
        sFormatCount = sFormatInfo->count;
        sBufferCur = 0;

        for ( sIterator = 0, sCalcInfo = sFormatInfo->format;
              sIterator < sFormatCount;
              sIterator++, sCalcInfo++ )
        {
            IDE_TEST( sCalcInfo->applyDateFormat(
                          aDateValue,
                          (SChar*) aCharValue->value,
                          &sBufferCur,
                          &sStringMaxLen,
                          sCalcInfo->string,
                          sCalcInfo->isFillMode )
                      != IDE_SUCCESS );
        }

        aCharValue->length = IDL_MIN( sBufferCur, sStringMaxLen );
    }
                            
    return IDE_SUCCESS;
    
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}
    
#define RESULT_CHUNK_SIZE  100

IDE_RC qmxSimple::executeFastSelect( smiTrans     * aSmiTrans,
                                     qcStatement  * aStatement,
                                     UShort       * aBindColInfo,
                                     UChar        * aBindBuffer,
                                     UInt           aShmSize,
                                     UChar        * aShmResult,
                                     UInt         * aResultSize,
                                     UInt         * aRowCount )
{
/***********************************************************************
 *
 *  Description : PROJ-2551 simple query ����ȭ
 *
 *  Implementation :
 *
 ***********************************************************************/

    idvSQL              * sStatistics = aStatement->mStatistics;
    qmncPROJ            * sPROJ = NULL;
    qmncSCAN            * sSCAN = NULL;
    qmncScanMethod      * sScanMethod = NULL;
    qcmIndex            * sIndex = NULL;
    SChar                 sCharBuffer[4096];
    SChar               * sBuffer = sCharBuffer;
    void                * sMtdValue[QC_MAX_KEY_COLUMN_COUNT * 2];
    UInt                  i;
    UInt                  j;

    idBool                sUseFastSmiStmt = ID_TRUE;
    smiStatement          sFastSmiStmt;
    smiStatement        * sSmiStmt = &sFastSmiStmt;
    UInt                  sSmiStmtFlag = 0;
    idBool                sBegined = ID_FALSE;
    idBool                sOpened = ID_FALSE;
    idBool                sRetryErr = ID_FALSE;

    idBool                sIsNullRange = ID_FALSE;
    const void          * sIndexHandle = NULL;
    smiRange            * sKeyRange = NULL;
    smiRange              sRange;
    qtcMetaRangeColumn    sRangeColumn[QC_MAX_KEY_COLUMN_COUNT * 2];
    smiTableCursor        sCursor;
    UInt                  sTraverse;
    UInt                  sPrevious;
    UInt                  sCursorFlag = 0;
    smiCursorProperties   sProperty;
    
    const void          * sRow = NULL;
    scGRID                sRid;
    const void          * sValue = NULL;

    SDouble               sToCharBuffer[ (ID_SIZEOF(UShort) +
                                          MTC_TO_CHAR_MAX_PRECISION + 7) / 8 ];
    mtdCharType         * sCharValue = (mtdCharType*) &sToCharBuffer;
    mtdDateType         * sDateValue = NULL;

    qcSimpleResult      * sPrevResult = NULL;
    qcSimpleResult      * sCurResult = NULL;
    qmnValueInfo        * sValueInfo = NULL;
    SChar               * sResult = NULL;
    UInt                  sResultSize = 0;
    UInt                  sCount = 0;
    idBool                sAlloc;

    SChar               * sResultTemp       = NULL;
    UInt                  sResultSizeTemp   = 0;
    UShort                sBindCount;
    UShort              * sBindInfo;
    const void          * sOrgRow = NULL;
    const void          * sPreRow = NULL;
    smiRecordLockWaitFlag sRecordLockWaitFlag; //PROJ-1677 DEQUEUE
    
    // �ʱ�ȭ
    aStatement->simpleInfo.results = NULL;
    
    sPROJ = (qmncPROJ*)aStatement->myPlan->plan;
    sSCAN = (qmncSCAN*)aStatement->myPlan->plan->left;
    sScanMethod = &sSCAN->method;
    sIndex = sScanMethod->index;

    if ( aBindColInfo != NULL )
    {
        sBindCount = *aBindColInfo;
        if ( sBindCount > 0 )
        {
            sBindInfo = aBindColInfo + 1;
        }
        else
        {
            sBindCount = sPROJ->targetCount;
            sBindInfo = NULL;
        }
    }
    else
    {
        sBindCount = sPROJ->targetCount;
        sBindInfo = NULL;
    }
    
    //fix BUG-17553
    IDV_SQL_SET( sStatistics, mMemoryTableAccessCount, 0 );

    // table lock
    if ( ( sSCAN->tableRef->tableFlag & SMI_TABLE_TYPE_MASK )
         == SMI_TABLE_FIXED )
    {
        // Nothing to do.
    }
    else
    {
        IDE_TEST( smiValidateAndLockObjects( aSmiTrans,
                                             sSCAN->tableRef->tableHandle,
                                             sSCAN->tableRef->tableSCN,
                                             SMI_TBSLV_DDL_DML,
                                             SMI_TABLE_LOCK_IS,
                                             ID_ULONG_MAX,
                                             ID_FALSE )
                  != IDE_SUCCESS );
    }
    
    // bind buffer �Ҵ�
    if ( sSCAN->simpleValueBufSize > ID_SIZEOF(sCharBuffer) )
    {
        IDE_TEST( aStatement->qmxMem->alloc(
                      sSCAN->simpleValueBufSize,
                      (void**)&sBuffer )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    if ( sSCAN->simpleRid == ID_TRUE )
    {
        // make simple rid range
        IDE_TEST( makeSimpleRidRange( aStatement,
                                      sSCAN,
                                      aBindBuffer,
                                      NULL,
                                      0,
                                      sMtdValue,
                                      sRangeColumn,
                                      & sRange,
                                      & sKeyRange,
                                      & sIsNullRange,
                                      & sBuffer )
                  != IDE_SUCCESS );
        
        // null range�� ����� ����.
        IDE_TEST_CONT( sIsNullRange == ID_TRUE, normal_exit );
        
        sIndexHandle = NULL;
        
        SMI_CURSOR_PROP_INIT_FOR_INDEX_SCAN( &sProperty,
                                             sStatistics,
                                             SMI_BUILTIN_GRID_INDEXTYPE_ID );
    }
    else
    {
        if ( sIndex != NULL )
        {
            // make simple key range
            IDE_TEST( makeSimpleKeyRange( aStatement,
                                          sSCAN,
                                          sIndex,
                                          aBindBuffer,
                                          NULL,
                                          0,
                                          sMtdValue,
                                          sRangeColumn,
                                          & sRange,
                                          & sKeyRange,
                                          & sIsNullRange,
                                          & sBuffer )
                      != IDE_SUCCESS );

            // null range�� ����� ����.
            IDE_TEST_CONT( sIsNullRange == ID_TRUE, normal_exit );
        
            sIndexHandle = sIndex->indexHandle;
        
            SMI_CURSOR_PROP_INIT_FOR_INDEX_SCAN( &sProperty,
                                                 sStatistics,
                                                 sIndex->indexTypeId );
        }
        else
        {
            sIndexHandle = NULL;
            sKeyRange = smiGetDefaultKeyRange();

            SMI_CURSOR_PROP_INIT_FOR_FULL_SCAN( &sProperty, sStatistics );
        }
    }

    if ( sSCAN->limit != NULL )
    {
        sProperty.mFirstReadRecordPos = sSCAN->limit->start.constant - 1;
        sProperty.mReadRecordCount = sSCAN->limit->count.constant;
    }
    else
    {
        // Nothing to do.
    }

    // use fast smiStmt
    if ( QC_SMI_STMT( aStatement ) != NULL )
    {
        sSmiStmt = QC_SMI_STMT( aStatement );
        
        sUseFastSmiStmt = ID_FALSE;
    }
    else
    {
        // Nothing to do.
    }
    
    sSmiStmtFlag &= ~SMI_STATEMENT_MASK;
    sSmiStmtFlag |= SMI_STATEMENT_NORMAL;

    sSmiStmtFlag &= ~SMI_STATEMENT_CURSOR_MASK;
    sSmiStmtFlag |= SMI_STATEMENT_MEMORY_CURSOR;

    // Traverse ������ ����
    if ( ( sSCAN->flag & QMNC_SCAN_TRAVERSE_MASK )
         == QMNC_SCAN_TRAVERSE_FORWARD )
    {
        sTraverse = SMI_TRAVERSE_FORWARD;
    }
    else
    {
        sTraverse = SMI_TRAVERSE_BACKWARD;
    }

    // Previous ��� ���� ����
    if ( ( sSCAN->flag & QMNC_SCAN_PREVIOUS_ENABLE_MASK )
         == QMNC_SCAN_PREVIOUS_ENABLE_TRUE )
    {
        sPrevious = SMI_PREVIOUS_ENABLE;
    }
    else
    {
        sPrevious = SMI_PREVIOUS_DISABLE;
    }
    
    sCursorFlag = sSCAN->lockMode | sTraverse | sPrevious;
    
  retry:
    
    sBegined = ID_FALSE;
    sOpened = ID_FALSE;
    sRetryErr = ID_FALSE;
    sPrevResult = NULL;
    sCurResult = NULL;
    sResult = NULL;
    sResultSize = 0;
    sCount = 0;
    
    // statement begin
    if ( sUseFastSmiStmt == ID_TRUE )
    {
        IDE_TEST( sSmiStmt->begin( sStatistics,
                                   aSmiTrans->getStatement(),
                                   sSmiStmtFlag )
                  != IDE_SUCCESS );

        QC_SMI_STMT( aStatement ) = sSmiStmt;
        sBegined = ID_TRUE;
    }
    else
    {
        // Nothing to do.
    }
    
    sCursor.initialize();

    // PROJ-1618
    sCursor.setDumpObject( sSCAN->dumpObject );

    //PROJ-1677 DEQUEUE
    sRecordLockWaitFlag = ( aStatement->myPlan->parseTree->stmtKind == QCI_STMT_DEQUEUE ) ?
        SMI_RECORD_NO_LOCKWAIT: SMI_RECORD_LOCKWAIT;

    IDE_TEST( sCursor.open( sSmiStmt,
                            sSCAN->table,
                            sIndexHandle,
                            sSCAN->tableSCN,
                            NULL,
                            sKeyRange,
                            smiGetDefaultKeyRange(),
                            smiGetDefaultFilter(),
                            sCursorFlag,
                            SMI_SELECT_CURSOR,
                            & sProperty,
                            sRecordLockWaitFlag )
              != IDE_SUCCESS );
    sOpened = ID_TRUE;
    
    IDE_TEST( sCursor.beforeFirst() != IDE_SUCCESS );

    do
    {
        // BUG-45454 QUEUE ����
RETRY_DEQUEUE:

        sOrgRow = sRow = sPreRow;

        IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT )
                  != IDE_SUCCESS );

        sPreRow = ( sRow == NULL ) ? sOrgRow : sRow;

        // Proj 1360 Queue
        // dequeue�������� �ش� row�� �����ؾ� �Ѵ�.
        if (( sRow != NULL ) &&
            (( sSCAN->flag &  QMNC_SCAN_TABLE_QUEUE_MASK )
             == QMNC_SCAN_TABLE_QUEUE_TRUE ))
        {
            IDE_TEST( sCursor.deleteRow() != IDE_SUCCESS );
                
            //PROJ-1677 DEQUEUE
            if ( sCursor.getRecordLockWaitStatus() ==
                 SMI_ESCAPE_RECORD_LOCKWAIT )
            {
                IDE_RAISE( RETRY_DEQUEUE );
            }
            else
            {
                // nothing to do
            }
        }
        else
        {
            // nothing to do
        }
        
        // BUG-45454 QUEUE ����
        if ( sRow == NULL )
        {
            break;
        }
        else
        {
            // nothing to do
        }
        
        //--------------------------------------
        // limit ó��
        //--------------------------------------
        
        if ( sPROJ->limit != NULL )
        {
            if ( sCount < sPROJ->limit->count.constant )
            {
                // Nothing to do.
            }
            else
            {
                break;
            }
        }
        else
        {
            // Nothing to do.
        }

        //--------------------------------------
        // fetch buffer �Ҵ�
        //--------------------------------------
        
        if ( aShmResult != NULL )
        {
            // mm���� result buffer�� �ִ� ���
            if ( sResult == NULL )
            {
                sResult = (SChar*)aShmResult;
            }
            else
            {
                // Nothing to do.
            }
        }
        else
        {
            // qp���� result buffer�� alloc�ϴ� ���
            
            // alloc result chunk
            if ( sCurResult == NULL )
            {
                sAlloc = ID_TRUE;
            }
            else
            {
                if ( sCurResult->count == RESULT_CHUNK_SIZE )
                {
                    sAlloc = ID_TRUE;
                }
                else
                {
                    sCurResult->count++;
                
                    sAlloc = ID_FALSE;
                }
            }
            
            if ( sAlloc == ID_TRUE )
            {
                IDE_TEST( aStatement->qmxMem->alloc(
                              ID_SIZEOF(qcSimpleResult) +
                              sPROJ->simpleResultSize * RESULT_CHUNK_SIZE,
                              (void**)&sCurResult )
                          != IDE_SUCCESS );

                // init
                sCurResult->result = ((SChar*)sCurResult) + ID_SIZEOF(qcSimpleResult);
                sCurResult->count  = 1;
                sCurResult->idx    = 0;
                sCurResult->next   = NULL;

                // link
                if ( sPrevResult == NULL )
                {
                    aStatement->simpleInfo.results = sCurResult;
                }
                else
                {
                    sPrevResult->next = sCurResult;
                }
            
                sPrevResult = sCurResult;
                sResult = sCurResult->result;
            }
            else
            {
                // Nothing to do.
            }
        }

        //--------------------------------------
        // fetch
        //--------------------------------------
        
        if ( aShmResult != NULL )
        {
            // record size�� ���� ����
            IDE_TEST_RAISE( sResultSize + 8 > aShmSize,
                            ERR_INSUFFICIENT_MEMORY );

            sResultTemp = sResult;
            sResultSizeTemp = sResultSize;
            
            *(ULong*)sResult = 0;  // �ʱ�ȭ
            sResult += 8;
            sResultSize += 8;
        }
        else
        {
            // Nothing to do.
        }
        
        for ( i = 0; i < sBindCount; i++ )
        {
            if ( sBindInfo != NULL )
            {
                j = sBindInfo[i];
                if ( (j < 1) || (j > sPROJ->targetCount) )
                {
                    continue;
                }
                else
                {
                    // Nothing to do.
                }

                j--;
            }
            else
            {
                j = i;
            }
            
            sValueInfo = &(sPROJ->simpleValues[j]);

            if ( aShmResult != NULL )
            {
                IDE_TEST_RAISE(
                    sResultSize + sPROJ->simpleValueSizes[j] + 8 > aShmSize,
                    ERR_INSUFFICIENT_MEMORY );
            }
            else
            {
                // Nothing to do.
            }
            
            switch ( sValueInfo->type )
            {
                case QMN_VALUE_TYPE_COLUMN:
                    sValue = mtc::value( & sValueInfo->column,
                                         sRow,
                                         MTD_OFFSET_USE );
                    break;

                case QMN_VALUE_TYPE_PROWID:
                    sValue = (void*)&sRid;
                    break;

                case QMN_VALUE_TYPE_CONST_VALUE:
                    sValue = sValueInfo->value.constVal;
                    break;

                case QMN_VALUE_TYPE_TO_CHAR:
                    sDateValue = (mtdDateType*) mtc::value(
                        & sValueInfo->value.columnVal.column,
                        sRow,
                        MTD_OFFSET_USE );
                        
                    IDE_TEST( calculateSimpleToChar( sValueInfo,
                                                     sDateValue,
                                                     sCharValue )
                              != IDE_SUCCESS );
                        
                    sValue = (const void*) sCharValue;
                    break;

                default:
                    IDE_RAISE( ERR_INVALID_TYPE );
                    break;
            }

            // column position�� ����Ѵ�.
            if ( aShmResult != NULL )
            {
                *(ULong*)sResult = 0;  // �ʱ�ȭ
                *(UShort*)sResult = (UShort)(j + 1);
                sResult += 8;
                sResultSize += 8;
            }
            else
            {
                // Nothing to do.
            }
            
            IDE_TEST( setSimpleMtdValue( & sValueInfo->column,
                                         sResult,
                                         sValue )
                      != IDE_SUCCESS );
            
            sResult += sPROJ->simpleValueSizes[j];
            sResultSize += sPROJ->simpleValueSizes[j];
        }

        // record size�� ����Ѵ�.
        if ( aShmResult != NULL )
        {
            *(UInt*)sResultTemp = sResultSize - sResultSizeTemp;
        }
        else
        {
            // Nothing to do.
        }
        
        sCount++;
        
        // BUG-45454 QUEUE ����
        // QUEUE�� �ϳ��� row �� FETCH.
        if (( sSCAN->flag &  QMNC_SCAN_TABLE_QUEUE_MASK )
             == QMNC_SCAN_TABLE_QUEUE_TRUE )

        {
            break;
        }
        else
        {
            // Nothing to do.
        }
        
        if ( sSCAN->simpleUnique == ID_TRUE )
        {
            break;
        }
        else
        {
            // Nothing to do.
        }
    } while ( sRow != NULL );

    sOpened = ID_FALSE;
    IDE_TEST( sCursor.close() != IDE_SUCCESS );
    
    // statement close
    if ( sUseFastSmiStmt == ID_TRUE )
    {
        sBegined = ID_FALSE;
        IDE_TEST( sSmiStmt->end( SMI_STATEMENT_RESULT_SUCCESS ) != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    // �ʱ�ȭ
    if ( sCount > 0 )
    {
        aStatement->mFlag &= ~QC_STMT_FAST_FIRST_RESULT_MASK;
        aStatement->mFlag |= QC_STMT_FAST_FIRST_RESULT_TRUE;
    }
    else
    {
        // Nothing to do.
    }

    if ( ( ( sSCAN->tableRef->tableFlag & SMI_TABLE_TYPE_MASK )
           != SMI_TABLE_FIXED ) &&
         ( sCount > 0 ) )
    {
        IDV_SQL_ADD( sStatistics,
                     mMemoryTableAccessCount,
                     sCount );
            
        IDV_SESS_ADD( sStatistics->mSess,
                      IDV_STAT_INDEX_MEMORY_TABLE_ACCESS_COUNT,
                      sCount );
    }
    else
    {
        // Nothing to do.
    }
    
    IDE_EXCEPTION_CONT( normal_exit );

    aStatement->simpleInfo.count = sCount;

    if ( aResultSize != NULL )
    {
        *aResultSize = sResultSize;
    }
    else
    {
        // Nothing to do.
    }
    
    if ( aRowCount != NULL )
    {
        *aRowCount = sCount;
    }
    else
    {
        // Nothing to do.
    }
    
    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_INSUFFICIENT_MEMORY )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::executeFastSelect",
                                  "IPCDA_DATABLOCK_SIZE is small" ) );
    }
    IDE_EXCEPTION( ERR_INVALID_TYPE )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::executeFastSelect",
                                  "invalid bind type" ) );
    }
    IDE_EXCEPTION_END;

    if ( sOpened == ID_TRUE )
    {
        // BUG-40126 retry error
        if ( ( ideGetErrorCode() & E_ACTION_MASK ) == E_ACTION_RETRY )
        {
            sRetryErr = ID_TRUE;
        }
        else
        {
            // Nothing to do.
        }

        (void) sCursor.close();
    }
    else
    {
        // Nothing to do.
    }

    if ( sBegined == ID_TRUE )
    {
        (void) sSmiStmt->end( SMI_STATEMENT_RESULT_FAILURE );
    }
    else
    {
        // Nothing to do.
    }

    if ( ( sRetryErr == ID_TRUE ) &&
         ( sUseFastSmiStmt == ID_TRUE ) )
    {
        goto retry;
    }
    else
    {
        // Nothing to do.
    }
    
    return IDE_FAILURE;
}

#define RESULT_ROW_CHUNK_SIZE  500

IDE_RC qmxSimple::executeFastJoinSelect( smiTrans     * aSmiTrans,
                                         qcStatement  * aStatement,
                                         UShort       * aBindColInfo,
                                         UChar        * aBindBuffer,
                                         UInt           aShmSize,
                                         UChar        * aShmResult,
                                         UInt         * aResultSize,
                                         UInt         * aRowCount )
{
/***********************************************************************
 *
 *  Description : PROJ-2551 simple query ����ȭ
 *
 *  Implementation :
 *
 ***********************************************************************/

    idvSQL              * sStatistics = aStatement->mStatistics;
    qmncPROJ            * sPROJ = NULL;
    qmncJOIN            * sJOIN = NULL;
    qmxFastRow          * sLastRow = NULL;
    qmxFastScanInfo     * sLastScan = NULL;
    qmxFastScanInfo     * sScanInfo = NULL;
    SInt                  sScanCount = 0;
    SChar                 sCharBuffer[4096];
    SChar               * sBuffer = sCharBuffer;
    SInt                  sScanIndex[QC_MAX_REF_TABLE_CNT];
    SInt                * sIndex = sScanIndex;
    UInt                  sSimpleValueBufSize = 0;
    UInt                  i;
    UInt                  j;
    SInt                  k;
    SInt                  l;
    UInt                  m;

    idBool                sUseFastSmiStmt = ID_TRUE;
    smiStatement          sFastSmiStmt;
    smiStatement        * sSmiStmt = &sFastSmiStmt;
    UInt                  sSmiStmtFlag = 0;
    idBool                sBegined = ID_FALSE;
    idBool                sRetryErr = ID_FALSE;
    idBool                sIsScanInfoInit = ID_FALSE;
    UInt                  sCount = 0;

    SDouble               sToCharBuffer[ (ID_SIZEOF(UShort) +
                                          MTC_TO_CHAR_MAX_PRECISION + 7) / 8 ];
    mtdCharType         * sCharValue = (mtdCharType*) &sToCharBuffer;
    mtdDateType         * sDateValue;
    
    SChar               * sResult = NULL;
    UInt                  sResultSize;
    qcSimpleResult      * sCurResult = NULL;
    qmnValueInfo        * sValueInfo = NULL;
    const void          * sValue = NULL;

    SChar               * sResultTemp       = NULL;
    UInt                  sResultSizeTemp   = 0;
    UShort                sBindCount;
    UShort              * sBindInfo;
    ULong                 sAllocSize = 0;
    
    // �ʱ�ȭ
    aStatement->simpleInfo.results = NULL;
    
    sPROJ = (qmncPROJ*)aStatement->myPlan->plan;
    sJOIN = (qmncJOIN*)(sPROJ->plan.left);

    if ( aBindColInfo != NULL )
    {
        sBindCount = *aBindColInfo;
        if ( sBindCount > 0 )
        {
            sBindInfo = aBindColInfo + 1;
        }
        else
        {
            sBindCount = sPROJ->targetCount;
            sBindInfo = NULL;
        }
    }
    else
    {
        sBindCount = sPROJ->targetCount;
        sBindInfo = NULL;
    }
    
    while ( 1 )
    {
        // �������� �׻� SCAN
        if ( sJOIN->plan.right->type == QMN_SCAN )
        {
            sScanCount++;
            
            // ������ JOIN�̰ų� SCAN
            if ( sJOIN->plan.left->type == QMN_JOIN )
            {
                sJOIN = (qmncJOIN*)sJOIN->plan.left;
            }
            else if ( sJOIN->plan.left->type == QMN_SCAN )
            {
                sScanCount++;
                break;
            }
            else
            {
                IDE_RAISE( ERR_INVALID_JOIN );
            }
        }
        else
        {
            IDE_RAISE( ERR_INVALID_JOIN );
        }
    }
    
    // BUG-43609
    IDE_TEST( aStatement->qmxMem->alloc( ID_SIZEOF(qmxFastScanInfo) * sScanCount,
                                         (void**)&sScanInfo )
              != IDE_SUCCESS );
    
    // make scan array
    // �ڿ��� ���� ä���.
    k = sScanCount - 1;
    
    sJOIN = (qmncJOIN*)(sPROJ->plan.left);
    
    while ( 1 )
    {
        // �������� �׻� SCAN
        if ( sJOIN->plan.right->type == QMN_SCAN )
        {
            sScanInfo[k].scan = (qmncSCAN*)sJOIN->plan.right;
            k--;
            
            // ������ JOIN�̰ų� SCAN
            if ( sJOIN->plan.left->type == QMN_JOIN )
            {
                sJOIN = (qmncJOIN*)sJOIN->plan.left;
            }
            else if ( sJOIN->plan.left->type == QMN_SCAN )
            {
                sScanInfo[k].scan = (qmncSCAN*)sJOIN->plan.left;
                break;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    
    IDE_DASSERT( k == 0 );
    
    //fix BUG-17553
    IDV_SQL_SET( sStatistics, mMemoryTableAccessCount, 0 );

    // table lock
    for ( k = 0; k < sScanCount; k++ )
    {
        if ( ( sScanInfo[k].scan->tableRef->tableFlag & SMI_TABLE_TYPE_MASK )
             == SMI_TABLE_FIXED )
        {
            // Nothing to do.
        }
        else
        {
            IDE_TEST( smiValidateAndLockObjects( aSmiTrans,
                                                 sScanInfo[k].scan->tableRef->tableHandle,
                                                 sScanInfo[k].scan->tableRef->tableSCN,
                                                 SMI_TBSLV_DDL_DML,
                                                 SMI_TABLE_LOCK_IS,
                                                 ID_ULONG_MAX,
                                                 ID_FALSE )
                      != IDE_SUCCESS );
        }

        if ( sSimpleValueBufSize < sScanInfo[k].scan->simpleValueBufSize )
        {
            sSimpleValueBufSize = sScanInfo[k].scan->simpleValueBufSize;
        }
        else
        {
            // Nothing to do.
        }
    }

    // bind buffer �Ҵ�
    if ( sSimpleValueBufSize > ID_SIZEOF(sCharBuffer) )
    {
        IDE_TEST( aStatement->qmxMem->alloc(
                      sSimpleValueBufSize,
                      (void**)&sBuffer )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    // target column index �Ҵ�
    if ( sPROJ->targetCount > QC_MAX_REF_TABLE_CNT )
    {
        IDE_TEST( aStatement->qmxMem->alloc(
                      ID_SIZEOF(SInt) * sPROJ->targetCount,
                      (void**)&sIndex )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    // use fast smiStmt
    if ( QC_SMI_STMT( aStatement ) != NULL )
    {
        sSmiStmt = QC_SMI_STMT( aStatement );
        
        sUseFastSmiStmt = ID_FALSE;
    }
    else
    {
        // Nothing to do.
    }
    
    sSmiStmtFlag &= ~SMI_STATEMENT_MASK;
    sSmiStmtFlag |= SMI_STATEMENT_NORMAL;

    sSmiStmtFlag &= ~SMI_STATEMENT_CURSOR_MASK;
    sSmiStmtFlag |= SMI_STATEMENT_MEMORY_CURSOR;

    sIsScanInfoInit = ID_TRUE;

  retry:

    // �ʱ�ȭ
    for ( k = 0; k < sScanCount; k++ )
    {
        sScanInfo[k].keyRange = NULL;
        sScanInfo[k].inited = ID_FALSE;
        sScanInfo[k].opened = ID_FALSE;
        sScanInfo[k].rowInfo = NULL;
        sScanInfo[k].rowCount = 0;
    }

    sBegined = ID_FALSE;
    sRetryErr = ID_FALSE;
    sResult = NULL;
    sResultSize = 0;
    sCount = 0;
    
    // statement begin
    if ( sUseFastSmiStmt == ID_TRUE )
    {
        IDE_TEST( sSmiStmt->begin( sStatistics,
                                   aSmiTrans->getStatement(),
                                   sSmiStmtFlag )
                  != IDE_SUCCESS );

        QC_SMI_STMT( aStatement ) = sSmiStmt;
        sBegined = ID_TRUE;
    }
    else
    {
        // Nothing to do.
    }

    //----------------------------
    // do left scan
    //----------------------------
    
    IDE_TEST( doFastScan( aStatement,
                          sSmiStmt,
                          aBindBuffer,
                          sBuffer,
                          sScanInfo,
                          0 )
              != IDE_SUCCESS );

    //----------------------------
    // do right scan
    //----------------------------

    for ( k = 1; k < sScanCount; k++ )
    {
        // set last scan
        sLastScan = sScanInfo + k - 1;

        for ( i = 0; i < sLastScan->rowCount; i++ )
        {
            if ( i == 0 )
            {
                sLastScan->curRowInfo = sLastScan->rowInfo;
                sLastScan->curRow = sLastScan->curRowInfo->rowBuf;
                sLastScan->curIdx = 0;
            }
            else
            {
                if ( sLastScan->curIdx == RESULT_ROW_CHUNK_SIZE )
                {
                    sLastScan->curRowInfo = sLastScan->curRowInfo->next;
                    sLastScan->curRow = sLastScan->curRowInfo->rowBuf;
                    sLastScan->curIdx = 0;
                }
                else
                {
                    // Nothing to do.
                }
            }
            
            // set last row
            sLastScan->row = sLastScan->curRow->row;
            
            // set left rows
            sLastRow = sLastScan->curRow;
            for ( l = k - 2; l >= 0; l-- )
            {
                sLastRow = sLastRow->leftRow;
                
                // set left row
                sScanInfo[l].row = sLastRow->row;
            }

            IDE_TEST( doFastScan( aStatement,
                                  sSmiStmt,
                                  aBindBuffer,
                                  sBuffer,
                                  sScanInfo,
                                  k )
                      != IDE_SUCCESS );

            sLastScan->curRow++;
            sLastScan->curIdx++;
        }
    }
    
    //----------------------------
    // do fetch
    //----------------------------

    sLastScan = sScanInfo + sScanCount - 1;

    if ( aShmResult != NULL )
    {
        // mm���� result buffer�� �ִ� ���
        if ( sResult == NULL )
        {
            sResult = (SChar*)aShmResult;
        }
        else
        {
            // Nothing to do.
        }
    }
    else
    {
        // qp���� result buffer�� alloc�ϴ� ���
            
        // alloc result chunk
        if ( sLastScan->rowCount > 0 )
        {
            /*BUG-45614 Fast Simple Join FATAL */
            sAllocSize = ((ULong)sPROJ->simpleResultSize * (ULong)sLastScan->rowCount);
            IDE_TEST_RAISE( ( ULONG_MAX - ID_SIZEOF(qcSimpleResult) ) <= sAllocSize, ERR_MEM_ALLOC );

            sAllocSize = ID_SIZEOF(qcSimpleResult) + sAllocSize;
            IDE_TEST( aStatement->qmxMem->alloc( sAllocSize, (void**)&sCurResult )
                      != IDE_SUCCESS );
            // init
            sCurResult->result = ((SChar*)sCurResult) + ID_SIZEOF(qcSimpleResult);
            sCurResult->count  = 0;
            sCurResult->idx    = 0;
            sCurResult->next   = NULL;
    
            // link
            aStatement->simpleInfo.results = sCurResult;
            sResult = sCurResult->result;
        }
        else
        {
            // Nothing to do.
        }
    }

    // make target column scan index
    if ( sLastScan->rowCount > 0 )
    {
        for ( i = 0; i < sPROJ->targetCount; i++ )
        {
            sValueInfo = &(sPROJ->simpleValues[i]);
        
            sIndex[i] = -1;

            if ( ( sValueInfo->type == QMN_VALUE_TYPE_COLUMN ) ||
                 ( sValueInfo->type == QMN_VALUE_TYPE_PROWID ) )
            {
                for ( k = 0; k < sScanCount; k++ )
                {
                    if ( sScanInfo[k].scan->tupleRowID ==
                         sValueInfo->value.columnVal.table )
                    {
                        sIndex[i] = k;
                        break;
                    }
                    else
                    {
                        // Nothing to do.
                    }
                }
                
                IDE_TEST_RAISE( sIndex[i] == -1,
                                ERR_INVALID_COLUMN );
            }
            else
            {
                // Nothing to do.
            }
        }
    }
    else
    {
        // Nothing to do.
    }
    
    for ( i = 0; i < sLastScan->rowCount; i++ )
    {
        if ( i == 0 )
        {
            sLastScan->curRowInfo = sLastScan->rowInfo;
            sLastScan->curRow = sLastScan->curRowInfo->rowBuf;
            sLastScan->curIdx = 0;
        }
        else
        {
            if ( sLastScan->curIdx == RESULT_ROW_CHUNK_SIZE )
            {
                sLastScan->curRowInfo = sLastScan->curRowInfo->next;
                sLastScan->curRow = sLastScan->curRowInfo->rowBuf;
                sLastScan->curIdx = 0;
            }
            else
            {
                // Nothing to do.
            }
        }

        if ( sPROJ->limit != NULL )
        {
            if ( sCount < sPROJ->limit->count.constant )
            {
                // Nothing to do.
            }
            else
            {
                break;
            }
        }
        else
        {
            // Nothing to do.
        }
        
        // set row
        sLastScan->row = sLastScan->curRow->row;
        
        // set left rows
        sLastRow = sLastScan->curRow;
        for ( l = sScanCount - 2; l >= 0; l-- )
        {
            sLastRow = sLastRow->leftRow;
            
            // set left row
            sScanInfo[l].row = sLastRow->row;
        }

        // copy target
        if ( aShmResult != NULL )
        {
            // record size�� ���� ����
            IDE_TEST_RAISE( sResultSize + 8 > aShmSize,
                            ERR_INSUFFICIENT_MEMORY );

            sResultTemp = sResult;
            sResultSizeTemp = sResultSize;
            
            *(ULong*)sResult = 0;  // �ʱ�ȭ
            sResult += 8;
            sResultSize += 8;
        }
        else
        {
            // Nothing to do.
        }
        
        for ( j = 0; j < sBindCount; j++ )
        {
            if ( sBindInfo != NULL )
            {
                m = sBindInfo[j];
                if ( (m < 1) || (m > sPROJ->targetCount) )
                {
                    continue;
                }
                else
                {
                    // Nothing to do.
                }
                
                m--;
            }
            else
            {
                m = j;
            }
            
            sValueInfo = &(sPROJ->simpleValues[m]);
            
            if ( aShmResult != NULL )
            {
                IDE_TEST_RAISE(
                    sResultSize + sPROJ->simpleValueSizes[m] + 8 > aShmSize,
                    ERR_INSUFFICIENT_MEMORY );
            }
            else
            {
                // Nothing to do.
            }
            
            switch ( sValueInfo->type )
            {
                case QMN_VALUE_TYPE_COLUMN:
                    sValue = mtc::value( & sValueInfo->column,
                                         sScanInfo[sIndex[m]].row,
                                         MTD_OFFSET_USE );
                    break;

                case QMN_VALUE_TYPE_PROWID:
                    sValue = (void*)&sScanInfo[sIndex[m]].rid;
                    break;

                case QMN_VALUE_TYPE_CONST_VALUE:
                    sValue = sValueInfo->value.constVal;
                    break;

                case QMN_VALUE_TYPE_TO_CHAR:
                    sDateValue = (mtdDateType*) mtc::value(
                        & sValueInfo->column,
                        sScanInfo[sIndex[m]].row,
                        MTD_OFFSET_USE );
                    
                    IDE_TEST( calculateSimpleToChar( sValueInfo,
                                                     sDateValue,
                                                     sCharValue )
                              != IDE_SUCCESS );
                        
                    sValue = (const void*) sCharValue;
                    break;

                default:
                    IDE_RAISE( ERR_INVALID_TYPE );
                    break;
            }

            // column position�� ����Ѵ�.
            if ( aShmResult != NULL )
            {
                *(ULong*)sResult = 0;  // �ʱ�ȭ
                *(UShort*)sResult = (UShort)(m + 1);
                sResult += 8;
                sResultSize += 8;
            }
            else
            {
                // Nothing to do.
            }
            
            IDE_TEST( setSimpleMtdValue( & sValueInfo->column,
                                         sResult,
                                         sValue )
                      != IDE_SUCCESS );
            
            sResult += sPROJ->simpleValueSizes[m];
            sResultSize += sPROJ->simpleValueSizes[m];
        }
        
        // record size�� ����Ѵ�.
        if ( aShmResult != NULL )
        {
            *(UInt*)sResultTemp = sResultSize - sResultSizeTemp;
        }
        else
        {
            sCurResult->count++;
        }

        sCount++;

        sLastScan->curRow++;
        sLastScan->curIdx++;
    }
    
    // close cursor
    for ( k = 0; k < sScanCount; k++ )
    {
        if ( sScanInfo[k].opened == ID_TRUE )
        {
            sScanInfo[k].opened = ID_FALSE;
            IDE_TEST( sScanInfo[k].cursor.close() != IDE_SUCCESS );
        }
        else
        {
            // Nothing to do.
        }
    }
    
    // statement close
    if ( sUseFastSmiStmt == ID_TRUE )
    {
        sBegined = ID_FALSE;
        IDE_TEST( sSmiStmt->end( SMI_STATEMENT_RESULT_SUCCESS ) != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    // �ʱ�ȭ
    if ( sCount > 0 )
    {
        aStatement->mFlag &= ~QC_STMT_FAST_FIRST_RESULT_MASK;
        aStatement->mFlag |= QC_STMT_FAST_FIRST_RESULT_TRUE;
    }
    else
    {
        // Nothing to do.
    }

    if ( ( ( sScanInfo->scan->tableRef->tableFlag & SMI_TABLE_TYPE_MASK )
           != SMI_TABLE_FIXED ) &&
         ( sCount > 0 ) )
    {
        IDV_SQL_ADD( sStatistics,
                     mMemoryTableAccessCount,
                     sCount );

        IDV_SESS_ADD( sStatistics->mSess,
                      IDV_STAT_INDEX_MEMORY_TABLE_ACCESS_COUNT,
                      sCount );
    }
    else
    {
        // Nothing to do.
    }

    aStatement->simpleInfo.count = sCount;
    
    if ( aResultSize != NULL )
    {
        *aResultSize = sResultSize;
    }
    else
    {
        // Nothing to do.
    }
    
    if ( aRowCount != NULL )
    {
        *aRowCount = sCount;
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_INSUFFICIENT_MEMORY )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::executeFastJoinSelect",
                                  "IPCDA_DATABLOCK_SIZE is small" ) );
    }
    IDE_EXCEPTION( ERR_INVALID_JOIN )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::executeFastJoinSelect",
                                  "invalid join" ) );
    }
    IDE_EXCEPTION( ERR_INVALID_COLUMN )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::executeFastJoinSelect",
                                  "invalid column" ) );
    }
    IDE_EXCEPTION( ERR_INVALID_TYPE )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::executeFastJoinSelect",
                                  "invalid bind type" ) );
    }
    IDE_EXCEPTION( ERR_MEM_ALLOC )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::executeFastJoinSelect",
                                  "alloc size too big" ) );
    }
    IDE_EXCEPTION_END;

    // BUG-43338 Null Pointer Dereference
    if ( (sScanInfo != NULL) &&
         (sIsScanInfoInit == ID_TRUE) )
    {
        for ( k = 0; k < sScanCount; k++ )
        {
            if ( sScanInfo[k].opened == ID_TRUE )
            {
                // BUG-40126 retry error
                if ( ( ( ideGetErrorCode() & E_ACTION_MASK ) == E_ACTION_RETRY ) &&
                     ( sRetryErr = ID_FALSE ) )
                {
                    sRetryErr = ID_TRUE;
                }
                else
                {
                    // Nothing to do.
                }

                sScanInfo[k].opened = ID_FALSE;
                (void) sScanInfo[k].cursor.close();
            }
            else
            {
                // Nothing to do.
            }
        }
    }
    else
    {
        // Nothing to do.
    }

    if ( sBegined == ID_TRUE )
    {
        (void) sSmiStmt->end( SMI_STATEMENT_RESULT_FAILURE );
    }
    else
    {
        // Nothing to do.
    }

    if ( ( sRetryErr == ID_TRUE ) &&
         ( sUseFastSmiStmt == ID_TRUE ) )
    {
        goto retry;
    }
    else
    {
        // Nothing to do.
    }

    return IDE_FAILURE;
}

IDE_RC qmxSimple::doFastScan( qcStatement      * aStatement,
                              smiStatement     * aSmiStmt,
                              UChar            * aBindBuffer,
                              SChar            * aBuffer,
                              qmxFastScanInfo  * aScanInfo,
                              UInt               aScanIndex )
{
    idvSQL              * sStatistics = aStatement->mStatistics;
    qmxFastScanInfo     * sScan = NULL;
    SChar               * sBuffer = aBuffer;
    idBool                sIsNullRange = ID_FALSE;
    UInt                  sTraverse;
    UInt                  sPrevious;
    UInt                  sCursorFlag;
    UInt                  sCount;

    sScan = aScanInfo + aScanIndex;
    sCount = sScan->rowCount;

    if ( sScan->rowInfo == NULL )
    {
        // row buffer �Ҵ�
        IDE_TEST( aStatement->qmxMem->alloc(
                      ID_SIZEOF(qmxFastRowInfo) +
                      ID_SIZEOF(qmxFastRow) * RESULT_ROW_CHUNK_SIZE,
                      (void**)&(sScan->curRowInfo) )
                  != IDE_SUCCESS );
        
        // �ʱ�ȭ
        sScan->curRowInfo->rowBuf = (qmxFastRow*)
            (((SChar*)sScan->curRowInfo) + ID_SIZEOF(qmxFastRowInfo));
        sScan->curRowInfo->next = NULL;

        // link
        sScan->rowInfo = sScan->curRowInfo;

        // ����
        sScan->curRow = sScan->curRowInfo->rowBuf;
        sScan->curIdx = 0;
    }
    else
    {
        // Nothing to do.
    }

    if ( sScan->inited == ID_FALSE )
    {
        sScan->index = sScan->scan->method.index;

        if ( sScan->index != NULL )
        {
            sScan->indexHandle = sScan->index->indexHandle;
            
            SMI_CURSOR_PROP_INIT_FOR_INDEX_SCAN( &(sScan->property),
                                                 sStatistics,
                                                 sScan->index->indexTypeId );
        }
        else
        {
            sScan->indexHandle = NULL;
            
            SMI_CURSOR_PROP_INIT_FOR_FULL_SCAN( &(sScan->property),
                                                sStatistics );
        }

        sScan->cursor.initialize();

        sScan->inited = ID_TRUE;
    }
    else
    {
        // Nothing to do.
    }

    if ( sScan->scan->simpleRid == ID_TRUE )
    {
        IDE_TEST( makeSimpleRidRange( aStatement,
                                      sScan->scan,
                                      aBindBuffer,
                                      aScanInfo,
                                      aScanIndex,
                                      sScan->mtdValue,
                                      sScan->rangeColumn,
                                      & sScan->range,
                                      & sScan->keyRange,
                                      & sIsNullRange,
                                      & sBuffer )
                  != IDE_SUCCESS );

        // null range�� ����� ����.
        IDE_TEST_CONT( sIsNullRange == ID_TRUE, normal_exit );
    }
    else
    {
        // make simple key range
        if ( sScan->index != NULL )
        {
            if ( sScan->keyRange == NULL )
            {
                IDE_TEST( makeSimpleKeyRange( aStatement,
                                              sScan->scan,
                                              sScan->index,
                                              aBindBuffer,
                                              aScanInfo,
                                              aScanIndex,
                                              sScan->mtdValue,
                                              sScan->rangeColumn,
                                              & sScan->range,
                                              & sScan->keyRange,
                                              & sIsNullRange,
                                              & sBuffer )
                          != IDE_SUCCESS );
            }
            else
            {
                IDE_TEST( updateSimpleKeyRange( sScan->scan,
                                                aScanInfo,
                                                aScanIndex,
                                                sScan->mtdValue,
                                                sScan->rangeColumn,
                                                & sIsNullRange )
                          != IDE_SUCCESS );
            }

            // null range�� ����� ����.
            IDE_TEST_CONT( sIsNullRange == ID_TRUE, normal_exit );
        }
        else
        {
            sScan->keyRange = smiGetDefaultKeyRange();
        }
    }
    
    // PROJ-1618
    sScan->cursor.setDumpObject( sScan->scan->dumpObject );

    if ( sScan->opened == ID_FALSE )
    {
        // Traverse ������ ����
        if ( ( sScan->scan->flag & QMNC_SCAN_TRAVERSE_MASK )
             == QMNC_SCAN_TRAVERSE_FORWARD )
        {
            sTraverse = SMI_TRAVERSE_FORWARD;
        }
        else
        {
            sTraverse = SMI_TRAVERSE_BACKWARD;
        }

        // Previous ��� ���� ����
        if ( ( sScan->scan->flag & QMNC_SCAN_PREVIOUS_ENABLE_MASK )
             == QMNC_SCAN_PREVIOUS_ENABLE_TRUE )
        {
            sPrevious = SMI_PREVIOUS_ENABLE;
        }
        else
        {
            sPrevious = SMI_PREVIOUS_DISABLE;
        }
    
        sCursorFlag = sScan->scan->lockMode | sTraverse | sPrevious;

        IDE_TEST( sScan->cursor.open( aSmiStmt,
                                      sScan->scan->table,
                                      sScan->indexHandle,
                                      sScan->scan->tableSCN,
                                      NULL,
                                      sScan->keyRange,
                                      smiGetDefaultKeyRange(),
                                      smiGetDefaultFilter(),
                                      sCursorFlag,
                                      SMI_SELECT_CURSOR,
                                      & (sScan->property) )
                  != IDE_SUCCESS );
            
        sScan->opened = ID_TRUE;
    }
    else
    {
        IDE_TEST( sScan->cursor.restart( sScan->keyRange,
                                         smiGetDefaultKeyRange(),
                                         smiGetDefaultFilter() )
                  != IDE_SUCCESS);
    }
    
    IDE_TEST( sScan->cursor.beforeFirst() != IDE_SUCCESS );

    IDE_TEST( sScan->cursor.readRow( &(sScan->curRow->row),
                                     &(sScan->rid),
                                     SMI_FIND_NEXT )
              != IDE_SUCCESS );

    while ( sScan->curRow->row != NULL )
    {
        // left row�� ����Ѵ�.
        if ( aScanIndex == 0 )
        {
            // first left scan
            sScan->curRow->leftRow = NULL;
        }
        else
        {
            sScan->curRow->leftRow = aScanInfo[aScanIndex - 1].curRow;
        }

        sScan->curRow++;
        sScan->curIdx++;
        sCount++;
        
        if ( sScan->curIdx == RESULT_ROW_CHUNK_SIZE )
        {
            // row buffer �Ҵ� & link
            IDE_TEST( aStatement->qmxMem->alloc(
                          ID_SIZEOF(qmxFastRowInfo) +
                          ID_SIZEOF(qmxFastRow) * RESULT_ROW_CHUNK_SIZE,
                          (void**)&(sScan->curRowInfo->next) )
                      != IDE_SUCCESS );

            sScan->curRowInfo = sScan->curRowInfo->next;
            
            // �ʱ�ȭ
            sScan->curRowInfo->rowBuf = (qmxFastRow*)
                (((SChar*)sScan->curRowInfo) + ID_SIZEOF(qmxFastRowInfo));
            sScan->curRowInfo->next = NULL;
            
            // ����
            sScan->curRow = sScan->curRowInfo->rowBuf;
            sScan->curIdx = 0;
        }
        else
        {
            // Nothing to do.
        }

        if ( sScan->scan->simpleUnique == ID_TRUE )
        {
            break;
        }
        else
        {
            // Nothing to do.
        }
        
        IDE_TEST( sScan->cursor.readRow( &(sScan->curRow->row),
                                         &(sScan->rid),
                                         SMI_FIND_NEXT )
                  != IDE_SUCCESS );
    }

    sScan->rowCount = sCount;

    IDE_EXCEPTION_CONT( normal_exit );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmxSimple::fastMoveNextResult( qcStatement  * aStatement,
                                      idBool       * aRecordExist )
{
    idBool  sExist = ID_FALSE;

    /* BUG-45613 EXECUTOR_FAST_SIMPLE_QUERY=1 ���� �� cursor fetch �� diff �߻��մϴ�. */
    IDE_TEST( iduCheckSessionEvent( QC_STATISTICS( aStatement ) )
              != IDE_SUCCESS );

    if ( ( aStatement->mFlag & QC_STMT_FAST_FIRST_RESULT_MASK )
         == QC_STMT_FAST_FIRST_RESULT_TRUE )
    {
        aStatement->mFlag &= ~QC_STMT_FAST_FIRST_RESULT_MASK;
        aStatement->mFlag |= QC_STMT_FAST_FIRST_RESULT_FALSE;

        if ( aStatement->simpleInfo.results != NULL )
        {
            aStatement->simpleInfo.results->idx = 0;
            
            sExist = ID_TRUE;
        }
        else
        {
            if ( ( aStatement->mFlag & QC_STMT_FAST_COPY_RESULT_MASK )
                 == QC_STMT_FAST_COPY_RESULT_TRUE )
            {
                sExist = ID_TRUE;
            }
            else
            {
                // Nothing to do.
            }
        }
    }
    else
    {
        if ( aStatement->simpleInfo.results != NULL )
        {
            aStatement->simpleInfo.results->idx++;
            
            if ( aStatement->simpleInfo.results->idx <
                 aStatement->simpleInfo.results->count )
            {
                sExist = ID_TRUE;
            }
            else
            {
                aStatement->simpleInfo.results =
                    aStatement->simpleInfo.results->next;
                
                if ( aStatement->simpleInfo.results != NULL )
                {
                    aStatement->simpleInfo.results->idx = 0;
                        
                    if ( aStatement->simpleInfo.results->idx <
                         aStatement->simpleInfo.results->count )
                    {
                        sExist = ID_TRUE;
                    }
                    else
                    {
                        // Nothing to do.
                    }
                }
                else
                {
                    // Nothing to do.
                }
            }
        }
        else
        {
            // Nothing to do.
        }
    }

    *aRecordExist = sExist;

    return IDE_SUCCESS;

    /* BUG-45613 EXECUTOR_FAST_SIMPLE_QUERY=1 ���� �� cursor fetch �� diff �߻��մϴ�. */
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmxSimple::executeFastInsert( smiTrans     * aSmiTrans,
                                     qcStatement  * aStatement,
                                     UChar        * aBindBuffer,
                                     UInt         * aRowCount )
{
/***********************************************************************
 *
 *  Description : PROJ-2551 simple query ����ȭ
 *
 *  Implementation :
 *
 ***********************************************************************/

    idvSQL              * sStatistics = aStatement->mStatistics;
    qmncINST            * sINST = NULL;
    smiValue              sSmiValues[QC_MAX_COLUMN_COUNT];
    SChar                 sCharBuffer[4096];
    SChar               * sBuffer = sCharBuffer;
    mtdDateType           sSysdate;
    idBool                sUseSysdate = ID_FALSE;
    qmnValueInfo        * sValueInfo = NULL;
    void                * sValue = NULL;
    UInt                  i;

    idBool                sUseFastSmiStmt = ID_TRUE;
    smiStatement          sFastSmiStmt;
    smiStatement        * sSmiStmt = &sFastSmiStmt;
    UInt                  sSmiStmtFlag = 0;
    idBool                sBegined = ID_FALSE;
    idBool                sOpened = ID_FALSE;

    smiTableCursor        sCursor;
    UInt                  sCursorFlag = 0;
    smiCursorProperties   sProperty;
    
    void                * sRow = NULL;
    scGRID                sRid;
    UInt                  sColumnOrder;
    iduMemoryStatus       sQmxMemStatus;
    
    sINST = (qmncINST*)aStatement->myPlan->plan;

    //fix BUG-17553
    IDV_SQL_SET( sStatistics, mMemoryTableAccessCount, 0 );

    // table lock
    IDE_TEST( smiValidateAndLockObjects( aSmiTrans,
                                         sINST->tableRef->tableHandle,
                                         sINST->tableRef->tableSCN,
                                         SMI_TBSLV_DDL_DML,
                                         SMI_TABLE_LOCK_IX,
                                         ID_ULONG_MAX,
                                         ID_FALSE )
              != IDE_SUCCESS );

    /* PROJ-2359 Table/Partition Access Option */
    IDE_TEST( qmx::checkAccessOption( sINST->tableRef->tableInfo,
                                      ID_TRUE /* aIsInsertion */ )
              != IDE_SUCCESS );
    
    // bind buffer, canonize buffer �Ҵ�
    if ( sINST->simpleValueBufSize > ID_SIZEOF(sCharBuffer) )
    {
        IDE_TEST( aStatement->qmxMem->alloc(
                      sINST->simpleValueBufSize,
                      (void**)&sBuffer )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    /* BUG-45373 Simple query sys_date_for_natc bug */
    sSysdate.year = 0;
    sSysdate.mon_day_hour = 0;
    sSysdate.min_sec_mic = 0;

    for ( i = 0; i < sINST->simpleValueCount; i++ )
    {
        sValueInfo = &(sINST->simpleValues[i]);

        switch ( sValueInfo->type )
        {
            case QMN_VALUE_TYPE_CONST_VALUE:
                
                IDE_TEST( getSimpleConstMtdValue(
                              aStatement,
                              & sValueInfo->column,
                              & sValue,
                              sValueInfo->value.constVal,
                              & sBuffer,
                              ID_TRUE,
                              sValueInfo->isQueue,
                              sINST->queueMsgIDSeq )
                          != IDE_SUCCESS );
                break;

            case QMN_VALUE_TYPE_HOST_VALUE:
                
                if ( aBindBuffer != NULL )
                {
                    IDE_TEST( getSimpleCValue(
                                  sValueInfo,
                                  & sValue,
                                  aStatement->pBindParam,
                                  aBindBuffer,
                                  & sBuffer,
                                  ID_TRUE )  // canonize�� �ʿ���
                              != IDE_SUCCESS );
                }
                else
                {
                    IDE_TEST( getSimpleMtdValue(
                                  sValueInfo,
                                  & sValue,
                                  aStatement->pBindParam,
                                  & sBuffer,
                                  ID_TRUE )
                              != IDE_SUCCESS );
                }
                break;

            case QMN_VALUE_TYPE_SYSDATE:
                if ( sUseSysdate == ID_FALSE )
                {
                    /* BUG-45373 SYSDATE_FOR_NATC Bug */
                    if ( QCU_DISPLAY_PLAN_FOR_NATC == 0 )
                    {
                        IDE_TEST( qtc::sysdate( &sSysdate ) != IDE_SUCCESS );
                    }
                    else
                    {
                        if ( QCU_SYSDATE_FOR_NATC[0] != '\0' )
                        {
                            if ( mtdDateInterface::toDate( &sSysdate,
                                                           (UChar*)QCU_SYSDATE_FOR_NATC,
                                                           idlOS::strlen(QCU_SYSDATE_FOR_NATC),
                                                           (UChar *)QCG_GET_SESSION_DATE_FORMAT(aStatement),
                                                           idlOS::strlen(QCG_GET_SESSION_DATE_FORMAT(aStatement)) )

                                      != IDE_SUCCESS )
                            {
                                IDE_TEST( qtc::sysdate( &sSysdate ) != IDE_SUCCESS );
                            }
                            else
                            {
                                /* Nothing to do */
                            }
                        }
                        else
                        {
                            IDE_TEST( qtc::sysdate( &sSysdate ) != IDE_SUCCESS );
                        }
                    }
                    sUseSysdate = ID_TRUE;
                }
                else
                {
                    // Nothing to do.
                }
                sValue = & sSysdate;
                break;
                
            default:
                IDE_RAISE( ERR_INVALID_TYPE );
                break;
        }

        // check not null
        IDE_TEST( checkSimpleNullValue( & sValueInfo->column,
                                        sValue )
                  != IDE_SUCCESS );
        
        // set smiValue
        IDE_TEST( setSimpleSmiValue( & sValueInfo->column,
                                     sValue,
                                     &(sSmiValues[i]) )
                  != IDE_SUCCESS );
    }

    // use fast smiStmt
    if ( QC_SMI_STMT( aStatement ) != NULL )
    {
        sSmiStmt = QC_SMI_STMT( aStatement );
        
        sUseFastSmiStmt = ID_FALSE;
    }
    else
    {
        // Nothing to do.
    }
    
    sSmiStmtFlag &= ~SMI_STATEMENT_MASK;
    sSmiStmtFlag |= SMI_STATEMENT_NORMAL;

    sSmiStmtFlag &= ~SMI_STATEMENT_CURSOR_MASK;
    sSmiStmtFlag |= SMI_STATEMENT_MEMORY_CURSOR;

    SMI_CURSOR_PROP_INIT_FOR_FULL_SCAN( &sProperty, sStatistics );
    
    sCursorFlag = SMI_LOCK_WRITE|SMI_TRAVERSE_FORWARD|SMI_PREVIOUS_DISABLE;

    // statement begin
    if ( sUseFastSmiStmt == ID_TRUE )
    {
        IDE_TEST( sSmiStmt->begin( sStatistics,
                                   aSmiTrans->getStatement(),
                                   sSmiStmtFlag )
                  != IDE_SUCCESS );

        QC_SMI_STMT( aStatement ) = sSmiStmt;
        sBegined = ID_TRUE;
    }
    else
    {
        // Nothing to do.
    }
    
    sCursor.initialize();

    IDE_TEST( sCursor.open( sSmiStmt,
                            sINST->tableRef->tableHandle,
                            NULL,
                            sINST->tableRef->tableSCN,
                            NULL,
                            smiGetDefaultKeyRange(),
                            smiGetDefaultKeyRange(),
                            smiGetDefaultFilter(),
                            sCursorFlag,
                            SMI_INSERT_CURSOR,
                            & sProperty )
              != IDE_SUCCESS );
    sOpened = ID_TRUE;
    
    IDE_TEST( sCursor.insertRow( sSmiValues, &sRow, &sRid ) != IDE_SUCCESS );
    
    sOpened = ID_FALSE;
    IDE_TEST( sCursor.close() != IDE_SUCCESS );
    
    // BUG-43410 foreign key ����
    if ( sINST->parentConstraints != NULL )
    {
        IDE_TEST( aStatement->qmxMem->getStatus( &sQmxMemStatus )
                  != IDE_SUCCESS);
        
        IDE_TEST( qdnForeignKey::checkParentRef(
                      aStatement,
                      NULL,
                      sINST->parentConstraints,
                      SIMPLE_STMT_TUPLE(aStatement, sINST->tableRef->table),
                      sRow,
                      0 )
                  != IDE_SUCCESS);

        IDE_TEST( aStatement->qmxMem->setStatus( &sQmxMemStatus )
                  != IDE_SUCCESS);
    }
    else
    {
        // Nothing to do.
    }
    
    // statement close
    if ( sUseFastSmiStmt == ID_TRUE )
    {
        sBegined = ID_FALSE;
        IDE_TEST( sSmiStmt->end( SMI_STATEMENT_RESULT_SUCCESS ) != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }
    
    if ( aRowCount != NULL )
    {
        *aRowCount = 1;
    }
    else
    {
        // Nothing to do.
    }
    
    // BUG-38129
    qcg::setLastModifiedRowGRID( aStatement, sRid );
    
    return IDE_SUCCESS;
    
    IDE_EXCEPTION( ERR_INVALID_TYPE )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::executeFastInsert",
                                  "invalid bind type" ) );
    }
    IDE_EXCEPTION_END;

    if ( ideGetErrorCode() == mtERR_ABORT_INVALID_LENGTH )
    {
        if ( ( sValueInfo != NULL ) &&
             ( i < sINST->simpleValueCount ) )
        {
            sColumnOrder = sValueInfo->column.column.id & SMI_COLUMN_ID_MASK;
            IDE_CLEAR();
            IDE_SET( ideSetErrorCode( mtERR_ABORT_INVALID_LENGTH_COLUMN,
                                      sINST->tableRef->tableInfo->columns[sColumnOrder].name ) );
        }
        else
        {
            /* Nothing to do */
        }
    }
    /* BUG-45680 insert ����� not null column�� ���� �����޽��� ������ column ���� ���. */
    else if ( ideGetErrorCode() == qpERR_ABORT_QMX_NOT_NULL_CONSTRAINT )
    {
        if ( ( sValueInfo != NULL ) &&
             ( i < sINST->simpleValueCount ) )
        {
            sColumnOrder = sValueInfo->column.column.id & SMI_COLUMN_ID_MASK;
            IDE_CLEAR();
            IDE_SET( ideSetErrorCode( qpERR_ABORT_QMX_NOT_NULL_CONSTRAINT,
                                      " : ",
                                      sINST->tableRef->tableInfo->columns[sColumnOrder].name ) );
        }
        else
        {
            /* Nothing to do */
        }
    }
    else
    {
        /* Nothing to do */
    }

    if ( sOpened == ID_TRUE )
    {
        (void) sCursor.close();
    }
    else
    {
        // Nothing to do.
    }

    if ( sBegined == ID_TRUE )
    {
        (void) sSmiStmt->end( SMI_STATEMENT_RESULT_FAILURE );
    }
    else
    {
        // Nothing to do.
    }
    
    return IDE_FAILURE;
}

IDE_RC qmxSimple::executeFastUpdate( smiTrans     * aSmiTrans,
                                     qcStatement  * aStatement,
                                     UChar        * aBindBuffer,
                                     UInt         * aRowCount )
{
/***********************************************************************
 *
 *  Description : PROJ-2551 simple query ����ȭ
 *
 *  Implementation :
 *
 ***********************************************************************/

    idvSQL              * sStatistics = aStatement->mStatistics;
    qmncUPTE            * sUPTE = NULL;
    qmncSCAN            * sSCAN = NULL;
    qmncScanMethod      * sScanMethod = NULL;
    qcmIndex            * sIndex = NULL;
    UInt                  sTableType;
    smiValue              sSmiValues[QC_MAX_COLUMN_COUNT];
    SChar                 sCharBuffer[4096];
    SChar               * sBuffer = sCharBuffer;
    void                * sMtdValue[QC_MAX_KEY_COLUMN_COUNT * 2];
    mtdDateType           sSysdate;
    idBool                sUseSysdate = ID_FALSE;
    qmnValueInfo        * sValueInfo = NULL;
    void                * sValue = NULL;
    UInt                  i;

    idBool                sUseFastSmiStmt = ID_TRUE;
    smiStatement          sFastSmiStmt;
    smiStatement        * sSmiStmt = &sFastSmiStmt;
    smiStatement        * sSmiStmtOrg;
    smiStatement          sSmiStmtNew;
    UInt                  sSmiStmtFlag = 0;
    idBool                sBegined = ID_FALSE;
    idBool                sOpened = ID_FALSE;
    idBool                sRetryErr = ID_FALSE;
    UInt                  sStage = 0;

    idBool                sNeedCalculate = ID_FALSE;
    idBool                sIsNullRange = ID_FALSE;
    const void          * sIndexHandle = NULL;
    smiRange            * sKeyRange = NULL;
    smiRange              sRange;
    qtcMetaRangeColumn    sRangeColumn[QC_MAX_KEY_COLUMN_COUNT * 2];
    smiTableCursor        sCursor;
    UInt                  sTraverse;
    UInt                  sPrevious;
    UInt                  sInplaceUpdate;
    UInt                  sCursorFlag = 0;
    smiCursorProperties   sProperty;
    
    const void          * sRow = NULL;
    scGRID                sRid;
    void                * sUptRow = NULL;
    scGRID                sUptRid;
    UInt                  sCount = 0;
    UInt                  sColumnOrder;
    iduMemoryStatus       sQmxMemStatus;
    
    SC_MAKE_NULL_GRID( sUptRid );
    
    sUPTE = (qmncUPTE*)aStatement->myPlan->plan;
    sSCAN = (qmncSCAN*)aStatement->myPlan->plan->left;
    sScanMethod = &sSCAN->method;
    sIndex = sScanMethod->index;
    
    //fix BUG-17553
    IDV_SQL_SET( sStatistics, mMemoryTableAccessCount, 0 );

    sTableType = sUPTE->tableRef->tableFlag & SMI_TABLE_TYPE_MASK;
    
    // table lock
    if ( ( QCU_UPDATE_IN_PLACE == 1 ) &&
         ( sUPTE->inplaceUpdate == ID_TRUE ) &&
         ( ( sTableType == SMI_TABLE_MEMORY ) ||
           ( sTableType == SMI_TABLE_VOLATILE ) ) &&
         ( aStatement->mInplaceUpdateDisableFlag == ID_FALSE ) )
    {
        IDE_TEST( smiValidateAndLockObjects( aSmiTrans,
                                             sUPTE->tableRef->tableHandle,
                                             sUPTE->tableRef->tableSCN,
                                             SMI_TBSLV_DDL_DML,
                                             SMI_TABLE_LOCK_X,
                                             ID_ULONG_MAX,
                                             ID_FALSE )
                  != IDE_SUCCESS );
    }
    else
    {
        IDE_TEST( smiValidateAndLockObjects( aSmiTrans,
                                             sUPTE->tableRef->tableHandle,
                                             sUPTE->tableRef->tableSCN,
                                             SMI_TBSLV_DDL_DML,
                                             SMI_TABLE_LOCK_IX,
                                             ID_ULONG_MAX,
                                             ID_FALSE )
                  != IDE_SUCCESS );
    }
    
    /* PROJ-2359 Table/Partition Access Option */
    IDE_TEST( qmx::checkAccessOption( sUPTE->tableRef->tableInfo,
                                      ID_FALSE /* aIsInsertion */ )
              != IDE_SUCCESS );
    
    // bind buffer, canonize buffer �Ҵ�
    if ( sSCAN->simpleValueBufSize +
         sUPTE->simpleValueBufSize > ID_SIZEOF(sCharBuffer) )
    {
        IDE_TEST( aStatement->qmxMem->alloc(
                      sSCAN->simpleValueBufSize + sUPTE->simpleValueBufSize,
                      (void**)&sBuffer )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    // set assignment
    for ( i = 0; i < sUPTE->updateColumnCount; i++ )
    {
        sValueInfo = &(sUPTE->simpleValues[i]);

        if ( sValueInfo->op == QMN_VALUE_OP_ASSIGN )
        {
            switch ( sValueInfo->type )
            {
                case QMN_VALUE_TYPE_CONST_VALUE:
                    IDE_TEST( getSimpleConstMtdValue(
                                  aStatement,
                                  & sValueInfo->column,
                                  & sValue,
                                  sValueInfo->value.constVal,
                                  & sBuffer,
                                  ID_TRUE,
                                  sValueInfo->isQueue,
                                  NULL )
                              != IDE_SUCCESS ); 
                    break;

                case QMN_VALUE_TYPE_HOST_VALUE:
                    if ( aBindBuffer != NULL )
                    {
                        IDE_TEST( getSimpleCValue(
                                      sValueInfo,
                                      & sValue,
                                      aStatement->pBindParam,
                                      aBindBuffer,
                                      & sBuffer,
                                      ID_TRUE )  // canonize�� �ʿ���
                                  != IDE_SUCCESS );
                    }
                    else
                    {
                        IDE_TEST( getSimpleMtdValue(
                                      sValueInfo,
                                      & sValue,
                                      aStatement->pBindParam,
                                      & sBuffer,
                                      ID_TRUE )
                                  != IDE_SUCCESS );
                    }
                    break;

                case QMN_VALUE_TYPE_SYSDATE:
                    if ( sUseSysdate == ID_FALSE )
                    {
                        /* BUG-45373 SYSDATE_FOR_NATC Bug */
                        if ( QCU_DISPLAY_PLAN_FOR_NATC == 0 )
                        {
                            IDE_TEST( qtc::sysdate( &sSysdate ) != IDE_SUCCESS );
                        }
                        else
                        {
                            if ( QCU_SYSDATE_FOR_NATC[0] != '\0' )
                            {
                                if ( mtdDateInterface::toDate( &sSysdate,
                                                               (UChar*)QCU_SYSDATE_FOR_NATC,
                                                               idlOS::strlen(QCU_SYSDATE_FOR_NATC),
                                                               (UChar *)QCG_GET_SESSION_DATE_FORMAT(aStatement),
                                                               idlOS::strlen(QCG_GET_SESSION_DATE_FORMAT(aStatement)) )

                                          != IDE_SUCCESS )
                                {
                                    IDE_TEST( qtc::sysdate( &sSysdate ) != IDE_SUCCESS );
                                }
                                else
                                {
                                    /* Nothing to do */
                                }
                            }
                            else
                            {
                                IDE_TEST( qtc::sysdate( &sSysdate ) != IDE_SUCCESS );
                            }
                        }
                        sUseSysdate = ID_TRUE;
                    }
                    else
                    {
                        // Nothing to do.
                    }
                    sValue = & sSysdate;
                    break;
                
                default:
                    IDE_RAISE( ERR_INVALID_TYPE );
                    break;
            }

            // check not null
            IDE_TEST( checkSimpleNullValue( & sValueInfo->column,
                                            sValue )
                      != IDE_SUCCESS );
        
            // set smiValue
            IDE_TEST( setSimpleSmiValue( & sValueInfo->column,
                                         sValue,
                                         &(sSmiValues[i]) )
                      != IDE_SUCCESS );
        }
        else
        {
            sNeedCalculate = ID_TRUE;
        }
    }

    // scan
    if ( sSCAN->simpleRid == ID_TRUE )
    {
        // make simple rid range
        IDE_TEST( makeSimpleRidRange( aStatement,
                                      sSCAN,
                                      aBindBuffer,
                                      NULL,
                                      0,
                                      sMtdValue,
                                      sRangeColumn,
                                      & sRange,
                                      & sKeyRange,
                                      & sIsNullRange,
                                      & sBuffer )
                  != IDE_SUCCESS );
        
        // null range�� ����� ����.
        IDE_TEST_CONT( sIsNullRange == ID_TRUE, normal_exit );
        
        sIndexHandle = NULL;
        
        SMI_CURSOR_PROP_INIT_FOR_INDEX_SCAN( &sProperty,
                                             sStatistics,
                                             SMI_BUILTIN_GRID_INDEXTYPE_ID );
    }
    else
    {
        if ( sIndex != NULL )
        {
            // make simple key range
            IDE_TEST( makeSimpleKeyRange( aStatement,
                                          sSCAN,
                                          sIndex,
                                          aBindBuffer,
                                          NULL,
                                          0,
                                          sMtdValue,
                                          sRangeColumn,
                                          & sRange,
                                          & sKeyRange,
                                          & sIsNullRange,
                                          & sBuffer )
                      != IDE_SUCCESS );
        
            // null range�� ����� ����.
            IDE_TEST_CONT( sIsNullRange == ID_TRUE, normal_exit );
       
            sIndexHandle = sIndex->indexHandle;
        
            SMI_CURSOR_PROP_INIT_FOR_INDEX_SCAN( &sProperty,
                                                 sStatistics,
                                                 sIndex->indexTypeId );
        }
        else
        {
            sIndexHandle = NULL;
            sKeyRange = smiGetDefaultKeyRange();

            SMI_CURSOR_PROP_INIT_FOR_FULL_SCAN( &sProperty, sStatistics );
        }
    }
    
    if ( sSCAN->limit != NULL )
    {
        sProperty.mFirstReadRecordPos = sSCAN->limit->start.constant - 1;
        sProperty.mReadRecordCount = sSCAN->limit->count.constant;
    }
    else
    {
        // Nothing to do.
    }
    
    // use fast smiStmt
    if ( QC_SMI_STMT( aStatement ) != NULL )
    {
        sSmiStmt = QC_SMI_STMT( aStatement );
        
        sUseFastSmiStmt = ID_FALSE;
    }
    else
    {
        // Nothing to do.
    }
    
    sSmiStmtFlag &= ~SMI_STATEMENT_MASK;
    sSmiStmtFlag |= SMI_STATEMENT_NORMAL;

    sSmiStmtFlag &= ~SMI_STATEMENT_CURSOR_MASK;
    sSmiStmtFlag |= SMI_STATEMENT_MEMORY_CURSOR;

    // Traverse ������ ����
    if ( ( sSCAN->flag & QMNC_SCAN_TRAVERSE_MASK )
         == QMNC_SCAN_TRAVERSE_FORWARD )
    {
        sTraverse = SMI_TRAVERSE_FORWARD;
    }
    else
    {
        sTraverse = SMI_TRAVERSE_BACKWARD;
    }

    // Previous ��� ���� ����
    if ( ( sSCAN->flag & QMNC_SCAN_PREVIOUS_ENABLE_MASK )
         == QMNC_SCAN_PREVIOUS_ENABLE_TRUE )
    {
        sPrevious = SMI_PREVIOUS_ENABLE;
    }
    else
    {
        sPrevious = SMI_PREVIOUS_DISABLE;
    }

    if ( ( sUPTE->inplaceUpdate == ID_TRUE ) &&
         ( aStatement->mInplaceUpdateDisableFlag == ID_FALSE ) )
    {
        sInplaceUpdate = SMI_INPLACE_UPDATE_ENABLE;
    }
    else
    {
        sInplaceUpdate = SMI_INPLACE_UPDATE_DISABLE;
    }
    
    sCursorFlag = SMI_LOCK_WRITE | sTraverse | sPrevious | sInplaceUpdate;
    
  retry:
    
    sBegined = ID_FALSE;
    sOpened = ID_FALSE;
    sRetryErr = ID_FALSE;
    
    // statement begin
    if ( sUseFastSmiStmt == ID_TRUE )
    {
        IDE_TEST( sSmiStmt->begin( sStatistics,
                                   aSmiTrans->getStatement(),
                                   sSmiStmtFlag )
                  != IDE_SUCCESS );

        QC_SMI_STMT( aStatement ) = sSmiStmt;
        sBegined = ID_TRUE;
    }
    else
    {
        // Nothing to do.
    }
    
    sCursor.initialize();

    IDE_TEST( sCursor.open( sSmiStmt,
                            sSCAN->table,
                            sIndexHandle,
                            sSCAN->tableSCN,
                            sUPTE->updateColumnList,
                            sKeyRange,
                            smiGetDefaultKeyRange(),
                            smiGetDefaultFilter(),
                            sCursorFlag,
                            SMI_UPDATE_CURSOR,
                            & sProperty )
              != IDE_SUCCESS );
    sOpened = ID_TRUE;
    
    IDE_TEST( sCursor.beforeFirst() != IDE_SUCCESS );
    
    IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT )
              != IDE_SUCCESS );

    if ( sSCAN->simpleUnique == ID_TRUE )
    {
        if ( sRow != NULL )
        {
            if ( sNeedCalculate == ID_TRUE )
            {
                IDE_TEST( calculateSimpleValues( aStatement,
                                                 sUPTE,
                                                 aBindBuffer,
                                                 sRow,
                                                 sSmiValues,
                                                 sBuffer )
                          != IDE_SUCCESS );
            }
            else
            {
                // Nothing to do.
            }
            
            IDE_TEST( sCursor.updateRow( sSmiValues,
                                         NULL,
                                         & sUptRow,
                                         & sUptRid )
                      != IDE_SUCCESS );
            
            sCount = 1;
        }
        else
        {
            // Nothing to do.
        }
    }
    else
    {
        while ( sRow != NULL )
        {
            if ( sNeedCalculate == ID_TRUE )
            {
                IDE_TEST( calculateSimpleValues( aStatement,
                                                 sUPTE,
                                                 aBindBuffer,
                                                 sRow,
                                                 sSmiValues,
                                                 sBuffer )
                          != IDE_SUCCESS );
            }
            else
            {
                // Nothing to do.
            }
            
            IDE_TEST( sCursor.updateRow( sSmiValues,
                                         NULL,
                                         & sUptRow,
                                         & sUptRid )
                      != IDE_SUCCESS );
            
            sCount++;
        
            IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT )
                      != IDE_SUCCESS );
        }
    }

    sOpened = ID_FALSE;
    IDE_TEST( sCursor.close() != IDE_SUCCESS );
    
    // BUG-43410 foreign key ����
    if ( ( sUPTE->parentConstraints != NULL ) && ( sCount > 0 ) )
    {
        if ( ( sSCAN->simpleUnique == ID_TRUE ) && ( sUptRow != NULL ) )
        {
            IDE_TEST( aStatement->qmxMem->getStatus( &sQmxMemStatus )
                      != IDE_SUCCESS );
                
            IDE_TEST( qdnForeignKey::checkParentRef(
                          aStatement,
                          sUPTE->updateColumnIDs,
                          sUPTE->parentConstraints,
                          SIMPLE_STMT_TUPLE(aStatement, sUPTE->tableRef->table),
                          sUptRow,
                          sUPTE->updateColumnCount )
                      != IDE_SUCCESS );
                
            IDE_TEST( aStatement->qmxMem->setStatus( &sQmxMemStatus )
                      != IDE_SUCCESS );
        }
        else
        {
            IDE_TEST( sCursor.beforeFirstModified( SMI_FIND_MODIFIED_NEW )
                      != IDE_SUCCESS );

            IDE_TEST( sCursor.readNewRow( &sRow, &sRid )
                      != IDE_SUCCESS );

            while ( sRow != NULL )
            {
                IDE_TEST( aStatement->qmxMem->getStatus( &sQmxMemStatus )
                          != IDE_SUCCESS );
                
                IDE_TEST( qdnForeignKey::checkParentRef(
                              aStatement,
                              sUPTE->updateColumnIDs,
                              sUPTE->parentConstraints,
                              SIMPLE_STMT_TUPLE(aStatement, sUPTE->tableRef->table),
                              sRow,
                              sUPTE->updateColumnCount )
                          != IDE_SUCCESS );
                
                IDE_TEST( aStatement->qmxMem->setStatus( &sQmxMemStatus )
                          != IDE_SUCCESS );
                
                IDE_TEST( sCursor.readNewRow( &sRow, &sRid )
                          != IDE_SUCCESS );
            }
        }
    }
    else
    {
        // Nothing to do.
    }

    // BUG-43410 foreign key ����
    if ( ( sUPTE->childConstraints != NULL ) && ( sCount > 0 ) )
    {
        // BUG-17940 parent key�� �����ϰ� child key�� ã����
        // parent row�� lock�� ���� ���� view�� ��������
        // ���ο� smiStmt�� �̿��Ѵ�.
        // Update cascade �ɼǿ� ����ؼ� normal�� �Ѵ�.
        sSmiStmtOrg = QC_SMI_STMT( aStatement );
        IDE_TEST( sSmiStmtNew.begin( sStatistics,
                                     aSmiTrans->getStatement(),
                                     SMI_STATEMENT_NORMAL |
                                     SMI_STATEMENT_SELF_TRUE |
                                     SMI_STATEMENT_MEMORY_CURSOR )
                  != IDE_SUCCESS );
        QC_SMI_STMT( aStatement ) = &sSmiStmtNew;
        sStage = 1;

        if ( ( sSCAN->simpleUnique == ID_TRUE ) && ( sRow != NULL ) )
        {
            IDE_TEST( aStatement->qmxMem->getStatus( &sQmxMemStatus )
                      != IDE_SUCCESS );
        
            IDE_TEST( qdnForeignKey::checkChildRefOnUpdate(
                          aStatement,
                          sUPTE->tableRef,
                          sUPTE->tableRef->tableInfo,
                          sUPTE->updateColumnIDs,
                          sUPTE->childConstraints,
                          sUPTE->tableRef->tableInfo->tableID,
                          SIMPLE_STMT_TUPLE(aStatement, sUPTE->tableRef->table),
                          sRow,
                          sUPTE->updateColumnCount )
                      != IDE_SUCCESS );

            IDE_TEST( aStatement->qmxMem->setStatus( &sQmxMemStatus )
                      != IDE_SUCCESS );
        }
        else
        {
            IDE_TEST( sCursor.beforeFirstModified( SMI_FIND_MODIFIED_OLD )
                      != IDE_SUCCESS );
            
            IDE_TEST( sCursor.readOldRow( &sRow, &sRid )
                      != IDE_SUCCESS );

            while ( sRow != NULL )
            {
                IDE_TEST( aStatement->qmxMem->getStatus( &sQmxMemStatus )
                          != IDE_SUCCESS );
        
                IDE_TEST( qdnForeignKey::checkChildRefOnUpdate(
                              aStatement,
                              sUPTE->tableRef,
                              sUPTE->tableRef->tableInfo,
                              sUPTE->updateColumnIDs,
                              sUPTE->childConstraints,
                              sUPTE->tableRef->tableInfo->tableID,
                              SIMPLE_STMT_TUPLE(aStatement, sUPTE->tableRef->table),
                              sRow,
                              sUPTE->updateColumnCount )
                          != IDE_SUCCESS );

                IDE_TEST( aStatement->qmxMem->setStatus( &sQmxMemStatus )
                          != IDE_SUCCESS );
                
                IDE_TEST( sCursor.readOldRow( &sRow, &sRid )
                          != IDE_SUCCESS );
            }
        }

        sStage = 0;
        QC_SMI_STMT( aStatement ) = sSmiStmtOrg;
        IDE_TEST( sSmiStmtNew.end(SMI_STATEMENT_RESULT_SUCCESS) != IDE_SUCCESS);
    }
    else
    {
        // Nothing to do.
    }
    
    // statement close
    if ( sUseFastSmiStmt == ID_TRUE )
    {
        sBegined = ID_FALSE;
        IDE_TEST( sSmiStmt->end( SMI_STATEMENT_RESULT_SUCCESS ) != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    if ( ( sTableType != SMI_TABLE_FIXED ) &&
         ( sCount > 0 ) )
    {
        IDV_SQL_ADD( sStatistics,
                     mMemoryTableAccessCount,
                     sCount );
            
        IDV_SESS_ADD( sStatistics->mSess,
                      IDV_STAT_INDEX_MEMORY_TABLE_ACCESS_COUNT,
                      sCount );
    }
    else
    {
        // Nothing to do.
    }

    IDE_EXCEPTION_CONT( normal_exit );
    
    if ( aRowCount != NULL )
    {
        *aRowCount = sCount;
    }
    else
    {
        // Nothing to do.
    }

    // BUG-38129
    qcg::setLastModifiedRowGRID( aStatement, sUptRid );
    
    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_INVALID_TYPE )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::executeFastUpdate",
                                  "invalid bind type" ) );
    }
    IDE_EXCEPTION_END;

    if ( ideGetErrorCode() == mtERR_ABORT_INVALID_LENGTH )
    {
        if ( ( sValueInfo != NULL ) &&
             ( i < sUPTE->updateColumnCount ) )
        {
            sColumnOrder = sValueInfo->column.column.id & SMI_COLUMN_ID_MASK;
            IDE_CLEAR();
            IDE_SET( ideSetErrorCode( mtERR_ABORT_INVALID_LENGTH_COLUMN,
                                      sUPTE->tableRef->tableInfo->columns[sColumnOrder].name ) );
        }
        else
        {
            /* Nothing to do */
        }
    }
    /* BUG-45680 insert ����� not null column�� ���� �����޽��� ������ column ���� ���. */
    else if ( ideGetErrorCode() == qpERR_ABORT_QMX_NOT_NULL_CONSTRAINT )
    {
        if ( ( sValueInfo != NULL ) &&
             ( i < sUPTE->updateColumnCount ) )
        {
            sColumnOrder = sValueInfo->column.column.id & SMI_COLUMN_ID_MASK;
            IDE_CLEAR();
            IDE_SET( ideSetErrorCode( qpERR_ABORT_QMX_NOT_NULL_CONSTRAINT,
                                      " : ",
                                      sUPTE->tableRef->tableInfo->columns[sColumnOrder].name ) );
        }
        else
        {
            /* Nothing to do */
        }
    }
    else
    {
        /* Nothing to do */
    }
    if ( sOpened == ID_TRUE )
    {
        // BUG-40126 retry error
        if ( ( ideGetErrorCode() & E_ACTION_MASK ) == E_ACTION_RETRY )
        {
            sRetryErr = ID_TRUE;
        }
        else
        {
            // Nothing to do.
        }
    
        (void) sCursor.close();
    }
    else
    {
        // Nothing to do.
    }

    if ( sBegined == ID_TRUE )
    {
        (void) sSmiStmt->end( SMI_STATEMENT_RESULT_FAILURE );
    }
    else
    {
        // Nothing to do.
    }

    if ( sStage == 1 )
    {
        QC_SMI_STMT( aStatement ) = sSmiStmtOrg;

        if ( sSmiStmtNew.end(SMI_STATEMENT_RESULT_FAILURE) != IDE_SUCCESS )
        {
            IDE_CALLBACK_FATAL("Check Child Key On Update smiStmt.end() failed in simple");
        }
        else
        {
            /* Nothing to do */
        }
    }
    else
    {
        /* Nothing to do */
    }
    if ( ( sRetryErr == ID_TRUE ) &&
         ( sUseFastSmiStmt == ID_TRUE ) )
    {
        goto retry;
    }
    else
    {
        // Nothing to do.
    }
    
    return IDE_FAILURE;
}

IDE_RC qmxSimple::executeFastDelete( smiTrans     * aSmiTrans,
                                     qcStatement  * aStatement,
                                     UChar        * aBindBuffer,
                                     UInt         * aRowCount )
{
/***********************************************************************
 *
 *  Description : PROJ-2551 simple query ����ȭ
 *
 *  Implementation :
 *
 ***********************************************************************/

    idvSQL              * sStatistics = aStatement->mStatistics;
    qmncDETE            * sDETE = NULL;
    qmncSCAN            * sSCAN = NULL;
    qmncScanMethod      * sScanMethod = NULL;
    qcmIndex            * sIndex = NULL;
    SChar                 sCharBuffer[4096];
    SChar               * sBuffer = sCharBuffer;
    void                * sMtdValue[QC_MAX_KEY_COLUMN_COUNT * 2];

    idBool                sUseFastSmiStmt = ID_TRUE;
    smiStatement          sFastSmiStmt;
    smiStatement        * sSmiStmt = &sFastSmiStmt;
    UInt                  sSmiStmtFlag = 0;
    smiStatement        * sSmiStmtOrg;
    smiStatement          sSmiStmtNew;
    idBool                sBegined = ID_FALSE;
    idBool                sOpened = ID_FALSE;
    idBool                sRetryErr = ID_FALSE;
    UInt                  sStage = 0;

    idBool                sIsNullRange = ID_FALSE;
    const void          * sIndexHandle = NULL;
    smiRange            * sKeyRange = NULL;
    smiRange              sRange;
    qtcMetaRangeColumn    sRangeColumn[QC_MAX_KEY_COLUMN_COUNT * 2];
    smiTableCursor        sCursor;
    UInt                  sTraverse;
    UInt                  sPrevious;
    UInt                  sCursorFlag = 0;
    smiCursorProperties   sProperty;
    
    const void          * sRow = NULL;
    scGRID                sRid;
    UInt                  sCount = 0;

    iduMemoryStatus       sQmxMemStatus;
    
    sDETE = (qmncDETE*)aStatement->myPlan->plan;
    sSCAN = (qmncSCAN*)aStatement->myPlan->plan->left;
    sScanMethod = &sSCAN->method;
    sIndex = sScanMethod->index;

    //fix BUG-17553
    IDV_SQL_SET( sStatistics, mMemoryTableAccessCount, 0 );

    // table lock
    IDE_TEST( smiValidateAndLockObjects( aSmiTrans,
                                         sDETE->tableRef->tableHandle,
                                         sDETE->tableRef->tableSCN,
                                         SMI_TBSLV_DDL_DML,
                                         SMI_TABLE_LOCK_IX,
                                         ID_ULONG_MAX,
                                         ID_FALSE )
              != IDE_SUCCESS );
    
    /* PROJ-2359 Table/Partition Access Option */
    IDE_TEST( qmx::checkAccessOption( sDETE->tableRef->tableInfo,
                                      ID_FALSE /* aIsInsertion */ )
              != IDE_SUCCESS );
    
    // bind buffer �Ҵ�
    if ( sSCAN->simpleValueBufSize > ID_SIZEOF(sCharBuffer) )
    {
        IDE_TEST( aStatement->qmxMem->alloc(
                      sSCAN->simpleValueBufSize,
                      (void**)&sBuffer )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    // scan
    if ( sSCAN->simpleRid == ID_TRUE )
    {
        // make simple rid range
        IDE_TEST( makeSimpleRidRange( aStatement,
                                      sSCAN,
                                      aBindBuffer,
                                      NULL,
                                      0,
                                      sMtdValue,
                                      sRangeColumn,
                                      & sRange,
                                      & sKeyRange,
                                      & sIsNullRange,
                                      & sBuffer )
                  != IDE_SUCCESS );
        
        // null range�� ����� ����.
        IDE_TEST_CONT( sIsNullRange == ID_TRUE, normal_exit );
        
        sIndexHandle = NULL;
        
        SMI_CURSOR_PROP_INIT_FOR_INDEX_SCAN( &sProperty,
                                             sStatistics,
                                             SMI_BUILTIN_GRID_INDEXTYPE_ID );
    }
    else
    {
        if ( sIndex != NULL )
        {
            // make simple key range
            IDE_TEST( makeSimpleKeyRange( aStatement,
                                          sSCAN,
                                          sIndex,
                                          aBindBuffer,
                                          NULL,
                                          0,
                                          sMtdValue,
                                          sRangeColumn,
                                          & sRange,
                                          & sKeyRange,
                                          & sIsNullRange,
                                          & sBuffer )
                      != IDE_SUCCESS );
        
            // null range�� ����� ����.
            IDE_TEST_CONT( sIsNullRange == ID_TRUE, normal_exit );
        
            sIndexHandle = sIndex->indexHandle;
        
            SMI_CURSOR_PROP_INIT_FOR_INDEX_SCAN( &sProperty,
                                                 sStatistics,
                                                 sIndex->indexTypeId );
        }
        else
        {
            sIndexHandle = NULL;
            sKeyRange = smiGetDefaultKeyRange();

            SMI_CURSOR_PROP_INIT_FOR_FULL_SCAN( &sProperty, sStatistics );
        }
    }
    
    if ( sSCAN->limit != NULL )
    {
        sProperty.mFirstReadRecordPos = sSCAN->limit->start.constant - 1;
        sProperty.mReadRecordCount = sSCAN->limit->count.constant;
    }
    else
    {
        // Nothing to do.
    }
    
    // use fast smiStmt
    if ( QC_SMI_STMT( aStatement ) != NULL )
    {
        sSmiStmt = QC_SMI_STMT( aStatement );
        
        sUseFastSmiStmt = ID_FALSE;
    }
    else
    {
        // Nothing to do.
    }
    
    sSmiStmtFlag &= ~SMI_STATEMENT_MASK;
    sSmiStmtFlag |= SMI_STATEMENT_NORMAL;

    sSmiStmtFlag &= ~SMI_STATEMENT_CURSOR_MASK;
    sSmiStmtFlag |= SMI_STATEMENT_MEMORY_CURSOR;

    // Traverse ������ ����
    if ( ( sSCAN->flag & QMNC_SCAN_TRAVERSE_MASK )
         == QMNC_SCAN_TRAVERSE_FORWARD )
    {
        sTraverse = SMI_TRAVERSE_FORWARD;
    }
    else
    {
        sTraverse = SMI_TRAVERSE_BACKWARD;
    }

    // Previous ��� ���� ����
    if ( ( sSCAN->flag & QMNC_SCAN_PREVIOUS_ENABLE_MASK )
         == QMNC_SCAN_PREVIOUS_ENABLE_TRUE )
    {
        sPrevious = SMI_PREVIOUS_ENABLE;
    }
    else
    {
        sPrevious = SMI_PREVIOUS_DISABLE;
    }
    
    sCursorFlag = SMI_LOCK_WRITE | sTraverse | sPrevious;
    
  retry:
    
    sBegined = ID_FALSE;
    sOpened = ID_FALSE;
    sRetryErr = ID_FALSE;
    
    // statement begin
    if ( sUseFastSmiStmt == ID_TRUE )
    {
        IDE_TEST( sSmiStmt->begin( sStatistics,
                                   aSmiTrans->getStatement(),
                                   sSmiStmtFlag )
                  != IDE_SUCCESS );

        QC_SMI_STMT( aStatement ) = sSmiStmt;
        sBegined = ID_TRUE;
    }
    else
    {
        // Nothing to do.
    }
    
    sCursor.initialize();

    IDE_TEST( sCursor.open( sSmiStmt,
                            sSCAN->table,
                            sIndexHandle,
                            sSCAN->tableSCN,
                            NULL,
                            sKeyRange,
                            smiGetDefaultKeyRange(),
                            smiGetDefaultFilter(),
                            sCursorFlag,
                            SMI_UPDATE_CURSOR,
                            & sProperty )
              != IDE_SUCCESS );
    sOpened = ID_TRUE;
    
    IDE_TEST( sCursor.beforeFirst() != IDE_SUCCESS );
    
    IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT )
              != IDE_SUCCESS );

    if ( sSCAN->simpleUnique == ID_TRUE )
    {
        if ( sRow != NULL )
        {
            IDE_TEST( sCursor.deleteRow() != IDE_SUCCESS );
            
            sCount = 1;
        }
        else
        {
            // Nothing to do.
        }
    }
    else
    {
        while ( sRow != NULL )
        {
            IDE_TEST( sCursor.deleteRow() != IDE_SUCCESS );
            
            sCount++;
        
            IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT )
                      != IDE_SUCCESS );
        }
    }

    sOpened = ID_FALSE;
    IDE_TEST( sCursor.close() != IDE_SUCCESS );
    
    // BUG-43410 foreign key ����
    if ( ( sDETE->childConstraints != NULL ) && ( sCount > 0 ) )
    {
        // BUG-17940 parent key�� �����ϰ� child key�� ã����
        // parent row�� lock�� ���� ���� view�� ��������
        // ���ο� smiStmt�� �̿��Ѵ�.
        // Update cascade �ɼǿ� ����ؼ� normal�� �Ѵ�.
        sSmiStmtOrg = QC_SMI_STMT( aStatement );
        IDE_TEST( sSmiStmtNew.begin( sStatistics,
                                     aSmiTrans->getStatement(),
                                     SMI_STATEMENT_NORMAL |
                                     SMI_STATEMENT_SELF_TRUE |
                                     SMI_STATEMENT_MEMORY_CURSOR )
                  != IDE_SUCCESS );
        QC_SMI_STMT( aStatement ) = &sSmiStmtNew;
        sStage = 1;

        if ( ( sSCAN->simpleUnique == ID_TRUE ) && ( sRow != NULL ) )
        {
            IDE_TEST( aStatement->qmxMem->getStatus( &sQmxMemStatus )
                      != IDE_SUCCESS);
        
            IDE_TEST( qdnForeignKey::checkChildRefOnDelete(
                          aStatement,
                          sDETE->childConstraints,
                          sDETE->tableRef->tableInfo->tableID,
                          SIMPLE_STMT_TUPLE(aStatement, sDETE->tableRef->table),
                          sRow,
                          ID_TRUE )
                      != IDE_SUCCESS );

            IDE_TEST( aStatement->qmxMem->setStatus( &sQmxMemStatus )
                      != IDE_SUCCESS);
        }
        else
        {
            IDE_TEST( sCursor.beforeFirstModified( SMI_FIND_MODIFIED_OLD )
                      != IDE_SUCCESS );

            IDE_TEST( sCursor.readOldRow( &sRow, &sRid )
                      != IDE_SUCCESS );

            while ( sRow != NULL )
            {
                IDE_TEST( aStatement->qmxMem->getStatus( &sQmxMemStatus )
                          != IDE_SUCCESS);
        
                IDE_TEST( qdnForeignKey::checkChildRefOnDelete(
                              aStatement,
                              sDETE->childConstraints,
                              sDETE->tableRef->tableInfo->tableID,
                              SIMPLE_STMT_TUPLE(aStatement, sDETE->tableRef->table),
                              sRow,
                              ID_TRUE )
                          != IDE_SUCCESS );

                IDE_TEST( aStatement->qmxMem->setStatus( &sQmxMemStatus )
                          != IDE_SUCCESS);
                
                IDE_TEST( sCursor.readOldRow( &sRow, &sRid )
                          != IDE_SUCCESS );
            }
        }
        sStage = 0;
        QC_SMI_STMT( aStatement ) = sSmiStmtOrg;
        IDE_TEST( sSmiStmtNew.end(SMI_STATEMENT_RESULT_SUCCESS) != IDE_SUCCESS);
    }
    else
    {
        // Nothing to do.
    }

    // statement close
    if ( sUseFastSmiStmt == ID_TRUE )
    {
        sBegined = ID_FALSE;
        IDE_TEST( sSmiStmt->end( SMI_STATEMENT_RESULT_SUCCESS ) != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    if ( ( ( sDETE->tableRef->tableFlag & SMI_TABLE_TYPE_MASK )
           != SMI_TABLE_FIXED ) &&
         ( sCount > 0 ) )
    {
        IDV_SQL_ADD( sStatistics,
                     mMemoryTableAccessCount,
                     sCount );
            
        IDV_SESS_ADD( sStatistics->mSess,
                      IDV_STAT_INDEX_MEMORY_TABLE_ACCESS_COUNT,
                      sCount );
    }
    else
    {
        // Nothing to do.
    }

    IDE_EXCEPTION_CONT( normal_exit );
    
    if ( aRowCount != NULL )
    {
        *aRowCount = sCount;
    }
    else
    {
        // Nothing to do.
    }
    
    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if ( sOpened == ID_TRUE )
    {
        // BUG-40126 retry error
        if ( ( ideGetErrorCode() & E_ACTION_MASK ) == E_ACTION_RETRY )
        {
            sRetryErr = ID_TRUE;
        }
        else
        {
            // Nothing to do.
        }
    
        (void) sCursor.close();
    }
    else
    {
        // Nothing to do.
    }

    if ( sBegined == ID_TRUE )
    {
        (void) sSmiStmt->end( SMI_STATEMENT_RESULT_FAILURE );
    }
    else
    {
        // Nothing to do.
    }

    if ( sStage == 1 )
    {
        QC_SMI_STMT( aStatement ) = sSmiStmtOrg;

        if ( sSmiStmtNew.end(SMI_STATEMENT_RESULT_FAILURE) != IDE_SUCCESS )
        {
            IDE_CALLBACK_FATAL("Check Child Key On Delete smiStmt.end() failed in simple");
        }
        else
        {
            /* Nothing to do */
        }
    }
    else
    {
        /* Nothing to do */
    }
    if ( ( sRetryErr == ID_TRUE ) &&
         ( sUseFastSmiStmt == ID_TRUE ) )
    {
        goto retry;
    }
    else
    {
        // Nothing to do.
    }
    
    return IDE_FAILURE;
}

IDE_RC qmxSimple::fastExecute( smiTrans     * aSmiTrans,
                               qcStatement  * aStatement,
                               UShort       * aBindColInfo,
                               UChar        * aBindBuffer,
                               UInt           aShmSize,
                               UChar        * aShmResult,
                               UInt         * aResultSize,
                               UInt         * aRowCount )
{
    qciStmtType  sStmtKind;
    qmnPlan    * sPlan = NULL;

    // PROJ-2551 simple query ����ȭ
    // simple query�̾�� �Ѵ�.
    IDE_TEST_RAISE( ( aStatement->mFlag & QC_STMT_FAST_EXEC_MASK )
                    == QC_STMT_FAST_EXEC_FALSE,
                    ERR_INVALID_FAST_CALL );
    
    sStmtKind = aStatement->myPlan->parseTree->stmtKind;
    sPlan = aStatement->myPlan->plan;
    
    switch ( sStmtKind )
    {
        case QCI_STMT_SELECT:
        case QCI_STMT_SELECT_FOR_UPDATE:
        case QCI_STMT_DEQUEUE:            
            if ( sPlan->left->type == QMN_SCAN )
            {
                IDE_TEST( executeFastSelect( aSmiTrans,
                                             aStatement,
                                             aBindColInfo,
                                             aBindBuffer,
                                             aShmSize,
                                             aShmResult,
                                             aResultSize,
                                             aRowCount )
                          != IDE_SUCCESS );
            }
            else
            {
                IDE_TEST( executeFastJoinSelect( aSmiTrans,
                                                 aStatement,
                                                 aBindColInfo,
                                                 aBindBuffer,
                                                 aShmSize,
                                                 aShmResult,
                                                 aResultSize,
                                                 aRowCount )
                          != IDE_SUCCESS );
            }

            aStatement->simpleInfo.numRows = 0;
            break;

        case QCI_STMT_INSERT:
        case QCI_STMT_ENQUEUE:
            IDE_TEST( executeFastInsert( aSmiTrans,
                                         aStatement,
                                         aBindBuffer,
                                         aRowCount )
                      != IDE_SUCCESS );

            aStatement->simpleInfo.numRows = *aRowCount;
            break;
                
        case QCI_STMT_UPDATE:
            IDE_TEST( executeFastUpdate( aSmiTrans,
                                         aStatement,
                                         aBindBuffer,
                                         aRowCount )
                      != IDE_SUCCESS );

            aStatement->simpleInfo.numRows = *aRowCount;
            break;
                
        case QCI_STMT_DELETE:
            IDE_TEST( executeFastDelete( aSmiTrans,
                                         aStatement,
                                         aBindBuffer,
                                         aRowCount )
                      != IDE_SUCCESS );

            aStatement->simpleInfo.numRows = *aRowCount;
            break;
                
        default:
            IDE_RAISE( ERR_INVALID_STMT_TYPE );
            break;
    }
    
    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_INVALID_FAST_CALL )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::executeFast",
                                  "invalid fast call" ) );
    }
    IDE_EXCEPTION( ERR_INVALID_STMT_TYPE )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmxSimple::executeFast",
                                  "unsupported statement type" ) );
    }
    IDE_EXCEPTION_END;

    aStatement->simpleInfo.numRows = 0;

    return IDE_FAILURE;
}
