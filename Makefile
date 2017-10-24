CC = /home/dakota/staging_dir/toolchain-arm_cortex-a7_gcc-4.8-linaro_uClibc-1.0.14_eabi/bin/arm-openwrt-linux-uclibcgnueabi-gcc
CFLAGS = -Wall -Os -pipe -march=armv7-a -mtune=cortex-a7 -fno-caller-saves -fhonour-copts -Wno-error=unused-but-set-variable -Wno-error=unused-result -mfloat-abi=soft

BUILD_OPTIMIZED=n
BUILD_DEBUG=y
BUILD_STATIC=y

STAGING_DIR=/home/dakota/staging_dir/target-arm_cortex-a7_uClibc-1.0.14_eabi

TARGET = libcc_dbg.so
LDFLAGS= -L/home/dakota/staging_dir/target-arm_cortex-a7_uClibc-1.0.14_eabi/usr/lib -L/home/dakota/staging_dir/target-arm_cortex-a7_uClibc-1.0.14_eabi/lib -L/home/dakota/staging_dir/toolchain-arm_cortex-a7_gcc-4.8-linaro_uClibc-1.0.14_eabi/usr/lib -L/home/dakota/staging_dir/toolchain-arm_cortex-a7_gcc-4.8-linaro_uClibc-1.0.14_eabi/lib

ifeq (${BUILD_DEBUG}, y)
CFLAGS+= -g
endif

SOURCE= \
        cc_dbg.c
		
OBJS = $(SOURCE:%.c=obj/%.o)

all: $(TARGET) 

$(TARGET): $(OBJS)
	$(CC) -fPIC -shared -o $@ $(OBJS) $(LDFLAGS)

cc_dbg: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)
	./test_cc_dbg.sh

obj/%.o:%.c
	@mkdir -p obj
	$(CC) -fPIC $(CFLAGS) -c $< -o $@ 

install:
	cp -f $(TARGET) /home/dakota/package/creatcomm/creatcomm-huawei-config-adp/files
clean:
	rm -fr obj $(OBJS) $(TARGET)