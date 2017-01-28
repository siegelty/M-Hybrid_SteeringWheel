//******************************************************************************
//! @file $RCSfile: sensor_drv.h,v $
//!
//! Copyright (c) 2007 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the prototypes and the macros of the
//!        low level functions (drivers) of:
//!             - 3 x sensors
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

#ifndef _SENSOR_DRV_
#define _SENSOR_DRV_

//_____ I N C L U D E S ________________________________________________________

#include "config.h"

//_____ D E F I N I T I O N S __________________________________________________

//_____ M A C R O S ____________________________________________________________

//_____ D E C L A R A T I O N S ________________________________________________

#define PU_THERM_VALUE     ((U16)(  100 ))      // Pull-up Res. for THERM = 100 Kohms
#define PU_LDRES_VALUE     ((U16)(   33 ))      // Pull-up Res. for LDRES = 3.3 Kohms
#define VIN_ADC_STEP       ((U16)(25600/0x3FF)) //  VREF / ADC_FULL_RANGE = 2.5 mV
#define VIN_RATIO_VALUE    ((U16)(    5 ))      // Pull-up = 400 Kohms, RVin = 100 Kohms

//_____ P R O T O T Y P E S ____________________________________________________

//------------------------------------------------------------------------------
//  @fn get_temperature
//!
//! Temperature acquisition on DVK90CAN1 board.
//!
//! @warning  See temperature sensor section in hardware user's manual
//!
//! @param  none
//!
//! @return  temperature_result
//!          Range: -15 (0xF1) to +60 (0x4A)
//!                 -15 = -15°C or less
//!                 +60 = +60°C or more
//!
S8  get_temperature (void);

//------------------------------------------------------------------------------
//  @fn get_luminosity
//!
//! Luminosity acquisition on DVK90CAN1 board. This is done with a light
//! dependent resistor (ldres)
//!
//! @warning See luminosity sensor section in hardware user's manual
//!
//! @param  none
//!
//! @return  luminosity_result
//!          Range:   0 to 160 lux
//!
U8  get_luminosity (void);

//------------------------------------------------------------------------------
//  @fn get_vin
//!
//! Voltage acquisition on DVK90CAN1 board. This voltage is positive and must
//! be in 0 to 12,8 V (5 * 2.56 V) range.
//!
//! @warning See voltage reading section in hardware user's manual
//!          Need of stabilization of Aref falling level before convertion
//!
//! @param  none
//!
//! @return  voltage_result
//!          Range:   0 to 1280
//!                   0 =     0  mV
//!                1280 = 12800  mV or 12.8 V
//!
U16 get_vin (void);
    
//______________________________________________________________________________

#endif  // _SENSOR_DRV_

