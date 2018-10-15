# Makefile with Genernal Options
#
# CVS Info : $Id: hp_hpux_gcc.mk 36452 2009-10-30 10:15:45Z mycomman $
#

# 전달되는 외부 변수들 : GCC

# ID_DIR      : SM 디렉토리 
# ID_ACE_ROOT : 라이브러리 패스
# compile64   : 컴파일 환경
# compat5     : CC 5.0 으로?

ifndef	BUILD_MODE
	@echo "ERROR BUILD_MODE!!!!"
	@exit
endif

.SUFFIXES: .cpp .$(OBJEXT) .a

CXX	= g++
CC	= gcc
LD	= $(CXX)

AR	= ar
ARFLAGS	= -ruv

# To porting TASK-1793 HP-11.11 GCC 4.0.2 porting 
# NM의 경로를 절대경로에서 상대경로로 설정
NM = nm 
NMFLAGS	= -t x

PURIFY		= purify -chain-length=100
QUANTIFY	= quantify
PURECOV		= purecov
PURIFYCOV   = $(PURIFY) $(PURECOV)

# IDL(ACE) Library
# Library

# readline library 설정

ifeq "$(USE_READLINE)" "1"
READLINE_INCLUDES = -I/usr/local/include/readline
READLINE_LIBRARY =  -lreadline -ltermcap 
endif # use readline library 


LIBS     = $(READLINE_LIBRARY) -lxti -lpthread -lrt -ldld -lcl

# 매크로 선언
CLASSIC_LIB = 
LIB64_DIRS  =
LIB32_DIRS  =


# ACE와 컴파일 옵션을 일치시킨다.
# inline = -D__ACE_INLINE__ 
# else   =  -DACE_NO_INLINE
#
#ACE_FLAG = -W -Wall -Wpointer-arith -pipe  -O2 -g -fno-implicit-templates   -fno-exceptions -fcheck-new -DACE_NO_INLINE -DACE_LACKS_ACE_TOKEN -DACE_LACKS_ACE_OTHER 

# 동적 컴파일 옵션 선언
#
EXTRA_CXXOPT  =
EXTRA_LOPT += $(LIB32_DIRS)

CXXOPT		= -c -Wall -D_POSIX_PTHREAD_SEMANTICS 
EOPT		= -E -D_POSIX_PTHREAD_SEMANTICS 
SOPT		= -S -D_POSIX_PTHREAD_SEMANTICS 
LOPT		= -L. 
AOPT            =
DEFINES		= -D_REENTRANT

CXXOPT_DEPENDANCY = -MM

#64bit or 32bit
ifeq ($(compile64),1)
	LOPT	+= -mlp64
endif

# Build Mode file for Makefile 
#
# CVS Info : $Id: hp_hpux_gcc.mk 36452 2009-10-30 10:15:45Z mycomman $
#

# BUILD_MODE의 종류
#	debug		: Debug 모드
#   prerelease      : -DDEBUG(x) -g (o)
#	release		: release 버젼, 실제 product에 해당

# LINK MODE 종류 
#	purify		: purify version
#	quantify	: quantify version
#	purecov		: purecov version
#	purifycov   : purifycov version


ifeq "$(BUILD_MODE)" "debug"
ACE_FLAG = -DACE_NO_INLINE -DACE_LACKS_ACE_TOKEN -DACE_LACKS_ACE_OTHER
EXTRA_CXXOPT	+= $(ACE_FLAG) -g $(SOURCE_BROWSER) -DDEBUG
EXTRA_LOPT	+= -g

else
ifeq "$(BUILD_MODE)" "prerelease"
ACE_FLAG = -D__ACE_INLINE__ -DACE_LACKS_ACE_TOKEN -DACE_LACKS_ACE_OTHER
EXTRA_CXXOPT	+= $(ACE_FLAG) -g
EXTRA_LOPT	+= -g

else
ifeq "$(BUILD_MODE)" "release"
ACE_FLAG = -D__ACE_INLINE__ -DACE_LACKS_ACE_TOKEN -DACE_LACKS_ACE_OTHER
EXTRA_CXXOPT	+= $(ACE_FLAG) -O3 -funroll-loops
EXTRA_LOPT	+= -O3 -funroll-loops
else
error:
	@echo "ERROR!!!! UNKNOWN BUILD_MODE($(BUILD_MODE))";
	@exit;

endif	# release
endif   # prerelease
endif	# debug

############### LINK MODE #####################################3
ifeq "$(LINK_MODE)" "normal"
LD		:=  $(LD)

else
ifeq "$(LINK_MODE)" "purify"
  ifeq "$(OS_TARGET)" "HP_HPUX"
    LD		:= $(PURIFY) -collector=/usr/ccs/bin/ld $(LD)
  else
    LD		:= $(PURIFY) $(LD)
  endif
else
ifeq "$(LINK_MODE)" "quantify"
LD		:= $(QUANTIFY) $(LD)

else
ifeq "$(LINK_MODE)" "purecov"
LD		:= $(PURECOV) $(LD)

else
ifeq "$(LINK_MODE)" "purifycov"
LD		:= $(PURIFYCOV) $(LD)
else
error:
	@echo "ERROR!!!! UNKNOWN LINK_MODE($(LINK_MODE))";
	@exit;

endif	# purifycov
endif	# purecov
endif	# quantify
endif	# purify
endif	# normal

CXXFLAGS	= $(CXXOPT) $(EXTRA_CXXOPT)

EFLAGS	= $(EOPT) $(EXTRA_CXXOPT)
SFLAGS	= $(SOPT) $(EXTRA_CXXOPT)
LFLAGS	= $(LOPT) $(EXTRA_LOPT)

ODBC_LIBS = -lstream -lCsup -lm -ldld

#############################
#  Choose Altibase Build    # 
#############################

# some platform like windows don;t have enough shell buffer for linking. 
# so use indirection file for linking.
NEED_INDIRECTION_BUILD = no

# some platform like aix 4.3 don't need to build shared library 
NEED_SHARED_LIBRARY = yes

# some platform like suse linux/windows have a problem for using libedit.
NEED_BUILD_LIBEDIT = yes

# some platform like a windows don;t have to build jdbc 
NEED_BUILD_JDBC = yes
