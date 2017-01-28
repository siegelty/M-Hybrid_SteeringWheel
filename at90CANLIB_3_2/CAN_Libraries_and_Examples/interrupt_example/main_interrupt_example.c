//******************************************************************************
//! @file $RCSfile: main_interrupt_example.c,v $
//!
//! Copyright (c) 2007 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief Test for interrupt management with AVR GCC.
//!
//! This file can be parsed by Doxygen for automatic documentation generation.
//! This file has been validated with AVRStudio-413528/WinAVR-20070122.
//!
//! @version $Revision: 3.20 $ $Name: jtellier $
//!
//! @todo
//! @bug
//******************************************************************************

//_____ I N C L U D E S ________________________________________________________
#include "config.h"
#include "led_drv.h"
#include "rtc_drv.h"
#include "key_drv.h"

//_____ M A C R O S ____________________________________________________________

//_____ D E F I N I T I O N S __________________________________________________

//_____ D E C L A R A T I O N S ________________________________________________
extern volatile U32 rtc_tics;
extern volatile U16 rtc_milliseconds;
extern volatile U8  rtc_seconds;
extern volatile U8  rtc_minutes;
extern volatile U8  rtc_hours;
extern volatile U8  rtc_days;
extern Bool rtc_running;

//_____ F U N C T I O N S ______________________________________________________

//------------------------------------------------------------------------------
//  @fn main
//!
//! Core of "main_interrupt_example.c".
//!
//! Display in decimal on DVK90CAN1 board the seconds, minutes, hours & days
//! given by the Unit declared as RTC.
//!     -   NO   key = seconds
//!     - NORTH  key = minutes
//!     - CENTER key = hours
//!     - SOUTH  key = days
//!
//! @warning Care on "#define"'s in "config.h".
//!
//! @param none.
//!
//! @return Integer 0
//!
//------------------------------------------------------------------------------
int main (void)
{
U8 temp1, temp2;

    asm("cli");

//--- I N I T
    // --- Clock prescaler Reset
    CLKPR = 0x80;  CLKPR = 0x00;

    // --- Init LED's & Keys
    led_init();  key_init();

    // --- Init "SECOND COUNTER"
    rtc_int_init();
   
    
//--- C O R E   O F   T H E    M A I N
    while(1)
    {
        //- Get display order
        switch (get_key_code())
        {
            case BUTTON_S_CODE:           // South for days display
                temp1 = rtc_days;
                break;
            case BUTTON_C_CODE:           // Center for hours display
                temp1 = rtc_hours;
                break;
            case BUTTON_N_CODE:           // North for minutes display
                temp1 = rtc_minutes;
                break;
            default:                      // Nothing for seconds display
                temp1 = rtc_seconds;
                break;
                
        }

        //- Display in decimal
        temp2 = temp1 % 10;
        temp2 |= (temp1/10)<<4;
        write_led(temp2);
    }
    return 0;
}
