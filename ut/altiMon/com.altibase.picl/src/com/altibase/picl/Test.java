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
 
package com.altibase.picl;

import java.util.Properties;

/*
 * BUG-46607 Need to a way to test PICL.
 *   Check if PICL support this platform.
 *   Test the functions used for Os Metrics.
 */
public class Test
{
    public static void main(String[] args) throws Exception
    {	    
        System.out.println("===========================================================");
        System.out.println("Platform Information Collection Library-PICL Testing Module");
        System.out.println("===========================================================");
        System.out.println("");

        Picl picl = new Picl();

        System.out.println("Initializing PICL(Platform Information Collection Library)...");
        if (!picl.isFileExist)
        {
            System.out.println(String.format(
                        "PICL file(%s) does not exist.",
                        picl.mPiclLibName));
            return;
        }
        if (!picl.isLoad)
        {
            System.out.println(String.format(
                        "PICL file(%s) could not be loaded.",
                        picl.mPiclLibName));
            return;
        }
        if (!picl.isSupported)
        {
            System.out.println("PICL does not support this platform.");
            return;
        }
        System.out.println("PICL Library : " + picl.mPiclLibName);

        String procPath = System.getenv("ALTIBASE_HOME") + "/bin/altibase";
        Cpu cpu = picl.getCpu();
        System.out.println("");
        System.out.println("----------");
        System.out.println("Process ID");
        System.out.println("----------");
        System.out.println("Altibase Process : " + procPath);
        System.out.println("PID : " + picl.getProcFinder().getProcessID(procPath));
        System.out.println("");

        System.out.println("---------------");
        System.out.println("Total CPU Usage");
        System.out.println("---------------");
        System.out.println("Sys  : " + cpu.getSysPerc());
        System.out.println("User : " + cpu.getUserPerc());
        System.out.println("");

        ProcCpu pcpu = picl.getProcCpu(procPath);
        pcpu.collect(picl, picl.getProcFinder().getProcessID(procPath));

        System.out.println("------------------");
        System.out.println("Altibase CPU Usage");
        System.out.println("------------------");
        System.out.println("Sys  : " + pcpu.getSysPerc());
        System.out.println("User : " + pcpu.getUserPerc());
        System.out.println("");	    

        System.out.println("------------");
        System.out.println("Memory Usage");
        System.out.println("------------");
        long memTotal = picl.getMem().getTotal();
        long memFree = picl.getMem().getFree();
        System.out.println("Total(KB) : " + memTotal);
        System.out.println("Used(KB)  : " + picl.getMem().getUsed());
        System.out.println("Free(KB)  : " + memFree);
        System.out.println("Free(%)   : " + memFree * 100.0 / memTotal);
        System.out.println("");

        System.out.println("---------------------------");
        System.out.println("Altibase Memory Usage (RSS)");
        System.out.println("---------------------------");
        long procMemUsed = picl.getProcMem(procPath).getUsed();
        System.out.println("Used(KB) : " + procMemUsed);
        System.out.println("Used(%)  : " + procMemUsed * 100.0 / picl.getMem().getTotal());
        System.out.println("");

        System.out.println("----------");
        System.out.println("Swap Usage");
        System.out.println("----------");
        long swapTotal = picl.getSwap().getSwapTotal();
        long swapFree = picl.getSwap().getSwapFree();
        System.out.println("Total(KB) : " + swapTotal);
        System.out.println("Used(KB)  : " + picl.getSwap().getSwapUsed());
        System.out.println("Free(KB)  : " + swapFree);
        System.out.println("Free(%)   : " + swapFree * 100.0 / swapTotal);
        System.out.println("");

        System.out.println("----------");
        System.out.println("Disk Usage");
        System.out.println("----------");
        Iops dev_iops;
        long diskAvail = 0;
        long diskUsed = 0;
        Device[] list = picl.getDeviceList();	    
        for(int j=0; j<list.length; j++)
        {		
            dev_iops = picl.getDiskLoad(list[j].getDirName());
            System.out.println("Disk        : " + dev_iops.getDirName());
            System.out.println("Device Name : " + list[j].getDevName());
            diskAvail = dev_iops.getAvail();
            diskUsed = dev_iops.getUsed();
            System.out.println("Total(KB)   : " + dev_iops.getTotal());
            System.out.println("Used(KB)    : " + diskUsed);
            System.out.println("Avail(KB)   : " + diskAvail);
            System.out.println("Free(KB)    : " + dev_iops.getFree());
            System.out.println("Free(%)     : " + diskAvail * 100.0 / (diskAvail + diskUsed));
            System.out.println("");
        }
    }
}
