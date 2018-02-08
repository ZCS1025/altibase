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

# include <idl.h>
# include <ide.h>
# include <idu.h>
# include <idp.h>
# include <smErrorCode.h>
# include <smuProperty.h>

# include <smxTransMgr.h>
# include <smxMinSCNBuild.h>

smxMinSCNBuild::smxMinSCNBuild() : idtBaseThread()
{

}

smxMinSCNBuild::~smxMinSCNBuild()
{

}

/**********************************************************************
 *
 * Description : Thread�� �ʱ�ȭ�Ѵ�.
 *
 **********************************************************************/
IDE_RC smxMinSCNBuild::initialize()
{
    mFinish = ID_FALSE;
    mResume = ID_FALSE;

    SM_INIT_SCN( &mSysMinDskViewSCN );

    // BUG-24885 wrong delayed stamping
    SM_INIT_SCN( &mSysMinDskFstViewSCN );

    // BUG-26881 �߸��� CTS stamping���� acces�� �� ���� row�� ������
    SM_INIT_SCN( &mSysMinOldestFstViewSCN );

    IDE_TEST( mMutex.initialize((SChar*)"TRANSACTION_MINVIEWSCN_BUILDER",
                                IDU_MUTEX_KIND_POSIX,
                                IDV_WAIT_INDEX_NULL )
              != IDE_SUCCESS );

    IDE_TEST_RAISE(mCV.initialize((SChar*)"TRANSACTION_MINVIEWSCN_BUILDER")
                   != IDE_SUCCESS, err_cond_var_init);

    return IDE_SUCCESS;

    IDE_EXCEPTION( err_cond_var_init );
    {
        IDE_SET( ideSetErrorCode(smERR_FATAL_ThrCondInit) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

/**********************************************************************
 *
 * Description : Thread�� �����Ѵ�.
 *
 **********************************************************************/
IDE_RC smxMinSCNBuild::destroy()
{
    IDE_TEST_RAISE( mCV.destroy() != IDE_SUCCESS, err_cond_destroy );

    IDE_TEST( mMutex.destroy() != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION( err_cond_destroy );
    {
        IDE_SET( ideSetErrorCode(smERR_FATAL_ThrCondDestroy) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**********************************************************************
 *
 * Description : Thread�� �����Ѵ�.
 *
 **********************************************************************/
IDE_RC smxMinSCNBuild::startThread()
{
    IDE_TEST(start() != IDE_SUCCESS);

    IDE_TEST(waitToStart(0) != IDE_SUCCESS);

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**********************************************************************
 *
 * Description : Thread�� �����Ѵ�.
 *
 **********************************************************************/
IDE_RC smxMinSCNBuild::shutdown()
{
    SInt          sState = 0;

    lock( NULL /* idvSQL* */);
    sState = 1;

    mFinish = ID_TRUE;

    IDE_TEST_RAISE(mCV.signal() != IDE_SUCCESS, err_cond_signal);

    sState = 0;
    unlock();

    IDE_TEST_RAISE(join() != IDE_SUCCESS, err_thr_join);

    return IDE_SUCCESS;

    IDE_EXCEPTION(err_thr_join);
    {
        IDE_SET(ideSetErrorCode(smERR_FATAL_Systhrjoin));
    }
    IDE_EXCEPTION(err_cond_signal);
    {
        IDE_SET(ideSetErrorCode(smERR_FATAL_ThrCondSignal));
    }
    IDE_EXCEPTION_END;

    if ( sState != 0 )
    {
        IDE_PUSH();

        unlock();

        IDE_POP();
    }

    return IDE_FAILURE;

}


/**********************************************************************
 *
 * Description : Thread�� Main Job�� �����Ѵ�.
 *
 * UPDATE_MIN_VIEWSCN_INTERVAL_ ������Ƽ�� ������ �ð����� �������
 * Minimum Disk ViewSCN�� ���Ͽ� �����Ѵ�.
 *
 **********************************************************************/
void smxMinSCNBuild::run()
{
    IDE_RC         sRC;
    SLong          sInterval;
    UInt           sState = 0;
    smSCN          sSysMinDskViewSCN;
    smSCN          sSysMinDskFstViewSCN;
    smSCN          sSysMinOldestFstViewSCN;
    PDL_Time_Value sCurrTimeValue;
    PDL_Time_Value sPropTimeValue;

restart :
    sState = 0;

    lock( NULL /* idvSQL* */);
    sState = 1;

    mResume = ID_TRUE;

    while( 1 )
    {
        // BUG-28819 REBUILD_MIN_VIEWSCN_INTERVAL_�� 0���� �����ϰ� restart�ϸ�
        // SysMinOldestFstViewSCN�� ���ŵ��� �ʰ� �ʱⰪ ��� 0�� �����Ǿ
        // ���� ���� page�� ���� �Ǿ� System�� ������ �����մϴ�.
        // Inserval�� 0�̴��� ó�� �ѹ��� �� ������ ���ϵ��� �����մϴ�.
        if( mResume == ID_TRUE )
        {
            // Time�� �� �� ��쿡�� ���� ������ ���ϰ�
            // Inserval�� ���� �� ��쿡�� ������ �ʽ��ϴ�.

            // BUG-24885 wrong delayed stamping
            // ��� active Ʈ����ǵ��� minimum disk FstSCN�� ���ϵ��� �߰�
            smxTransMgr::getDskSCNsofAll(
                &sSysMinDskViewSCN,
                &sSysMinDskFstViewSCN,
                &sSysMinOldestFstViewSCN );  // BUG-26881

            // BUG-24885 wrong delayed stamping
            // set the minimum disk FstSCN
            SM_SET_SCN( &mSysMinDskViewSCN, &sSysMinDskViewSCN );
            SM_SET_SCN( &mSysMinDskFstViewSCN, &sSysMinDskFstViewSCN );

            // BUG-26881 �߸��� CTS stamping���� acces�� �� ���� row�� ������
            SM_SET_SCN( &mSysMinOldestFstViewSCN, &sSysMinOldestFstViewSCN );
        }

        mResume   = ID_FALSE;
        sInterval = smuProperty::getRebuildMinViewSCNInterval();

        if ( sInterval != 0 )
        {
            sPropTimeValue.set( 0, sInterval * 1000 );

            sCurrTimeValue  = idlOS::gettimeofday();
            sCurrTimeValue += sPropTimeValue;

            sState = 0;
            sRC = mCV.timedwait(&mMutex, &sCurrTimeValue, IDU_IGNORE_TIMEDOUT);
            sState = 1;
        }
        else
        {
            ideLog::log( SM_TRC_LOG_LEVEL_TRANS,
                         SM_TRC_TRANS_DISABLE_MIN_VIEWSCN_BUILDER );

            sRC = mCV.wait(&mMutex);
            IDE_TEST_RAISE( sRC != IDE_SUCCESS, err_cond_wait );
        }

        if ( mFinish == ID_TRUE )
        {
            break;
        }

        IDE_TEST_RAISE( sRC != IDE_SUCCESS , err_cond_wait );

        if(mCV.isTimedOut() == ID_TRUE)
        {
            mResume = ID_TRUE;
        }
        else
        {
            /* do nothing */
        }
    }

    mResume = ID_FALSE;

    sState = 0;
    unlock();

    return;

    IDE_EXCEPTION(err_cond_wait);
    {
        IDE_SET(ideSetErrorCode(smERR_FATAL_ThrCondWait));
    }
    IDE_EXCEPTION_END;

    if ( sState != 0 )
    {
        IDE_PUSH();

        unlock();

        IDE_POP();
    }

    goto restart;
}

/**********************************************************************
 *
 * Description : ALTER SYSTEM �������� ����ڰ� Disk Minimum View SCN�� �����Ѵ�.
 *
 *               ALTER SYSTEM REBUILD MIN_VIEWSCN;
 *
 *               Rebuild ����̹Ƿ� �Լ� �Ϸ� �������� Disk Min View SCN��
 *               ������ �־�� �Ѵ�. Thread �۾��� ������ �ʰ� ���� ���Ѵ�.
 *
 * aStatistics  - [IN] statistics
 *
 **********************************************************************/
IDE_RC smxMinSCNBuild::resumeAndWait( idvSQL  * aStatistics )
{
    SInt    sState = 0;
    smSCN   sSysMinDskViewSCN;
    smSCN   sSysMinDskFstViewSCN;
    smSCN   sSysMinOldestFstViewSCN;

    lock( aStatistics );
    sState = 1;

    // BUG-24885 wrong delayed stamping
    // ��� active Ʈ����ǵ��� minimum disk FstSCN�� ���ϵ��� �߰�
    smxTransMgr::getDskSCNsofAll(
        &sSysMinDskViewSCN,
        &sSysMinDskFstViewSCN,
        &sSysMinOldestFstViewSCN );  // BUG-26881

    // BUG-24885 wrong delayed stamping
    // set the minimum disk FstSCN
    SM_SET_SCN( &mSysMinDskViewSCN, &sSysMinDskViewSCN );
    SM_SET_SCN( &mSysMinDskFstViewSCN, &sSysMinDskFstViewSCN );

    IDU_FIT_POINT( "1.BUG-32650@smxMinSCNBuild::resumeAndWait" );

    // BUG-26881 �߸��� CTS stamping���� acces�� �� ���� row�� ������
    SM_SET_SCN( &mSysMinOldestFstViewSCN, &sSysMinOldestFstViewSCN );

    IDE_TEST( clearInterval() != IDE_SUCCESS );

    sState = 0;
    unlock();

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if ( sState != 0 )
    {
        IDE_PUSH();

        unlock();

        IDE_POP();
    }

    return IDE_FAILURE;

}


/**********************************************************************
 *
 * Description : MinDskViewSCN �����ֱ⸦ �����Ѵ�.
 *
 * ALTER SYSTEM SET UPDATE_MIN_VIEWSCN_INTERVAL_ .. ��������
 * ����ڰ� Minimum Disk View SCN�� �����ֱ⸦ �����Ѵ�.
 *
 **********************************************************************/
IDE_RC smxMinSCNBuild::resetInterval()
{
    SInt   sState = 0;

    lock( NULL /* idvSQL* */ );
    sState = 1;

    IDE_TEST( clearInterval() != IDE_SUCCESS );

    sState = 0;
    unlock();

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if ( sState != 0 )
    {
        IDE_PUSH();
        unlock();
        IDE_POP();
    }

    return IDE_FAILURE;
}



/**********************************************************************
 *
 * Description : �������� ���� Interval�� �ʱ�ȭ�Ѵ�.
 *
 **********************************************************************/
IDE_RC smxMinSCNBuild::clearInterval()
{
    mResume = ID_FALSE;
    IDE_TEST_RAISE( mCV.signal() != IDE_SUCCESS, err_cond_signal );

    return IDE_SUCCESS;

    IDE_EXCEPTION( err_cond_signal );
    {
        IDE_SET( ideSetErrorCode(smERR_FATAL_ThrCondSignal) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}
