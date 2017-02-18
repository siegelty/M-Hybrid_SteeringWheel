#include "libraries/lib_mcu/can/config.h"
#include "libraries/lib_mcu/can/can_lib.h"
// #include <util/delay.h>


// //_____ I N C L U D E S ________________________________________________________
// #include "libraries/lib_mcu/compiler.h"
// #include <avr/io.h>
// #include <avr/interrupt.h>
// #include "libraries/lib_mcu/at90can_drv.h"
// #include "libraries/lib_board/dvk90can1_board.h"

// //_____ M A C R O S ____________________________________________________________

// //_____ D E F I N I T I O N S __________________________________________________

//     // -------------- MCU LIB CONFIGURATION
// #define FOSC           16000        // 8 MHz External cristal
// #define F_CPU          (FOSC*1000) // Need for AVR GCC

//     // -------------- CAN LIB CONFIGURATION
// #define CAN_BAUDRATE   1000        // in kBit
// //#define CAN_BAUDRATE   CAN_AUTOBAUD

//     // -------------- MISCELLANEOUS
//     // Using TIMER_2 as RTC
// #define USE_TIMER8       TIMER8_2
// //#define RTC_TIMER        2      // See "board.h"
// //#define RTC_CLOCK        32     // See "board.h"

#define MY_ID_TAG   0x69

char state = 0x00;

void send_val(U8 data);

void updateState(int pin, int toSet) {
    if (toSet) {
        state |= (1 << pin);
    } else {
        state &= ~(1 << pin);
    }
}

ISR(INT0_vect) {
    updateState(0, ~(PIND & (1 << 0))); // Update state of the pin 0 based on PIND
    send_val(state); // Send new value of state
    
}

ISR(INT1_vect) {
    updateState(1, ~(PIND & (1 << 1))); // Update state of the pin 0 based on PIND
    send_val(state); // Send new value of state
    
}

ISR(INT2_vect) {
    updateState(2, ~(PIND & (1 << 2))); // Update state of the pin 0 based on PIND
    send_val(state); // Send new value of state
    
}

void initInterrupt0(void) {
    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC00);
}

void initInterrupt1(void) {
    EIMSK |= (1 << INT1);
    EICRA |= (1 << ISC10);
}

void initInterrupt2(void) {
    EIMSK |= (1 << INT2);
    EICRA |= (1 << ISC20);
}

void send_val(U8 data) {
    st_cmd_t toSend;

    toSend.pt_data = &data; // Pointer to first data byte
    toSend.id.std = MY_ID_TAG; // ID of message
    toSend.dlc = 1; // Num bytes
    toSend.cmd = CMD_TX_DATA; // Type of can command

    while(can_cmd(&toSend) != CAN_CMD_ACCEPTED);
    while(can_get_status(&toSend) == CAN_STATUS_NOT_COMPLETED);
}


int main() {
    PORTD |= (1 << PD2);
    DDRD &= ~(1 << DDD2);
    initInterrupt0();
    initInterrupt1();
    initInterrupt2();


    sei();



    can_init(0x0);
    

    while(1) {
      
    }
    return 0;
}
