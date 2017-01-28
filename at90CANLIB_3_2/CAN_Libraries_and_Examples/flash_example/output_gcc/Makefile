###############################################################################
# Makefile for the project flash_example_gcc
###############################################################################

## General Flags
PROJECT = flash_example_gcc
MCU = at90can128
TARGET = flash_example_gcc.elf
CC = avr-gcc.exe

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -Os -funsigned-char -funsigned-bitfields
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=flash_example_gcc.map
LDFLAGS += -Wl,-section-start=.text=0x1e000


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"D:\Work_space\01-Avr8\03-work_jacq_WinAVR\work_space\flash_example\." -I"D:\Work_space\01-Avr8\03-work_jacq_WinAVR\work_space\flash_example\..\libraries\lib_board" -I"D:\Work_space\01-Avr8\03-work_jacq_WinAVR\work_space\flash_example\..\libraries\lib_mcu" -I"D:\Work_space\01-Avr8\03-work_jacq_WinAVR\work_space\flash_example\..\libraries\lib_mcu\flash" 

## Objects that must be built in order to link
OBJECTS = main_flash_example.o flash_boot_drv.o flash_boot_lib.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) flash_example_gcc.hex flash_example_gcc.eep flash_example_gcc.lss size

## Compile
main_flash_example.o: ../main_flash_example.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

flash_boot_drv.o: ../../libraries/lib_mcu/flash/flash_boot_drv.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

flash_boot_lib.o: ../../libraries/lib_mcu/flash/flash_boot_lib.c
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
	-rm -rf $(OBJECTS) flash_example_gcc.elf dep/* flash_example_gcc.hex flash_example_gcc.eep flash_example_gcc.lss flash_example_gcc.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

