COMPILE.c=gcc -D_GNU_SOURCE -W -Wall -pipe -D_POSIX_PTHREAD_SEMANTICS -D_POSIX_THREADS -D_POSIX_THREAD_SAFE_FUNCTIONS -D_REENTRANT -DPDL_HAS_AIO_CALLS -m64 -mtune=k8 -O3 -funroll-loops -fno-strict-aliasing -fno-omit-frame-pointer -DPDL_NDEBUG -D_GNU_SOURCE -DACP_CFG_COMPILE_64BIT -DACP_CFG_COMPILE_BIT=64 -DACP_CFG_COMPILE_BIT_STR=64 -DCOMPILER_OPT_FLAGS="-O3 -funroll-loops -fno-strict-aliasing -fno-omit-frame-pointer" -MMD -MP -c
SHCOMPILE.c=
CCOUT=-o
LD=g++
LFLAGS=-Wl,-relax -Wl,--no-as-needed -L. -O3
ODBC_LIBS=-lodbc
LIBS= -ldl -lpthread -lcrypt -lrt -lstdc++
SH_LIBS= -ldl -lpthread -lcrypt -lrt -lstdc++
IDROPT=-I
LDROPT=-L
