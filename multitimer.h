#ifndef __MULTITIMER_H
#define __MULTITIMER_H

//==============================================================================
// multitimer interf�sz v0.11 (itt adjuk meg a param�tereket)
//==============================================================================

// a projectre glob�lis param�terek itt vannak megadva
#include "HardwareProfile.h"

// id�z�t� forr�s�rajele (Hz)
// #define SystemClock 48000000

// ha valamelyik id�z�t� elt�r� �rajelr�l megy, az k�l�n megadhat� pl:
// (ebben az esetben az inicializ�l� f�ggv�nyh�v�s ut�n az �rajelforr�st is �ll�tsuk be)
// #define Timer0SrcClock 32768
// #define Timer1SrcClock 32768
// #define Timer2SrcClock 32768
// #define Timer3SrcClock 32768
// #define Timer4SrcClock 32768
// #define Timer5SrcClock 32768

// frekvencia megad�sa (Hz)
#define TIME0CLK          4
// #define TIME1CLK    1000
// #define TIME2CLK    1000
// #define TIME3CLK    1000
// #define TIME4CLK    1000
// #define TIME5CLK    1000

// PIC18: timer IRQ priorit�sa (-1 = nincs priorit�s, 0 = alacsony, 1 = magas)
// ha priorit�s n�lk�li megszak�t�skezel�st hazn�lunk, akkor mind az legyen!)
#define TIMER0PR18    -1
#define TIMER1PR18    -1
#define TIMER2PR18    -1
#define TIMER3PR18    -1

// PIC24: timer IRQ priorit�sa (1=legalacsonyabb, 7=legmagasabb)
#define TIMER1PR24     1
#define TIMER2PR24     1
#define TIMER3PR24     1
#define TIMER4PR24     1
#define TIMER5PR24     1

// �sszefoglal� a k�l�nf�le vez�rl�csal�dok eset�n felhaszn�lhat� f�ggv�nyekr�l
// (a * jel�l�s� �res f�ggv�ny, elhagyhat�)
/*==============================================================================|
|        AVR        |       PIC16       |       PIC18       |       PIC24       |
|===================|===================|===================|===================|
|                                 Inicializ�l�s                                 |
|   Timer0Init()    |   Timer0Init()    |   Timer0Init()    |                   |
|   Timer1Init()    |   Timer1Init()    |   Timer1Init()    |   Timer1Init()    |
|   Timer2Init()    |   Timer2Init()    |   Timer2Init()    |   Timer2Init()    |
|                   |                   |   Timer3Init()    |   Timer3Init()    |
|                   |                   |                   |   Timer4Init()    |
|                   |                   |                   |   Timer5Init()    |
|-------------------|-------------------|-------------------|-------------------|
|                          Glob�lis megszak�t�s enged�lyez�se                   |
|   (inicializ�l�s ut�n ezt is �rdemes megtenni, ha nem m�shol tessz�k meg)     |
|                   | TimersGlobalIrq() | TimersGlobalIrq() |                   |
|-------------------|-------------------|-------------------|-------------------|
|                      Timer Compar�tor hi�ny�nak p�tl�sa                       |
|                (megszak�t�skiszolg�l� f�ggv�nybe kell berakni)                |
|                   | Timer0Compless()  | Timer0Compless()  |                   |
|                   | Timer1Compless()  | Timer1Compless()  |                   |
|                   | Timer2Compless()* | Timer2Compless()* |                   |
|                   |                   | Timer3Compless()  |                   |
|                   |                   |                   |                   |
|                   |                   |                   |                   |
|-------------------|-------------------|-------------------|-------------------|
|                  Timer Start (megszak�t�s�nak enged�lyez�se)                  |
|   Timer0Start()   |   Timer0Start()   |   Timer0Start()   |                   |
|   Timer1Start()   |   Timer1Start()   |   Timer1Start()   |   Timer1Start()   |
|   Timer2Start()   |   Timer2Start()   |   Timer2Start()   |   Timer2Start()   |
|                   |                   |   Timer3Start()   |   Timer3Start()   |
|                   |                   |                   |   Timer4Start()   |
|                   |                   |                   |   Timer5Start()   |
|-------------------|-------------------|-------------------|-------------------|
|                     Timer Stop (megszak�t�s�nak tilt�sa)                      |
|   Timer0Stop()    |   Timer0Stop()    |   Timer0Stop()    |                   |
|   Timer1Stop()    |   Timer1Stop()    |   Timer1Stop()    |   Timer1Stop()    |
|   Timer2Stop()    |   Timer2Stop()    |   Timer2Stop()    |   Timer2Stop()    |
|                   |                   |   Timer3Stop()    |   Timer3Stop()    |
|                   |                   |                   |   Timer4Stop()    |
|                   |                   |                   |   Timer5Stop()    |
|-------------------|-------------------|-------------------|-------------------|
|               Timer megszak�t�skiszolg�l� f�ggv�ny defini�l�sa                |
|               (a void visszat�r� �rt�k nem szabad ki�rni el�!)                |
|    Timer0Int()    |    Timer0Int()    |    Timer0Int()    |                   |
|    Timer1Int()    |    Timer1Int()    |    Timer1Int()    |    Timer1Int()    |
|    Timer2Int()    |    Timer2Int()    |    Timer2Int()    |    Timer2Int()    |
|                   |                   |    Timer3Int()    |    Timer3Int()    |
|                   |                   |                   |    Timer4Int()    |
|                   |                   |                   |    Timer5Int()    |
|-------------------|-------------------|-------------------|-------------------|
|        Timer megszak�t�kiszolg�l� f�ggv�nyben a megszak�t�s nyugt�z�sa        |
|                   |  Timer0IrqAck()   |  Timer0IrqAck()   |                   |
|                   |  Timer1IrqAck()   |  Timer1IrqAck()   |  Timer1IrqAck()   |
|                   |  Timer2IrqAck()   |  Timer2IrqAck()   |  Timer2IrqAck()   |
|                   |                   |  Timer3IrqAck()   |  Timer3IrqAck()   |
|                   |                   |                   |  Timer4IrqAck()   |
|                   |                   |                   |  Timer5IrqAck()   |
|-------------------|-------------------|-------------------|-------------------|
|                   K�z�s megszak�t�kiszolg�l� f�ggv�nyb�l a                    |
|                  timer megszak�t�skiszolg�l� f�ggv�ny h�v�sa                  |
| pl. Timer0IntProcess(Timer0Process) -> Timer0Process f�ggv�ny fog megh�v�dni  |
|                   |Timer0IntProcess(f)|Timer0IntProcess(f)|                   |
|                   |Timer1IntProcess(f)|Timer1IntProcess(f)|                   |
|                   |Timer2IntProcess(f)|Timer2IntProcess(f)|                   |
|                   |                   |Timer3IntProcess(f)|                   |
|==============================================================================*/


//==============================================================================
// multitimer v0.11 (innent?l nem sz�ks�ges beleny�lni
//==============================================================================

#if defined(__AVR__)
#define TimerSrcClock (1UL*SystemClock)
#elif (defined(_PIC14) || defined(_PIC14E) || defined(__18CXX) || defined(__PICC18__) || defined(__dsPIC30F__))
#define TimerSrcClock (1UL*SystemClock/4)
#elif (defined(__dsPIC33F__) || defined(__PIC24F__) || defined(__PIC24FK__) || defined(__PIC24H__))
#define TimerSrcClock (1UL*SystemClock/2)
#else
#error "ismeretlen processzort�pus"
#endif // defined(xxx proci)

// ennyivel kell osztani a rendszer�rajelet
#ifdef TIME0CLK
#ifndef Timer0SrcClock
#define Timer0SrcClock TimerSrcClock
#endif
#define TIME0DIV  ((Timer0SrcClock+TIME0CLK/2)/TIME0CLK)
#endif

#ifdef TIME1CLK
#ifndef Timer1SrcClock
#define Timer1SrcClock TimerSrcClock
#endif
#define TIME1DIV  ((Timer1SrcClock+TIME1CLK/2)/TIME1CLK)
#endif

#ifdef TIME2CLK
#ifndef Timer2SrcClock
#define Timer2SrcClock TimerSrcClock
#endif
#define TIME2DIV  ((Timer2SrcClock+TIME2CLK/2)/TIME2CLK)
#endif

#ifdef TIME3CLK
#ifndef Timer3SrcClock
#define Timer3SrcClock TimerSrcClock
#endif
#define TIME3DIV  ((Timer3SrcClock+TIME3CLK/2)/TIME3CLK)
#endif

#ifdef TIME4CLK
#ifndef Timer4SrcClock
#define Timer4SrcClock TimerSrcClock
#endif
#define TIME4DIV  ((Timer4SrcClock+TIME4CLK/2)/TIME4CLK)
#endif

#ifdef TIME5CLK
#ifndef Timer5SrcClock
#define Timer5SrcClock TimerSrcClock
#endif
#define TIME5DIV  ((Timer5SrcClock+TIME5CLK/2)/TIME5CLK)
#endif

//==============================================================================
// AVR
#if defined(__AVR__)
#include <avr/interrupt.h>
#include <compat/ina90.h>

//------------------------------------------------------------------------------
// AVR timer0
#ifdef TIME0CLK

// oszt�0
#if TIME0DIV < 256
#define TM0CLKDIV        1
#define TM0CLKSEL        1
#elif TIME0DIV < (256*8)
#define TM0CLKDIV        8
#define TM0CLKSEL        2
#elif TIME0DIV < (256*64)
#define TM0CLKDIV       64
#define TM0CLKSEL        3
#elif TIME0DIV < (256*256)
#define TM0CLKDIV      256
#define TM0CLKSEL        4
#elif TIME0DIV < (256*1024)
#define TM0CLKDIV     1024
#define TM0CLKSEL        5
#else
#error "AVR: timer0 frekvencia t�l alacsony"
#endif // TIME0DIV

// timer0 kompar�torba ennyit kell berakni hogy a k�v�nt frekvencia legyen
#define TM0COMP (((Timer0SrcClock/TM0CLKDIV)+TIME0CLK/2)/TIME0CLK-1)

// timer0 inicializ�l�s
#ifdef  TCCR0A
#ifdef  TCCR0B // (A/B regiszteres TIMER0)
#define Timer0Init() {\
  OCR0A = TM0COMP;                      /* comparator */\
  TCCR0A = (1<<WGM01);                  /* MODE1 */\
  TCCR0B = (TM0CLKSEL<<CS00);           /* frekvencia oszt�s */\
  sei();}
#else // TCCR0B (A regiszteres TIMER0)
#define Timer0Init() {\
  OCR0A = TM0COMP;                      /* comparator */\
  TCCR0A = (1<<CTC0)|(TM0CLKSEL<<CS00); /* CTC mode, frekvencia oszt�s */\
  sei();}
#endif // else TCCR0B
#else  // TCCR0A
#define Timer0Init() {\
  OCR0 = TM0COMP;                       /* comparator */\
  TCCR0 = (1<<WGM01)|(TM0CLKSEL<<CS00); /* CTC mode, frekvencia oszt�s */\
  sei();}
#endif // TCCR0A

// timer0 bekapcsol�s
#ifdef TIMSK0
#define Timer0Start()  TIMSK0 = (1<<OCIE0A)
#else  // TIMSK0
#define Timer0Start()  TIMSK |= (1<<OCIE0)
#endif // TIMSK0

// timer0 kikapcsol�s
#ifdef TIMSK0
#define Timer0Stop()   TIMSK0 = 0
#else  // TIMSK0
#define Timer0Stop()   TIMSK &= ~(1<<OCIE0)
#endif // TIMSK0

// timer0 megszak�t�s kiszolg�l� f�ggv�ny
#ifdef  TIMER0_COMP_vect
#define Timer0Int()    ISR (TIMER0_COMP_vect)
#else
#define Timer0Int()    ISR (TIMER0_COMPA_vect)
#endif

#endif // TIME0CLK

//------------------------------------------------------------------------------
// AVR timer1
#ifdef TIME1CLK

// oszt�1
#if TIME1DIV < 65536
#define TM1CLKDIV        1
#define TM1CLKSEL        1
#elif TIME1DIV < (65536*8)
#define TM1CLKDIV        8
#define TM1CLKSEL        2
#elif TIME1DIV < (65536*64)
#define TM1CLKDIV       64
#define TM1CLKSEL        3
#elif TIME1DIV < (65536*256)
#define TM1CLKDIV      256
#define TM1CLKSEL        4
#elif TIME1DIV < (65536*1024)
#define TM1CLKDIV     1024
#define TM1CLKSEL        5
#else
#error "AVR: timer1 frekvencia t�l alacsony"
#endif // TIME1DIV

// timer1 kompar�torba ennyit kell berakni hogy a k�v�nt frekvencia legyen
#define TM1COMP (((Timer1SrcClock/TM1CLKDIV)+TIME1CLK/2)/TIME1CLK-1)

// timer1 inicializ�l�s
#define Timer1Init() {\
  OCR1AH = TM1COMP >> 8;                /* comparator HI */\
  OCR1AL = (unsigned char)TM1COMP;      /* comparator LO */\
  TCCR1A = (0<<WGM10);                  /* mode4 (CTC) */\
  TCCR1B = (1<<WGM12)|(TM1CLKSEL<<CS10);/* mode4, Clk = ClkIO/1..8..64..256..1024 */\
  sei();}
  
// timer1 bekapcsol�s
#ifdef TIMSK1
#define Timer1Start()  TIMSK1 = (1<<OCIE1A)
#else  // TIMSK1
#define Timer1Start()  TIMSK |= (1<<OCIE1A)
#endif // TIMSK1

// timer1 kikapcsol�s
#ifdef TIMSK1
#define Timer1Stop()   TIMSK1 = 0
#else  // TIMSK1
#define Timer1Stop()   TIMSK &= ~(1<<OCIE1A)
#endif // TIMSK1

// timer1 megszak�t�s kiszolg�l� f�ggv�ny
#define Timer1Int()    ISR (TIMER1_COMPA_vect)

#endif // TIME1CLK
//------------------------------------------------------------------------------
// AVR timer2
#ifdef TIME2CLK

// oszt�2
#if TIME2DIV < 256
#define TM2CLKDIV        1
#define TM2CLKSEL        1
#elif TIME2DIV < (256*8)
#define TM2CLKDIV        8
#define TM2CLKSEL        2
#elif TIME2DIV < (256*32)
#define TM2CLKDIV       32
#define TM2CLKSEL        3
#elif TIME2DIV < (256*64)
#define TM2CLKDIV       64
#define TM2CLKSEL        4
#elif TIME2DIV < (256*128)
#define TM2CLKDIV      128
#define TM2CLKSEL        5
#elif TIME2DIV < (256*256)
#define TM2CLKDIV      256
#define TM2CLKSEL        6
#elif TIME2DIV < (256*1024)
#define TM2CLKDIV     1024
#define TM2CLKSEL        7
#else
#error "AVR: timer2 frekvencia t�l alacsony"
#endif // TIME2DIV

// timer2 kompar�torba ennyit kell berakni hogy a k�v�nt frekvencia legyen
#define TM2COMP (((Timer2SrcClock/TM2CLKDIV)+TIME2CLK/2)/TIME2CLK-1)

// timer2 inicializ�l�s
#ifdef TCCR2A
#define Timer2Init() {\
  OCR2A = TM2COMP;                      /* comparator */\
  TCCR2A = (1<<WGM21);                  /* mode 1 (CTC) */\
  TCCR2B = (TM2CLKSEL<<CS20);           /* oszt� */\
  sei();}
#else  // TCCR2A
#define Timer2Init() {\
  OCR2  = TM2COMP;                      /* comparator */\
  TCCR2  = (1<<WGM21)|(TM2CLKSEL<<CS20);/* mode (CTC), oszt� */\
  sei();}
#endif // TCCR2A
  
// timer2 bekapcsol�s
#ifdef TIMSK2
#define Timer2Start()  TIMSK2 = (1<<OCIE2A)
#else  // TIMSK1
#define Timer2Start()  TIMSK |= (1<<OCIE2)
#endif // TIMSK1

// timer2 kikapcsol�s
#ifdef TIMSK2
#define Timer2Stop()   TIMSK2 = 0
#else  // TIMSK1
#define Timer2Stop()   TIMSK &= ~(1<<OCIE2)
#endif // TIMSK1

// timer2 megszak�t�s kiszolg�l� f�ggv�ny
#ifdef  TIMER0_COMP_vect
#define Timer2Int() ISR (TIMER2_COMP_vect)
#else
#define Timer2Int() ISR (TIMER2_COMPA_vect)
#endif

#endif // TIME2CLK

// AVR
//==============================================================================
// PIC16

#elif (defined(_PIC14) || defined(_PIC14E))

//------------------------------------------------------------------------------
// PIC16 timer0
#ifdef TIME0CLK

// oszt�0
#if TIME0DIV < 256
#define TM0CLKDIV      1
#define TM0CLKSEL      OPTION_REGbits.PSA = 1
#define TM0COMPCOR     2
#elif TIME0DIV < (256*2)
#define TM0CLKDIV      2
#define TM0CLKSEL      OPTION_REGbits.PS = 0; OPTION_REGbits.PSA = 0
#define TM0COMPCOR     1
#elif TIME0DIV < (256*4)
#define TM0CLKDIV      4
#define TM0CLKSEL      OPTION_REGbits.PS = 1; OPTION_REGbits.PSA = 0
#define TM0COMPCOR     0
#elif TIME0DIV < (256*8)
#define TM0CLKDIV      8
#define TM0CLKSEL      OPTION_REGbits.PS = 2; OPTION_REGbits.PSA = 0
#define TM0COMPCOR     0
#elif TIME0DIV < (256*16)
#define TM0CLKDIV      16
#define TM0CLKSEL      OPTION_REGbits.PS = 3; OPTION_REGbits.PSA = 0
#define TM0COMPCOR     0
#elif TIME0DIV < (256*32)
#define TM0CLKDIV      32
#define TM0CLKSEL      OPTION_REGbits.PS = 4; OPTION_REGbits.PSA = 0
#define TM0COMPCOR     0
#elif TIME0DIV < (256*64)
#define TM0CLKDIV      64
#define TM0CLKSEL      OPTION_REGbits.PS = 5; OPTION_REGbits.PSA = 0
#define TM0COMPCOR     0
#elif TIME0DIV < (256*128)
#define TM0CLKDIV      128
#define TM0CLKSEL      OPTION_REGbits.PS = 6; OPTION_REGbits.PSA = 0
#define TM0COMPCOR     0
#elif TIME0DIV < (256*256)
#define TM0CLKDIV      256
#define TM0CLKSEL      OPTION_REGbits.PS = 7; OPTION_REGbits.PSA = 0
#define TM0COMPCOR     0
#else
#error "PIC16: timer0 frekvencia t�l alacsony"
#endif  // TIME0DIV

// timer0 kompar�torba ennyit kell berakni hogy a k�v�nt frekvencia legyen
#define TM0COMP (((Timer0SrcClock/TM0CLKDIV)+TIME0CLK/2)/TIME0CLK-1)

// timer0 kompar�tor hi�ny�nak p�tl�sa
#define Timer0Compless()  TMR0 += (TM0COMPCOR-TM0COMP)

// timer0 inicializ�l�s
#define Timer0Init() {\
  OPTION_REGbits.T0CS = 0;              /* forr�s �rajel = systemclock */\
  TM0CLKSEL;                            /* oszt� */\
  TMR0 = (255+TM0COMPCOR-TM0COMP);}     /* timer kezd��rt�k felt�lt�se */

// timer0 bekapcsol�s (nem bekapcsolhat�, csak IRQ-t lehet enged�lyezni)
#define Timer0Start()  if(!INTCONbits.T0IE){TMR0 = (255+TM0COMPCOR-TM0COMP); INTCONbits.T0IE = 1;}

// timer0 kikapcsol�s (nem kikapcsolhat�, csak IRQ-t lehet tiltani)
#define Timer0Stop()   INTCONbits.T0IE = 0

// timer0 megszak�t�s nyugt�z�s
#define Timer0IrqAck() INTCONbits.T0IF = 0

// K�z�s megszak�t�kiszolg�l� f�ggv�nyb�l a timer0 megszak�t�skiszolg�l� f�ggv�ny h�v�sa
#define Timer0Intprocess(f) if(INTCONbits.T0IE && INTCONbits.T0IF) f()

#endif // TIME0CLK

//------------------------------------------------------------------------------
// PIC16 timer1
#ifdef TIME1CLK

// oszt�1
#if TIME1DIV < 65536
#define TM1CLKDIV      1
#define TM1CLKSEL      T1CONbits.T1CKPS = 0
#define TM1COMPCOR     1
#elif TIME1DIV < (65536*2)
#define TM1CLKDIV      2
#define TM1CLKSEL      T1CONbits.T1CKPS = 1
#define TM1COMPCOR     0
#elif TIME1DIV < (65536*4)
#define TM1CLKDIV      4
#define TM1CLKSEL      T1CONbits.T1CKPS = 2
#define TM1COMPCOR     0
#elif TIME1DIV < (65536*8)
#define TM1CLKDIV      8
#define TM1CLKSEL      T1CONbits.T1CKPS = 3
#define TM1COMPCOR     0
#else
#error "PIC16: timer1 frekvencia t�l alacsony"
#endif  // TIME1DIV

// timer1 kompar�torba ennyit kell berakni hogy a k�v�nt frekvencia legyen
#define TM1COMP (((Timer1SrcClock/TM1CLKDIV)+TIME1CLK/2)/TIME1CLK-1)

// timer1 kompar�tor hi�ny�nak p�tl�sa
#define Timer1Compless()  TMR1 += (65535+TM1COMPCOR-TM1COMP)

// timer1 inicializ�l�s
#define Timer1Init() {\
  T1CONbits.TMR1CS = 0;                 /* forr�s �rajel = systemclock */\
  TM1CLKSEL;                            /* oszt� */\
  PIE1bits.TMR1IE = 1;                  /* megszak�t�s enged�lyez�s */\
  TMR1 = 65535+TM1COMPCOR-TM1COMP;}     /* timer kezd��rt�k felt�lt�se */

// timer1 bekapcsol�s
#define Timer1Start()  if(!T1CONbits.TMR1ON){TMR1 = 65535+TM1COMPCOR-TM1COMP; T1CONbits.TMR1ON = 1;}

// timer1 kikapcsol�s
#define Timer1Stop()   T1CONbits.TMR1ON = 0

// timer1 megszak�t�s nyugt�z�s
#define Timer1IrqAck() PIR1bits.TMR1IF = 0

// K�z�s megszak�t�kiszolg�l� f�ggv�nyb�l a timer1 megszak�t�skiszolg�l� f�ggv�ny h�v�sa
#define Timer1IntProcess(f) if(PIR1bits.TMR1IF) f()

#endif // TIME1CLK

//------------------------------------------------------------------------------
// PIC16 timer2
#ifdef TIME2CLK

// oszt�2
#if TIME2DIV < 256
#define TM2CLKDIV      1
#define TM2CLKSEL      T2CONbits.T2CKPS = 0; T2CONbits.TOUTPS = 0
#elif TIME2DIV < (256*2)
#define TM2CLKDIV      2
#define TM2CLKSEL      T2CONbits.T2CKPS = 0; T2CONbits.TOUTPS = 1
#elif TIME2DIV < (256*4)
#define TM2CLKDIV      4
#define TM2CLKSEL      T2CONbits.T2CKPS = 1; T2CONbits.TOUTPS = 0
#elif TIME2DIV < (256*4*2)
#define TM2CLKDIV      8
#define TM2CLKSEL      T2CONbits.T2CKPS = 1; T2CONbits.TOUTPS = 1
#elif TIME2DIV < (256*16*1)
#define TM2CLKDIV      16
#define TM2CLKSEL      T2CONbits.T2CKPS = 2; T2CONbits.TOUTPS = 0
#elif TIME2DIV < (256*16*2)
#define TM2CLKDIV      32
#define TM2CLKSEL      T2CONbits.T2CKPS = 2; T2CONbits.TOUTPS = 1
#elif TIME2DIV < (256*16*4)
#define TM2CLKDIV      64
#define TM2CLKSEL      T2CONbits.T2CKPS = 2; T2CONbits.TOUTPS = 3
#elif TIME2DIV < (256*16*8)
#define TM2CLKDIV      128
#define TM2CLKSEL      T2CONbits.T2CKPS = 2; T2CONbits.TOUTPS = 7
#elif TIME2DIV < (256*16*16)
#define TM2CLKDIV      256
#define TM2CLKSEL      T2CONbits.T2CKPS = 2; T2CONbits.TOUTPS = 15
#else
#error "PIC16: timer2 frekvencia t�l alacsony"
#endif // TIME2DIV

// timer2 kompar�torba ennyit kell berakni hogy a k�v�nt frekvencia legyen
#define TM2COMP (((Timer2SrcClock/TM2CLKDIV)+TIME2CLK/2)/TIME2CLK-1)

// mivel van timer kompar�tor, nem kell szoftverb�l p�tolni
#define Timer2Compless() ;

// timer2 inicializ�l�s
#define Timer2Init() {\
  TM2CLKSEL;                            /* oszt� */\
  T2CONbits.TMR2ON = 1;                 /* timer enged�lyez�s */\
  PR2 = TM2COMP;}

// timer2 bekapcsol�s (megszak�t�s enged�ly be)
#define Timer2Start()  PIE1bits.TMR2IE = 1

// timer2 kikapcsol�s (megszak�t�s enged�ly ki)
#define Timer2Stop()   PIE1bits.TMR2IE = 0

// timer2 megszak�t�s nyugt�z�s
#define Timer2IrqAck() PIR1bits.TMR2IF = 0

// K�z�s megszak�t�kiszolg�l� f�ggv�nyb�l a timer2 megszak�t�skiszolg�l� f�ggv�ny h�v�sa
#define Timer2IntProcess(f) if(PIR1bits.TMR2IF) f()

#endif // TIME2CLK

// perif�ria IRQ �s glob�lis IRQ eng
#define TimersGlobalIrq() {INTCONbits.PEIE = 1; INTCONbits.GIE = 1;}

// PIC16
//==============================================================================
// PIC18
#elif defined(__18CXX)

//------------------------------------------------------------------------------
// PIC18 timer0
#ifdef TIME0CLK

// PIC18: timer IRQ priorit�sa (0 = alacsony, 1 = magas)
#if TIMER0PR18 == -1
// priorit�s n�lk�li megszak�t�s
#define TIMER0PR18REG   1
#elif TIMER0PR18 == 0
// alacsony priorit�s
#define TIMER0PR18REG   0
#elif TIMER0PR18 == 1
// magas priorit�s
#define TIMER0PR18REG   1
#else
#error "LCD TIMER0PR18 csak -1, 0, 1 lehet!"
#endif

// oszt�0
#if TIME0DIV < 65536
#define TM0CLKDIV      1
#define TM0CLKSEL      T0CONbits.PSA = 1  /* el�oszt� tilt�s */
#define TM0COMPCOR     7
#elif TIME0DIV < (65536*2)
#define TM0CLKDIV      2
#define TM0CLKSEL      T0CONbits.PSA = 0; T0CONbits.T0PS2 = 0; T0CONbits.T0PS1 = 0; T0CONbits.T0PS0 = 0  /* el�oszt� eng, oszt�s = 2 */
#define TM0COMPCOR     4
#elif TIME0DIV < (65536*4)
#define TM0CLKDIV      4
#define TM0CLKSEL      T0CONbits.PSA = 0; T0CONbits.T0PS2 = 0; T0CONbits.T0PS1 = 0; T0CONbits.T0PS0 = 1  /* el�oszt� eng, oszt�s = 4 */
#define TM0COMPCOR     2
#elif TIME0DIV < (65536*8)
#define TM0CLKDIV      8
#define TM0CLKSEL      T0CONbits.PSA = 0; T0CONbits.T0PS2 = 0; T0CONbits.T0PS1 = 1; T0CONbits.T0PS0 = 0  /* el�oszt� eng, oszt�s = 8 */
#define TM0COMPCOR     0
#elif TIME0DIV < (65536*16)
#define TM0CLKDIV      16
#define TM0CLKSEL      T0CONbits.PSA = 0; T0CONbits.T0PS2 = 0; T0CONbits.T0PS1 = 1; T0CONbits.T0PS0 = 1  /* el�oszt� eng, oszt�s = 16 */
#define TM0COMPCOR     0
#elif TIME0DIV < (65536*32)
#define TM0CLKDIV      32
#define TM0CLKSEL      T0CONbits.PSA = 0; T0CONbits.T0PS2 = 1; T0CONbits.T0PS1 = 0; T0CONbits.T0PS0 = 0  /* el�oszt� eng, oszt�s = 32 */
#define TM0COMPCOR     0
#elif TIME0DIV < (65536*64)
#define TM0CLKDIV      64
#define TM0CLKSEL      T0CONbits.PSA = 0; T0CONbits.T0PS2 = 1; T0CONbits.T0PS1 = 0; T0CONbits.T0PS0 = 1  /* el�oszt� eng, oszt�s = 64 */
#define TM0COMPCOR     0
#elif TIME0DIV < (65536*128)
#define TM0CLKDIV      128
#define TM0CLKSEL      T0CONbits.PSA = 0; T0CONbits.T0PS2 = 1; T0CONbits.T0PS1 = 1; T0CONbits.T0PS0 = 0  /* el�oszt� eng, oszt�s = 128 */
#define TM0COMPCOR     0
#elif TIME0DIV < (65536*256)
#define TM0CLKDIV      256
#define TM0CLKSEL      T0CONbits.PSA = 0; T0CONbits.T0PS2 = 1; T0CONbits.T0PS1 = 1; T0CONbits.T0PS0 = 1  /* el�oszt� eng, oszt�s = 256 */
#define TM0COMPCOR     0
#else
#error "PIC18: timer0 frekvencia t�l alacsony"
#endif // TIME0DIV

// timer0 kompar�torba ennyit kell berakni hogy a k�v�nt frekvencia legyen
#define TM0COMP (((Timer0SrcClock/TM0CLKDIV)+TIME0CLK/2)/TIME0CLK-1)

// timer0 kompar�tor hi�ny�nak p�tl�s�hoz �tmeneti v�ltoz�
union
{
  struct
  {
    unsigned char LO;
    unsigned char HI;
  };
  unsigned short W;
}MultiTimer0Temp;

// timer kompar�tor hi�ny�nak p�tl�sa
#define Timer0Compless() {                             \
  MultiTimer0Temp.LO = TMR0L;                        \
  MultiTimer0Temp.HI = TMR0H;                        \
  MultiTimer0Temp.W += 65535 + TM0COMPCOR - TM0COMP; \
  TMR0H = MultiTimer0Temp.HI;                        \
  TMR0L = MultiTimer0Temp.LO;}

// timer0 inicializ�l�s
#define Timer0Init() {\
  T0CONbits.T08BIT = 0;                 /* 16bites timer */\
  T0CONbits.T0CS = 0;                   /* forr�s �rajel = systemclock */\
  TM0CLKSEL;                            /* oszt� */\
  INTCON2bits.TMR0IP = TIMER0PR18REG;   /* priorit�s */\
  INTCONbits.TMR0IE = 1;                /* megszak�t�s enged�lyez�s */\
  TMR0H = (65535+TM0COMPCOR-TM0COMP) >> 8;/* timerH kezd��rt�k felt�lt�se bitid�re */\
  TMR0L = 65535+TM0COMPCOR-TM0COMP;}    /* timerL kezd��rt�k felt�lt�se bitid�re */

// timer0 bekapcsol�s
#define Timer0Start()  T0CONbits.TMR0ON = 1

// timer0 kikapcsol�s
#define Timer0Stop()   T0CONbits.TMR0ON = 0
  
// timer0 megszak�t�s nyugt�z�s
#define Timer0IrqAck() INTCONbits.TMR0IF = 0

// K�z�s megszak�t�kiszolg�l� f�ggv�nyb�l a timer0 megszak�t�skiszolg�l� f�ggv�ny h�v�sa
#define Timer0IntProcess(f) if(INTCONbits.TMR0IF) f()

#endif // TIME0CLK

//------------------------------------------------------------------------------
// PIC18 timer1
#ifdef TIME1CLK

// PIC18: timer IRQ priorit�sa (0 = alacsony, 1 = magas)
#if TIMER1PR18 == -1
// priorit�s n�lk�li megszak�t�s
#define TIMER1PR18REG   1
#elif TIMER1PR18 == 0
// alacsony priorit�s
#define TIMER1PR18REG   0
#elif TIMER1PR18 == 1
// magas priorit�s
#define TIMER1PR18REG   1
#else
#error "LCD TIMER1PR18 csak -1, 0, 1 lehet!"
#endif

// oszt�1
#if TIME1DIV < 65536
#define TM1CLKDIV      1
#define TM1CLKSEL      T1CONbits.T1CKPS1 = 0; T1CONbits.T1CKPS0 = 0 /* el�oszt� 1:1 */
#define TM1COMPCOR     5
#elif TIME1DIV < (65536*2)
#define TM1CLKDIV      2
#define TM1CLKSEL      T1CONbits.T1CKPS1 = 0; T1CONbits.T1CKPS0 = 1 /* el�oszt� 1:2 */
#define TM1COMPCOR     3
#elif TIME1DIV < (65536*4)
#define TM1CLKDIV      4
#define TM1CLKSEL      T1CONbits.T1CKPS1 = 1; T1CONbits.T1CKPS0 = 0 /* el�oszt� 1:4 */
#define TM1COMPCOR     2
#elif TIME1DIV < (65536*8)
#define TM1CLKDIV      8
#define TM1CLKSEL      T1CONbits.T1CKPS1 = 1; T1CONbits.T1CKPS0 = 1 /* el�oszt� 1:8 */
#define TM1COMPCOR     1
#else
#error "PIC18: timer1 frekvencia t�l alacsony"
#endif // TIME1DIV

// timer1 kompar�torba ennyit kell berakni hogy a k�v�nt frekvencia legyen
#define TM1COMP (((Timer1SrcClock/TM1CLKDIV)+TIME1CLK/2)/TIME1CLK-1)

// timer1 kompar�tor hi�ny�nak p�tl�s�hoz �tmeneti v�ltoz�
union
{
  struct
  {
    unsigned char LO;
    unsigned char HI;
  };
  unsigned short W;
}MultiTimer1Temp;

// timer kompar�tor hi�ny�nak p�tl�sa
#define Timer1Compless() {                             \
  MultiTimer1Temp.LO = TMR1L;                        \
  MultiTimer1Temp.HI = TMR1H;                        \
  MultiTimer1Temp.W += 65535 + TM1COMPCOR - TM1COMP; \
  TMR1H = MultiTimer1Temp.HI;                        \
  TMR1L = MultiTimer1Temp.LO;}

// timer1 inicializ�l�s
#define Timer1Init() {\
  T1CONbits.RD16 = 1;                   /* 16bites �r�s/olv */\
  T1CONbits.TMR1CS = 0;                 /* forr�s �rajel = systemclock */\
  TM1CLKSEL;                            /* oszt� */\
  IPR1bits.TMR1IP = TIMER1PR18REG;      /* priorit�s */\
  PIE1bits.TMR1IE = 1;                  /* megszak�t�s enged�lyez�s */\
  RCONbits.IPEN = 1;                    /* k�tszint� priorit�s� enged�lyez�se */\
  TMR1H = (65535+TM1COMPCOR-TM1COMP) >> 8;/* timerH kezd��rt�k felt�lt�se bitid�re */\
  TMR1L = 65535+TM1COMPCOR-TM1COMP;}    /* timerL kezd��rt�k felt�lt�se bitid�re */

// timer1 bekapcsol�s
#define Timer1Start()  T1CONbits.TMR1ON = 1

// timer1 kikapcsol�s
#define Timer1Stop()   T1CONbits.TMR1ON = 0
  
// timer1 megszak�t�s nyugt�z�s
#define Timer1IrqAck() PIR1bits.TMR1IF = 0

// K�z�s megszak�t�kiszolg�l� f�ggv�nyb�l a timer1 megszak�t�skiszolg�l� f�ggv�ny h�v�sa
#define Timer1IntProcess(f) if(PIR1bits.TMR1IF) f()

#endif // TIME1CLK

//------------------------------------------------------------------------------
// PIC18 timer2
#ifdef TIME2CLK

// PIC18: timer IRQ priorit�sa (0 = alacsony, 1 = magas)
#if TIMER2PR18 == -1
// priorit�s n�lk�li megszak�t�s
#define TIMER2PR18REG   1
#elif TIMER2PR18 == 0
// alacsony priorit�s
#define TIMER2PR18REG   0
#elif TIMER2PR18 == 1
// magas priorit�s
#define TIMER2PR18REG   1
#else
#error "LCD TIMER2PR18 csak -1, 0, 1 lehet!"
#endif

// oszt�2
#if TIME2DIV < 256
#define TM2CLKDIV      1
#define TM2CLKSEL      T2CONbits.T2CKPS1=0; T2CONbits.T2CKPS0=0; T2CONbits.T2OUTPS3=0; T2CONbits.T2OUTPS2=0; T2CONbits.T2OUTPS1=0; T2CONbits.T2OUTPS0=0
#elif TIME2DIV < (256*2)
#define TM2CLKDIV      2
#define TM2CLKSEL      T2CONbits.T2CKPS1=0; T2CONbits.T2CKPS0=1; T2CONbits.T2OUTPS3=0; T2CONbits.T2OUTPS2=0; T2CONbits.T2OUTPS1=0; T2CONbits.T2OUTPS0=1
#elif TIME2DIV < (256*4)
#define TM2CLKDIV      4
#define TM2CLKSEL      T2CONbits.T2CKPS1=0; T2CONbits.T2CKPS0=1; T2CONbits.T2OUTPS3=0; T2CONbits.T2OUTPS2=0; T2CONbits.T2OUTPS1=0; T2CONbits.T2OUTPS0=0
#elif TIME2DIV < (256*8)
#define TM2CLKDIV      8
#define TM2CLKSEL      T2CONbits.T2CKPS1=0; T2CONbits.T2CKPS0=1; T2CONbits.T2OUTPS3=0; T2CONbits.T2OUTPS2=0; T2CONbits.T2OUTPS1=0; T2CONbits.T2OUTPS0=1
#elif TIME2DIV < (256*16)
#define TM2CLKDIV      16
#define TM2CLKSEL      T2CONbits.T2CKPS1=1; T2CONbits.T2CKPS0=0; T2CONbits.T2OUTPS3=0; T2CONbits.T2OUTPS2=0; T2CONbits.T2OUTPS1=0; T2CONbits.T2OUTPS0=0
#elif TIME2DIV < (256*16*2)
#define TM2CLKDIV      32
#define TM2CLKSEL      T2CONbits.T2CKPS1=1; T2CONbits.T2CKPS0=0; T2CONbits.T2OUTPS3=0; T2CONbits.T2OUTPS2=0; T2CONbits.T2OUTPS1=0; T2CONbits.T2OUTPS0=1
#elif TIME2DIV < (256*16*4)
#define TM2CLKDIV      64
#define TM2CLKSEL      T2CONbits.T2CKPS1=1; T2CONbits.T2CKPS0=0; T2CONbits.T2OUTPS3=0; T2CONbits.T2OUTPS2=0; T2CONbits.T2OUTPS1=1; T2CONbits.T2OUTPS0=1
#elif TIME2DIV < (256*16*8)
#define TM2CLKDIV      128
#define TM2CLKSEL      T2CONbits.T2CKPS1=1; T2CONbits.T2CKPS0=0; T2CONbits.T2OUTPS3=0; T2CONbits.T2OUTPS2=1; T2CONbits.T2OUTPS1=1; T2CONbits.T2OUTPS0=1
#elif TIME2DIV < (256*16*16)
#define TM2CLKDIV      256
#define TM2CLKSEL      T2CONbits.T2CKPS1=1; T2CONbits.T2CKPS0=0; T2CONbits.T2OUTPS3=1; T2CONbits.T2OUTPS2=1; T2CONbits.T2OUTPS1=1; T2CONbits.T2OUTPS0=1
#else
#error "PIC18: timer frekvencia t�l alacsony"
#endif // TIME2DIV

// timer2 kompar�torba ennyit kell berakni hogy a k�v�nt frekvencia legyen
#define TM2COMP (((Timer2SrcClock/TM2CLKDIV)+TIME2CLK/2)/TIME2CLK-1)

// mivel van timer kompar�tor, nem kell szoftverb�l p�tolni
#define Timer2Compless() ;

// timer2 inicializ�l�s
#define Timer2Init() {\
  TM2CLKSEL;                            /* oszt� */\
  PR2 = TM2COMP;                        /* kompar�tor */\
  IPR1bits.TMR2IP = TIMER2PR18REG;      /* priorit�s */\
  T2CONbits.TMR2ON = 1;                 /* timer enged�lyez�s */\
  RCONbits.IPEN = 1;}                   /* k�tszint� priorit�s� enged�lyez�se */

// timer2 bekapcsol�s (megszak�t�s enged�ly be)
#define Timer2Start()  PIE1bits.TMR2IE = 1

// timer2 bekapcsol�s (megszak�t�s enged�ly ki)
#define Timer2Stop()   PIE1bits.TMR2IE = 0
  
// timer2 megszak�t�s nyugt�z�s
#define Timer2IrqAck() PIR1bits.TMR2IF = 0

// K�z�s megszak�t�kiszolg�l� f�ggv�nyb�l a timer2 megszak�t�skiszolg�l� f�ggv�ny h�v�sa
#define Timer2IntProcess(f) if(PIR1bits.TMR2IF) f()

#endif // TIME2CLK

//------------------------------------------------------------------------------
// PIC18 timer3
#ifdef TIME3CLK

// PIC18: timer IRQ priorit�sa (0 = alacsony, 1 = magas)
#if TIMER3PR18 == -1
// priorit�s n�lk�li megszak�t�s
#define TIMER3PR18REG   1
#elif TIMER3PR18 == 0
// alacsony priorit�s
#define TIMER3PR18REG   0
#elif TIMER3PR18 == 1
// magas priorit�s
#define TIMER3PR18REG   1
#else
#error "LCD TIMER3PR18 csak -1, 0, 1 lehet!"
#endif

// oszt�3
#if TIME3DIV < 65536
#define TM3CLKDIV      1
#define TM3CLKSEL      T3CONbits.T3CKPS0 = 0; T3CONbits.T3CKPS1 = 0 /* el�oszt� 1:1 */
#define TM3COMPCOR     5
#elif TIME3DIV < (65536*2)
#define TM3CLKDIV      2
#define TM3CLKSEL      T3CONbits.T3CKPS0 = 0; T3CONbits.T3CKPS1 = 1 /* el�oszt� 1:2 */
#define TM3COMPCOR     3
#elif TIME3DIV < (65536*4)
#define TM3CLKDIV      4
#define TM3CLKSEL      T3CONbits.T3CKPS0 = 1; T3CONbits.T3CKPS1 = 0 /* el�oszt� 1:4 */
#define TM3COMPCOR     2
#elif TIME3DIV < (65536*8)
#define TM3CLKDIV      8
#define TM3CLKSEL      T3CONbits.T3CKPS0 = 1; T3CONbits.T3CKPS1 = 1 /* el�oszt� 1:8 */
#define TM3COMPCOR     1
#else
#error "PIC18: timer3 frekvencia t�l alacsony"
#endif // TIME3DIV

// timer3 kompar�torba ennyit kell berakni hogy a k�v�nt frekvencia legyen
#define TM3COMP (((Timer3SrcClock/TM3CLKDIV)+TIME3CLK/2)/TIME3CLK-1)

// timer kompar�tor hi�ny�nak p�tl�s�hoz �tmeneti v�ltoz�
union
{
  struct
  {
    unsigned char LO;
    unsigned char HI;
  };
  unsigned short W;
}MultiTimer3Temp;

// timer kompar�tor hi�ny�nak p�tl�sa
#define Timer3Compless() {                             \
  MultiTimer3Temp.LO = TMR3L;                        \
  MultiTimer3Temp.HI = TMR3H;                        \
  MultiTimer3Temp.W += 65535 + TM3COMPCOR - TM3COMP; \
  TMR3H = MultiTimer3Temp.HI;                        \
  TMR3L = MultiTimer3Temp.LO;}

// timer3 inicializ�l�s
#define Timer3Init() {\
  T3CONbits.RD16 = 1;                   /* 16bites �r�s/olv */\
  T3CONbits.TMR3CS = 0;                 /* forr�s �rajel = systemclock */\
  TM3CLKSEL;                            /* oszt� */\
  IPR2bits.TMR3IP = TIMER3PR18REG;      /* priorit�s */\
  PIE2bits.TMR3IE = 1;                  /* megszak�t�s enged�lyez�s */\
  TMR3H = (65535+TM3COMPCOR-TM3COMP) >> 8;/* timerH kezd��rt�k felt�lt�se bitid�re */\
  TMR3L = 65535+TM3COMPCOR-TM3COMP;}    /* timerL kezd��rt�k felt�lt�se bitid�re */

// timer3 bekapcsol�s
#define TIMERSTART  T3CONbits.TMR3ON = 1

// timer3 kikapcsol�s
#define TIMERSTOP   T3CONbits.TMR3ON = 0
  
// timer3 megszak�t�s nyugt�z�s
#define TIMERIRQACK PIR2bits.TMR3IF = 0

// K�z�s megszak�t�kiszolg�l� f�ggv�nyb�l a timer3 megszak�t�skiszolg�l� f�ggv�ny h�v�sa
#define Timer3IntProcess(f) if(PIR2bits.TMR3IF) f()

#endif // TIME3CLK

 // priorit�s be�ll�t�s + glob�lis IRQ eng
#if (TIMER0PR18 != -1) || (TIMER1PR18 != -1) || (TIMER2PR18 != -1) || (TIMER3PR18 != -1)
#define TimersGlobalIrq()  {RCONbits.IPEN = 1; INTCONbits.GIEL = 1; INTCONbits.GIEH = 1}
#else
#define TimersGlobalIrq()  {RCONbits.IPEN = 0; INTCONbits.GIE = 1; INTCONbits.PEIE = 1}
#endif

// PIC18
//==============================================================================
// PIC24
#elif defined(__C30__)

//------------------------------------------------------------------------------
// PIC24 timer1
#ifdef TIME1CLK

// oszt�1
#if TIME1DIV < 65536
#define TM1CLKDIV      1
#define TM1CLKSEL      T1CONbits.TCKPS = 0
#elif TIME1DIV < (65536*8)
#define TM1CLKDIV      8
#define TM1CLKSEL      T1CONbits.TCKPS = 1
#elif TIME1DIV < (65536*64)
#define TM1CLKDIV      64
#define TM1CLKSEL      T1CONbits.TCKPS = 2
#elif TIME1DIV < (65536*256)
#define TM1CLKDIV      256
#define TM1CLKSEL      T1CONbits.TCKPS = 3
#else
#error "PIC24: timer1 frekvencia t�l alacsony"
#endif  // TIME1DIV

// timer1 kompar�torba ennyit kell berakni hogy a k�v�nt frekvencia legyen
#define TM1COMP (((Timer1SrcClock/TM1CLKDIV)+TIME1CLK/2)/TIME1CLK-1)

// mivel van timer kompar�tor, nem kell szoftverb�l p�tolni
#define Timer1Compless() ;

// timer1 inicializ�l�s
#define Timer1Init() {\
  T1CONbits.TCS = 0;                    /* forr�s �rajel = systemclock */\
  TM1CLKSEL;                            /* el�oszt� */\
  IPC0bits.T1IP = TIMER1PR24;           /* priorit�s */\
  PR1 = TM1COMP;                        /* id�z�t� be�ll�t�sa a bitid�h�z */\
  T1CONbits.TON = 1;}                   /* timer enged�lyez�s */

// timer1 bekapcsol�s (megszak�t�s enged�ly be)
#define Timer1Start()  IEC0bits.T1IE = 1

// timer bekapcsol�s (megszak�t�s enged�ly ki)
#define Timer1Stop()   IEC0bits.T1IE = 0
  
// timer megszak�t�s nyugt�z�s
#define Timer1IrqAck() IFS0bits.T1IF = 0

// timer megszak�t�s kiszolg�l� f�ggv�ny
#define Timer1Int() void __attribute__ ((interrupt, auto_psv)) _T1Interrupt(void)

#endif // TIME1CLK

//------------------------------------------------------------------------------
// PIC24 timer2
#ifdef TIME2CLK

// oszt�2
#if TIME2DIV < 65536
#define TM2CLKDIV      1
#define TM2CLKSEL      T2CONbits.TCKPS = 0
#elif TIME2DIV < (65536*8)
#define TM2CLKDIV      8
#define TM2CLKSEL      T2CONbits.TCKPS = 1
#elif TIME2DIV < (65536*64)
#define TM2CLKDIV      64
#define TM2CLKSEL      T2CONbits.TCKPS = 2
#elif TIME2DIV < (65536*256)
#define TM2CLKDIV      256
#define TM2CLKSEL      T2CONbits.TCKPS = 3
#else
#error "PIC24: timer2 frekvencia t�l alacsony"
#endif  // TIME2DIV

// timer2 kompar�torba ennyit kell berakni hogy a k�v�nt frekvencia legyen
#define TM2COMP (((Timer2SrcClock/TM2CLKDIV)+TIME2CLK/2)/TIME2CLK-1)

// timer2 inicializ�l�s
#define Timer2Init() {\
  T2CONbits.TCS = 0;                    /* forr�s �rajel = systemclock */\
  TM2CLKSEL;                            /* oszt� */\
  IPC1bits.T2IP = TIMER2PR24;           /* priorit�s */\
  PR2 = TM2COMP;                        /* id�z�t� be�ll�t�sa a bitid�h�z */\
  T2CONbits.TON = 1;}                   /* timer enged�lyez�s */

// timer2 bekapcsol�s (megszak�t�s enged�ly be)
#define Timer2Start()  IEC0bits.T2IE = 1

// timer2 bekapcsol�s (megszak�t�s enged�ly be)
#define Timer2Stop()   IEC0bits.T2IE = 0
  
// timer2 megszak�t�s nyugt�z�s
#define Timer2IrqAck() IFS0bits.T2IF = 0

// timer2 megszak�t�s kiszolg�l� f�ggv�ny
#define Timer2Int() void __attribute__ ((interrupt, auto_psv)) _T2Interrupt(void)

#endif // TIME2CLK

//------------------------------------------------------------------------------
// PIC24 timer3
#ifdef TIME3CLK

// oszt�3
#if TIME3DIV < 65536
#define TM3CLKDIV      1
#define TM3CLKSEL      T3CONbits.TCKPS = 0
#elif TIME3DIV < (65536*8)
#define TM3CLKDIV      8
#define TM3CLKSEL      T3CONbits.TCKPS = 1
#elif TIME3DIV < (65536*64)
#define TM3CLKDIV      64
#define TM3CLKSEL      T3CONbits.TCKPS = 2
#elif TIME3DIV < (65536*256)
#define TM3CLKDIV      256
#define TM3CLKSEL      T3CONbits.TCKPS = 3
#else
#error "PIC24: timer3 frekvencia t�l alacsony"
#endif  // TIME3DIV

// timer3 kompar�torba ennyit kell berakni hogy a k�v�nt frekvencia legyen
#define TM3COMP (((Timer3SrcClock/TM3CLKDIV)+TIME3CLK/2)/TIME3CLK-1)

// timer3 inicializ�l�s
#if defined(__dsPIC30F__)
#define TIME3RINIT {\
  T3CONbits.TCS = 0;                    /* forr�s �rajel = systemclock */\
  TM3CLKSEL;                            /* oszt� */\
  IPC1bits.T3IP = TIMER3PR24;           /* priorit�s */\
  PR3 = TM3COMP;                        /* id�z�t� be�ll�t�sa a bitid�h�z */\
  T3CONbits.TON = 1;}                   /* timer enged�lyez�s */
#else
#define TIME3RINIT {\
  T3CONbits.TCS = 0;                    /* forr�s �rajel = systemclock */\
  TM3CLKSEL;                            /* oszt� */\
  IPC2bits.T3IP = TIMER3PR24;           /* priorit�s */\
  PR3 = TM3COMP;                        /* id�z�t� be�ll�t�sa a bitid�h�z */\
  T3CONbits.TON = 1;}                   /* timer enged�lyez�s */
#endif

// timer3 bekapcsol�s (megszak�t�s enged�ly be)
#define Timer3Start()  IEC0bits.T3IE = 1

// timer3 bekapcsol�s (megszak�t�s enged�ly be)
#define Timer3Stop()   IEC0bits.T3IE = 0
  
// timer3 megszak�t�s nyugt�z�s
#define Timer3IrqAck() IFS0bits.T3IF = 0

// timer3 megszak�t�s kiszolg�l� f�ggv�ny
#define Timer3Int() void __attribute__ ((interrupt, auto_psv)) _T3Interrupt(void)

#endif // TIME3CLK

//------------------------------------------------------------------------------
// PIC24 timer4
#ifdef TIME4CLK

// oszt�4
#if TIME4DIV < 65536
#define TM4CLKDIV      1
#define TM4CLKSEL      T4CONbits.TCKPS = 0
#elif TIME4DIV < (65536*8)
#define TM4CLKDIV      8
#define TM4CLKSEL      T4CONbits.TCKPS = 1
#elif TIME4DIV < (65536*64)
#define TM4CLKDIV      64
#define TM4CLKSEL      T4CONbits.TCKPS = 2
#elif TIME4DIV < (65536*256)
#define TM4CLKDIV      256
#define TM4CLKSEL      T4CONbits.TCKPS = 3
#else
#error "PIC24: timer4 frekvencia t�l alacsony"
#endif  // TIME4DIV

// timer4 kompar�torba ennyit kell berakni hogy a k�v�nt frekvencia legyen
#define TM4COMP (((Timer4SrcClock/TM4CLKDIV)+TIME4CLK/2)/TIME4CLK-1)

// timer4 inicializ�l�s
#if defined(__dsPIC30F__)
#define Timer4Init() {\
  T4CONbits.TCS = 0;                    /* forr�s �rajel = systemclock */\
  TM4CLKSEL;                            /* oszt� */\
  IPC5bits.T4IP = TIMER4PR24;           /* priorit�s */\
  PR4 = TM4COMP;                        /* id�z�t� be�ll�t�sa a bitid�h�z */\
  T4CONbits.TON = 1;}                   /* timer enged�lyez�s */
#else
#define Timer4Init() {\
  T4CONbits.TCS = 0;                    /* forr�s �rajel = systemclock */\
  TM4CLKSEL;                            /* oszt� */\
  IPC6bits.T4IP = TIMER4PR24;           /* priorit�s */\
  PR4 = TM4COMP;                        /* id�z�t� be�ll�t�sa a bitid�h�z */\
  T4CONbits.TON = 1;}                   /* timer enged�lyez�s */
#endif

// timer4 bekapcsol�s (megszak�t�s enged�ly be)
#define Timer4Start()  IEC1bits.T4IE = 1

// timer4 bekapcsol�s (megszak�t�s enged�ly be)
#define Timer4Stop()   IEC1bits.T4IE = 0
  
// timer4 megszak�t�s nyugt�z�s
#define Timer4IrqAck() IFS1bits.T4IF = 0

// timer4 megszak�t�s kiszolg�l� f�ggv�ny
#define Timer4Int() void __attribute__ ((interrupt, auto_psv)) _T4Interrupt(void)

#endif // TIME4CLK

//------------------------------------------------------------------------------
// PIC24 timer5
#ifdef TIME5CLK

// oszt�5
#if TIME5DIV < 65536
#define TM5CLKDIV      1
#define TM5CLKSEL      T5CONbits.TCKPS = 0
#elif TIME5DIV < (65536*8)
#define TM5CLKDIV      8
#define TM5CLKSEL      T5CONbits.TCKPS = 1
#elif TIME5DIV < (65536*64)
#define TM5CLKDIV      64
#define TM5CLKSEL      T5CONbits.TCKPS = 2
#elif TIME5DIV < (65536*256)
#define TM5CLKDIV      256
#define TM5CLKSEL      T5CONbits.TCKPS = 3
#else
#error "PIC24: timer5 frekvencia t�l alacsony"
#endif  // TIME5DIV

// timer5 kompar�torba ennyit kell berakni hogy a k�v�nt frekvencia legyen
#define TM5COMP (((Timer5SrcClock/TM5CLKDIV)+TIME5CLK/2)/TIME5CLK-1)

// timer5 inicializ�l�s
#if defined(__dsPIC30F__)
#define Timer5Init() {\
  T5CONbits.TCS = 0;                    /* forr�s �rajel = systemclock */\
  TM5CLKSEL;                            /* el�oszt� */\
  IPC5bits.T5IP = TIMER5PR24;           /* priorit�s */\
  PR5 = TM5COMP;                        /* id�z�t� be�ll�t�sa a bitid�h�z */\
  T5CONbits.TON = 1;}                   /* timer enged�lyez�s */
#else
#define Timer5Init() {\
  T5CONbits.TCS = 0;                    /* forr�s �rajel = systemclock */\
  TM5CLKSEL;                            /* el�oszt� */\
  IPC7bits.T5IP = TIMER5PR24;           /* priorit�s */\
  PR5 = TM5COMP;                        /* id�z�t� be�ll�t�sa a bitid�h�z */\
  T5CONbits.TON = 1;}                   /* timer enged�lyez�s */
#endif

// timer5 bekapcsol�s (megszak�t�s enged�ly be)
#define Timer5Start()  IEC1bits.T5IE = 1

// timer5 bekapcsol�s (megszak�t�s enged�ly be)
#define Timer5Stop()   IEC1bits.T5IE = 0
  
// timer5 megszak�t�s nyugt�z�s
#define Timer5IrqAck() IFS1bits.T5IF = 0

// timer5 megszak�t�s kiszolg�l� f�ggv�ny
#define Timer5Int() void __attribute__ ((interrupt, auto_psv)) _T5Interrupt(void)

#endif // TIME5CLK

#endif // defined(__C30__)

//==============================================================================
// timer (v�ge)
//==============================================================================

#endif // __MULTITIMER_H