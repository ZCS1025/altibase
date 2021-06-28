@echo off
"%ALTIBASE_HOME%\bin\isql.exe" -s localhost -u sys -p MANAGER %*
