#include <msp430.h>
#include <stdint.h>
#include <stdio.h>

// UART1 utilized USB
void configureUART1() {
    P1DIR |= BIT0;                              // Sets P1.0 LED to the output direction
    P1OUT &= ~BIT0;                             // Initially clears P1.0 LED
    P4SEL |= BIT4 | BIT5;                       // P4.5 is connected to RX and P4.4 is connected to TX
    UCA1CTL1 = UCSWRST;                         // Enables software reset
    UCA1CTL1 = UCSSEL_1;                        // Sets ACLK as the clock source
    UCA1BR0 = 3;                                // Sets the baud rate to 9600
    UCA1BR1 = 0;                                // Sets the baud rate to 9600
    UCA1MCTL |= UCBRS_3 | UCBRF_0;              //
    UCA1CTL1 &= ~UCSWRST;                       // Disables software reset
    UCA1IE |= UCRXIE;                           // Enables RX based interrupts
    UCA1IE |= UCTXIE;                           // Enables TX based interrupts
}

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                     // Disables watchdog timer
  configureUART1();                             // Configures UART1
  while (1){
    __bis_SR_register(GIE);                     // Enables global interrupts
  }
}

#pragma vector=USCI_A1_VECTOR
__interrupt void UART1(void) {
    P1OUT |= BIT0;                              // Turns on the P1.0 LED
    if (UCA1IFG & UCTXIFG) {                    // If the TX interrupt flag is triggered
        UCA1IFG &= ~UCTXIFG;                    // Clears the TX interrupt flag
    } if (UCA1IFG & UCRXIFG){                   // If the RX interrupt flag is triggered
        UCA1TXBUF = UCA1RXBUF;
    }
    P1OUT &= ~BIT0;                             // Turns off the P1.0 LED
}
