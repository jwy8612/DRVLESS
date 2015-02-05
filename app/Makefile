

OBJS =
OBJS_SAMPLE =

CC    := $(CROSS_COMPILE)gcc
AR    := $(CROSS_COMPILE)ar
STRIP := $(CROSS_COMPILE)strip


CFLAGS += -I$(GNUHF_LIBC_INC) -I./inc/ -I../lib/inc
OBJS += ./src/run.o ./src/func.o ./src/com_get.o
LDFLAGS += -L$(WORK_LIB_PATH) -lmpi -lpthread

FORCE_STATIC := --static
#FORCE_STATIC := 

LOCAL_MODULE  := run

TARGET_MODULE := $(WORK_OUTPUT_PATH)/bin/run

.PHONY: all
all: $(LOCAL_MODULE)

.PHONY: clean
clean: 
	@rm -f $(OBJS) $(LOCAL_MODULE) $(TARGET_MODULE)

$(OBJS):%o:%c
	@$(CC) -c $(CFLAGS) $< -o $@

$(LOCAL_MODULE): $(OBJS)
	@echo "--Compiling '$(LOCAL_MODULE)' ..."
	@$(CC) $^ $(LDFLAGS) -o $@ $(FORCE_STATIC)
	@$(STRIP) $@
	@cp -f $@ $(TARGET_MODULE)

