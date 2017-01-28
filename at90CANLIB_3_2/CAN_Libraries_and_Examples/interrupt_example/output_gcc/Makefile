###############################################################################
# Makefile for the project interrupt_example_gcc
###############################################################################

## General Flags
PROJECT = interrupt_example_gcc
MCU = at90can128
TARGET = interrupt_example_gcc.elf
CC = avr-gcc.exe

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2   -Os -funsigned-char -funsigned-bitfields
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=interrupt_example_gcc.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"D:\Work_space\01-Avr8\03-work_jacq_WinAVR\work_space\interrupt_example\." -I"D:\Work_space\01-Avr8\03-work_jacq_WinAVR\work_space\interrupt_example\..\libraries\lib_mcu" -I"D:\Work_space\01-Avr8\03-work_jacq_WinAVR\work_space\interrupt_example\..\libraries\lib_board" -I"D:\Work_space\01-Avr8\03-work_jacq_WinAVR\work_space\interrupt_example\..\libraries\lib_board\key" -I"D:\Work_space\01-Avr8\03-work_jacq_WinAVR\work_space\interrupt_example\..\libraries\lib_board\led" -I"D:\Work_space\01-Avr8\03-work_jacq_WinAVR\work_space\interrupt_example\..\libraries\lib_board\rtc" -I"D:\Work_space\01-Avr8\03-work_jacq_WinAVR\work_space\interrupt_example\..\libraries\lib_mcu\timer" 

## Objects that must be built in order to link
OBJECTS = main_interrupt_example.o key_drv.o led_drv.o rtc_drv.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) interrupt_example_gcc.hex interrupt_example_gcc.eep interrupt_example_gcc.lss size

## Compile
main_interrupt_example.o: ../main_interrupt_example.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

key_drv.o: ../../libraries/lib_board/key/key_drv.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

led_drv.o: ../../libraries/lib_board/led/led_drv.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

rtc_drv.o: ../../libraries/lib_board/rtc/rtc_drv.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) interrupt_example_gcc.elf dep/* interrupt_example_gcc.hex interrupt_example_gcc.eep interrupt_example_gcc.lss interrupt_example_gcc.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

