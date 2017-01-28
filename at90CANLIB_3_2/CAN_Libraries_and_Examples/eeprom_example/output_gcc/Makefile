###############################################################################
# Makefile for the project eeprom_example_gcc
###############################################################################

## General Flags
PROJECT = eeprom_example_gcc
MCU = at90can128
TARGET = eeprom_example_gcc.elf
CC = avr-gcc.exe

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2  -O0 -funsigned-char -funsigned-bitfields
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=eeprom_example_gcc.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"D:\Work_space\01-Avr8\03-work_jacq_WinAVR\work_space\eeprom_example\." -I"D:\Work_space\01-Avr8\03-work_jacq_WinAVR\work_space\eeprom_example\..\libraries\lib_board" -I"D:\Work_space\01-Avr8\03-work_jacq_WinAVR\work_space\eeprom_example\..\libraries\lib_mcu" -I"D:\Work_space\01-Avr8\03-work_jacq_WinAVR\work_space\eeprom_example\..\libraries\lib_mcu\eeprom" 

## Objects that must be built in order to link
OBJECTS = main_eeprom_example.o eeprom_lib.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) eeprom_example_gcc.hex eeprom_example_gcc.eep eeprom_example_gcc.lss size

## Compile
main_eeprom_example.o: ../main_eeprom_example.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

eeprom_lib.o: ../../libraries/lib_mcu/eeprom/eeprom_lib.c
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
	-rm -rf $(OBJECTS) eeprom_example_gcc.elf dep/* eeprom_example_gcc.hex eeprom_example_gcc.eep eeprom_example_gcc.lss eeprom_example_gcc.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

