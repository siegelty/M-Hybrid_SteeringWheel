//******************************************************************************
//! @file $RCSfile: main_can_sensor_node_example.c,v $
//!
//! Copyright (c) 2007 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief Reply 3 on-board sensor values after an master request
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
#include "can_lib.h"
#include "adc_drv.h"
#include "led_drv.h"
#include "audio_drv.h"
#include "sensor_drv.h"
#include "rtc_drv.h"

//_____ M A C R O S ____________________________________________________________

//_____ D E F I N I T I O N S __________________________________________________
#define MY_ID_TAG 0x80       //- 128 decimal

//_____ D E C L A R A T I O N S ________________________________________________
extern void send_sensor_values(void);

//_____ F U N C T I O N S ______________________________________________________

//------------------------------------------------------------------------------
//  @fn main
//!
//! Core of "main_can_sensor_node_example.c".
//!
//! This program performs a response to an remote frame of a master.
//! The response is a data frame that contents (in the order) the local
//! temperature, the local luminosity and the local VCC values.
//!
//! @warning - Use DVK90CAN1 board,
//!          - Define CAN_BAUDRATE in "config.h".
//!
//! @param  none
//!
//! @return  Integer 0
//!
//------------------------------------------------------------------------------
int main (void)
{	
    CLKPR = 0x80;  CLKPR = 0x00;  // Clock prescaler Reset
    audio_init();
    led_init(); write_led(MY_ID_TAG);   //- Display MY_ID_TAG on LEDs
    can_init(0);
    send_sensor_values();
    while(1);
    return 0;
}

//------------------------------------------------------------------------------
//  @fn send_sensor_values
//!
//! An Identifier value (CAN2.0A) is given to DVK90CAN1 board (ex: 0x080).
//! 1) The first operation is to detect a CAN receive remote frame with
//!    ID == MY_ID_TAG.
//! 2) The second operation is to send a CAN data frame with the same ID and as
//!    data:
//!    - the temperatue value (1 byte )
//!    - the luminosity value (1 byte )
//!    - the VCC-IN value     (2 bytes).
//!
//! @warning - Use DVK90CAN1 board,
//!          - can_init() must be perform before
//!
//! @param  none
//!
//! @return  none
//!
//------------------------------------------------------------------------------
void send_sensor_values(void)
{
    U8  sensor_buffer[8];
    U16 temp;
    st_cmd_t sensor_message;

    U16 ack_jingle[] = { 100, 30, A7, 0 };

    // --- Init Reply data
    sensor_message.pt_data = &sensor_buffer[0];

    while(1)
    {
        // --- Prepare data for the reply
//        for(i=0; i<6; i++) temp = get_vin();  //_To stabilize the Aref
        sensor_buffer[0] = (U8)(get_temperature());
        sensor_buffer[1] = get_luminosity();
        temp = get_vin();
        sensor_buffer[2] = (U8)(temp >> 8 );
        sensor_buffer[3] = (U8)(temp      );
		
        // --- Auto-reply Command
        sensor_message.ctrl.ide = 0;            //- CAN 2.0A
        sensor_message.dlc = 4;                 //- Message with 4-byte data
        sensor_message.id.std = MY_ID_TAG;
        sensor_message.cmd = CMD_REPLY_MASKED;

        // --- Enable reply
        while(can_cmd(&sensor_message) != CAN_CMD_ACCEPTED);
        // --- Wait for Reply completed
        while(can_get_status(&sensor_message) == CAN_STATUS_NOT_COMPLETED);

        // --- Acoustic acknowledge 
        audio_play_song(ack_jingle);
    }
}
