#include <p18cxxx.h>
#include "uart.h"
#include "HardwareProfile.h"

#pragma udata UART_BUFFER = UARTBUFFERADDR
char uRxBuffer[URXBUFFERSIZE];         // v�teli buffer
char uTxBuffer[UTXBUFFERSIZE];         // ad�s buffer

#pragma udata

// v�tel
//char uRxBufferLost = 0;
volatile unsigned char  uRxStart = 0, uRxLength = 0, uRxBufFree = URXBUFFERSIZE;

// ad�s
volatile char uTxBufferLost = 0;
volatile unsigned char  uTxStart = 0, uTxLength = 0, uTxBufFree = UTXBUFFERSIZE;

#define TRIS_(p, m)            TRIS ## p ## bits.TRIS ## p ## m
#define LAT_(p, m)             LAT ## p ## bits.LAT ## p ## m
#define PORT_(p, m)            PORT ## p ## bits.R ## p ## m

#define IOIN(x)                x(TRIS_) = 1
#define IOOUT(x)               x(TRIS_) = 0
#define SET(x)                 x(LAT_) = 1
#define CLR(x)                 x(LAT_) = 0
#define GET(x)                 x(PORT_)

//=============================================================================
void UartInit(void)
{
  #define BAUDDIV (SystemClock/BAUDRATE)

  #if BAUDDIV < (65536*4)
  #define BRG_DIV          4
  #define CLKSEL           1
  #elif BAUDDIV < (65536*16)
  #define BRG_DIV         16
  #define CLKSEL           0
  #else
  #error "PIC18: ez a BAUDRATE nem el��ll�that�"
  #endif // BAUDDIV

  #define BAUDRATEREG ((SystemClock+((BRG_DIV/2)*BAUDRATE))/BRG_DIV/BAUDRATE-1)
  SPBRG = BAUDRATEREG;                  // als� byte;
  SPBRGH = BAUDRATEREG >> 8;            // fels� byte
  TXSTAbits.BRGH = CLKSEL;              // lo/hi speed
  BAUDCONbits.BRG16 = 1;                // 16 bites
  IOIN(UARTRX); IOOUT(UARTTX);          // RX bemenet, TX kimenet
  RCSTAbits.SPEN = 1;                   // soros port eng
  TXSTAbits.SYNC = 0;                   // aszinkron m�d

  #if (RXPR18 == -1 && TXPR18 != -1) || (TXPR18 == -1 && RXPR18 != -1)
  #error "Hiba: priorit�s n�lk�li haszn�lat eset�n az RX �s a TX-nek is annak kell lennie!"
  #endif

  #if RXPR18 == -1

  #elif RXPR18 == 0
  IPR1bits.RCIP = 0;                    // alacsony priorit�s
  #elif RXPR18 == 1
  IPR1bits.RCIP = 1;                    // magas priorit�s
  #endif

  #if TXPR18 == -1

  #elif TXPR18 == 0
  IPR1bits.TXIP = 0;                    // alacsony priorit�s
  RCONbits.IPEN = 1;                    // k�tszint� IRQ eng
  #elif TXPR18 == 1
  IPR1bits.TXIP = 1;                    // magas priorit�s
  RCONbits.IPEN = 1;                    // k�tszint� IRQ eng
  #endif
   
  TXSTAbits.TXEN = 1;                   // ad�s enged�lyez�s
  RCSTAbits.CREN = 1;                   // v�tel enged�lyez�s
  PIE1bits.RCIE  = 1;                   // v�tel IRQ eng 
  // ad�s megszak�t�s enged�lyez�st csak forgalmaz�skor kell megtenni, ha m�r TXREG nem szabad
  
//  INTCONbits.GIEL = 1;                  // glob�lis IRQ eng
//  INTCONbits.GIEH = 1;                  // glob�lis IRQ eng
  INTCONbits.PEIE = 1;                  // perif�ria IRQ eng
}

//=============================================================================
// Rx pufferb�l egy byte-ot kiolvas, t�rl�dik is a pufferb�l
unsigned char UartRx(void)
{
  unsigned char ch;
  ch = 0;
  PIE1bits.RCIE = 0;                    // megszak�t�s tilt�s (hogy ne v�ltozhasson meg k�zben a puffermutat�)
  if(uRxLength)                         // van m�g adat a pufferben ?
  {
    ch = uRxBuffer[uRxStart++];
    uRxStart &= (URXBUFFERSIZE - 1);
    uRxLength--; uRxBufFree++;
  }
  PIE1bits.RCIE = 1;                    // megszak�t�s enged�lyez�s
  return ch;
}

//=============================================================================
// Rx pufferb�l �gy olvas ki egy byte-ot hog nem t�rl�dik a pufferb�l
unsigned char UartRxTop(void)
{
  return uRxBuffer[uRxStart];
}

//=============================================================================
void UartRxProcess(void)
{
  unsigned char c;
  c = RCREG;
  if(RCSTAbits.FERR | RCSTAbits.OERR)
  {
    RCSTAbits.CREN = 0; //clear error (if any)
    RCSTAbits.CREN = 1; //Enables Receiver
  }
  else
  {
    if (uRxLength != (URXBUFFERSIZE - 1)) // van m�g hely a pufferben ?
    {   
      uRxBuffer[(uRxStart + uRxLength) & (URXBUFFERSIZE - 1)] = c; // egy karakter megy a pufferbe
      uRxLength++; uRxBufFree--;
    }
  }
  PIR1bits.RCIF = 0;
}

#ifdef UARTDEBUG
//=============================================================================
void UartRxWrite(unsigned char ch)
{
  if (uRxLength != (URXBUFFERSIZE - 1)) // van m�g hely a pufferben ?
  {   
    uRxBuffer[(uRxStart + uRxLength) & (URXBUFFERSIZE - 1)] = ch; // egy karakter megy a pufferbe
    uRxLength++; uRxBufFree--;
  }
}
#endif

//=============================================================================
void UartTx(unsigned char ch)
{
  if (!uTxLength && PIR1bits.TXIF)      // buffer �res, �s TXREG �rht�
  {
    TXREG = ch;
  }
  else
  {
    PIE1bits.TXIE = 0;                  // TX IRQ tilt�s (hogy a megszak�t�s ne ny�ljon hozz� a puffermutat�khoz)
    if(uTxBufFree)                      // van m�g hely a pufferben ?
    {   
      uTxBuffer[(uTxStart + uTxLength) & (UTXBUFFERSIZE - 1)] = ch; // egy karakter megy a pufferbe
      uTxLength++; uTxBufFree--;
    }
    else
      uTxBufferLost = 1;                // itt jelezz�k hogy buffert�lcsordul�s miatt adatveszt�s t�rt�nt
    PIE1bits.TXIE = 1;                  // TX IRQ enged�lyez�s
  }
}
//=============================================================================
void UartTxProcess(void)
{
  PIR1bits.TXIF = 0;                    // megszak�t�s forr�s jelz�s t�rl�se
  if(!uTxLength)                        // ha m�r nincs mit adni
    PIE1bits.TXIE = 0;                  // TX IRQ tilt�s
  else
  {
    TXREG = uTxBuffer[uTxStart++];      // TXREG = adat mehet
    uTxStart &= (UTXBUFFERSIZE - 1);
    uTxLength--; uTxBufFree++;
  }
}
