//******************************************************************************
//! @file $RCSfile: sensor_drv.c,v $
//!
//! Copyright (c) 2007 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the low level functions (drivers) of:
//!             - 3 x sensors
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
#include "sensor_drv.h"
#include "adc_drv.h"

//_____ M A C R O S ____________________________________________________________

//_____ D E F I N I T I O N S __________________________________________________

//_____ D E C L A R A T I O N S ________________________________________________

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
//------------------------------------------------------------------------------
#ifndef AVCC_EQUAL_VCC
        #error You must define AVCC_EQUAL_VCC in board.h
    #else

const U16 thermistor_table[] =    //-- 76x thermistor (Kohm) values 
    {                             //-- corresponding to -15 to +60 °C
    913,856,804,755,707,667,628,591,556,524,494,465,439,414,391,369,            
        349,330,312,295,279,264,250,237,225,213,202,192,182,173,164,          
        156,148,141,134,128,121,116,110,105,100, 95, 91, 87, 83, 79,
         75, 72, 69, 66, 63, 60, 58, 55, 53, 50, 48, 46, 44, 43, 41,
         39, 38, 36, 35, 33, 32, 31, 29, 28, 27, 26, 25, 24, 23, 22,
    };

S8 get_temperature (void)
{
    U16 therm_value, temp_value;
    S8  temperature_result;
    S16 i;
    
    //-- Channel initialization
    adc_init(AVCC_AS_VREF, NO_LEFT_ADJUST, TEMPERATURE_CHANNEL);
    //-- Get ADC value
    temp_value = adc_single_conversion(TEMPERATURE_INPUT_PIN);
    //-- Compute thermistance value (in Kohms)
    therm_value = (PU_THERM_VALUE * temp_value) / (ADC_FULL_RANGE - temp_value);
    for (i=39; i< 76; i++)        //-- Starting from +25 to +59°C
    {
        if (therm_value >= thermistor_table[i]) break;
    }
    i--;          //-- Rounded to the lower value
    if (i==38)    //-- Re-starting from +25 to -15°C
    {
        for (i=39; i>=1; i--)
        {
            if (therm_value < thermistor_table[i]) break;
        }
    }
    temperature_result = i-15; 
    return (temperature_result); 
}
    
#endif // AVCC_EQUAL_VCC for get_temperature()

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
//------------------------------------------------------------------------------
#ifndef AVCC_EQUAL_VCC
        #error You must define AVCC_EQUAL_VCC in board.h
    #else

const U16 luminosity_table[] =  //-- 24x couples of lux value & light
                                //-- dependent resistor (x100 ohms) values 
    {                           //-- corresponding to 0 up to 160 lux
     0,1800,  1,1050,  2,840,   4,700,   8,610,  12,560,  17,520,  23,470,
    30,420,  36,380,  44,330,  50,300,  57,260,  63,220,  70,180,  77,140,
    85,100,  91,70,   98,40,  103,20,  110,10,  120,5,   135,2,   160,1,
    };

U8 get_luminosity (void)
{
    U16 ldres_value, temp_value;
    U8  luminosity_result;
    S8  i;
    
    //-- Channel initialization
    adc_init(AVCC_AS_VREF, NO_LEFT_ADJUST, LUMINOSITY_CHANNEL);
    //-- Get ADC value
    temp_value = adc_single_conversion(LUMINOSITY_INPUT_PIN);
    //-- Compute light dependent resistor value (in x100 ohms)
    ldres_value = (PU_LDRES_VALUE * temp_value) / (ADC_FULL_RANGE - temp_value);
    for (i=1; i< 48; i+=2)        //-- Starting from 0 lux
        {
        if (ldres_value >= luminosity_table[i]) break;
        }
    luminosity_result = (U8)(luminosity_table[i-1]); 
    return (luminosity_result);
}
    
#endif // AVCC_EQUAL_VCC for get_luminosity()

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
//------------------------------------------------------------------------------
#ifndef AREF_IS_OUTPUT_PIN
        #error You must define AREF_IS_OUTPUT_PIN in board.h
    #else

U16 get_vin (void)
{
    U32 vin_value;
    U32 temp_value;
    U16 voltage_result;
    
    //-- Channel initialization
    adc_init(INTERNAL_VREF, NO_LEFT_ADJUST, VIN_CHANNEL);
    //-- Get ADC value
    temp_value = (U32)(adc_single_conversion(VIN_INPUT_PIN));
    //-- Compute Vin value (in x100 microV)
    vin_value = ( temp_value * VIN_ADC_STEP * VIN_RATIO_VALUE);
    //-- Compute voltage_result in x10mv or x0.01V   
    voltage_result = (U16)(vin_value / 100);
    return (voltage_result);
}
    
#endif // AREF_IS_OUTPUT_PIN for get_vin()

