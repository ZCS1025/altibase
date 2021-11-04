#!/bin/sh
###########################
# Creat Analytic Data      #
###########################

LOAD_FILE=$1
echo " ========================================"
echo " Start create analytic data              "
echo " ========================================"
echo " Filst Job SAM File Load : < $LOAD_FILE >"
echo " ========================================"
#python3 tag_truncate.py
python3 tag_DB_load.py $LOAD_FILE
echo " "
echo " ================================"
echo " Second Job Pivot Table Creation "
echo " ================================"
python3 tag_data_pivot.py
echo " "
echo " ============================"
echo " Third Job Timelist Creation "
echo " ============================"
python3 tag_timelist_cre.py
echo " "
echo " =========================="
echo " Forth Job Timelist Merge "
echo " =========================="
python3 tag_merge_cre.py
echo " "
echo " ============================"
echo " Fifth Job Missing Data Fill "
echo " ============================"
python3 tag_fillna_cre.py
echo " "
echo " =============================="
echo " Sixth Job Clean Data Creation "
echo " =============================="
python3 tag_clean_cre.py
echo " "
echo " ==============================="
echo " Seventh Job DCOR Data Creation "
echo " ==============================="
python3 tag_dcor_cre.py
echo " "
echo " =================================================="
echo " Eighth Job Minute and Hour Summary  Data Creation "
echo " =================================================="
python3 tag_minute_cre.py
python3 tag_hour_cre.py
echo " "
echo " ========================================="
echo " Ninth Job  Static Data Creation "
echo " ========================================="
python3 tag_data_anal.py
echo " "
echo " =========================="
echo "            END  JOB       "
echo " =========================="
