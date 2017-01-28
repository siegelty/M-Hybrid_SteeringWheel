//******************************************************************************
//! @file $RCSfile: key_drv.h,v $
//!
//! Copyright (c) 2007 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the prototypes and the macros of the
//!        low level functions (drivers) of:
//!             - keyboard
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

#ifndef _KEY_DRV_H_
#define _KEY_DRV_H_

//_____ I N C L U D E S ________________________________________________________

//_____ D E F I N I T I O N S __________________________________________________

#define KEY_MASK ((1<<KEY_NORTH)|(1<<KEY_SOUTH)|(1<<KEY_WEST)|(1<<KEY_EAST)|(1<<KEY_CENTER))

//-- Read & Returned values for buttons
    //-- No button
#define  NO_BUTTON    0x00   // No button !
    //-- 1 button
#define  BUTTON_N     (1<<KEY_NORTH )   // North or Up
#define  BUTTON_S     (1<<KEY_SOUTH )   // South or Down
#define  BUTTON_W     (1<<KEY_WEST  )   // West  or Left
#define  BUTTON_E     (1<<KEY_EAST  )   // East or Right
#define  BUTTON_C     (1<<KEY_CENTER)   // Center or Push or Enter
    //-- 2 buttons
#define  BUTTON_NW    ((1<<KEY_NORTH)|(1<<KEY_WEST)  ) // North-West
 #define BUTTON_WN    ((1<<KEY_NORTH)|(1<<KEY_WEST)  ) // West-North
#define  BUTTON_SW    ((1<<KEY_SOUTH)|(1<<KEY_WEST)  ) // South-West
 #define BUTTON_WS    ((1<<KEY_SOUTH)|(1<<KEY_WEST)  ) // West-South
#define  BUTTON_NE    ((1<<KEY_NORTH)|(1<<KEY_EAST)  ) // North-East 
 #define BUTTON_EN    ((1<<KEY_NORTH)|(1<<KEY_EAST)  ) // East-North 
#define  BUTTON_SE    ((1<<KEY_SOUTH)|(1<<KEY_EAST)  ) // South-East 
 #define BUTTON_ES    ((1<<KEY_SOUTH)|(1<<KEY_EAST)  ) // East-South     
#define  BUTTON_NS    ((1<<KEY_NORTH)|(1<<KEY_SOUTH) ) // North-South 
 #define BUTTON_SN    ((1<<KEY_NORTH)|(1<<KEY_SOUTH) ) // South-North 
#define  BUTTON_WE    ((1<<KEY_WEST) |(1<<KEY_EAST)  ) // West-East 
 #define BUTTON_EW    ((1<<KEY_WEST) |(1<<KEY_EAST)  ) // East-West 
#define  BUTTON_NC    ((1<<KEY_NORTH)|(1<<KEY_CENTER)) // North-Center
 #define BUTTON_CN    ((1<<KEY_NORTH)|(1<<KEY_CENTER)) // Center-North
#define  BUTTON_SC    ((1<<KEY_SOUTH)|(1<<KEY_CENTER)) // South-Center
 #define BUTTON_CS    ((1<<KEY_SOUTH)|(1<<KEY_CENTER)) // Center-South
#define  BUTTON_WC    ((1<<KEY_WEST) |(1<<KEY_CENTER)) // West-Center 
 #define BUTTON_CW    ((1<<KEY_WEST) |(1<<KEY_CENTER)) // Center-West 
#define  BUTTON_EC     ((1<<KEY_EAST) |(1<<KEY_CENTER)) // East-Center
 #define BUTTON_CE    ((1<<KEY_EAST) |(1<<KEY_CENTER)) // Center-East
    //-- 3 buttons
#define  BUTTON_NCS   ((1<<KEY_NORTH)|(1<<KEY_CENTER)|(1<<KEY_SOUTH)) // North-Center-South
 #define BUTTON_SCN   ((1<<KEY_NORTH)|(1<<KEY_CENTER)|(1<<KEY_SOUTH)) // South-Center-North
#define  BUTTON_WCE   ((1<<KEY_WEST) |(1<<KEY_CENTER)|(1<<KEY_EAST) ) // West-Center-East
 #define BUTTON_ECW   ((1<<KEY_WEST) |(1<<KEY_CENTER)|(1<<KEY_EAST) ) // East-Center-West       
#define  BUTTON_NCW   ((1<<KEY_NORTH)|(1<<KEY_CENTER)|(1<<KEY_WEST) ) // North-Center-West
 #define BUTTON_WCN   ((1<<KEY_NORTH)|(1<<KEY_CENTER)|(1<<KEY_WEST) ) // West-Center-North
#define  BUTTON_NCE   ((1<<KEY_NORTH)|(1<<KEY_CENTER)|(1<<KEY_EAST) ) // North-Center-East
 #define BUTTON_ECN   ((1<<KEY_NORTH)|(1<<KEY_CENTER)|(1<<KEY_EAST) ) // East-Center-North  
#define  BUTTON_SCW   ((1<<KEY_SOUTH)|(1<<KEY_CENTER)|(1<<KEY_WEST) ) // South-Center-West
 #define BUTTON_WCS   ((1<<KEY_SOUTH)|(1<<KEY_CENTER)|(1<<KEY_WEST) ) // West-Center-South
#define  BUTTON_SCE   ((1<<KEY_SOUTH)|(1<<KEY_CENTER)|(1<<KEY_EAST) ) // South-Center-East
 #define BUTTON_ECS   ((1<<KEY_SOUTH)|(1<<KEY_CENTER)|(1<<KEY_EAST) ) // East-Center-South      
#define  BUTTON_WNE   ((1<<KEY_WEST) |(1<<KEY_NORTH) |(1<<KEY_EAST) ) // West-North-East
 #define BUTTON_ENW   ((1<<KEY_WEST) |(1<<KEY_NORTH) |(1<<KEY_EAST) ) // East-North-West
#define  BUTTON_WSE   ((1<<KEY_WEST) |(1<<KEY_SOUTH) |(1<<KEY_EAST) ) // West-South-East
 #define BUTTON_ESW   ((1<<KEY_WEST) |(1<<KEY_SOUTH) |(1<<KEY_EAST) ) // East-South-West
#define  BUTTON_NWS   ((1<<KEY_NORTH)|(1<<KEY_WEST)  |(1<<KEY_SOUTH)) // North-West-South
 #define BUTTON_SWN   ((1<<KEY_NORTH)|(1<<KEY_WEST)  |(1<<KEY_SOUTH)) // South-West-North
#define  BUTTON_NES   ((1<<KEY_NORTH)|(1<<KEY_EAST)  |(1<<KEY_SOUTH)) // North-East-South
 #define BUTTON_SEN   ((1<<KEY_NORTH)|(1<<KEY_EAST)  |(1<<KEY_SOUTH)) // South-East-North
    //-- 4 buttons
#define  BUTTON_WNCE  ((1<<KEY_WEST) |(1<<KEY_NORTH)|(1<<KEY_CENTER)|(1<<KEY_EAST) ) // West-North-Center-East
 #define BUTTON_WCNE  ((1<<KEY_WEST) |(1<<KEY_NORTH)|(1<<KEY_CENTER)|(1<<KEY_EAST) ) // West-Center-North-East
 #define BUTTON_ENCW  ((1<<KEY_WEST) |(1<<KEY_NORTH)|(1<<KEY_CENTER)|(1<<KEY_EAST) ) // East-North-Center-West
 #define BUTTON_ECNW  ((1<<KEY_WEST) |(1<<KEY_NORTH)|(1<<KEY_CENTER)|(1<<KEY_EAST) ) // East-Center-North-West  
#define  BUTTON_WSCE  ((1<<KEY_WEST) |(1<<KEY_SOUTH)|(1<<KEY_CENTER)|(1<<KEY_EAST) ) // West-South-Center-East
 #define BUTTON_WCSE  ((1<<KEY_WEST) |(1<<KEY_SOUTH)|(1<<KEY_CENTER)|(1<<KEY_EAST) ) // West-Center-South-East
 #define BUTTON_ESCW  ((1<<KEY_WEST) |(1<<KEY_SOUTH)|(1<<KEY_CENTER)|(1<<KEY_EAST) ) // East-South-Center-West
 #define BUTTON_ECSW  ((1<<KEY_WEST) |(1<<KEY_SOUTH)|(1<<KEY_CENTER)|(1<<KEY_EAST) ) // East-Center-South-West  
#define  BUTTON_NWCS  ((1<<KEY_NORTH)|(1<<KEY_WEST) |(1<<KEY_CENTER)|(1<<KEY_SOUTH)) // North-West-Center-South
 #define BUTTON_NCWS  ((1<<KEY_NORTH)|(1<<KEY_WEST) |(1<<KEY_CENTER)|(1<<KEY_SOUTH)) // North-Center-West-South
 #define BUTTON_SWCN  ((1<<KEY_NORTH)|(1<<KEY_WEST) |(1<<KEY_CENTER)|(1<<KEY_SOUTH)) // South-West-Center-North
 #define BUTTON_SCWN  ((1<<KEY_NORTH)|(1<<KEY_WEST) |(1<<KEY_CENTER)|(1<<KEY_SOUTH)) // South-Center-West-North 
#define  BUTTON_NECS  ((1<<KEY_NORTH)|(1<<KEY_EAST) |(1<<KEY_CENTER)|(1<<KEY_SOUTH)) // North-East-Center-South
 #define BUTTON_NCES  ((1<<KEY_NORTH)|(1<<KEY_EAST) |(1<<KEY_CENTER)|(1<<KEY_SOUTH)) // North-Center-East-South
 #define BUTTON_SECN  ((1<<KEY_NORTH)|(1<<KEY_EAST) |(1<<KEY_CENTER)|(1<<KEY_SOUTH)) // South-East-Center-North
 #define BUTTON_SCEN  ((1<<KEY_NORTH)|(1<<KEY_EAST) |(1<<KEY_CENTER)|(1<<KEY_SOUTH)) // South-Center-East-North
#define  BUTTON_NSWE  ((1<<KEY_NORTH)|(1<<KEY_SOUTH)|(1<<KEY_WEST)  |(1<<KEY_EAST) ) // North-South-West-East
 #define BUTTON_NSEW  ((1<<KEY_NORTH)|(1<<KEY_SOUTH)|(1<<KEY_WEST)  |(1<<KEY_EAST) ) // North-South-East-West
 #define BUTTON_SNWE  ((1<<KEY_NORTH)|(1<<KEY_SOUTH)|(1<<KEY_WEST)  |(1<<KEY_EAST) ) // South-North-West-East
 #define BUTTON_SNEW  ((1<<KEY_NORTH)|(1<<KEY_SOUTH)|(1<<KEY_WEST)  |(1<<KEY_EAST) ) // South-North-East-West
 #define BUTTON_WENS  ((1<<KEY_NORTH)|(1<<KEY_SOUTH)|(1<<KEY_WEST)  |(1<<KEY_EAST) ) // West-East-North-South
 #define BUTTON_WESN  ((1<<KEY_NORTH)|(1<<KEY_SOUTH)|(1<<KEY_WEST)  |(1<<KEY_EAST) ) // West-East-South-North
 #define BUTTON_EWNS  ((1<<KEY_NORTH)|(1<<KEY_SOUTH)|(1<<KEY_WEST)  |(1<<KEY_EAST) ) // East-West-North-South
 #define BUTTON_EWSN  ((1<<KEY_NORTH)|(1<<KEY_SOUTH)|(1<<KEY_WEST)  |(1<<KEY_EAST) ) // East-West-South-North   
    //-- 5 buttons
#define ALL_BUTTON   ((1<<KEY_NORTH)|(1<<KEY_SOUTH)|(1<<KEY_WEST)|(1<<KEY_EAST)|(1<<KEY_CENTER)) // All buttons
#define ALL_BUTTONS  ALL_BUTTON     // All buttons
    
//-- Read & Returned coded (only on ls.digit) values for buttons
    //-- No button
#define  NO_BUTTON_CODE    0x00   // No button !
    //-- 1 button
#define  BUTTON_N_CODE     0x01    // North or Up
#define  BUTTON_S_CODE     0x02    // South or Down
#define  BUTTON_W_CODE     0x03    // West  or Left
#define  BUTTON_E_CODE     0x04    // East or Right
#define  BUTTON_C_CODE     0x05    // Center or Push or Enter
    //-- 2 buttons
#define  BUTTON_NW_CODE    0x06    // North-West
 #define BUTTON_WN_CODE    0x06    // West-North
#define  BUTTON_SW_CODE    0x07    // South-West
 #define BUTTON_WS_CODE    0x07    // West-South
#define  BUTTON_NE_CODE    0x08    // North-East 
 #define BUTTON_EN_CODE    0x08    // East-North 
#define  BUTTON_SE_CODE    0x09    // South-East 
 #define BUTTON_ES_CODE    0x09    // East-South     
#define  BUTTON_NS_CODE    0x0A    // North-South 
 #define BUTTON_SN_CODE    0x0A    // South-North 
#define  BUTTON_WE_CODE    0x0B    // West-East 
 #define BUTTON_EW_CODE    0x0B    // East-West 
#define  BUTTON_NC_CODE    0x0C    // North-Center
 #define BUTTON_CN_CODE    0x0C    // Center-North
#define  BUTTON_SC_CODE    0x0D    // South-Center
 #define BUTTON_CS_CODE    0x0D    // Center-South
#define  BUTTON_WC_CODE    0x0E    // West-Center 
 #define BUTTON_CW_CODE    0x0E    // Center-West 
#define  BUTTON_EC_CODE    0x0F    // East-Center
 #define BUTTON_CE_CODE    0x0F    // Center-East
 
//_____ M A C R O S ____________________________________________________________

//_____ D E C L A R A T I O N S ________________________________________________

//_____ P R O T O T Y P E S ____________________________________________________

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
extern void key_init(void);

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
extern U8   get_key(void);

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
extern U8 get_key_code(void);

//______________________________________________________________________________

#endif  // _KEY_DRV_H_
