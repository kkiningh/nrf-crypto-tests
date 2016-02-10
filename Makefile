# First target is default
.PHONY: all
all: debug

# Project
TARGET   := test.hex
SRC      += src/test.c src/uECC.c src/app_error.c src/SEGGER_RTT.c src/SEGGER_RTT_printf.c
INC      += src

# SDK
SDK_VERSION := 11.0.0
SDK_PATH    := lib/nRF51_SDK_v$(SDK_VERSION)
SDK_SRC     += $(SDK_PATH)/components/drivers_nrf/rng/nrf_drv_rng.c \
		       $(SDK_PATH)/components/drivers_nrf/rtc/nrf_drv_rtc.c \
			   $(SDK_PATH)/components/drivers_nrf/clock/nrf_drv_clock.c \
			   $(SDK_PATH)/components/libraries/fifo/app_fifo.c \
			   $(SDK_PATH)/components/drivers_nrf/common/nrf_drv_common.c \
			   $(SDK_PATH)/components/drivers_nrf/delay/nrf_delay.c \
			   $(SDK_PATH)/components/libraries/util/app_util_platform.c \
			   $(SDK_PATH)/components/libraries/sha256/sha256.c

SDK_INC     += $(SDK_PATH)/components/drivers_nrf/rng/ \
			   $(SDK_PATH)/components/drivers_nrf/rtc/ \
			   $(SDK_PATH)/components/drivers_nrf/clock/ \
			   $(SDK_PATH)/components/drivers_nrf/delay/ \
			   $(SDK_PATH)//components/drivers_nrf/hal/ \
			   $(SDK_PATH)/components/softdevice/s130/headers/ \
			   $(SDK_PATH)/components/drivers_nrf/config \
			   $(SDK_PATH)/components/drivers_nrf/common/ \
			   $(SDK_PATH)/components/libraries/fifo/ \
			   $(SDK_PATH)/components/libraries/sha256/

# Softdevice
#SD_VERSION  := s130

# Project specific flags
ASMFLAGS += -D__HEAP_SIZE=0   # Don't use any heap
CFLAGS += -fomit-frame-pointer

include mk/sdk.mk
include mk/softdevice.mk
include mk/Makefile.mk
