#include <msp430.h>
//#include "driverlib/usci_a_uart.h"
//#include "driverlib/eusci_a_uart.h"
/*
 * Definig personal vars
 */
#define LED1            BIT0
#define LED2            BIT7
#define LED3            BIT2
#define SWITHC          BIT1
#define PDL1            P1DIR
#define POL1            P1OUT
#define PDL2            P4DIR
#define POL2            P4OUT
#define time            25000000
//Defining usable pins
#define B1              BIT1
#define B2              BIT1
#define B3              BIT1
#define B4              BIT1
#define B5              BIT1
#define B6              BIT1
#define B7              BIT1
#define B8              BIT1
/*
 * Energia.h functions
 */
#define true                        0x1
#define false                       0x0

#define PI                          3.1415926535897932384626433832795
#define HALF_PI                     1.5707963267948966192313216916398
#define TWO_PI                      6.283185307179586476925286766559
#define DEG_TO_RAD                  0.017453292519943295769236907684886
#define RAD_TO_DEG                  57.295779513082320876798154814105

#define min(a,b)                    ((a)<(b)?(a):(b))
#define max(a,b)                    ((a)>(b)?(a):(b))
#define constrain(amt,low,high)     ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)                    ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg)                ((deg)*DEG_TO_RAD)
#define degrees(rad)                ((rad)*RAD_TO_DEG)
#define sq(x)                       ((x)*(x))
/*
*main.c
*/
volatile unsigned int i;
int init_flags[3]={ 0 };
int flags[3]={0};
void configSwitch(int port,int pin){

}
void null(){}
void b1(void){
    init_flags[0]==0?P1DIR |= 0x01:null();
	for(;;){
	    if(flags[0])
	      goto end;
		P1OUT |= ~0x01;
		i=(int)time;
		do{
		    i--;
		}while(i);
	}
  end:
}
void b2(){
    P1DIR |= BIT1;

    P2REN |= BIT1;
    P2OUT |= BIT1;
    P2IES |= BIT1;  // P2.1 Hi/lo edge
    P2IFG |= BIT1;  // P2.1 zIFG cleared just in case

	for(;;){
	    if(flags[1])
	      goto end;
		P1OUT |= ~0x01;
		i=(int)time;
		do{
		    i--;
		}while(i);
	}
  end:;
}
void b3(oid){
  P3SEL |= BIT3+BIT4;                       // P3.3,4 = USCI_A0 TXD/RXD
  UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = 9;                              // 1MHz 115200 (see User's Guide)
  UCA0BR1 = 0;                              // 1MHz 115200
  UCA0MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

  for(;;){
      if(flags[2])
	      goto end;
		P1OUT |= ~LED3;
		i=(int)time;
		do{
		    i--;
		}while(i);
	}
  end:;
}
void __low_level_init(void){
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
}

int main(void){
    P1DIR=B1;
    P2DIR=B2;
    P3DIR=B3;
    P4DIR=B4;
    P5DIR=B5;
    P6DIR=B6;
    P7DIR=B7;
    P8DIR=B8;
    void (*test)(void);
	test= &b1;
  func:
    test();
    goto func;
//UBR00=0x1A; UBR10=0x00; UMCTL0=0x00;  uart0 1000000Hz 38461bps
//UBR01=0x1A; UBR11=0x00; UMCTL1=0x00;  uart1 1000000Hz 38461bps
/*
  P3SEL |= BIT3+BIT4;                       // P3.3,4 = USCI_A0 TXD/RXD
  UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = 9;                              // 1MHz 115200 (see User's Guide)
  UCA0BR1 = 0;                              // 1MHz 115200
  UCA0MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
*/
  //__bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
  //__no_operation();                         // For debugger
}

// Port 2 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT2_VECTOR)))PORT2_ISR (void)
#else
#error Compiler not supported!
#endif
{
    P4OUT ^= ~BIT7;     // Toggle LED at P1.0
    P2IFG &= ~BIT1;     // P2.1 IFG cleared
}

// Echo back RXed character, confirm TX buffer is ready first
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCA0IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG
    while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
    UCA0TXBUF = UCA0RXBUF;                  // TX -> RXed character
    break;
  case 4:break;                             // Vector 4 - TXIFG
  default: break;
  }
}
