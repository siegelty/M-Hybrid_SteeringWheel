//******************************************************************************
//! @file $RCSfile: main_can_bootloader_example.c,v $
//!
//! Copyright (c) 2007 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief: This application performs a CAN boot loader for AT90CAN128/64/32
//!         with aut-bitrate, IAP (In Application Programming) and API
//!         (Application Programming Interface).
//!
//! This boot loader is an example (application note like), the customer
//! needs to appropriate this code to fit exactly with its application and
//! its environment. Some specific calls (or jump) in customer application
//! are necessary to use correctly the IAP and API features.
//!
//! This file can be parsed by Doxygen for automatic documentation generation.
//! This file has been validated with AVRStudio-413528/WinAVR-20070122.
//!
//! @version $Revision: 3.20 $ $Name: jtellier $
//!
//! @todo
//! @bug
//******************************************************************************

//_____  I N C L U D E S _______________________________________________________
#include "config.h"
#include "can_isp_protocol.h"
#include "isp_lib.h"
#include "reduced_can_lib.h"
#include "can_drv.h"
#include "flash_boot_lib.h"
#include "flash_api_lib.h"


//_____ M A C R O S ____________________________________________________________

//_____ D E F I N I T I O N S __________________________________________________

//_____ D E C L A R A T I O N S ________________________________________________

//_____ F U N C T I O N S ______________________________________________________

//------------------------------------------------------------------------------
//  @fn main
//!
//! Core of "main_can_boot_loader.c".
//!
//! @brief: This application performs a CAN boot loader for AT90CAN128/64/32
//!         with aut-bitrate, IAP (In Application Programming) and API
//!         (Application Programming Interface).
//!
//! @warning: Don't forget to full locate the boot loader in "Boot Loader
//!           Flash Section".
//!           xxx_HWCB defined in "board.h" file
//!
//! @param none.
//!
//! @return Integer 0
//!
//------------------------------------------------------------------------------
int main (void)
{
    U8      u8_temp;
    U8      hwcb = FALSE;
    Bool    auto_b = 1;

//! --- First of all, disabling the Global Interrupt
    Disable_interrupt();

//! --- If comming from RESET then test of the HardWare Condition Bit
    if (MCUSR != 0)
    {
        //- Clear all reset flags
        MCUSR = 0;
        //- Get HWCB (HardWare Condition Bit)
        HWCB_PORT_DIR &= ~(1<<HWCB_INPUT_PIN);      // HWCB pin in input
        if (HWCB_PULLUP_ON == 1)
        {
            HWCB_PORT_OUT |= (1<<HWCB_INPUT_PIN);   // Pull-up on HWCB
        }
        hwcb = HWCB_PORT_IN;
        if (HWCB_ACTIVE_LEVEL == 0)
        {
            hwcb = ((~hwcb)&(1<<HWCB_INPUT_PIN));
        }

        //- Start application else start boot loader
        u8_temp = get_conf_byte(BSB);

        if ( ((u8_temp!=BSB_DEFAULT)&&(hwcb==0)) || ((u8_temp==BSB_DEFAULT)&&(hwcb!=0)) )
        {
            isp_jump_to ( (((U16)get_conf_byte(SA_H))<<8) | ((U16)get_conf_byte(SA_L)) );   // Start application
        }
        // else go to BOOT LOADER
    }

//! --- Open CAN communication

    can_communication_opened = 0;   // Not opened

    //- Pull-up on TxCAN & RxCAN one by one to use bit-addressing
    CAN_PORT_DIR &= ~(1<<CAN_INPUT_PIN );
    CAN_PORT_DIR &= ~(1<<CAN_OUTPUT_PIN);
    CAN_PORT_OUT |=  (1<<CAN_INPUT_PIN );
    CAN_PORT_OUT |=  (1<<CAN_OUTPUT_PIN);

    while (1)
    {
        auto_b = ((~auto_b)&0x01);      // Flip of "auto_b"

        //- Wait until activity on RxCAN
        while ((CAN_PORT_IN & (1<<CAN_INPUT_PIN)) != 0);

        //- Reset CAN peripheral
	    Can_reset();

	    //- Set CAN Bit-timming
        if (get_conf_byte(EB) != 0xFF)
        {
            CANBT1 = get_conf_byte(BTC1);       // Correction: 11 MArch 2007 - JT
            CANBT2 = get_conf_byte(BTC2);       //  idem
            CANBT3 = get_conf_byte(BTC3);       //  idem
        }
        else
        //- Loop on auto-bitrate until it is performed
        {
            while ((Can_bit_timing(auto_b))==0);        // c.f. macro in "can_drv.h"
        }

	    //- Enable CAN peripheral
        can_clear_all_mob();                        // c.f. function in "can_drv.c"
        Can_enable();                               // c.f. macro in "can_drv.h"

//! --- CAN ISP protocol execution
        //- Initialization of CAN ISP PROTOCOL only when CAN communication NOT opened
        if(!can_communication_opened) can_isp_protocol_init();

        //- Exit with "0" only if CAN hardware error occurs
        while(can_isp_protocol_task());
    }
    // End of "main"
    return(0);

}

