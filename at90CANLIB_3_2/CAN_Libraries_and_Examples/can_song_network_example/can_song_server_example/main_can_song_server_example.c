//******************************************************************************
//! @file $RCSfile: main_can_song_server_example.c,v $
//!
//! Copyright (c) 2007 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief Via CAN bus, this server sends some large strings (unknown length).
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
//! 
//! The template of a song string is given in "audio_drv.c" file.
//! The elements are U16, the first one is not used, the end of file is 0x0000.
//! Different songs will be sent by the server.
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
extern void put_song(U16 *song_string, U16 song_id);

//----- Music(s)
//--        1st element= Tempo (not used)
//--        2nd element= Tone duration
//--        3rd element= Tone code (c.f. "audio_drv.h")
//--        4th element= Tone duration
//--        5th element= Tone code
//--        ......................
//--       Last element= End of string
//--
//--    Note on RTTTL description:
//--       Example: Tempo=160 => (60*1000)/160 = 375. In fact 375 representes 288 ms !!
//-----

// EXORCIST: RTTTL description
// exorcist:d=4,o=6,b=160:8e, 8a, 8e, 8b, 8e, 8g, 8a, 8e, 8c7, 8a, 8d7, 8e, 8b, 8c7, 8e, 8b,
// 8e, 8a, 8e, 8b, 8g, 8a, 8e, 8c7, 8e, 8d7, 8e, 8b, 8c7, 8e, 8b, 8e, 8a, 8e, 8b, 8e, 8g, 2a
U16 exorcist[] =
     {
     288,
     144,E6, 144,A6, 144,E6, 144,B6, 144,E6, 144,G6, 144,A6, 144,E6, 144,C7, 144,A6, 144,D7,
     144,E6, 144,B6, 144,C7, 144,E6, 144,B6, 144,E6, 144,A6, 144,E6, 144,B6, 144,G6, 144,A6,
     144,E6, 144,C7, 144,E6, 144,D7, 144,E6, 144,B6, 144,C7, 144,E6, 144,B6, 144,E6, 144,A6,
     144,E6, 144,B6, 144,E6, 144,G6, 576,A6,
     0
     };
/*
U16 exorcist[] =      // for test
     {
     0xBEEF,
     0x0102,0x0304, 0x0506,0x0708, 0x090A,0x0B0C, 0x0D0E,0x0F10, 0x1112,0x1314, 0x1516,
     0
     };
*/

// BEVERLY HILLS: RTTTL description
// beverly hi:d=4,o=6,b=112:8f5, 8p, 8g#.5, 16f5, 16p, 16f5, 8a#5, 8f5, 8d#5, 8f5, 8p, 8c.,
// 16f5, 16p, 16f5, 8c#, 8c5, 8g#5, 8f5, 8c, 8f5, 16f5, 16d#5, 16p, 16d#5, 8c5, 8g5, f.5, 2p.,
// 8f5, 8p, 8g#.5, 16f5, 16p, 16f5, 8a#5, 8f5, 8d#5, 8f5, 8p, 8c., 16f5, 16p, 16f5, 8c#, 8c5,
// 8g#5, 8f5, 8c, 8f5
U16 beverly_hills[] =
     {
     411,
     205,F5,  205,P,   307,G5X, 103,F5,  103,P,   103,F5,  205,A5X, 205,F5,  205,D5X, 205,F5,  205,P,
     307,C6,  103,F5,  103,P,   103,F5,  205,C6X, 205,C5,  205,G5X, 205,F5,  205,C6,  205,F5,  103,F5,
     103,D5X, 103,P,   103,D5X, 205,C5,  205,G5,  615,F5,  1233,P,  205,F5,  205,P,   307,G5X, 103,F5,
     103,P,   103,F5,  205,A5X, 205,F5,  205,D5X, 205,F5,  205,P,   307,C6,  103,F5,  103,P,   103,F5,
     205,C6X, 205,C5,  205,G5X, 205,F5,  205,C6,  205,F5,  103,F5,  103,D5X, 103,P,   103,D5X, 205,C5,
     205,G5,  615,F5,  205,P,
     0
     };

// PULP FICTION: RTTTL description
// Pulp Fiction:d=4,o=6,b=160:16a#5, 16a#5, 16a#5, 16a#5, 16a#5, 16a#5, 16a#5, 16a#5, 16d, 16d,
// 16d, 16d, 16d#, 16d#, 16d#, 16d#, 16f, 16f, 16f, 16f, 16f, 16f, 16f, 16a#5, 16a, 16a, 16a,
// 16a, 16f#, 16f#, 16f#, 16f#, 16f, 16f, 16f, 16f, 16f, 16f, 16f, 16f, 16f, 16f, 16f, 16f,
// 16f, 16f, 16f, 16f, 16f#, 16f#, 16f#, 16f#, 16f, 16f, 16f#, 16f#, 16f, 16f, 16f, 16f, 16d#,
// 16d#, 16d#, 16d#, 16f, 16f, 16f, 16f, 16d#, 16d#, 16f, 16f, d#, 8d, 8b5
U16 pulp_fiction[] =
     {
     288,
     82,A5X,  82,A5X, 82,A5X, 82,A5X, 82,A5X, 82,A5X, 82,A5X, 82,A5X, 82,D6, 82,D6,  82,D6,  82,D6,
     82,D6X,  82,D6X, 82,D6X, 82,D6X, 82,F6,  82,F6,  82,F6,  82,F6,  82,F6, 82,F6,  82,F6,  82,A5X,
     82,A6,   82,A6,  82,A6,  82,A6,  82,F6X, 82,F6X, 82,F6X, 82,F6X, 82,F6,  82,F6, 82,F6,  82,F6,
     82,F6,   82,F6,  82,F6,  82,F6,  82,F6,  82,F6,  82,F6,  82,F6,  82,F6,  82,F6,  82,F6, 82,F6,
     82,F6X,  82,F6X, 82,F6X, 82,F6X, 82,F6,  82,F6,  82,F6X, 82,F6X, 82,F6,  82,F6,  82,F6,  82,F6,
     82,D6X,  82,D6X, 82,D6X, 82,D6X, 82,F6,  82,F6,  82,F6,  82,F6,  82,D6X, 82,D6X, 82,F6,  82,F6,
     288,D6X, 144,D6, 144,B5,
     0
     };

// THE GOOD, THE BAD AND THE UGLY: RTTTL description
// Good the B:d=4,o=6,b=63:32c, 32f, 32c, 32f, c, 8g#5, 8a#5, f5, 8p, 32c, 32f,
// 32c, 32f, c, 8g#5, 8a#5, d#, 8p, 32c, 32p, 32d#, f, 16g#, 32p, 16g, 32p, 32f,
// 32p, d#, 8p, 32c, 32f, 32c, 32f, c, 8a#5, f5
U16 good_bad_ugly[] =
    {
    732,
    92,C6,   92,F6,  92,C6,   92,F6,   732,C6,  366,G5X, 366,A5X, 732,F5, 366,P, 92,C6,  92,F6,
    92,C6,   92,F6,  732,C6,  366,G5X, 366,A5X, 732,D6X, 366,P,   92,C6,  92,P,  92,D6X, 732,F6,
    183,G6X, 92,P,   183,G6,  92,P,    92,F6,   92,P,    732,D6X, 366,P,  92,C6, 92,F6,  92,C6,
    92,F6,   732,C6, 366,A5X, 732,F5,
    0
    };

// INDIANA JONES: RTTTL description
// Indiana Jo:d=4,o=6,b=225:e, 8p, 8f, 8g, 8p, 1c7, 8p, d, 8p, 8e, 1f, p, g, 8p,
// 8a, 8b, 8p, 1f7, p, a, 8p, 8b, 2c7, 2d7, 2e7, e, 8p, 8f, 8g, 8p, 1c7, p, d7
// 8p, 8e7, 1f7, 8p, g, 8p, g, 2e7, 2d7, 8p, g, 8p, f7, e7, 8p, 2d7, 1c7
U16 indiana_jones[] =
     {
     174,
     174,E6, 87,P,   87,F6,  87,G6,  87,P,   696,C7, 87,P,   174,D6, 87,P,   87,E6,  696,F6, 174,P,
     174,G6, 87,P,   87,A6,  87,B6,  87,P,   696,F7, 87,P,   174,A6, 87,P,   87,B6,  348,C7, 348,D7,
     348,E7, 174,E6, 87,P,   87,F6,  87,G6,  87,P,   696,C7, 174,P,  174,D7, 87,P,   87,E7,  696,F7,
     87,P,   174,G6, 87,P,   174,G6, 348,E7, 348,D7, 87,P,   174,G6, 87,P,   174,F7, 87,P,   348,D7,
     696,C7,
     0
     };

// COCA COLA: RTTTL description
// Coca Cola:d=4,o=6,b=180:g#.5, 8d#5, 8p, 8d#5, 8p, 8d#5, a#5, p, 2p, a#.5, 8d#5,
// 8p, 8d#5, 8p, 8d#5, g#5, p, 2p, g#.5, 8d#5, 8p, 8d#5, 8p, 8d#5, a#5, p, 2p, a#.5,
// 8d#5, 8p, 8d#5, 8p, 8d#5, 8g#5, 8c, 8p, 8g#5, f5, d#5, g#.5, 8d#5, 8p, 8d#5, 8p,
// 8d#5, a#5, p, 2p, a#.5, 8d#5, 8p, 8d#5, 8p, 8d#5, g#5, p, 2p, g#.5, 8d#5, 8p, 8d#5,
// 8p, 8d#5, a#5, p, 2p, a#.5, 8d#5, 8p, 8d#5, 8p, 8d#5, 8g#5, 8c, 8p, 8g#5, f5, d#5,
// 8g#5,
U16 coca_cola[] =
     {
     256,
     384,G5X, 128,D5X, 128,P,   128,D5X, 128,P,   128,D5X, 256,A5X, 256,P,   512,P,   384,A5X,
     128,D5X, 128,P,   128,D5X, 128,P,   128,D5X, 256,G5X, 256,P,   512,P,   384,G5X, 128,D5X,
     128,P,   128,D5X, 128,P,   128,D5X, 256,A5X, 256,P,   512,P,   384,A5X, 128,D5X, 128,P,
     128,D5X, 128,P,   128,D5X, 128,G5X, 128,C6,  128,P,   128,D5X, 256,F5,  256,D5X, 384,G5X,
     128,D5X, 128,P,   128,D5X, 128,P,   128,D5X, 256,A5X, 256,P,   512,P,   384,A5X, 128,D5X,
     128,P,   128,D5X, 128,P,   128,D5X, 256,G5X, 256,P,   512,P,   384,G5X, 128,D5X, 128,P,
     128,D5X, 128,P,   128,D5X, 256,A5X, 256,P,   512,P,   384,A5X, 128,D5X, 128,P,   128,D5X,
     128,P,   128,D5X, 128,G5X, 128,C6,  128,P,   128,D5X, 256,F5,  256,D5X,
     128,G5X,
     0
     };

//_____ F U N C T I O N S ______________________________________________________

//------------------------------------------------------------------------------
//  @fn main
//!
//! Core of "main_can_song_server_example.c".
//!
//! This program perodically sends on the CAN bus some strings containing data
//! for song to play (c.f. can_song_player_example project)
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
    can_init(0);
    while(1)
    {
        // --- Song: Beverly Hills
        write_led(0x01);        //-- Write on LEDs
        put_song(beverly_hills, BEVERLY_HILLS_ID_TAG);

        // --- Song: Exorcist
        write_led(0x02);        //-- Write on LEDs
        put_song(exorcist, EXORCIST_ID_TAG);

        // --- Song: Pulp Fiction
        write_led(0x04);        //-- Write on LEDs
        put_song(pulp_fiction, PULP_FICTION_ID_TAG);

        // --- Song: The Good, the Bad and the Ugly
        write_led(0x08);        //-- Write on LEDs
        put_song(good_bad_ugly, GOOD_BAD_UGLY_ID_TAG);

        // --- Song: Indiana Jones
        write_led(0x10);        //-- Write on LEDs
        put_song(indiana_jones, INDIANA_JONES_ID_TAG);

        // --- Song: Coca Cola
        write_led(0x20);        //-- Write on LEDs
        put_song(coca_cola, COCA_COLA_ID_TAG);

    } // While (1 ...
    return 0;
}

//------------------------------------------------------------------------------
//  @fn put_song
//!
//! @brief This function sends fragmented messages.
//!
//! @warning - Use DVK90CAN1 board,
//!          - CAN 2.0A,
//!          - can_init() must be perform before,
//!          - Using Global Variable: song_id
//!
//! @param  1- pointer on U16 song_string,
//!         2- song_id
//!
//! @return  none
//!
//------------------------------------------------------------------------------
void put_song(U16 *song_string, U16 song_id)
{
    U8 on_going, temp, i;
    U16 indx;
    U8 buffer[8];
    st_cmd_t msg;
    U8* string_sent;

    string_sent = (U8*)(&(song_string[0]));
    on_going=1;
    indx = 0;

    while(on_going)
    {
        // --- Init Tx buffer
        msg.pt_data = &buffer[0];
        buffer[0] = (U8)(indx>>8);
        buffer[1] = (U8)(indx);
        for(i=2; i<8; )
        {
            temp  = buffer[i++]=(*string_sent++);
            temp |= buffer[i++]=(*string_sent++);
            if (temp==0)
            {
                //-- Last fragment
                on_going = 0;
                break;  //-- Out of for(i=2 ...
            }
        }
        indx += ((U16)(i-2));

        // --- Init Tx Commands

        msg.ctrl.ide = 0;   //-- CAN 2.0A
        msg.dlc = i;        //-- Max
        msg.id.std = song_id;
        msg.cmd = CMD_TX_DATA;

        // --- Tx Command
        while(can_cmd(&msg) != CAN_CMD_ACCEPTED);
        while(can_get_status(&msg) == CAN_STATUS_NOT_COMPLETED);

        // --- Small tempo: Inteframe=256us instead of 96us
        for(i=0; i<0xFF; i++) GPIOR0=i;
    }
}
