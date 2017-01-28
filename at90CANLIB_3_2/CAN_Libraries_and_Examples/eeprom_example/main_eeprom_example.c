//******************************************************************************
//! @file $RCSfile: main_eeprom_example.c,v $
//!
//! Copyright (c) 2007 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief Example for some "movements in EEPROM with AVR GCC and test of the
//!        full EEPROM memory
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
#include "at90can_drv.h"
#include "eeprom_drv.h"
#include "eeprom_lib.h"

//_____ M A C R O S ____________________________________________________________

//_____ D E F I N I T I O N S __________________________________________________
#define EE0  u8_eeprom[0]
#define EE1  u8_eeprom[1]
#define EE2  u8_eeprom[2]
#define EE3  u8_eeprom[3]
#define EE4  u8_eeprom[4]

//_____ D E C L A R A T I O N S ________________________________________________

U8 _EEPROM_TYPE_ u8_eeprom[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0, 0 };

//_____ F U N C T I O N S ______________________________________________________

//------------------------------------------------------------------------------
//  @fn main
//!
//! Swap values in u8_eeprom string.
//! Test of the full EEPROM memory
//! If no error, ledchaser ...  
//!
//! @warning none
//!
//! @param none.
//!
//! @return Integer 0
//!
//------------------------------------------------------------------------------

int main (void)
{
U8  u8_temp[5], aa;
U16 i;
U8  u8_src[256];

    DDRA = 0xFF;
    PORTA = 0;

    // Function to read EEPROM array
    eeprom_rd_block((U16)u8_eeprom, u8_temp, 5);

    // Function to clear EEPROM
    eeprom_full_erase();

    // Function to read EEPROM bytes
    eeprom_wr_byte((U16)&EE0, u8_temp[4]);
    eeprom_wr_byte((U16)&EE1, u8_temp[3]);
    eeprom_wr_byte((U16)&EE3, u8_temp[1]);
    eeprom_wr_byte((U16)&EE4, u8_temp[0]);
       
    // Function to write EEPROM bytes
    eeprom_wr_byte((U16)&EE2, u8_temp[2]);
    // or
    eeprom_wr_byte((U16)&u8_eeprom[5], Reg_rd_byte(&SPH));
    eeprom_wr_byte((U16)&u8_eeprom[6], Reg_rd_byte(&SPL));

    // 1st test on the full EEPROM
    PORTA = 0x01;
    for(i=0;i <256; i++) u8_src[i] = ~i;
    for(i=0;i <((E2_END+1)/256); i++) eeprom_wr_block(u8_src, ((U16)(i*256)), 256);
    for(i=0;i <4096; i++)
    {
        aa = eeprom_rd_byte(i);
        if (aa != u8_src[(U8)i]) { PORTA = 0xF1; while (1); }   // Error
    }

    // 2nd test on the full EEPROM
    PORTA = 0x02;
    for(i=0;i <256; i++) u8_src[i] = i;
    for(i=0;i <((E2_END+1)/256); i++) eeprom_wr_block(u8_src, ((U16)(i*256)), 256);
    for(i=0;i <4096; i++)
    {
        aa = eeprom_rd_byte(i);
        if (aa != u8_src[(U8)i]) { PORTA = 0xF2; while (1); }   // Error
    }

    // 3rd test on the full EEPROM
    PORTA = 0x03;
    eeprom_full_erase();
    for(i=0;i <(E2_END+1); i++)
    {
        if (eeprom_rd_byte(i) != EEPROM_BLANK_BYTE) { PORTA = 0xF3; while (1); }   // Error
    }

    // No error then LED chaser
    aa=0x01; for(i=0; i<8;  i++) { u8_src[(U8)i] = aa; aa= aa<<1;}
    aa=0x80; for(i=8; i<16; i++) { u8_src[(U8)i] = aa; aa= aa>>1;}   
    while(1)
    {
        for(aa=0; aa<16; aa++) { PORTA = u8_src[(U8)aa]; for(i=0;i<0x1500;i++); }
    }
    
    return 0;
}
