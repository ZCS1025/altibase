include $(ALTIBASE_HOME)/install/altibase_env.mk

TEST = failoversample$(BINEXT)

BINS = $(TEST)

SRCS = failoversample.cpp
OBJS = $(SRCS:.cpp=.$(OBJEXT))

all : compile

compile: $(BINS)

failoversample$(BINEXT): failoversample.$(OBJEXT)
	 $(LD) $(LFLAGS) $(LDOUT)$@ $^ $(LIBOPT)shardcli$(LIBAFT)  $(LIBS)
clean:
	rm -rf core* *.$(OBJEXT) *.res *.d $(BINS)
