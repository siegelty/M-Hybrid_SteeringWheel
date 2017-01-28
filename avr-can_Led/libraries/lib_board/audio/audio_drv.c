//******************************************************************************
//! @file $RCSfile: audio_drv.c,v $
//!
//! Copyright (c) 2007 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the low level functions (drivers) of:
//!             - audio transducer
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
#include "audio_drv.h"
#include "rtc_drv.h"

//_____ M A C R O S ____________________________________________________________

//_____ D E F I N I T I O N S __________________________________________________

//_____ D E C L A R A T I O N S ________________________________________________

//------------------------------------------------------------------------------
//  @fn audio_init
//!
//! Timer_1 initialization for audio transducer on DVK90CAN1 board.
//! Timer_0 for mute function.
//!
//! @warning - Doesn't use TIMER libraries
//!          - Do not use TIMER_1 for other function
//!          - See piezo audio transducer section in hardware user's manual
//!
//! @param  none
//!
//! @return  none
//!
//------------------------------------------------------------------------------
void  audio_init (void)

{
#if (MUTE_ON_AUDIO_OUT == ON)
    // TIMER 0 FOR MUTE
    //=================
    // Timer 0 initialized "OFF"  
    Timer0_off();               // Must be reset first to stop timer
    OCR0A  = 0x01;              // highest frequency for tune 
        
    Timer0a_oc0a_disa();               // OC0A of Timer 0 "OFF"
    TCCR0A |= (1<<WGM01)|(0<<WGM00 );  // Mode 2: CTC on top=OCR, toggle OC0A
    TCCR0A |= 0x01;                    // Prescaler to max frequency
    Set_oc0a_or_oc1a();                // Set logic OR OC0A/OC1C0
#endif // MUTE INITIALIZATION
        
    // TIMER 1 FOR SOUND
    //==================
    // Timer/counter1 stopped & reset
    // WARNING: KEEP THE ORDER & DIFFERENTIATION FOR REGISTER SETTING (H before L)
    TCCR1B = 0x00;                            // Must be reset first to stop timer
    Timer1_off();
    TCCR1C = 0x00;
    OCR1AL = OCR1AH = 0x00;      // This will stay null
    ICR1L  = ICR1H  = 0x00;      // This will contain the tone period

    Timer1_toogle_mode();              // Channel initialization, non-PWM
    TCCR1A &= ~(1<<WGM11)|(1<<WGM10 ); // Mode 12: CTC on top=ICR, toggle OC1x
    TCCR1B  =  (1<<WGM13)|(1<<WGM12 );
    TCCR1B |=  TIMER1_PRESCALER;       // Timer1 "ON"
    Audio_output_pin_as_out();         // Set AUDIO_OUTPUT_PIN as output
}
    
//------------------------------------------------------------------------------
//  @fn audio_play
//!
//! Playing one tone with volume controlled on DVK90CAN1 board.
//!
//! @warning See piezo audio transducer section in hardware user's manual
//!
//! @param  tone_value (see 'audio_drv.h' definitions)
//!
//! @return  none
//!
//------------------------------------------------------------------------------
void  audio_play (U16 tone_value)

{
    // WARNING: KEEP THE ORDER & DIFFERENTIATION FOR REGISTER SETTING (H before L)
    ICR1H = (U8)(tone_value>>8);
    ICR1L = (U8)(tone_value);
}

//------------------------------------------------------------------------------
//  @fn audio_play_song
//!
//! Playing a song using audio_play with volume controlled on DVK90CAN1 board.
//!
//!----- Music(s)
//! Source= RTTTL description
//! Example:
//!     exorcist:d=4,o=6,b=160:8e, 8a, 8e, 8b, 8e, 8g, 8a, 8e, 8c7, 8a, 8d7,
//!     8e, 8b, 8c7, 8e, 8b, 8e, 8a, 8e, 8b, 8g, 8a, 8e, 8c7, 8e, 8d7, 8e, 8b,
//!     8c7, 8e, 8b, 8e, 8a, 8e, 8b, 8e, 8g, 2a
//! Note on Tempo:
//!     Tempo=160 => (60*1000)/160 = 375. In fact 375 representes 288 ms !!
//! The source is transformed as this.
//! U16 exorcist[] = 
//!        {
//!        288,
//!        144,E6, 144,A6, 144,E6, 144,B6, 144,E6, 144,G6, 144,A6, 144,E6,
//!        144,C7, 144,A6, 144,D7, 144,E6, 144,B6, 144,C7, 144,E6, 144,B6,
//!        144,E6, 144,A6, 144,E6, 144,B6, 144,G6, 144,A6, 144,E6, 144,C7,
//!        144,E6, 144,D7, 144,E6, 144,B6, 144,C7, 144,E6, 144,B6, 144,E6,
//!        144,A6, 144,E6, 144,B6, 144,E6, 144,G6, 576,A6,
//!        0
//!        };
//! The first data is unused but means the tempo. Then there is a couple of
//! data. The first element of the couple is the tone duration, the second
//! is the coding of the tone (refer to "audio_drv.h"). The end of file is
//! noted with a tone duration=0 (as in a string for "printf()").
//!-----
//!
//! @warning - See piezo audio transducer section in hardware user's manual.
//!          - This function needs "wait_for()", "see rtc_drv.h" definitions.
//!            and #define RTC_TIMER 2 (in "dvk90can1_board.h") is largely
//!            recommended.
//!
//! @param  Pointer on U16 song_string
//!
//! @return  none
//!
//------------------------------------------------------------------------------
void audio_play_song(U16 *song_string)
{
U16 tone_duration;

    audio_init(); 
    song_string ++;

    while ((tone_duration=(*song_string++))!=0)
    {
        audio_play(*song_string++);
        wait_for(tone_duration);    // Play note
        audio_play(0); wait_for(2); // Play end of note
    }
    audio_stop ();     
}
//------------------------------------------------------------------------------
//  @fn audio_stop
//!
//! Shut down the audio transducer on DVK90CAN1 board.
//!
//! @warning See piezo audio transducer section in hardware user's manual
//!
//! @param  none
//!
//! @return  none
//!
//------------------------------------------------------------------------------
void  audio_stop (void)

{
// WARNING: KEEP THE ORDER & DIFFERENTIATION FOR REGISTER SETTING (H before L)
    Timer1_off();
    ICR1L = ICR1H = 0x00;

#if (MUTE_ON_AUDIO_OUT == ON)
    Timer0_off();
#endif // MUTE OFF
    
    AUDIO_PORT_DIR &= ~(1<<AUDIO_OUTPUT_PIN);   // Set AUDIO_OUTPUT_PIN as input
    AUDIO_PORT_OUT &= ~(1<<AUDIO_OUTPUT_PIN);   // Set AUDIO_OUTPUT_PIN as input
}

//------------------------------------------------------------------------------
//  @fn mute_control
//!
//! Enable or disable mute system on DVK90CAN1 board.
//!
//! @warning See piezo audio transducer section in hardware user's manual
//!
//! @param  none
//!
//! @return  none
//!
//------------------------------------------------------------------------------
void  mute_control (Bool mute)

{
#if (MUTE_ON_AUDIO_OUT == ON)       
    if (mute == ON)
    {
        Timer0a_toogle_mode();  // Mute "ON"
    }
    else
    {
        Timer0a_oc0a_disa();    // Mute "OFF"
    }
#endif // MUTE ON           
}
