//******************************************************************************
//! @file $RCSfile: main_flash_example.c,v $
//!
//! Copyright (c) 2007 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief Example for some "movements" in Flash with AVR GCC.
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
#include "flash_boot_drv.h"
#include "flash_boot_lib.h"

//_____ M A C R O S ____________________________________________________________

//_____ D E F I N I T I O N S __________________________________________________
#define FLASH0  &u8_flash[0]
#define FLASH1  &u8_flash[1]
#define FLASH2  &u8_flash[2]
#define FLASH3  &u8_flash[3]
#define FLASH4  &u8_flash[4]
#define FLASH5  &u8_flash[5]
#define FLASH6  &u8_flash[6]

//_____ D E C L A R A T I O N S ________________________________________________
U8 u8_flash[7] __attribute__((__progmem__)) = {0,0,0,0,0,0,0};

//_____ F U N C T I O N S ______________________________________________________

//------------------------------------------------------------------------------
//  @fn main
//!
//! Core of "main_flash_example.c".
//!
//! Values "0x01,0x02,0x03,0x04,0x05,0x06" from a buffer of the first Flash page
//!          are output on PORTA (LEDs), then
//! values "0x10,0x20,0x30,0x40,0x50,0x60" from the same buffer are output on 
//!          PORTA (LEDs).
//! Then, values "0x11,0x22,0x33,0x44,0x55,0x66" from NRWW Flash section are
//!          output on PORTA (LEDs).
//! Then, the RWW Flash section is erased and some Flash pages are blank
//!          checked. If an error occurs, the wrong value found is display on
//!          PORTA, else 0x00 is output.
//! If no error, ledchaser ...  
//!
//! @warning All the program is placed in the NRWW Flash section thanks to the
//!          compiler plug-in for AVR-GCC:
//!          - "Project Options -> Momory Settings"
//!          In the "Memory Segments" folder adds:
//!          - Memory Type = Flash
//!            Name        = .text
//!            Address     = 0xF000 (in accordance with the fuses setting)
//!
//! @param none.
//!
//! @return Integer 0
//!
//------------------------------------------------------------------------------

int main (void)
{
U32 i;
U8 j,k;
U8 u8_temp_1[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};
U8 u8_temp_2[] = {0x00,0x10,0x20,0x30,0x40,0x50,0x60,0x70};
U8 u8_temp_3[] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77};


    DDRA = 0xFF;
    
    //----- Init u8_flash[]
    
    RAMPZ = 1;
        
    flash_wr_block(u8_temp_3, (U16)u8_flash, 7);

    //----- Set and display a Flash buffer containing "u8_temp_1"

    RAMPZ = 0;
    
    flash_wr_block(u8_temp_1, 0x11, 7);   // Flash buffer at @ 0x00011
    
    PORTA = flash_rd_byte((U16)0x12);      // Addr 0x00012 of Flash
    for(i=0;i<0x15000;i++);
    PORTA = flash_rd_byte((U16)0x13);      // Addr 0x00013 of Flash
    for(i=0;i<0x15000;i++);
    PORTA = flash_rd_byte((U16)0x14);      // Addr 0x00014 of Flash
    for(i=0;i<0x15000;i++);
    PORTA = flash_rd_byte((U16)0x15);      // Addr 0x00015 of Flash
    for(i=0;i<0x15000;i++);
    PORTA = flash_rd_byte((U16)0x16);      // Addr 0x00016 of Flash
    for(i=0;i<0x15000;i++);
    PORTA = flash_rd_byte((U16)0x17);      // Addr 0x00017 of Flash

    for(i=0;i<0x60000;i++);   
    
    //----- Set and display a Flash buffer containing "u8_temp_2"
    
    RAMPZ = 0;
    
    flash_wr_block(u8_temp_2, 0x21, 7);   // Flash buffer at @ 0x00021);
                                                                      
    PORTA = flash_rd_byte((U16)0x22);      // Addr 0x00022 of Flash   
    for(i=0;i<0x15000;i++);                                           
    PORTA = flash_rd_byte((U16)0x23);      // Addr 0x00023 of Flash   
    for(i=0;i<0x15000;i++);                                           
    PORTA = flash_rd_byte((U16)0x24);      // Addr 0x00024 of Flash   
    for(i=0;i<0x15000;i++);                                           
    PORTA = flash_rd_byte((U16)0x25);      // Addr 0x00025 of Flash   
    for(i=0;i<0x15000;i++);                                           
    PORTA = flash_rd_byte((U16)0x26);      // Addr 0x00026 of Flash   
    for(i=0;i<0x15000;i++);                                           
    PORTA = flash_rd_byte((U16)0x27);      // Addr 0x00027 of Flash   

    for(i=0;i<0x60000;i++);   
    
    //----- Set and display "u8_flash"
    
    RAMPZ = 1;
    
    PORTA = flash_rd_byte((U16)FLASH1);
    for(i=0;i<0x15000;i++);   
    PORTA = flash_rd_byte((U16)FLASH2);
    for(i=0;i<0x15000;i++);   
    PORTA = flash_rd_byte((U16)FLASH3);
    for(i=0;i<0x15000;i++);   
    PORTA = flash_rd_byte((U16)FLASH4);
    for(i=0;i<0x15000;i++);   
    PORTA = flash_rd_byte((U16)FLASH5);
    for(i=0;i<0x15000;i++);   
    PORTA = flash_rd_byte((U16)FLASH6);
    
    for(i=0;i<0x60000;i++);
       
    //----- Erase RWW Flash section & verify the first page
    
    PORTA = 0;
    
    RAMPZ = 0;
    
    flash_full_erase();
    
    k = 0;
    while (j==0)
    {
        PORTA = j = (U8) flash_page_blank_check(k);
        k++;
        if (k > 96) break;  //- 96 = MAX of pages in RWW section of AT90CAN32
    }
       
    if (j != 0) while(1);   //- ERROR
    
    for(i=0;i<0x60000;i++);   
    
    //----- END (LED chaser)
       
    k=1; for(j=0; j<8; j++) { u8_temp_1[j] = k; k= k<<1;}
    k=8; for(j=0; j<8; j++) { u8_temp_2[j] = k; k= k>>1;}
    
    while(1)
    {
        for(j=0; j<8; j++) { PORTA = u8_temp_1[j]; for(i=0;i<0x1000;i++); }
        for(j=0; j<8; j++) { PORTA = u8_temp_2[j]; for(i=0;i<0x1000;i++); }
    }
    
    return 0;
}
