###############################################################################
# Makefile for the project can_sensor_iap_node_example_gcc
###############################################################################

## General Flags
PROJECT = can_sensor_iap_node_example_gcc
MCU = at90can128
TARGET = can_sensor_iap_node_example_gcc.elf
CC = avr-gcc.exe

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -O0
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS += 


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0


## Objects that must be built in order to link
OBJECTS = main_can_sensor_iap_node_example.o can_drv.o can_lib.o adc_drv.o led_drv.o audio_drv.o sensor_drv.o rtc_drv.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) can_sensor_iap_node_example_gcc.hex can_sensor_iap_node_example_gcc.eep size

## Compile
main_can_sensor_iap_node_example.o: ../main_can_sensor_iap_node_example.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

can_drv.o: ../../../libraries/lib_mcu/can/can_drv.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

can_lib.o: ../../../libraries/lib_mcu/can/can_lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

adc_drv.o: ../../../libraries/lib_mcu/adc/adc_drv.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

led_drv.o: ../../../libraries/lib_board/led/led_drv.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

audio_drv.o: ../../../libraries/lib_board/audio/audio_drv.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sensor_drv.o: ../../../libraries/lib_board/sensor/sensor_drv.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

rtc_drv.o: ../../../libraries/lib_board/rtc/rtc_drv.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) can_sensor_iap_node_example_gcc.elf dep/* can_sensor_iap_node_example_gcc.hex can_sensor_iap_node_example_gcc.eep

## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

