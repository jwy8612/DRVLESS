
OBJS =


CC    := $(CROSS_COMPILE)gcc
AR    := $(CROSS_COMPILE)ar
STRIP := $(CROSS_COMPILE)strip


CFLAGS  +=

CFLAGS  += -I$(GNUHF_LIBC_INC) -I./../../inc -Wall
LDFLAGS  += -lm
EXTRA_LD_FLAGS  +=

OBJS += ./order_send_api.o

STATIC_LIBS  := libosd.a
SHARED_LIBS  := libosd.so


.PHONY: all
all: $(STATIC_LIBS) $(SHARED_LIBS)

.PHONY: clean
clean: 
	@rm -f $(STATIC_LIBS) $(SHARED_LIBS) $(OBJS) $(WORK_OUTPUT_PATH)/lib/libosd.*

$(OBJS):%o:%c
	@$(CC) -c $(LDFLAGS) $(CFLAGS) $< $(EXTRA_LD_FLAGS) $(FORCE_STATIC) -o $@

$(STATIC_LIBS): $(OBJS)
	@echo "--Compiling 'static libs' ..."
	@$(AR) -r $@ $^
	cp -f $@ $(WORK_OUTPUT_PATH)/lib

$(SHARED_LIBS): $(OBJS)
	@echo "--Compiling 'shared libs' ..."
	@$(CHECK_SDK_LIB_DIR)
	@$(CC) $^ -fPIC -shared -o $@
	$(STRIP) $@
	cp -f $@ $(WORK_OUTPUT_PATH)/lib

