# $Id: Makefile 85045 2019-03-20 01:40:10Z andrew.shin $

include ../mtEnv.mk

SRCS = $(MTX_SRCS)

OBJS=$(SRCS:$(DEV_DIR)/%.cpp=$(TARGET_DIR)/%.$(OBJEXT))

all: $(COMPILE)

shobjs: $(VSHOBJS)

compile: $(OBJS)

clean:
	$(RM) $(TARGET_DIR)/src/mt/mtx *~ .shobj

-include $(OBJS:.$(OBJEXT)=.d)
