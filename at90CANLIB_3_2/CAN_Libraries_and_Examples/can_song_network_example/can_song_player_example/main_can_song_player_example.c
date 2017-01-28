//******************************************************************************
//! @file $RCSfile: main_can_song_player_example.c,v $
//!
//! Copyright (c) 2007 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief Via CAN bus, download and play a large song string with unknown
//!        length.
//!
//! A burst of CAN messages with the same identifier value (CAN2.0A) is
//! sent on the CAN bus by a MASTER. This burst containts a song to play
//! by any receiver. The data is sent as fragments.
//! Example:
//!    If a string is "U8 yy[20]{a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t}"
//!        1- The first message is:  ID=xxx,DLC=8,DATA=0,0,a,b,c,d,e,f
//!                  (0,0 is the U16 index of the first available data-a)
//!        2- The second message is: ID=xxx,DLC=8,DATA=0,6,g,h,i,j,k,l
//!                  (0,6 is the U16 index of the first available data-g)
//!        3- The thrid message is:  ID=xxx,DLC=8,DATA=0,12,m,n,o,p,q,r
//!                  (0,12 is the U16 index of the first available data-m)
//!        4- The fourth message is: ID=xxx,DLC=6,DATA=0,18,s,t,0,0
//!                  (0,18 is the U16 index of the first available data-s)
//!                  (0,0 at the end means end of string, so DLC=6)
//! Once pushing a key on keyboard of DVK90CAN1, this will download the wanted
//! song string and will play the song.
//!
//! The template of a song string is given in "audio_drv.c" file.
//! The elements are U16, the first one is not used, the end of file is 0x0000.
//! Different songs will be sent to the trainees.
//!
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
#include "can_lib.h"
#include "led_drv.h"
#include "audio_drv.h"
#include "key_drv.h"
#include "rtc_drv.h"

//_____ M A C R O S ____________________________________________________________

//_____ D E F I N I T I O N S __________________________________________________
#define SONGS_ID_TAG             0x020          //-- in CAN 2.0A
#define EXORCIST_ID_TAG         (SONGS_ID_TAG+0)
#define BEVERLY_HILLS_ID_TAG    (SONGS_ID_TAG+1)
#define PULP_FICTION_ID_TAG     (SONGS_ID_TAG+2)
#define GOOD_BAD_UGLY_ID_TAG    (SONGS_ID_TAG+3)
#define INDIANA_JONES_ID_TAG    (SONGS_ID_TAG+4)
#define COCA_COLA_ID_TAG        (SONGS_ID_TAG+5)

//_____ D E C L A R A T I O N S ________________________________________________
extern void get_song(U16 song_id);

U16 song_to_play[200];  //-- 200 elements max for song buffer

//_____ F U N C T I O N S ______________________________________________________

//------------------------------------------------------------------------------
//  @fn main
//!
//! Core of "main_can_song_player_example.c".
//!
//! This program filters the icoming frames following the key pushed and play
//! the song!
//!
//! @warning - Use DVK90CAN1 board,
//!          - Define CAN_BAUDRATE in "config.h".
//!
//! @param none
//!
//! @return Integer 0
//!
//------------------------------------------------------------------------------
int main (void)
{	
    CLKPR = 0x80;  CLKPR = 0x00;  //-- Clock prescaler Reset
    led_init();
    key_init();
    audio_init();
    can_init(0);
    while(1)
    {
        switch (get_key_code())
        {
            case BUTTON_N_CODE:         //-- Song: Beverly Hills
                write_led(0x01);        //-- Write on LEDs
                get_song(BEVERLY_HILLS_ID_TAG);
                audio_play_song(song_to_play);
            break;

            case BUTTON_S_CODE:         //-- Song: Exorcist
                write_led(0x02);        //-- Write on LEDs
                get_song(EXORCIST_ID_TAG);
                audio_play_song(song_to_play);
            break;

            case BUTTON_W_CODE:         //-- Song: Pulp Fiction
                write_led(0x04);        //-- Write on LEDs
                get_song(PULP_FICTION_ID_TAG);
                audio_play_song(song_to_play);
            break;

            case BUTTON_E_CODE:         //-- Song: The Good, the Bad and the Ugly
                write_led(0x08);        //-- Write on LEDs
                get_song(GOOD_BAD_UGLY_ID_TAG);
                audio_play_song(song_to_play);
            break;

            case BUTTON_C_CODE:         //-- Song: Indiana Jones
                write_led(0x10);        //-- Write on LEDs
                get_song(INDIANA_JONES_ID_TAG);
                audio_play_song(song_to_play);
            break;

            case BUTTON_EW_CODE:        //-- Song: Coca Cola
                write_led(0x20);        //-- Write on LEDs
                get_song(COCA_COLA_ID_TAG);
                audio_play_song(song_to_play);
            break;

            default: //-- No key
                write_led(0x00);        //-- Write on LEDs
                wait_for(100);
            break;

        } //  switch ( (get_key_code ...
    } // While (1 ...
    return 0;
}

//------------------------------------------------------------------------------
//  @fn get_song
//!
//! @brief This function receives one fragmented message and places it in
//!        "song_to_play[]" string.
//!
//! @warning - Use DVK90CAN1 board,
//!          - CAN 2.0A,
//!          - can_init() must be perform before
//!
//! @param  song_id
//!
//! @return  none
//!
//------------------------------------------------------------------------------
void get_song(U16 song_id)
{
    U8 on_going, start_of_song;
    U8 temp, i;
    U16 indx;
    U8 buffer[8];
    st_cmd_t msg;
    U8* string_rec;

    // --- Set variables
    start_of_song = 0;
    on_going = 1;
    
    // --- Set correspondance of string pointers
    string_rec = (U8*)(&(song_to_play[0]));

    while(on_going)
    {
        // --- Init Rx data
        msg.pt_data = &buffer[0];
        for(i=0; i<8; i++) buffer[i]=0;

        // --- Rx Command
        msg.ctrl.ide = 0;   //-- CAN 2.0A
        msg.dlc = 8;        //-- Max
        msg.id.std = song_id;
        msg.cmd = CMD_RX_DATA_MASKED;

        // --- Enable Rx
		while(can_cmd(&msg) != CAN_CMD_ACCEPTED);
        // --- Wait for Rx completed
		while(can_get_status(&msg) == CAN_STATUS_NOT_COMPLETED);

        // --- We can work with "buffer" (rather with "msg.pt_data") because
        //     we are at the same level than "msg" & "buffer" mem. allocation. 
        indx  = (U16)(buffer[0]<<8);
        indx |= (U16)(buffer[1]   );

        // --- Look for the first fragment
        if (indx == 0) start_of_song = 1;
        if (start_of_song)
        {
            for (i=2; i<(msg.dlc); )
            {
                // --- Record data
                temp  = string_rec[indx++] = buffer[i++];
                temp |= string_rec[indx++] = buffer[i++];
                // --- Look for the last fragment
                if (temp==0) on_going=0;
            }
        }
    }
}
