#ifndef __UART_H
#define __UART_H

//==============================================================================
// Uart param�tereket itt lehet be�ll�tani
// - Inicializ�l�s: UartInit()
// - Ad�s: UartTx(unsigned char ch) (ha a h�v�s el�tt uTxBufFree == 0 -> nem �rdemes elk�ldeni, mert meg van telve)
// - V�tel: ch = UartRx() (csak akkor ad eredm�nyt, ha a h�v�s el�tt uRxLength > 0)

//------------------------------------------------------------------------------
#include "hardwareprofile.h"
// Ha k�ls� hardverprofilt haszn�lunk akkor azt itt adhatjuk meg
// - bels� hardverprofil eset�n kommentelj�k ki
// - k�ls� hardverprofil eset�n az ott megadott param�tereket ebben a file-ban kommentelj�k ki

//------------------------------------------------------------------------------
// Ad�s puffer m�rete (4, 8, 16, 32, 64, 128)
// #define UTXBUFFERSIZE 64

//------------------------------------------------------------------------------
// V�teli puffer m�rete (4, 8, 16, 32, 64, 128)
// #define URXBUFFERSIZE 64

//------------------------------------------------------------------------------
// Rendszer �rajel (UART oszt� be�ll�t�s�hoz)
// #define SystemClock   48000000

//------------------------------------------------------------------------------
// Soros port k�v�nt sebess�ge
// #define BAUDRATE      31250

//------------------------------------------------------------------------------
// Uart l�bak I/O be�ll�t�sa (inicializ�l�skor t�rt�nik ez meg)
// #define UARTPININIT   TRISCbits.TRISC6 = 0; TRISCbits.TRISC7 = 1

//------------------------------------------------------------------------------
// Teszt (v�teli puffer programb�l is fet�lthet� lesz, ha enged�lyezz�k)
// #define UARTDEBUG

//------------------------------------------------------------------------------
// csak PIC18xxx: melyik priorit�s� megszak�t�st haszn�lja a soros port
// lehets�ges �rt�kei:
// - -1 : nincs a t�bbszint� priorit�s haszn�lva (ilyenkor minden csak a HIGH interruptban megy)
// -  0 : alacsony priorit�s
// -  1 : magas priorit�s
#define RXPR18        -1
#define TXPR18        -1

//******************************************************************************
// Innent�l kezdve nem kell beleny�lni !
//
//==============================================================================
extern volatile unsigned char uRxLength; // v�teli pufferban lev� byte-ok sz�ma
extern volatile unsigned char uRxBufFree;// v�teli pufferben mennyi a szabad hely
extern volatile unsigned char uTxLength; // ad�s pufferben lev� byte-ok sz�ma
extern volatile unsigned char uTxBufFree;// ad�s pufferben mennyi a szabad hely

void UartInit(void);                   // inicializ�l�s

unsigned char UartRx(void);            // egy karakter v�tele (ha van mit)
unsigned char UartRxTop(void);         // a v�teli puffer legels�nek berakott eleme, de nem t�rl�dik a pufferb�l
#ifdef  UARTDEBUG
void UartRxWrite(unsigned char ch);    // tesztel�shez fel lehet t�lteni a v�teli puffert
#else
#define UartRxWrite(ch)  ;
#endif

void UartRxProcess(void);              // uart v�tel polling vagy megszak�t�s kiszolg�l� f�ggv�ny
#define UartRxIntProcess() if(PIE1bits.RCIE && PIR1bits.RCIF)UartRxProcess()

void UartTx(unsigned char ch);         // egy karakter k�ld�se (pufferelt m�don)
void UartTxProcess(void);              // uart ad�s polling vagy megszak�t�s kiszolg�l� f�ggv�ny
#define UartTxIntProcess() if(PIE1bits.TXIE && PIR1bits.TXIF)UartTxProcess()

#endif // __UART_H
