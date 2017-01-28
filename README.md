# M-Hybrid_SteeringWheel

## How to program with AVR DUDE

### Compiling
1. Make sure avr-libc is installed 
2. Use command avr-gcc -mmcu=at90can128 -g <your_code>.c -o <Output file>

### Flashing Code
1. Plug in JTAG device
2. Power board with 12V
3. Compile (See compile section above)
4. run command avrdude -p c128 -P /dev/<GET YOUR USB DEVICE> -c jtag1 -U flash:w:<INSERT_YOUR_FILE>.hex 
  -p c128 is for the steering wheel
    - Go to http://www.nongnu.org/avrdude/user-manual/avrdude_4.html for other devices avrdude supports
  -c jtag1 specifies the programming device we currerntly use
  -P specifies the usb port... use ls /dev to find the port to use
  -U specfies the method of memory manipulation (flash), the type of manipulation (w) and the file to use
  
### Using the Makefile
1. Confirm that avr-libc, avr-gcc, avr-dude are installed using homebrew or whatever
2. Plug in the JTAG to the USB, the board to the JTAG programmer, and power the board with 12V
3. Go to the the avr-can-led folder 
4. Run "ls /dev" and find the USB port used for the JTAG
5. Update the USB variable in the makefile to what was found in step 3
6. To build, run "make build" 
7. Once you've build, run "make load" to flash the code to the board
8. You can also build in flash with the default make (just type "make") or make flash. 
