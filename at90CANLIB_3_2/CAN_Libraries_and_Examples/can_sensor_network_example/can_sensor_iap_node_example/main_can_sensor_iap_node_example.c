//******************************************************************************
//! @file $RCSfile: main_can_sensor_node_iap_example.c,v $
//!
//! Copyright (c) 2007 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief Reply 3 on-board sensor values after an master request.
//!        Re-programmable application thank to ATMEL Bootloader.
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
#define     Farflash_rd_byte(addr_byte)             \
            ({                                      \
                U32 addr = (U32)addr_byte;          \
                U8 result;                          \
                asm                                 \
                (                                   \
                    "out %2, %C1"     "\n\t"        \
                    "mov r31, %B1"    "\n\t"        \
                    "mov r30, %A1"    "\n\t"        \
                    "elpm"            "\n\t"        \
                    "mov %0, r0"      "\n\t"        \
                    : "=r" (result)                 \
                    : "r" (addr),                   \
                      "I" (_SFR_IO_ADDR(RAMPZ))     \
                    : "r0", "r30", "r31"            \
                );                                  \
                result;                             \
            })

//_____ D E F I N I T I O N S __________________________________________________
#define MY_ID_TAG      0x80       //-- 128 decimal

//#define BOOT_SIZE      ( (U16)(0x1FFF) )  //-- GCC Bootloader
#define BOOT_SIZE      ( (U16)(0x0FFF) )  //-- IAR Bootloader
#define BOOT_W_ADDRESS ( (U16)((((U32)FLASHEND) - BOOT_SIZE ) >> 1 ) )
//#define NNB_ADDRESS    ( (U32)(BOOT_W_ADDRESS + 0x94 + 6) )  //-- GCC Bootloader
//#define CRIS_ADDRESS   ( (U32)(BOOT_W_ADDRESS + 0x94 + 7) )  //-- GCC Bootloader
#define NNB_ADDRESS    ( (U32)((FLASHEND - 9 ) + 6 ) )  //-- IAR Bootloader
#define CRIS_ADDRESS   ( (U32)((FLASHEND - 9 ) + 7 ) )  //-- IAR Bootloader

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
    CLKPR = 0x80;  CLKPR = 0x00;  //-- Clock prescaler Reset       
    MCUSR = 0;      //-- Clear all reset flags - needed by IAP
    audio_init();
    led_init(); write_led(MY_ID_TAG);   //-- Display MY_ID_TAG on LEDs
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
//!    - the temperatue value (1 byte ),
//!    - the luminosity value (1 byte ),
//!    - the VCC-IN value     (2 bytes).
//! 3) IAP detection.
//!
//! @warning - Use DVK90CAN1 board,
//!          - can_init() must be perform before,
//!          - CAN_Bootloader_IAR Ver3 needed.
//!
//! @param  none
//!
//! @return  none
//!
//------------------------------------------------------------------------------
void send_sensor_values(void)
{
    U8  temp, expected_nnb;
    U8  iap_buffer[8];
    U8  sensor_buffer[8];
    U16 v_in;
    st_cmd_t iap_message;
    st_cmd_t sensor_message;

    U16 ack_jingle[] = { 100, 30, A7, 0 };

    // --- Init data and command for IAP detection
    expected_nnb = Farflash_rd_byte(NNB_ADDRESS);
    iap_message.pt_data = &iap_buffer[0];
    iap_message.ctrl.ide = 0;              //-- CAN 2.0A
    iap_message.dlc = 1;                   //-- Message with 1-byte data = NNB
    iap_message.id.std = 0x7FF & (((U16)(Farflash_rd_byte(CRIS_ADDRESS)))<< 4);
    iap_message.cmd = CMD_RX_DATA_MASKED;
    
    // --- Init data and command for REPLY-sensor message
    sensor_message.pt_data = &sensor_buffer[0];
    sensor_message.ctrl.ide = 0;            //-- CAN 2.0A
    sensor_message.dlc = 4;                 //-- Message with 4-byte data
    sensor_message.id.std = MY_ID_TAG;
    sensor_message.cmd = CMD_REPLY_MASKED;
    sensor_buffer[0] = (U8)(get_temperature());
    sensor_buffer[1] = get_luminosity();
    v_in = get_vin();
    sensor_buffer[2] = (U8)(v_in >> 8 );
    sensor_buffer[3] = (U8)(v_in      );

    // --- Enable IAP detection
    while(can_cmd(&iap_message) != CAN_CMD_ACCEPTED);
    // --- Enable REPLY for sensor message
    while(can_cmd(&sensor_message) != CAN_CMD_ACCEPTED);

    while(1)
    {       
        // --- Test REPLY completed
        temp = can_get_status(&sensor_message);
        if(temp == CAN_STATUS_COMPLETED)
        {
            // --- Re-prepare data for the reply
            sensor_buffer[0] = (U8)(get_temperature());
            sensor_buffer[1] = get_luminosity();
            v_in = get_vin();
            sensor_buffer[2] = (U8)(v_in >> 8 );
            sensor_buffer[3] = (U8)(v_in      );
            // --- Re-enable REPLY for sensor message
            while(can_cmd(&sensor_message) != CAN_CMD_ACCEPTED);
            // --- Acoustic acknowledge (must be short !)
            audio_play_song(ack_jingle);
        }

        // --- Test IAP detected
        temp = can_get_status(&iap_message);
        if(temp == CAN_STATUS_COMPLETED)
        {
            // --- Control of length & data
            if((iap_buffer[0] == expected_nnb) && (iap_message.dlc == 1))
            {
                Indirect_jump_to(BOOT_W_ADDRESS);   //-- Bye !
            }
            else
            {
                // --- Re-enable IAP detection
                while(can_cmd(&iap_message) != CAN_CMD_ACCEPTED);
            }
        }
    } // while(
}
