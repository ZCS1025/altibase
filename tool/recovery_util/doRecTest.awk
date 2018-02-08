#
# Usage : gawk  -f doRecTest.awk
#
#
# Altibase �ҽ��ڵ忡�� SMU_REC_POINT()��� ���Ǹ� �� �κ���
# Recovery �׽�Ʈ ����� �Ǹ�,
# recovery.dat�� ����� �ȴ�.(sm/src/���� make gen_rec_list �� ����)
#
#  Formats
#
#  1. �ҽ��ڵ� : SMU_REC_POINT()
#
#  2. killPoint ȭ�� : | ȭ�ϸ�  ���ι�ȣ  ����Ÿ�� ����| 
#     sKillPointFile = ENVIRON["ALTIBASE_HOME"]"/trc/killPoint.dat";
#
#  3. ����� boot.log ����
#   |  SMU_REC_POINT ȭ�ϸ� ���ι�ȣ |
#

BEGIN {

    #-------------------------- 1. DEFINE ----------------------------------------
    # �׽�Ʈ�� ������ ���� ��ȣ : 1���� ���� not 0
    sStartScenarioNum = 1;

    # �ó����� �׽�Ʈ �� �߻��ϴ� �޽��� ȭ��
    sMsgLogFile = LOG;

    # �ó����� �׽�Ʈ �� �߻��ϴ� �޽��� ȭ��
    #  0 : ���, 1 : Count, 2 : Random
    KILL_TYPE = 0;

    # Recovery Test Point�� ���� ȭ�� (sm ���丮���� make gen_rec_list;
    # ��� �ϸ� ������)
    sPointListFile = LIST;
    sKillPointFile = ENVIRON["ALTIBASE_HOME"]"/trc/killPoint.dat";

    
    line = 1;
    #-----------------------------------------------------------------------------

    #--------------------------  2. Startup �غ�  ---------------------------
    system("server kill");
    printf ("\n\n[%s] BEGIN Of Recovery Test \n", strftime())  >> sMsgLogFile;

    i = 0;
    while ( (getline < sPointListFile) < 0)
    {
        printf("Loading Recovery Database File Error : Can't Find [%s] \n", sPointListFile);
        printf("generating...\n");
        res = system("sh ./gen.sh");
        if (i == 2 || res < 0)
        {
            printf("go to sm/src. and execute (make gen_rec_list)");
            exit;
        }
        i++;
    }
    close(sPointListFile);
    
    #--------------------------  3. Recovery Test ���� ---------------------------

    while( (getline killPointName < sPointListFile) > 0)
    {
        # �׽�Ʈ ������ ��ġ���� Skip 
        if ( line < sStartScenarioNum )
        {
            line++;
            continue;
        }
        # 1. Begin �α�
        printf ("\n [%s] LineNum %d : \n",
                strftime(),  line ) >> sMsgLogFile;
        
        if((getline RecPointName < sPointListFile) <= 0)
		{
            printf("invalid recovery.dat (run gen.sh)");
            exit;
		}

        printf ("[%s]\n", RecPointName) >> sMsgLogFile;

        # 2. killPoint ȭ�� ���� : altibase�� ���� ������.
        makeKillingPoint(KILL_TYPE)

        # 3. ���� ���� �� �˻�
        if (system("server start") != 0)
        {
            printf("Error of Server Start");
            exit 0;
        }
        checkSuccessStartup(sPointListFile, line);

        # 4. �ó����� SQL ����
        doScenario();
        
        # 5. ������ ������ �������� �ƴ��� �˻� �� �α�
        result = checkScenarioResult();

        if (result == "failure")
        {
            logMsg(" Killing Altibase. Bacause of scenario failure..");
            system("server kill");
        }

        # 6. Next Line���� 
        line++;
    }

    # ������ Scenario�� ���� �˻�. (startup �� ���� �ʿ�
    logMsg("\n  Last Scenario Starup Test");
    if (system("server start") != 0)
    {
        printf("Error of Server Start");
        exit 0;
    }
    checkSuccessStartup(sPointListFile, line);
    
    #------------------- 4. End Of Test -----------------------------------------
    printf ("\n[%s] END Of Recovery Test\n", strftime())  >> sMsgLogFile;

    close(sMsgLogFile);
    close(sPointListFile);
    exit 0;
}

# ============================ Do Scenario =======================================

function doScenario()
{
    logMsg(" Running  Scenario");
    system("is -f scenario.sql");
}

# ============================ Check Result =======================================

function checkScenarioResult()
{
    sBootLogFile = ENVIRON["ALTIBASE_HOME"]"/trc/altibase_boot.log";
    
    lastline = getLastLine(sBootLogFile); # boot.log�� ������ ����
    
    regx =  "[ ]*SMU_REC_POINT_KILL[ ]*"killPointName"[ ]*";
    
    printf("check String [%s]", killPointName);
    if (match(lastline, regx))
    {
        logMsg(" [SUCCESS] Died!!. going to restart recovery...");
        return "success";
    }
    else
    {
        logMsg(" [ERROR] Still Alive.. Check Your Scenario.");
        return "failure";
    }
}


# ============================ Generation Kill Point ===========================
# Point ȭ�� ����
#  +-----------------------------------------+
#  | ȭ�ϸ�  ���ι�ȣ  ����Ÿ�� ���� Ȯ������| 
#  +-----------------------------------------+
#
#   C���� (%s %d %s %d %d) ���·� ���� �� �ֵ��� ��.
#
#  1. ȭ�ϸ� : Recovery ��ũ�ΰ� �Էµ� �ҽ��ڵ�
#  2. �ش� ȭ�Ͽ��� ����� ���ι�ȣ
#  3. ���� Ÿ��
#     0. IMMEDIATE  : ��� ������. (���ڴ� 0���� �Է��ؾ� ��)
#     1. DECREASE   : ���ڷ� ��õ� ����ŭ �ݺ��ϰ�, 0�� �Ǹ�, ����.
#     2. RANDOM     : ����/1000 �� Ȯ���� ���� ������ �Ǻ�. 1�� ���
#                     0.1%�� Ȯ���� ������.
#  4. ���� : ���� Ÿ�Կ� ���� �ΰ����� UInt
#
#  5. Ȯ������ : ������ �÷��׸� �� �� �ֵ��� ��.
#                0 => Restart Recovery �ÿ��� KillPoint�� skip
#                1 => Restart Recovery �ÿ��� Kill Point�� ����.
#
function makeKillingPoint(aKillType)
{
    sExtFlag = 0; # restart �ÿ��� Skip
    
    if (aKillType == 0)
    {
        printf ("%s KILL_IMME 0 %d\n", killPointName, sExtFlag) > sKillPointFile;
        #printf ("%s\n", RecPointName) > sKillPointFile;
    }
    else
    {
        if (aKillType == 1)
        {
            printf ("%s KILL_DECR 1 %d\n", killPointName, sExtFlag) > sKillPointFile;
            #printf ("%s\n", RecPointName) > sKillPointFile;
        }
        else
        {
            if (aKillType == 2)
            {
                printf ("%s KILL_RAND 500 %d\n", killPointName, sExtFlag) > sKillPointFile;
                printf ("%s\n", RecPointName) > sKillPointFile;
            }
            else
            {
                logMsg("Error of aKillType : %d\n", aKillType);
            }
        }
    }
    close(sKillPointFile);
}

# ============================ Startup �˻� ====================================
function checkSuccessStartup(aPointListFile, aLine)
{
    sBootLogFile = ENVIRON["ALTIBASE_HOME"]"/trc/altibase_boot.log";
    
    lastline = getLine(sBootLogFile); # boot.log�� ������ ����
    
    regx =  "[ ]*STARTUP Process SUCCESS*";
    
    if (match(lastline, regx))
    {
        logMsg(" [SUCCESS] Startup Success.");
    }
    else
    {
        logMsg(" [ERROR] Startup Failure.. Check Recovery Source. But, proceeding ... ");
		sComComp = "sh artComp "aPointListFile"-"line;
		system(sComComp);
        system("clean");
        system("server start");
    }
}

# ============================ LIBRARY =========================================
function logMsg(msg)
{
    printf(" %s\n", msg) >> sMsgLogFile;
}

function getLine(aLogFile)
{
	system("sleep 2;");
    getlinecomm = "tail -4 " aLogFile " | head -1";
    getlinecomm | getline sLastLine;
    close(getlinecomm);
    #printf(" %s\n", sLastLine) >> sMsgLogFile;
    return sLastLine;
}

function getLastLine(aLogFile)
{
    getlinecomm = "cat " aLogFile " | tail -1 ";
    getlinecomm | getline sLastLine;
    close(getlinecomm);
    return sLastLine;
}
