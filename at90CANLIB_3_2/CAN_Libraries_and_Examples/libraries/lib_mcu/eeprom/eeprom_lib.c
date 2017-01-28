//******************************************************************************
//! @file $RCSfile: eeprom_lib.c,v $
//!
//! Copyright (c) 2007 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the library of functions of:
//!             - EEPROM
//!             - AT90CAN128/64/32
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
#include "eeprom_lib.h"
#include "eeprom_drv.h"

//_____ D E F I N I T I O N S __________________________________________________

//_____ F U N C T I O N S ______________________________________________________

//------------------------------------------------------------------------------
//  @fn eeprom_rd_byte
//!
//! This function allows to read a byte in up to 64K bytes of Eeprom memory.
//!
//! @warning
//!
//! @param: addr_byte   Address to read the byte in Eeprom memory.
//!
//! @return:    Value read in the Eeprom memory.
//!
//------------------------------------------------------------------------------
U8 eeprom_rd_byte(U16 addr_byte)
{
    Eeprom_prog_completed;
    return (Eeprom_rd_byte(addr_byte));
}

//------------------------------------------------------------------------------
//  @fn eeprom_wr_byte
//!
//! This function allows to write a byte in up to 64K of Eeprom memory.
//!
//! @warning
//!
//! @param: addr_byte   Address in Eeprom memory to write the byte.
//!         value       Value to write in the Eeprom memory.
//!
//! @return none
//!
//------------------------------------------------------------------------------
void eeprom_wr_byte(U16 addr_byte, U8 value)
{
    Eeprom_prog_completed;
    Eeprom_wr_byte(addr_byte, value);
}

//------------------------------------------------------------------------------
//  @fn eeprom_rd_block
//!
//! This function allows to read up to 65535 bytes (64K Bytes-1 byte) of
//! the Eeprom memory.
//!
//! @warning: 1 - This function isn't able to address the fully 64K bytes
//!               but we cannot find in the device a source buffer up to
//!               64K bytes.
//!
//! @param: src    Start address in Eeprom memory where write data.
//!         *dest  Address of data to write.
//!         n      number of byte to write.
//!
//! @return none
//!
//------------------------------------------------------------------------------
void eeprom_rd_block(U16 src, U8* dest, U16 n)
{
    for (; n != 0 ; n--)
    {
       *dest = eeprom_rd_byte(src);
        src++; dest++;
    }
}
//------------------------------------------------------------------------------
//  @fn eeprom_wr_block
//!
//! This function allows to write up to 65535 bytes (64K Bytes-1 byte) in
//! the Eeprom memory.
//!
//! @warning: 1 - This function isn't able to address the fully 64K bytes
//!               but we cannot find in the device a source buffer up to
//!               64K bytes.
//!
//! @param: *src   Address of data to write.
//!         dest   Start address in Eeprom memory where write data.
//!         n      number of byte to write.
//!
//! @return none
//!
//------------------------------------------------------------------------------
void eeprom_wr_block(U8* src, U16 dest, U16 n)
{
    for (; n != 0 ; n--)
    {
        eeprom_wr_byte(dest,*src);
        src++; dest++;
    }
}

//------------------------------------------------------------------------------
//  @fn eeprom_full_erase
//!
//! This function erases the whole Eeprom memory.
//!
//! @warning none
//!
//! @param none
//!
//! @return none
//!
//------------------------------------------------------------------------------
void eeprom_full_erase(void)
{
U16 addr;

    for(addr = 0; addr < (U16)(E2_END+1); addr++)
    {
        if ((eeprom_rd_byte(addr)) != EEPROM_BLANK_BYTE)
        {
            eeprom_wr_byte(addr, EEPROM_BLANK_BYTE);
        }
    }
}
