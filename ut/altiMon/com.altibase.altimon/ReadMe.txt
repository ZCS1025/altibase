###################################################################
Altibase Monitoring Daemon Guide
Altibase, Inc.
http://www.altibase.com
###################################################################

I. Configuration

	conf/config.xml       - altimon & DB Configuration File
	conf/Metrics.xml      - Metrics Configuration
	conf/GroupMetrics.xml - Group Metrics Configuration

	You must write information about the Altibase to 'config.xml' file,
    and add your metrics to Metrics.xml and GroupMetrics.xml. 

II. Start-up & Shut-down
	
	* Start altimon as a Daemon
	  # altimon.sh start
	* Stop altimon
	  # altimon.sh stop

* Test PICL feasibility
    # cd $ALTIBASE_HOME/altiMon
    # java -Dpicl="<picl_lib_file>" -jar lib/com.altibase.picl.jar

