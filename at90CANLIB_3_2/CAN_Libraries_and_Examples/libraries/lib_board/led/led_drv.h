//******************************************************************************
//! @file $RCSfile: led_drv.h,v $
//!
//! Copyright (c) 2007 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the prototypes and the macros of the
//!        low level functions (drivers) of:
//!             - On-board LEDs (PortA)
//!             - for DVK90CAN1 board
//! These macros need some #define's of "dvk90can1_board.h" file.
//!
//! This file can be parsed by Doxygen for automatic documentation generation.
//! This file has been validated with AVRStudio-413528/WinAVR-20070122.
//!
//! @version $Revision: 3.20 $ $Name: jtellier $
//!
//! @todo
//! @bug
//******************************************************************************

#ifndef _LED_DRV_
#define _LED_DRV_

//_____ I N C L U D E S ________________________________________________________

#include "config.h"

//_____ D E F I N I T I O N S __________________________________________________

//_____ M A C R O S ____________________________________________________________

//------------------------------------------------------------------------------
//! Set LED. Setting LED means ligh "ON" a LED referenced by its number.
//!
//! @param led_number   Position of the LED.
//!
#define Set_led(led_number) write_led_number(led_number,1)

//------------------------------------------------------------------------------
//! Clear LED. Clearing LED means ligh "OFF" a LED referenced by its number.
//!
//! @param led_number   Position of the LED.
//!
#define Clr_led(led_number) write_led_number(led_number,0)

//_____ D E C L A R A T I O N S ________________________________________________

//_____ P R O T O T Y P E S ____________________________________________________

//------------------------------------------------------------------------------
//  @fn led_init
//!
//! On-board LEDs initialization. Initialize I/O lines for LEDs & clear LEDs. 
//!
//! @warning  none
//!
//! @param  none
//!
//! @return  none
//!
extern void led_init(void);

//------------------------------------------------------------------------------
//  @fn write_led
//!
//! On-board LEDs writing byte. Write hexa value on LEDs display.
//!
//! @warning  none
//!
//! @param  led_display = Hexa display value
//!
//! @return (none)
//!
extern void write_led(U8 led_display);

//------------------------------------------------------------------------------
//  @fn swap_nibble_led
//!
//! Swap nibbles of on-board LEDs. Exchange ls-nibble and ms-nibble.
//!
//! @warning  none
//!
//! @param  none
//!
//! @return  none
//!
extern void swap_nibble_led(void);

//------------------------------------------------------------------------------
//  @fn toggle_led
//!
//! On-board LEDs toggle. Toggle value of each LED.
//!
//! @warning  none
//!
//! @param  none
//!
//! @return  none
//!
extern void toggle_led(void);

//------------------------------------------------------------------------------
//  @fn write_led_number
//!
//! Set or clear one LED of the on-board LEDs. See "Set_led(U8 led_number)"
//! and "Set_led(U8 led_number)" macros.
//!
//! @warning 0 <= "LED number" param <= 7, led_level >=2.
//!
//! @param  led_number = Position of the LED.
//! @param  led_level  = Bool: 1=set, 0=clear.
//!
//! @return  range error..
//!             FALSE: led_number >= 8 or led_level >=2,
//!             TRUE:  In range
//!
extern U8   write_led_number(U8 led_number, Bool led_level);
    
//______________________________________________________________________________

#endif  // _LED_DRV_

