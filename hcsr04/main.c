#include <intrinsics.h>
#include <stdint.h>
#include <msp430.h>				

#define TRIGGER_PIN BIT5   // P6.1  //2.5
#define ECHO_PIN BIT3  // P1.3  
#define LED_PIN BIT0   // P1.0
#define DISTANCE_THRESHOLD 10  // cm
#define MEASURE_INTERVAL 2048  // ~250 ms

void triggerMeasurement() 
{
	// Start trigger
	P2OUT |= TRIGGER_PIN;

	// Wait a small amount of time with trigger high, > 10us required (~10 clock cycles at 1MHz MCLK)
	__delay_cycles(10);

	// End trigger
	P2OUT &= ~TRIGGER_PIN;
}

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;

	// Configure trigger pin, low to start
	P2DIR |= TRIGGER_PIN;
	P2OUT &= ~TRIGGER_PIN;

	// Configure LED, off to start
	P1DIR |= LED_PIN;
	P1OUT &= ~LED_PIN;

	// Configure echo pin as capture input to TA0CCR2
	P1DIR &= ~ECHO_PIN;
	P1SEL |= ECHO_PIN;

	// Set up TA0 to capture in CCR2 on both edges from P1.3 (echo pin)
	TA0CCTL2 = CM_3 | CCIS_0 | SCS | CAP | CCIE;

	// Set up TA0 to compare CCR0 (measure interval)
	TA0CCR0 = MEASURE_INTERVAL;
	TA0CCTL0 = CCIE;

	// Set up TA0 with ACLK / 4 = 8192 Hz
	TA0CTL = TASSEL__ACLK | ID__4 | MC__CONTINUOUS | TACLR;

	uint16_t lastCount = 0;
	uint32_t distance = 0;

	for (;;)
	{
		triggerMeasurement();

		// Wait for echo start
		__low_power_mode_3();

		lastCount = TA0CCR2;

		// Wait for echo end
		__low_power_mode_3();

		distance = TA0CCR2 - lastCount;
		distance *= 34000;
		distance >>= 14;  // division by 16384 (2 ^ 14)

		if (distance <= DISTANCE_THRESHOLD)
		{
			// Turn on LED
			P1OUT |= LED_PIN;
		}
		else
		{
			// Turn off LED
			P1OUT &= ~LED_PIN;
		}

		// Wait for the next measure interval tick
		__low_power_mode_3();
	}
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR (void) {
	// Measure interval tick
	__low_power_mode_off_on_exit();
	TA0CCR0 += MEASURE_INTERVAL;
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR (void) {
	// Echo pin state toggled
	__low_power_mode_off_on_exit();
	TA0IV = 0;
}