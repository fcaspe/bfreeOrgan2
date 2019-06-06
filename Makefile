
TARGET=main
STM_LIB_DIR=src/engine/libs

###################################################

CC=arm-none-eabi-g++
LD=arm-none-eabi-g++
AR=arm-none-eabi-ar
AS=arm-none-eabi-as
CP=arm-none-eabi-objcopy
OD=arm-none-eabi-objdump
SE=arm-none-eabi-size
SF=st-flash

###################################################

SRCDIR = src/ \
		 $(STM_LIB_DIR)/StdPeriph_Driver/src \
		 $(STM_LIB_DIR)/syscalls \
		 $(STM_LIB_DIR)/USB_Device/Class/midi/src \
		 $(STM_LIB_DIR)/USB_Device/Core/src \
		 $(STM_LIB_DIR)/USB_OTG/src \
		 src/engine \
		 src/engine/hardware \
		 src/engine/hardware/output_discovery \
		 src/flexynth \
		 src/flexynth/components \
		 src/engine/midi \
		 src/engine/midi/midi_usb

vpath %.c $(SRCDIR)		 
vpath %.cpp $(SRCDIR)

STARTUPDIR = src/engine/startup
vpath %.S $(STARTUPDIR)

INC_DIRS = $(STM_LIB_DIR)/StdPeriph_Driver/inc
INC_DIRS += $(STM_LIB_DIR)/CMSIS/device
INC_DIRS += $(STM_LIB_DIR)/CMSIS/core
INC_DIRS += $(STM_LIB_DIR)/USB_Device/Class/midi/inc
INC_DIRS += $(STM_LIB_DIR)/USB_Device/Core/inc
INC_DIRS += $(STM_LIB_DIR)/USB_OTG/inc
INC_DIRS += src/engine
INC_DIRS += src/engine/startup
INC_DIRS += src/engine/hardware
INC_DIRS += src/engine/hardware/output_discovery
INC_DIRS += src/engine/midi
INC_DIRS += src/engine/midi/midi_usb
INC_DIRS += src/engine/midi/midi_usb/Conf
INC_DIRS += src/flexynth
INC_DIRS += src/flexynth/components

INCLUDE = $(addprefix -I,$(INC_DIRS))

LINK_SCRIPT = src/engine/LinkerScript.ld

###################################################

CFLAGS  = -g -O2 -Wall -T$(LINK_SCRIPT)
CFLAGS += -mthumb -march=armv7e-m
CFLAGS += -DUSE_STDPERIPH_DRIVER
CFLAGS += -DUSE_MIDI

###################################################

SRCS =  main.c \
		engine_discovery.cpp \
		env_ar.cpp \
		synth_organ.cpp \
		voice_organ.cpp \
		efx_chorus.cpp \
		cc_drawbars.cpp \
		midi_usb.cpp \
		output_discovery.cpp

SRCS += cs43l22.c

SRCS += usb_bsp.c \
		usbd_usr.c \
		usbd_desc.c \
		usbd_core.c \
		usbd_ioreq.c \
		usbd_req.c \
		usbd_midi_core.c \
		usb_core.c \
		usb_dcd.c \
		usb_dcd_int.c \
		usb_hcd.c \
		usb_hcd_int.c \
		usb_otg.c

SRCS += stm32f4xx_rcc.c \
	stm32f4xx_gpio.c \
	stm32f4xx_dma.c \
	stm32f4xx_tim.c \
	stm32f4xx_spi.c \
	stm32f4xx_adc.c \
	stm32f4xx_exti.c \
	stm32f4xx_i2c.c \
	system_stm32f4xx.c \
	misc.c \
	syscalls.c \
	startup_stm32f4xx.S

###################################################

.PHONY: $(TARGET)

$(TARGET): $(TARGET).elf

$(TARGET).elf: $(SRCS)
	$(CC) $(INCLUDE) $(CFLAGS) $^ -o $@ 
	$(CP) -O ihex $(TARGET).elf $(TARGET).hex
	$(CP) -O binary $(TARGET).elf $(TARGET).bin
	mkdir bin
	mv $(TARGET).* ./bin/

clean:
	rm -f *.o $(TARGET).elf $(TARGET).hex $(TARGET).bin
	rm -r bin

objdump:
	$(OD) -d $	$(TARGET).elf >dump.lst

flash: 
	$(SF) write $(TARGET).bin 0x08000000
