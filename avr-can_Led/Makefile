# Add files for compiler
TARGET=can_test.c ./libraries/lib_mcu/can/can_lib.c ./libraries/lib_mcu/can/can_drv.c 

# Change based on computer
USB=tty.usbserial-A902ZULV 

# Change for output file
OUTPUT=wheel.hex

build: $(TARGET)
	avr-gcc -mmcu=at90can128 -g $(TARGET) -o $(OUTPUT)

load: $(OUTPUT)
	avrdude -p c128 -P /dev/$(USB) -c jtag1 -U flash:w:$(OUTPUT) 

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OUTPUT)