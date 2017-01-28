//******************************************************************************
//! @file $RCSfile: audio_drv.h,v $
//!
//! Copyright (c) 2007 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the prototypes and the macros of the
//!        low level functions (drivers) of:
//!             - audio transducer
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

#ifndef _AUDIO_DRV_
#define _AUDIO_DRV_

//_____ I N C L U D E S ________________________________________________________

#include "config.h"

//_____ D E F I N I T I O N S __________________________________________________

//_____ M A C R O S ____________________________________________________________

#define Timer0a_toogle_mode()   (TCCR0A |=  ((0<<COM0A1) | (1<<COM0A0)))
#define Timer0a_oc0a_disa()     (TCCR0A &= ~((1<<COM0A1) | (1<<COM0A0)))

#define Audio_output_pin_as_out() (AUDIO_PORT_DIR |= (1<<AUDIO_OUTPUT_PIN))
#define Set_oc0a_or_oc1a()        (AUDIO_PORT_OUT |= (1<<AUDIO_OUTPUT_PIN)) 

#define Timer0_off()  (TCCR0A = 0x00)
#define Timer1_off()  (TCCR1A = 0x00)


//_____ D E C L A R A T I O N S ________________________________________________
        
#ifndef FOSC
        #error You must define FOSC in config.h
    #elif FOSC == 1000
        #define TIMER1_PRESCALER 0x01
    #elif FOSC == 8000
        #define TIMER1_PRESCALER 0x02
    #elif FOSC == 16000
        #define TIMER1_PRESCALER 0x03
    #else
        #error FOSC definition must be 1, 8 or 16 MHz$
#endif // FOSC 

#ifndef AUDIO_OUTPUT_PIN
        #error You must define AUDIO_OUTPUT_PIN in board.h
    #elif AUDIO_OUTPUT_PIN == 5
        #define Timer1_toogle_mode() (TCCR1A |= (0<<COM1A1)|(1<<COM1A0)) // Toggle OC1A on compare match
    #elif AUDIO_OUTPUT_PIN == 6
        #define Timer1_toogle_mode() (TCCR1A |= (0<<COM1B1)|(1<<COM1B0)) // Toggle OC1B on compare match
    #elif AUDIO_OUTPUT_PIN == 7
        #define Timer1_toogle_mode() (TCCR1A |= (0<<COM1C1)|(1<<COM1C0)) // Toggle OC1C on compare match
    #else
        #error Wrong AUDIO_OUTPUT_PIN definition$
#endif // AUDIO_OUTPUT_PIN

#ifndef OCR0A
    #define OCR0A OCR0   //--  Patch to correct ioCAN128v.h or mcu.h when OCR0A is not defined 
#endif


    //-- Pause 
#define P         0x0001
#define PAUSE     0x0001
      
    //-- Min Frequency 
#define MIN_TONE      A0  

#define A0        0x8E0B   //!<    13.8 Hz (French La  )  
#define A0X       0x863C   //!<    14.6 Hz (French La# )  
#define B0        0x7E6A   //!<    15.4 Hz (French Si  )
#define C0        0x7717   //!<    16.4 Hz (French Do  )
#define C0X       0x70E5   //!<    17.3 Hz (French Do# )
#define D0        0x6A25   //!<    18.4 Hz (French Ré  )
#define D0X       0x64AD   //!<    19.4 Hz (French Ré# )
#define E0        0x5ECF   //!<    20.6 Hz (French Mi  )
#define F0        0x5997   //!<    21.8 Hz (French Fa  )
#define F0X       0x548D   //!<    23.1 Hz (French Fa# )
#define G0        0x4FB8   //!<    24.5 Hz (French Sol )
#define G0X       0x4B1E   //!<    26   Hz (French Sol#)

#define A1        0x4705   //!<    27.5 Hz (French La  )
#define A1X       0x431E   //!<    29.1 Hz (French La# )
#define B1        0x3F35   //!<    30.9 Hz (French Si  )
#define C1        0x3BBA   //!<    32.7 Hz (French Do  )
#define C1X       0x3872   //!<    34.6 Hz (French Do# )
#define D1        0x3537   //!<    36.7 Hz (French Ré  )
#define D1X       0x3235   //!<    38.9 Hz (French Ré# )
#define E1        0x2F67   //!<    41.2 Hz (French Mi  )
#define F1        0x2CB1   //!<    43.7 Hz (French Fa  )
#define F1X       0x2A46   //!<    46.2 Hz (French Fa# )
#define G1        0x27DC   //!<    49   Hz (French Sol )
#define G1X       0x25A1   //!<    51.9 Hz (French Sol#)

#define A2        0x2382   //!<    55   Hz (French La  )
#define A2X       0x2180   //!<    58.3 Hz (French La# )
#define B2        0x1FA7   //!<    61.7 Hz (French Si  )
#define C2        0x1DDD   //!<    65.4 Hz (French Do  )
#define C2X       0x1C2F   //!<    69.3 Hz (French Do# )
#define D2        0x1A9B   //!<    73.4 Hz (French Ré  )
#define D2X       0x191A   //!<    77.8 Hz (French Ré# )
#define E2        0x17B3   //!<    82.4 Hz (French Mi  )
#define F2        0x165F   //!<    87.3 Hz (French Fa  )
#define F2X       0x151D   //!<    92.5 Hz (French Fa# )
#define G2        0x13EE   //!<    98   Hz (French Sol )
#define G2X       0x12D0   //!<   103.8 Hz (French Sol#)

#define A3        0x11C1   //!<   110   Hz (French La  )
#define A3X       0x10C3   //!<   116.5 Hz (French La# )
#define B3        0x0FD0   //!<   123.5 Hz (French Si  )
#define C3        0x0EEE   //!<   130.8 Hz (French Do  )
#define C3X       0x0e17   //!<   138.6 Hz (French Do# )
#define D3        0x0D4D   //!<   146.8 Hz (French Ré  )
#define D3X       0x0C8D   //!<   155.6 Hz (French Ré# )
#define E3        0x0BD9   //!<   164.8 Hz (French Mi  )
#define F3        0x0B2F   //!<   174.6 Hz (French Fa  )
#define F3X       0x0A8E   //!<   185   Hz (French Fa# )
#define G3        0x09F7   //!<   196   Hz (French Sol )
#define G3X       0x0967   //!<   207.7  Hz (French Sol#)

#define A4        0x08E0   //!<   220   Hz (French La  )
#define A4X       0x0861   //!<   233.1 Hz (French La# )
#define B4        0x07E9   //!<   246.9 Hz (French Si  )
#define C4        0x0777   //!<   261.6 Hz (French Do  )
#define C4X       0x070B   //!<   277.2 Hz (French Do# )
#define D4        0x06A6   //!<   293.7 Hz (French Ré  )
#define D4X       0x0647   //!<   311.1 Hz (French Ré# )
#define E4        0x05EC   //!<   329.6 Hz (French Mi  )
#define F4        0x0597   //!<   349.2 Hz (French Fa  )
#define F4X       0x0547   //!<   370   Hz (French Fa# )
#define G4        0x04FB   //!<   392   Hz (French Sol )
#define G4X       0x04B3   //!<   415.3 Hz (French Sol#)

#define A5        0x0470   //!<   440   Hz (French La  )
#define A5X       0x0430   //!<   466.2 Hz (French La# )
#define B5        0x03F4   //!<   493.9 Hz (French Si  )
#define C5        0x03BB   //!<   523.3 Hz (French Do  )
#define C5X       0x0385   //!<   554.4 Hz (French Do# )
#define D5        0x0353   //!<   587.3 Hz (French Ré  )
#define D5X       0x0323   //!<   622.3 Hz (French Ré# )
#define E5        0x02F6   //!<   659.3 Hz (French Mi  )
#define F5        0x02CB   //!<   698.5 Hz (French Fa  )
#define F5X       0x02A3   //!<   740   Hz (French Fa# )
#define G5        0x027D   //!<   784   Hz (French Sol )
#define G5X       0x0259   //!<   830.6 Hz (French Sol#)

#define A6        0x0238   //!<   880   Hz (French La  )
#define A6X       0x0218   //!<   932.3 Hz (French La# )
#define B6        0x01FA   //!<   987.8 Hz (French Si  )
#define C6        0x01DD   //!<  1046.5 Hz (French Do  )
#define C6X       0x01C2   //!<  1108.7 Hz (French Do# )
#define D6        0x01A9   //!<  1174.7 Hz (French Ré  )
#define D6X       0x0191   //!<  1244.5 Hz (French Ré# )
#define E6        0x017B   //!<  1318.5 Hz (French Mi  )
#define F6        0x0165   //!<  1396.9 Hz (French Fa  )
#define F6X       0x0151   //!<  1480   Hz (French Fa# )
#define G6        0x013E   //!<  1568   Hz (French Sol )
#define G6X       0x012C   //!<  1661.2 Hz (French Sol#)

#define A7        0x011C   //!<  1760   Hz (French La  )
#define A7X       0x010C   //!<  1864.7 Hz (French La# )
#define B7        0x00FD   //!<  1975.5 Hz (French Si  )
#define C7        0x00EE   //!<  2093   Hz (French Do  )
#define C7X       0x00E1   //!<  2217.5 Hz (French Do# )
#define D7        0x00D4   //!<  2349.3 Hz (French Ré  )
#define D7X       0x00C8   //!<  2489   Hz (French Ré# )
#define E7        0x00BD   //!<  2637   Hz (French Mi  )
#define F7        0x00B2   //!<  2793.8 Hz (French Fa  )
#define F7X       0x00A8   //!<  2960   Hz (French Fa# )
#define G7        0x009F   //!<  3136   Hz (French Sol )
#define G7X       0x0096   //!<  3322.4 Hz (French Sol#)
    
#define A8        0x008E   //!<  3520   Hz (French La  )
#define A8X       0x0086   //!<  3729.3 Hz (French La# )
#define B8        0x007E   //!<  3951.1 Hz (French Si  )
#define C8        0x0077   //!<  4186   Hz (French Do  )
#define C8X       0x0070   //!<  4434.9 Hz (French Do# )
#define D8        0x006A   //!<  4698.6 Hz (French Ré  )
#define D8X       0x0064   //!<  4978   Hz (French Ré# )
#define E8        0x005E   //!<  5274   Hz (French Mi  )
#define F8        0x0059   //!<  5587.7 Hz (French Fa  )
#define F8X       0x0054   //!<  5919.9 Hz (French Fa# )
#define G8        0x004F   //!<  6271.9 Hz (French Sol )
#define G8X       0x004B   //!<  6644.9 Hz (French Sol#)

#define A9        0x0047   //!<  7040   Hz (French La  )
#define A9X       0x0043   //!<  7458.6 Hz (French La# )
#define B9        0x003F   //!<  7902.1 Hz (French Si  )
#define C9        0x003B   //!<  8372   Hz (French Do  )
#define C9X       0x0038   //!<  8869.8 Hz (French Do# )
#define D9        0x0035   //!<  9397.3 Hz (French Ré  )
#define D9X       0x0032   //!<  9956.1 Hz (French Ré# )
#define E9        0x002F   //!< 10548.1 Hz (French Mi  )
#define F9        0x002C   //!< 11175.3 Hz (French Fa  )
#define F9X       0x002A   //!< 11839.8 Hz (French Fa# )
#define G9        0x0027   //!< 12543.9 Hz (French Sol )
#define G9X       0x0025   //!< 13289.8 Hz (French Sol#)
    //-- Max Frequency
#define MAX_TONE      B7
    
//_____ P R O T O T Y P E S ________________________________________________

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
//! @param none
//!
//! @return none
//!
extern void  audio_init (void);

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
extern void  audio_play (U16 tone_value);

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
//! @warning See piezo audio transducer section in hardware user's manual.
//! @warning This function needs "wait_for()", "see rtc_drv.h" definitions.
//!
//! @param  Pointer on U16 song_string
//!
//! @return  none
//!
extern void  audio_play_song(U16 *song_string);

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
extern void  audio_stop (void);

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
extern void  mute_control (Bool mute);

//______________________________________________________________________________

#endif  // _AUDIO_DRV_

