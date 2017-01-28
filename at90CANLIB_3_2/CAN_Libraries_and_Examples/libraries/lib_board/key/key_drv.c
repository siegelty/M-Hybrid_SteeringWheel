//******************************************************************************
//! @file $RCSfile: key_drv.c,v $
//!
//! Copyright (c) 2007 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the low level functions (drivers) of:
//!             - audio transducer
//!             - for DVK90CAN1 board
//! These routines need some #define's of "dvk90can1_board.h" file.
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
#include "key_drv.h"

//_____ M A C R O S ____________________________________________________________

//_____ D E F I N I T I O N S __________________________________________________

//_____ D E C L A R A T I O N S ________________________________________________

//------------------------------------------------------------------------------
//  @fn key_init
//!
//! On-board keyboard initialization. Initialize I/O lines for keys. 
//!
//! @warning Set correct definition for KEY_CENTER.
//!
//! @param  none
//!
//! @return  none
//!
//------------------------------------------------------------------------------
void key_init(void)
{
    //-- Init port pins
    KEY_PORT_DIR &= ~(1<<KEY_NORTH);
    KEY_PORT_DIR &= ~(1<<KEY_SOUTH);
    KEY_PORT_DIR &= ~(1<<KEY_WEST);
    KEY_PORT_DIR &= ~(1<<KEY_EAST);
#ifdef KEY_CENTER_PORT_DIR
    KEY_CENTER_PORT_DIR &= ~(1<<KEY_CENTER_IN);
#else   
    KEY_PORT_DIR &= ~(1<<KEY_CENTER);
#endif
    //-- Pull_up on pins
    KEY_PORT_OUT |= (1<<KEY_NORTH);
    KEY_PORT_OUT |= (1<<KEY_SOUTH);
    KEY_PORT_OUT |= (1<<KEY_WEST);
    KEY_PORT_OUT |= (1<<KEY_EAST);
#ifdef KEY_CENTER_PORT_OUT
    KEY_CENTER_PORT_OUT |= (1<<KEY_CENTER_IN);
#else   
    KEY_PORT_OUT |= (1<<KEY_CENTER);
#endif
}

//------------------------------------------------------------------------------
//  @fn get_key
//!
//! Get on-board keyboard value. Refer to key_drv.h and board.h.
//!
//! @warning Set correct definition for KEY_CENTER.
//!
//! @param  none
//!
//! @return  keyboard value
//!
//------------------------------------------------------------------------------
U8 get_key(void)
{
    //-- See "board.h" for keys (push-buttons) definition:
    U8 key_0;
    U8 key_1;
    
    //-- Read 3 times to remove overshoots
    key_0 = KEY_MASK & ((~KEY_PORT_IN) & (~KEY_PORT_IN) & (~KEY_PORT_IN));
    key_1 = 0;

#if (KEY_CENTER_PORT_IN && KEY_CENTER_IN)
    //-- Remove KEY_CENTER bit
    key_0 &= ~(1<<KEY_CENTER);
    //-- Read 3 times to remove overshoots and mask
    key_1 = KEY_CENTER_IN & ((~KEY_CENTER_PORT_IN) & (~KEY_CENTER_PORT_IN) & (~KEY_CENTER_PORT_IN));
    Key_1 = ((key_1>>KEY_CENTER_IN)<<KEY_CENTER);
#endif
    key_0 |= key_1;
    return key_0;
}

//------------------------------------------------------------------------------
//  @fn get_key_code
//!
//! Return a (compressed) coding of keys. Refer to key_drv.h and board.h.
//!
//! @warning Set correct definition for KEY_CENTER.
//!
//! @param  keyboard value
//!
//! @return  keyboard compressed value
//!
//------------------------------------------------------------------------------
U8 get_key_code(void)
{
    //-- See "board.h" for keys (push-buttons) definition:
    U8 key_0;
    U8 key_1;
    
    //-- Read 3 times to remove overshoots
    key_0 = KEY_MASK & ((~KEY_PORT_IN) & (~KEY_PORT_IN) & (~KEY_PORT_IN));
    key_1 = 0;

#if (KEY_CENTER_PORT_IN && KEY_CENTER_IN)
    //-- Remove KEY_CENTER bit
    key_0 &= ~(1<<KEY_CENTER);
    //-- Read 3 times to remove overshoots and mask
    key_1 = KEY_CENTER_IN & ((~KEY_CENTER_PORT_IN) & (~KEY_CENTER_PORT_IN) & (~KEY_CENTER_PORT_IN));
    Key_1 = ((key_1>>KEY_CENTER_IN)<<KEY_CENTER);
#endif
    switch (key_0 |= key_1)
    {
      case BUTTON_N:           //-- BUTTON_N_CODE: 0x01
        key_0=BUTTON_N_CODE;
        break;
      case BUTTON_S:           //-- BUTTON_S_CODE: 0x02
        key_0=BUTTON_S_CODE;
        break;
      case BUTTON_W:           //-- BUTTON_W_CODE: 0x03
        key_0=BUTTON_W_CODE;
        break;
      case BUTTON_E:           //-- BUTTON_E_CODE: 0x04
        key_0=BUTTON_E_CODE;
        break;
      case BUTTON_C:           //-- BUTTON_C_CODE: 0x05
        key_0=BUTTON_C_CODE;
        break;

      case BUTTON_NW:          //-- BUTTON_NW_CODE: 0x06
        key_0=BUTTON_NW_CODE;
        break;
      case BUTTON_SW:          //-- BUTTON_SW_CODE: 0x07
        key_0=BUTTON_SW_CODE;
        break;
      case BUTTON_NE:          //-- BUTTON_NE_CODE: 0x08
        key_0=BUTTON_NE_CODE;
        break;
      case BUTTON_SE:          //-- BUTTON_SE_CODE: 0x09
        key_0=BUTTON_SE_CODE;
        break;
      case BUTTON_NS:          //-- BUTTON_NS_CODE: 0x0A
        key_0=BUTTON_NS_CODE;
        break;
      case BUTTON_WE:          //-- BUTTON_WE_CODE: 0x0B
        key_0=BUTTON_WE_CODE;
        break;
     case BUTTON_NC:           //-- BUTTON_NC_CODE: 0x0C
        key_0=BUTTON_NC_CODE;
        break;
      case BUTTON_SC:          //-- BUTTON_SC_CODE: 0x0D
        key_0=BUTTON_SC_CODE;
        break;
      case BUTTON_WC:          //-- BUTTON_WC_CODE: 0x0E
        key_0=BUTTON_WC_CODE;
        break;
      case BUTTON_EC:          //-- BUTTON_EC_CODE: 0x0F
        key_0=BUTTON_EC_CODE;
        break;
    }
    return key_0;
}
