CROSS_COMPILE ?= arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy

CFLAGS += -Istm32/include -mcpu=cortex-m0 -mthumb \
	-DSTM32L0 -DBIGCLOWN -I../libopencm3/include \
	-Iplatforms/stm32

LDFLAGS = -lopencm3_stm32l0 -Wl,--defsym,_stack=0x20005000 \
	-Wl,-T,platforms/stm32/stm32l083cz.ld -nostartfiles -lc -lnosys \
	-Wl,-Map=mapfile -mthumb -mcpu=cortex-m0 -Wl,-gc-sections \
	-L../libopencm3/lib

VPATH += platforms/stm32

SRC += 	cdcacm.c	\
	usbuart.c	\
	serialno.c	\
	timing.c	\
	timing_stm32.c	\

all:	blackmagic.bin

host_clean:
	-$(Q)$(RM) blackmagic.bin
