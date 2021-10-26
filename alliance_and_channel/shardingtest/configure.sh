#!/bin/sh

rm -f altibase_env.mk
grep "COMPILE.c=" $ALTIBASE_HOME/install/altibase_env.mk >>altibase_env.mk
grep "CCOUT="     $ALTIBASE_HOME/install/altibase_env.mk >>altibase_env.mk
grep "LD="        $ALTIBASE_HOME/install/altibase_env.mk >>altibase_env.mk
grep "LFLAGS="    $ALTIBASE_HOME/install/altibase_env.mk >>altibase_env.mk
grep "LIBS="      $ALTIBASE_HOME/install/altibase_env.mk >>altibase_env.mk
grep "IDROPT="    $ALTIBASE_HOME/install/altibase_env.mk >>altibase_env.mk
grep "LDROPT="    $ALTIBASE_HOME/install/altibase_env.mk >>altibase_env.mk
