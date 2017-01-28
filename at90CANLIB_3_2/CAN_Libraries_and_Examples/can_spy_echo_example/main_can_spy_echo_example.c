//******************************************************************************
//! @file $RCSfile: main_can_spy_echo_example.c,v $
//!
//! Copyright (c) 2007 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief CAN spy displaying CAN activity on UART's
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
#include "uart_lib.h"
#include "uart_drv.h"
#include "can_lib.h"
#include "can_drv.h"
#include "rtc_drv.h"
#include "led_drv.h"
#include "Key_drv.h"

//_____ M A C R O S ____________________________________________________________

//_____ D E F I N I T I O N S __________________________________________________
#define RXCAN   'R'
#define TXCAN   'T'

//_____ D E C L A R A T I O N S ________________________________________________
void spy_printing(U8 rxtx, st_cmd_t* msg);
void can_error_mode(void);
extern U8 txt_logo_atmel[];

//_____ F U N C T I O N S ______________________________________________________

//------------------------------------------------------------------------------
//  @fn main
//!
//! Core of "main_can_spy_echo_example.c".
//!
//! - CAN communications are listen, captured and a resume is sent on UART_0 and
//!   UART_1 (The UART number is at the end & at the beginning of each generated
//!   printing; i.e. 0 or 1).
//! - An CAN Echo also is sent: ID(sent)=ID(received)+1
//!                             DLC = 8
//!                             RTR = 0
//!                             Data[6:7]=CANSTMH:CANSTML
//! Note: If some key is pushed while RESET, only Spy mode is enabled.
//! Note: Exit in UART ECHO MODE if CAN ID == 0 as CAN error simulation
//!
//! @warning UART's at 38400 bauds
//!          CAN in auto-bitrate
//!
//! @param none.
//!
//! @return Integer 0
//!
//------------------------------------------------------------------------------
int main (void)
{
U8 mode;    // mode==0: spy+echo, mode!=0: only spy
U8 txt_can_ana_1[]="======= UART in CAN ANALYZER =======";
U8 txt_can_ana_2[]="======= Echo + UART in CAN ANALYZER =======";

U8 i, u8_temp;
U8 led_out = 0;
U8 buffer[8];
st_cmd_t message;

//--- I N I T
    // Clock prescaler Reset
    CLKPR = 0x80;  CLKPR = 0x00;
    DDRA =0xFF;

    //- Pull-up on TxCAN & RxCAN one by one to use bit-addressing
    CAN_PORT_DIR &= ~(1<<CAN_INPUT_PIN );
    CAN_PORT_DIR &= ~(1<<CAN_OUTPUT_PIN);
    CAN_PORT_OUT |=  (1<<CAN_INPUT_PIN );
    CAN_PORT_OUT |=  (1<<CAN_OUTPUT_PIN);

    // --- Init an get Keys configuration:
    key_init(); mode = get_key();

    // --- Init LED's configuration:
    led_init(); write_led(led_out);

    // Init UARTs
        // Init UART-0 at 38400 bauds
    Uart_select(UART_0);
    uart_init(CONF_8BIT_NOPAR_1STOP,38400);
        
    uart_mini_printf ("\r\n\n-0- ======= CAN SPY ECHO EXAMPLE ======= -0-\r\n");
    uart_mini_printf ("%s",txt_logo_atmel);
    if(mode==0) uart_mini_printf ("-0 %s 0-\r\n\n",txt_can_ana_2);
    else uart_mini_printf ("-0 %s 0-\r\n\n",txt_can_ana_1);
    uart_mini_printf ("-0- ... No Activity on CAN Bus ! ...\r");

        // Init UART-0 at 19200 bauds
    Uart_select(UART_1);
    uart_init(CONF_8BIT_NOPAR_1STOP,38400);
        
    uart_mini_printf ("\r\n\n-1- ======= CAN SPY ECHO EXAMPLE ======= -1-\r\n");
    uart_mini_printf ("%s",txt_logo_atmel);
    if(mode==0) uart_mini_printf ("-0 %s 0-\r\n\n",txt_can_ana_2);
    else uart_mini_printf ("-0 %s 0-\r\n\n",txt_can_ana_1);
    uart_mini_printf ("-1- ... No Activity on CAN Bus ! ...\r");

    // --- Init CAN (special AUTOBAUD)  
        //- Wait until activity on RxCAN
    while ((CAN_PORT_IN & (1<<CAN_INPUT_PIN)) != 0);
        //- Reset CAN peripheral
    Can_reset();
        //- Set CAN Bit-timming
    can_init((U16)CAN_BAUDRATE);        // c.f. macro in "can_drv.h"
        //- Set CAN Timer Prescaler
    CANTCON = CANBT1;                   // Why not !


//---------------------------------------------------------------------------
//--- C O R E   O F   C A N - E C H O --- C O R E   O F   C A N - E C H O --- 
//---------------------------------------------------------------------------

    while(1)
    {
    //- CAN ECHO: WAIT FOR RECEIVED
        // --- Init Rx data
        message.pt_data = &buffer[0];
        for(i=0; i<8; i++) buffer[i]=0;

        // --- Rx Command
        message.cmd = CMD_RX;
        
        // --- Enable Rx
        while(can_cmd(&message) != CAN_CMD_ACCEPTED);
        // --- Wait for Rx completed
        while(1)
        {
            u8_temp = can_get_status(&message);
            if (u8_temp != CAN_STATUS_NOT_COMPLETED) break; // Out of while
        }
        if (u8_temp == CAN_STATUS_ERROR) break; // Out of the function

    //- CAN ECHO: PRINT-UART OF CAN FRAME RECEIVED
        write_led(led_out); led_out++;
        spy_printing(RXCAN, &message);

        // ---- Exit if CAN ID == 0
        if (message.id.ext == 0) break; // Out of function (CAN ERROR simulation)
        if (message.id.std == 0) break; // Out of function (CAN ERROR simulation)
        
        
    //- CAN ECHO: RESPONSE
        
        if(mode==0)
        {
            // --- Init Tx data
            buffer[6] = CANSTMH;
            buffer[7] = CANSTML;
            
            // --- Tx Command
            message.id.ext++;   // Incrementation of ID to revome possible clashes
            message.dlc = 8;
            message.cmd = CMD_TX_DATA;
            
            // --- Enable Tx
            while(can_cmd(&message) != CAN_CMD_ACCEPTED);
            // --- Wait for Tx completed        
            while(1)
            {
                u8_temp = can_get_status(&message);
                if (u8_temp != CAN_STATUS_NOT_COMPLETED) break; // Out of while
            }
            //- CAN ECHO: PRINT-UART OF CAN FRAME TRANSMITTED
            spy_printing(TXCAN, &message);

                        // ---- Exit if CAN error(s)
            if (u8_temp == CAN_STATUS_ERROR) break; // Out of function
        }
    }
    can_error_mode();
    return 0;
}

//------------------------------------------------------------------------------
//  @fn spy_printing
//!
//! CAN communications are resumed on UART_0 and UART_1 (The UART number is at
//!   the end & at the beginning of each generated
//!
//! Note: If some key is pushed while RESET, only Spy mode is enabled.
//!
//! @warning UART's at 38400 bauds
//!
//! @param  - 'R' for receive frame, 'T' for transmit frame.
//!         - CAN message 
//!
//! @return  none
//!
//------------------------------------------------------------------------------
void spy_printing(U8 rxtx, st_cmd_t* msg)
{
U8 indx;

    if (msg->ctrl.ide) 
    {
        Uart_select(UART_0);
        uart_mini_printf ("-0- %cxCAN @ %02X%02X: 0x%08lX(Ext.), L=%d, ", rxtx, CANSTMH, CANSTML, msg->id.ext, msg->dlc);
        Uart_select(UART_1);
        uart_mini_printf ("-1- %cxCAN @ %02X%02X: 0x%08lX(Ext.), L=%d, ", rxtx, CANSTMH, CANSTML, msg->id.ext, msg->dlc);
    }
    else
    {
        Uart_select(UART_0);
        uart_mini_printf ("-0- %cxCAN @ %02X%02X:      0x%03X(Std.), L=%d, ", rxtx, CANSTMH, CANSTML, msg->id.std, msg->dlc);
        Uart_select(UART_1);
        uart_mini_printf ("-1- %cxCAN @ %02X%02X:      0x%03X(Std.), L=%d, ", rxtx, CANSTMH, CANSTML, msg->id.std, msg->dlc);
    }
    if (msg->ctrl.rtr)
    {
        Uart_select(UART_0); uart_mini_printf ("Remote\r\n"); 
        Uart_select(UART_1); uart_mini_printf ("Remote\r\n"); 
    } 
    else
    {
        for(indx=0; indx< (msg->dlc-1); indx++)
        {
            Uart_select(UART_0); uart_mini_printf ("%02X-", *(msg->pt_data + indx)); 
            Uart_select(UART_1); uart_mini_printf ("%02X-", *(msg->pt_data + indx)); 
        }
        Uart_select(UART_0); uart_mini_printf ("%02X\r\n", *(msg->pt_data + indx));                  
        Uart_select(UART_1); uart_mini_printf ("%02X\r\n", *(msg->pt_data + indx));
    }                  

}

//------------------------------------------------------------------------------
//  @fn can_error_mode
//!
//! Exit in UART ECHO MODE if CAN ID == 0 as CAN error simulation.
//!
//! @warning UART's at 38400 bauds
//!
//! @param  none.
//!
//! @return  none
//!
//------------------------------------------------------------------------------

    
//--- C A N    E R R O R

void can_error_mode(void)
{
U8 i;
U8 txt_ech_mod[]="======= UART in ECHO MODE =======";

    Uart_select(UART_0); uart_mini_printf ("\r\n-0- ======= CAN LINE ERROR ");
    Uart_select(UART_1); uart_mini_printf ("\r\n-1- ======= CAN LINE ERROR ");
    
    for(i=0; i<6; i++)
    {
        wait_for(1000); //- Wait for 1000 milli-seconds (1 s)
        Uart_select(UART_0); uart_mini_printf ("*");
        Uart_select(UART_1); uart_mini_printf ("*");
    }

    wait_for(1000); //- Wait for 1000 milli seconds

    Uart_select(UART_0); uart_mini_printf (" Exit From Program! ======= -0-\r\n\n");
    Uart_select(UART_1); uart_mini_printf (" Exit From Program! ======= -1-x\r\n\n");
    
//- UART ECHO
    Uart_select(UART_0); uart_mini_printf ("-0 %s 0-\r\n",txt_ech_mod);
    Uart_select(UART_1); uart_mini_printf ("-1 %s 1-\r\n",txt_ech_mod);
    while(1)
    {
        Uart_select(UART_0);
        if (uart_test_hit())
        {
            uart_putchar(uart_getchar());   // Echo on UART-0
        }
        Uart_select(UART_1);
        if (uart_test_hit())
        {
            uart_putchar(uart_getchar());   // Echo on UART-1
        }
    }
}
