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

/*
 * picl_linux.c
 *
 *  Created on: 2010. 1. 13
 *      Author: admin
 */

#include "picl_os.h"
#include "util_picl.h"

/*
 * Class:     com_altibase_picl_Cpu
 * Method:    update
 * Signature: (Lcom/altibase/picl/Picl;)V
 */
JNIEXPORT void JNICALL Java_com_altibase_picl_Cpu_update (
        JNIEnv *env, jobject cpu_obj, jobject picl_obj)
{
    jfieldID prev_fid, sys_fid, user_fid;
    jlongArray mPrevTime;
    float mUserPerc;
    float mSysPerc;

    jboolean isCopy = JNI_TRUE;

    /* Variable for calculate CPU Percentage */
    unsigned long long sDeltaCpuTotal;
    unsigned long long sPrevCpuTotal;

    /* Get a reference to obj's class */
    jclass cls = (*env)->GetObjectClass(env, cpu_obj);
    jclass picl_cls = (*env)->GetObjectClass(env, picl_obj);

    /* Look for the instance field mPrevTime in cls */
    prev_fid = (*env)->GetFieldID(env, picl_cls, "mPrevTime", "[J");
    sys_fid = (*env)->GetFieldID(env, cls, "mSysPerc", "D");
    user_fid = (*env)->GetFieldID(env, cls, "mUserPerc", "D");

    if(prev_fid == NULL || sys_fid == NULL || user_fid == NULL)
    {
        return;// failed to find the field
    }

    // JNI Object Type
    mPrevTime = (jlongArray)(*env)->GetObjectField(env, picl_obj, prev_fid);
    mSysPerc = (*env)->GetDoubleField(env, cpu_obj, sys_fid);
    mUserPerc = (*env)->GetDoubleField(env, cpu_obj, user_fid);

    // Native Type
    jlong* mPrevTimes = (*env)->GetLongArrayElements(env, mPrevTime, &isCopy);

    // Calculate OS level CPU utilizatoins
    cpu_t cpu;

    getCpuTime(&cpu);

    // TODO: Keep this value in Java member variable
    sPrevCpuTotal = mPrevTimes[CP_USER] + mPrevTimes[CP_SYS] + 
                    mPrevTimes[CP_IDLE] + mPrevTimes[CP_WAIT] + 
                    mPrevTimes[CP_NICE];

    sDeltaCpuTotal = cpu.total - sPrevCpuTotal;

    if(sDeltaCpuTotal == 0)
    {
        mSysPerc  = 0.0;
        mUserPerc = 0.0;
    }
    else
    {
        mSysPerc  = 100.00 * ( (double)(cpu.sys - mPrevTimes[CP_SYS]) / sDeltaCpuTotal );
        mUserPerc = 100.00 * ( (double)(cpu.user - mPrevTimes[CP_USER]) / sDeltaCpuTotal );
    }

    mPrevTimes[CP_USER] = cpu.user;
    mPrevTimes[CP_SYS]  = cpu.sys;
    mPrevTimes[CP_WAIT] = cpu.wait;
    mPrevTimes[CP_IDLE] = cpu.idle;
    mPrevTimes[CP_NICE] = cpu.nice;

    (*env)->ReleaseLongArrayElements(env, mPrevTime, mPrevTimes, 0);

    /* Read the instance field mSysPerc */
    (*env)->SetObjectField(env, picl_obj, prev_fid, mPrevTime);
    (*env)->SetDoubleField(env, cpu_obj, sys_fid, mSysPerc);
    (*env)->SetDoubleField(env, cpu_obj, user_fid, mUserPerc);
}

/*
 * Class:     com_altibase_picl_ProcCpu
 * Method:    update
 * Signature: (Lcom/altibase/picl/Picl;J)V
 */
JNIEXPORT void JNICALL Java_com_altibase_picl_ProcCpu_update(
        JNIEnv * env,
        jobject proccpu_obj,
        __attribute__((unused)) jobject picl_obj,
        jlong pid)
{
    jfieldID sys_fid, user_fid, prev_ctime_fid, prev_utime_fid, prev_stime_fid, prev_cutime_fid, prev_cstime_fid;

    double mUserPerc;
    double mSysPerc;

    /*
    #define PROC_USER 0
    #define PROC_SYS  1
    #define CPU_USER  2
    #define CPU_SYS   3
    #define CPU_TOTAL 4 
    */
    int i;
    int sArrCnt = CPU_TOTAL + 1;
    
    unsigned long long sDeltaTimes[sArrCnt];
    unsigned long long sPrevTimes [sArrCnt];
    unsigned long long sCurrTimes [sArrCnt];

    /* Get a reference to obj's class */
    jclass proccpu_cls = (*env)->GetObjectClass(env, proccpu_obj);

    /* Look for the instance field mPrevTime in cls */
    sys_fid = (*env)->GetFieldID(env, proccpu_cls, "mSysPerc", "D");
    user_fid = (*env)->GetFieldID(env, proccpu_cls, "mUserPerc", "D");
    /* TODO: Make it array in Java will be better */
    prev_utime_fid  = (*env)->GetFieldID(env, proccpu_cls, "mPrevUtime", "J");
    prev_stime_fid  = (*env)->GetFieldID(env, proccpu_cls, "mPrevStime", "J");
    prev_cutime_fid = (*env)->GetFieldID(env, proccpu_cls, "mPrevCUtime", "J");
    prev_cstime_fid = (*env)->GetFieldID(env, proccpu_cls, "mPrevCStime", "J");
    prev_ctime_fid  = (*env)->GetFieldID(env, proccpu_cls, "mPrevCtime", "J");

    if( sys_fid == NULL ||
        user_fid == NULL ||
        prev_ctime_fid == NULL ||
        prev_utime_fid == NULL ||
        prev_stime_fid == NULL ||
        prev_cstime_fid == NULL ||
        prev_cutime_fid == NULL )
    {
        return; // failed to find the field
    }

    /* Get the ProcCpu object member variable */
    // Set previous times
    mSysPerc = (*env)->GetDoubleField(env, proccpu_obj, sys_fid);
    mUserPerc = (*env)->GetDoubleField(env, proccpu_obj, user_fid);
    
    sPrevTimes[PROC_USER] = (*env)->GetLongField(env, proccpu_obj, prev_utime_fid);
    sPrevTimes[PROC_SYS]  = (*env)->GetLongField(env, proccpu_obj, prev_stime_fid);
    sPrevTimes[CPU_USER]  = (*env)->GetLongField(env, proccpu_obj, prev_cutime_fid);
    sPrevTimes[CPU_SYS]   = (*env)->GetLongField(env, proccpu_obj, prev_cstime_fid);
    sPrevTimes[CPU_TOTAL] = (*env)->GetLongField(env, proccpu_obj, prev_ctime_fid);

    // Calculate Process (PID) CPU utilization
    proc_cpu_t proc_cpu;
    cpu_t cpu;

    proc_cpu.utime = -1;
    proc_cpu.stime = -1;

    getCpuTime(&cpu);

    if( getProcCpuMtx(&proc_cpu, pid) == 0 )
    {
        return;
    }

    if( pid == -1 )
    {
        return;
    }

    sCurrTimes[PROC_USER] = proc_cpu.utime;
    sCurrTimes[PROC_SYS]  = proc_cpu.stime;
    sCurrTimes[CPU_USER]  = cpu.user;
    sCurrTimes[CPU_SYS]   = cpu.sys;
    sCurrTimes[CPU_TOTAL] = cpu.total;

    // Set deltas
    for ( i = 0; i < sArrCnt; i++)
    {
        sDeltaTimes[i]  = sCurrTimes[i] - sPrevTimes[i];
    }
   
    // Can it be happened?
    if(sDeltaTimes[CPU_USER] < sDeltaTimes[PROC_USER])
    {
        sDeltaTimes[PROC_USER] = sDeltaTimes[CPU_USER];
    }

    // Can it be happened?
    if(sDeltaTimes[CPU_SYS] < sDeltaTimes[PROC_SYS])
    {
        sDeltaTimes[PROC_SYS] = sDeltaTimes[CPU_SYS];
    }

    if(sDeltaTimes[CPU_TOTAL] == 0)
    {
        mSysPerc = 0.0;
        mUserPerc = 0.0;
    }
    else
    {
        mSysPerc = 100.0 * ( (double) sDeltaTimes[PROC_SYS] ) / sDeltaTimes[CPU_TOTAL];
        mUserPerc = 100.0 * ( (double) sDeltaTimes[PROC_USER] ) / sDeltaTimes[CPU_TOTAL];
    }

    for ( i = 0; i < sArrCnt; i++)
    {
        sPrevTimes[i] = sCurrTimes[i];
    }

    (*env)->SetDoubleField(env, proccpu_obj, sys_fid, mSysPerc);
    (*env)->SetDoubleField(env, proccpu_obj, user_fid, mUserPerc);
    (*env)->SetLongField(env, proccpu_obj, prev_utime_fid,  sPrevTimes[PROC_USER]);
    (*env)->SetLongField(env, proccpu_obj, prev_stime_fid,  sPrevTimes[PROC_SYS]);
    (*env)->SetLongField(env, proccpu_obj, prev_cutime_fid, sPrevTimes[CPU_USER]);
    (*env)->SetLongField(env, proccpu_obj, prev_cstime_fid, sPrevTimes[CPU_SYS]);
    (*env)->SetLongField(env, proccpu_obj, prev_ctime_fid,  sPrevTimes[CPU_TOTAL]);
}

/*
 * Class:     com_altibase_picl_Memory
 * Method:    update
 * Signature: (Lcom/altibase/picl/Picl;)V
 */
JNIEXPORT void JNICALL Java_com_altibase_picl_Memory_update(
        JNIEnv * env,
        jobject mem_obj,
        __attribute__((unused)) jobject picl_obj)
{
    jfieldID memUsage_fid;
    jlongArray memUsage;
    jlong* memUsageArr;

    jboolean isCopy = JNI_TRUE;

    /* Variable for calculate Memory Usage */

    /* Get a reference to obj's class */
    jclass mem_cls = (*env)->GetObjectClass(env, mem_obj);

    /* Look for the instance field mMemoryUsage in cls */
    memUsage_fid = (*env)->GetFieldID(env, mem_cls, "mMemoryUsage", "[J");

    if(memUsage_fid == NULL)
    {
        return;// failed to find the field
    }

    memUsage = (jlongArray)(*env)->GetObjectField(env, mem_obj, memUsage_fid);

    // Native Type
    memUsageArr = (*env)->GetLongArrayElements(env, memUsage, &isCopy);

    // Calculate
    char sMemInfo[BUFSIZ];

    sMemInfo[0] = '\0'; /* BUG-43351 Uninitialized Variable */
    if(convertName2Content(TOTAL_MEMINFO, sMemInfo, sizeof(sMemInfo)) != 1)
    {
        return;
    }

    memUsageArr[MemTotal] = getMem(sMemInfo, GET_PARAM_MEM(PARAM_MEMTOTAL)) / 1024;
    memUsageArr[MemFree] = getMem(sMemInfo, GET_PARAM_MEM(PARAM_MEMFREE)) / 1024;
    memUsageArr[MemUsed] = memUsageArr[MemTotal] - memUsageArr[MemFree];

    // Save into Java Object
    (*env)->ReleaseLongArrayElements(env, memUsage, memUsageArr, 0);
    (*env)->SetObjectField(env, mem_obj, memUsage_fid, memUsage);
}

/*
 * Class:     com_altibase_picl_ProcMemory
 * Method:    update
 * Signature: (Lcom/altibase/picl/Picl;J)V
 */
JNIEXPORT void JNICALL Java_com_altibase_picl_ProcMemory_update(
        JNIEnv * env,
        jobject procmem_obj,
        __attribute__((unused)) jobject picl_obj,
        jlong pid)
{
    jfieldID memUsed_fid;
    long memUsed;

    /* Get a reference to obj's class */
    jclass procmem_cls = (*env)->GetObjectClass(env, procmem_obj);

    /* Look for the instance field mMemoryUsage in cls */
    memUsed_fid = (*env)->GetFieldID(env, procmem_cls, "mMemUsed", "J");

    if(memUsed_fid == NULL)
    {
        return;// failed to find the field
    }

    /* Get the ProcCpu object member variable */
    memUsed = (*env)->GetLongField(env, procmem_obj, memUsed_fid);

    // Calculate
    char sProcMemInfo[BUFSIZ];
    char *index = sProcMemInfo;

    if(getProcString(sProcMemInfo, sizeof(sProcMemInfo), pid, MEM_STATM, sizeof(MEM_STATM)-1) != 1)
    {
        return;
    }
    // matric is byte
    EXTRACTDECIMALULL(index);  //skip other mem info
    memUsed = EXTRACTDECIMALULL(index) * getpagesize() / 1024;

    // Save into Java Object
    (*env)->SetLongField(env, procmem_obj, memUsed_fid, memUsed);
}

/*
 * Class:     com_altibase_picl_Swap
 * Method:    update
 * Signature: (Lcom/altibase/picl/Picl;)V
 */
JNIEXPORT void JNICALL Java_com_altibase_picl_Swap_update(
        JNIEnv * env,
        jobject swap_obj,
        __attribute__((unused)) jobject picl_obj)
{
    jfieldID swapTotal_fid, swapFree_fid;
    unsigned long long sSwapTotal, sSwapFree;

    /* Get a reference to obj's class */
    jclass swap_cls = (*env)->GetObjectClass(env, swap_obj);

    /* Look for the instance field in swap_cls */
    swapTotal_fid = (*env)->GetFieldID(env, swap_cls, "mSwapTotal", "J");
    swapFree_fid = (*env)->GetFieldID(env, swap_cls, "mSwapFree", "J");

    if(swapTotal_fid == NULL || swapFree_fid == NULL)
    {
        return;// failed to find the field
    }

    /* Get the ProcCpu object member variable */
    sSwapTotal = (*env)->GetLongField(env, swap_obj, swapTotal_fid);
    sSwapFree = (*env)->GetLongField(env, swap_obj, swapFree_fid);

    // Calculate
    char sBuff[BUFSIZ];

    sBuff[0] = '\0'; /* BUG-43351 Uninitialized Variable */
    if(convertName2Content(TOTAL_SWAP, sBuff, sizeof(sBuff)) != 1)
    {
        return;
    }

    sSwapTotal = getMem(sBuff, GET_PARAM_MEM(PARAM_SWAP_TOTAL)) / 1024;
    sSwapFree = getMem(sBuff, GET_PARAM_MEM(PARAM_SWAP_FREE)) / 1024;

    // Save into Java Object
    (*env)->SetLongField(env, swap_obj, swapTotal_fid, sSwapTotal);
    (*env)->SetLongField(env, swap_obj, swapFree_fid, sSwapFree);
}

/*
 * Class:     com_altibase_picl_Disk
 * Method:    update
 * Signature: (Lcom/altibase/picl/Picl;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_altibase_picl_Disk_update(
        JNIEnv *env,
        jobject disk_obj,
        __attribute__((unused)) jobject picl_obj,
        jstring aPath)
{
    jfieldID dirusage_fid;

    const char *sDirName;
    dir_usage_t sDirUsage;

    /* Get a reference to obj's class */
    jclass disk_cls = (*env)->GetObjectClass(env, disk_obj);

    /* Look for the instance field mMemoryUsage in cls */
    dirusage_fid = (*env)->GetFieldID(env, disk_cls, "mDirUsage", "J");

    sDirUsage.disk_usage = -1;

    sDirName = (*env)->GetStringUTFChars(env, aPath, NULL);

    getDirUsage(sDirName, &sDirUsage);

    // Save into Java Object
    (*env)->SetLongField(env, disk_obj, dirusage_fid, sDirUsage.disk_usage);
}

/*
 * Class:     com_altibase_picl_DiskLoad
 * Method:    getDeviceListNative
 * Signature: ()[Lcom/altibase/picl/Device;
 * Description: This function only returns the device list, because we can choose specific device about what we want to know.
 */
JNIEXPORT jobjectArray JNICALL Java_com_altibase_picl_DiskLoad_getDeviceListNative(
        JNIEnv *env,
        __attribute__((unused)) jobject device_obj)
{
    int status;
    int i;
    device_list_t_ device_list;
    jobjectArray device_arr;
    jfieldID fids[DEVICE_FIELD_MAX];
    jclass device_cls = (*env)->FindClass(env, "com/altibase/picl/Device");

    /* BUG-43351 Uninitialized Variable */
    device_list.number = 0;
    device_list.size = 0;
    device_list.data = NULL;

    if ((status = getDeviceList(&device_list)) != 1) {
        return NULL;
    }

    fids[DEVICE_DIRNAME] = (*env)->GetFieldID(env, device_cls, "mDirName", "Ljava/lang/String;");
    fids[DEVICE_DEVNAME] = (*env)->GetFieldID(env, device_cls, "mDevName", "Ljava/lang/String;");

    device_arr = (*env)->NewObjectArray(env, device_list.number, device_cls, 0);

    for (i=0; i<(long)device_list.number; i++) {
        device_t_ *device = &(device_list.data)[i];
        jobject temp_obj;

        temp_obj = (*env)->AllocObject(env, device_cls);

        (*env)->SetObjectField(env, temp_obj, fids[DEVICE_DIRNAME], (*env)->NewStringUTF(env, device->dir_name));
        (*env)->SetObjectField(env, temp_obj, fids[DEVICE_DEVNAME], (*env)->NewStringUTF(env, device->dev_name));

        (*env)->SetObjectArrayElement(env, device_arr, i, temp_obj);
    }

    if (device_list.size > 0 ) {
        if (device_list.data != NULL) { /* BUG-43351 Free Null Pointer */
            free(device_list.data);
        }
        device_list.number = 0;
        device_list.size = 0;
    }

    return device_arr;
}

/*
 * Class:     com_altibase_picl_DiskLoad
 * Method:    update
 * Signature: (Lcom/altibase/picl/Picl;)V
 */
JNIEXPORT jobject JNICALL Java_com_altibase_picl_DiskLoad_update(
        JNIEnv *env,
        __attribute__((unused)) jobject diskload_obj,
        __attribute__((unused)) jobject picl_obj,
        jstring dirname)
{
    disk_load_t disk_io;
    jclass iops_cls = (*env)->FindClass(env, "com/altibase/picl/Iops");
    jfieldID fids[IO_FIELD_MAX];
    jobject iops_obj;
    const char* name;
    struct timeval currentTime;

    name = (*env)->GetStringUTFChars(env, dirname, 0);

    /* BUG-43351 Uninitialized Variable */
    disk_io.numOfReads = 0;
    disk_io.numOfWrites = 0;
    disk_io.write_bytes = 0;
    disk_io.read_bytes = 0;
    disk_io.total_bytes = 0;
    disk_io.avail_bytes = 0;
    disk_io.used_bytes = 0;
    disk_io.free_bytes = 0;

    getDiskIo(name, &disk_io);//Get current disk io value
    gettimeofday(&currentTime, NULL);
    fids[IO_FIELD_DIRNAME] = (*env)->GetFieldID(env, iops_cls, "mDirName", "Ljava/lang/String;");
    fids[IO_FIELD_READ] = (*env)->GetFieldID(env, iops_cls, "mRead", "J");
    fids[IO_FIELD_WRITE] = (*env)->GetFieldID(env, iops_cls, "mWrite", "J");
    fids[IO_FIELD_TIME] = (*env)->GetFieldID(env, iops_cls, "mTime4Sec", "J");
    fids[IO_FIELD_TOTAL] = (*env)->GetFieldID(env, iops_cls, "mTotalSize", "J");
    fids[IO_FIELD_AVAIL] = (*env)->GetFieldID(env, iops_cls, "mAvailSize", "J");
    fids[IO_FIELD_USED] = (*env)->GetFieldID(env, iops_cls, "mUsedSize", "J");
    fids[IO_FIELD_FREE] = (*env)->GetFieldID(env, iops_cls, "mFreeSize", "J");

    iops_obj = (*env)->AllocObject(env, iops_cls);

    (*env)->SetObjectField(env, iops_obj, fids[IO_FIELD_DIRNAME], (*env)->NewStringUTF(env, name));
    (*env)->SetLongField(env, iops_obj, fids[IO_FIELD_READ], disk_io.numOfReads);
    (*env)->SetLongField(env, iops_obj, fids[IO_FIELD_WRITE], disk_io.numOfWrites);
    (*env)->SetLongField(env, iops_obj, fids[IO_FIELD_TIME], currentTime.tv_sec);
    (*env)->SetLongField(env, iops_obj, fids[IO_FIELD_TOTAL], disk_io.total_bytes);
    (*env)->SetLongField(env, iops_obj, fids[IO_FIELD_AVAIL], disk_io.avail_bytes);
    (*env)->SetLongField(env, iops_obj, fids[IO_FIELD_USED], disk_io.used_bytes);
    (*env)->SetLongField(env, iops_obj, fids[IO_FIELD_FREE], disk_io.free_bytes);

    return iops_obj;
}


/*
 * Class:     com_altibase_picl_ProcessFinder
 * Method:    getPid
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_altibase_picl_ProcessFinder_getPid(
        JNIEnv *env,
        __attribute__((unused)) jclass cls,
        jstring aProcPath)
{
    DIR *procDir;                 // A variable to point directory /proc/pid
    struct dirent *entry;         // A structure to select specific file to use I-node
    struct stat fileStat;         // A structure to store file information

    int pid;
    int status = 0;
    char cmdLine[256];
    char tempPath[256];
    const jbyte *str;

    int pathLength;

    str = (jbyte *) ((*env)->GetStringUTFChars(env, aProcPath, NULL));

    if(str == NULL) {
        return -1;
    }

    /* BUG-43351 Null Pointer Dereference */
    if ( (procDir = opendir("/proc")) == NULL )
    {
        return -1;
    }

    while((entry = readdir(procDir)) != NULL ) 
    {
        sprintf(tempPath, "/proc/%s", entry->d_name);
        if (lstat(tempPath, &fileStat) < 0)
        {
            continue;
        }

        if(!S_ISDIR(fileStat.st_mode))
        {
            continue;
        }

        pid = atoi(entry->d_name);

        if(pid <= 0)
        {
            continue;
        }

        sprintf(tempPath, "/proc/%d/cmdline", pid);

        memset(cmdLine, 0, sizeof(cmdLine));
        if(getCmdLine(tempPath, cmdLine)==0)
        {
            continue;
        }

        pathLength = strlen((char *)str);

        if(strncmp((char*)str, cmdLine, pathLength)==0)
        {
            status = 1;
            break;
        }
    }

    closedir(procDir);

    (*env)->ReleaseStringUTFChars(env, aProcPath, (char*)str);

    if(status==1)
    {
        return (long)pid;
    }
    else
    {
        return -1;
    }
}
