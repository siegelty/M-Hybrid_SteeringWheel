//******************************************************************************
//! @file $RCSfile: config.h,v $
//!
//! Copyright (c) 2007 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief Configuration file for the following project:
//!             - can_bootloader_example_gcc
//!
//! This file can be parsed by Doxygen for automatic documentation generation.
//! This file has been validated with AVRStudio-413528/WinAVR-20070122.
//!
//! @version $Revision: 3.20 $ $Name: jtellier $
//!
//! @todo
//! @bug
//******************************************************************************

#ifndef _CONFIG_H_
#define _CONFIG_H_

//_____ I N C L U D E S ____________________________________________________

#include "compiler.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "at90can_drv.h"
#include "dvk90can1_board.h"

//_____ M A C R O S ____________________________________________________________

//_____ D E F I N I T I O N S ______________________________________________

    // -------------- BOOT LOADER DEFINITION
#define     BOOT_LOADER_SIZE            0x2000  // Size in bytes: 8KB
#define     MAX_FLASH_SIZE_TO_ERASE     ( FLASH_SIZE - ((U32)(BOOT_LOADER_SIZE)) )

    // -------------- PROCESSOR DEFINITION
#define     MANUF_ID            0x1E        // ATMEL
#define     FAMILY_CODE         0x81        // AT90CANxx family

#define     XRAM_END            XRAMEND     // Defined in "iocan128/64/32.h"
#define     RAM_END             RAMEND      // Defined in "iocan128/64/32.h"
#define     E2_END              E2END       // Defined in "iocan128/64/32.h"
#define     FLASH_END           FLASHEND    // Defined in bytes in "iocan128/64/32.h"
#define     FLASH_SIZE          ((U32)(FLASH_END)) + 1 // Size in bytes
#define     FLASH_PAGE_SIZE     256         // Size in bytes, constant for AT90CANxx devices

// Switches for specific definitions & switches
#if   defined(__AVR_AT90CAN128__)
#   define  PRODUCT_NAME        0x97        // 128 Kbytes of Flash
#   define  PRODUCT_REV         0x00        // Rev 0
#   define  _RAMPZ_IS_USED_     1           // RAMPZ register used if Flash memory upper than 64K bytes


#elif defined(__AVR_AT90CAN64__)
#   define  PRODUCT_NAME        0x96        // 64 Kbytes of Flash
#   define  PRODUCT_REV         0x00        // Rev 0

#elif defined(__AVR_AT90CAN32__)
#   define  PRODUCT_NAME        0x95        // 32 Kbytes of Flash
#   define  PRODUCT_REV         0x00        // Rev 0

#else
#   error Wrong device selection in Compiler plug-in for AVR-GCC: "Project Options -> Device"
#endif

    // -------------- MCU LIB CONFIGURATION
#define FOSC           8000        // 8 MHz External cristal
#define F_CPU          (FOSC*1000) // Need for AVR GCC

    //---------------- CAN DEFINITION -------------
//#define   CAN_BAUDRATE        500         // Baudrate in kBit
#define     CAN_BAUDRATE        CAN_AUTOBAUD


//_____ D E C L A R A T I O N S ________________________________________________

//______________________________________________________________________________

#endif  // _CONFIG_H

