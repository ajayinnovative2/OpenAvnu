AVB_FEATURE_IGB ?= 1
AVB_FEATURE_ATL ?= 1

ifeq ($(AVB_FEATURE_IGB),1)
AVB_IGB_OBJ = avb_igb.o
EXTRA_CFLAGS = -DAVB_FEATURE_IGB=1
endif

ifeq ($(AVB_FEATURE_ATL),1)
AVB_ATL_OBJ = avb_atl.o
EXTRA_CFLAGS = -DAVB_FEATURE_ATL=1
endif

CC ?= gcc
OPT = -O2 -g
WARN = -Wall -Wextra -Wno-parentheses
CFLAGS = $(OPT) $(WARN) $(EXTRA_CFLAGS)
ALL_OBJS = avb_avtp.o avb_gptp.o $(AVB_IGB_OBJ) $(AVB_ATL_OBJ)

.PHONY: all clean

all: $(ALL_OBJS)

avb_igb.o: CPPFLAGS = -I../igb_avb/lib
avb_igb.o: avb_igb.c avb_igb.h
avb_atl.o: CPPFLAGS = -I../atl_avb/lib
avb_atl.o: avb_atl.c avb_atl.h
avb_avtp.o: avb_avtp.c avb_avtp.h
avb_gptp.o: avb_gptp.c avb_gptp.h

clean:
	$(RM) $(ALL_OBJS)
	$(RM) `find . -name "*~" -o -name "*.[oa]" -o -name "\#*\#" -o -name TAGS -o -name core -o -name "*.orig"`
