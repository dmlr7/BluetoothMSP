#include <msp430.h> 
#include "driverlib/usci_a_uart.h"
#include "driverlib/eusci_a_uart.h"

/*
*main.c
*/
volatile unsigned int i;
void b1(void){
    P1DIR |= 0x01;
	for(;;){
		P1OUT |= ~0x01;
		i=50000;
		do{
		    i--;
		}while(i);
	}
    
}
void b2(){
    P1DIR |= BIT1;
    
    P2REN |= BIT1;
    P2OUT |= BIT1;
    P2IES |= BIT1;  // P2.1 Hi/lo edge
    P2IFG |= BIT1;  // P2.1 zIFG cleared just in case
    
	for(;;){
		P1OUT |= ~0x01;
		i=50000;
		do{
		    i--;
		}while(i);
	}
}
void __low_level_init(void){
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
}

int main(void){

	b1();

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
#pragma vector=PORT1_VECTOR
__interrupt void PORT2_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT2_VECTOR)))PORT2_ISR (void)
#else
#error Compiler not supported!
#endif
{
    P1OUT ^= ~BIT1;     // Toggle LED at P1.0
    P2IFG &= ~BIT1; // P2.1 IFG cleared

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
