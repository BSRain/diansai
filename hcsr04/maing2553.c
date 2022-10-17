#include <msp430.h>
#include <stdint.h>

uint16_t volatile contador_milisegundos; // miliseconds counter
uint32_t volatile start_time = 0; // start time of ECHO pulse
uint32_t volatile end_time = 0; // end time of ECHO pulse
uint16_t volatile distancia_cm = 0; // distance measured

#define HZ 16000000UL // 16MHz MCLK


void Inicializacion_Relojes(void) // to use the SMCLK --> 16 MHz
{
        __bis_SR_register(SCG0); // Disable the FLL control loop
    UCSCTL0 = 0x0000; // Ponemos el DCOx y MODx al minimo posible
    UCSCTL1 = DCORSEL_5; // Seleccionamos un rango de operaci��n del DCO range de 16MHz
    UCSCTL2 = FLLD_0 | 487; // Poniendo FLLD_0 hacemos tomamos como 1 el divisor de la frecuencia de entrada del cristal de cuarzo y 487 es el valor multiplicador de FLLN
    // (N + 1) * (FLLRef/n) = Fdcomsp430
    // (487 + 1) * (32768/1) = 16MHz
    UCSCTL3 = 0; // FLL SELREF = XT1CLK y divisor de FLL = 1 (FLLREFDIV = FLLREFCLK/1)
    UCSCTL4 |= SELA_0 | SELS_4 | SELM_4; // Tomamos ACLK = XT1CLK (Cuarzo externo de 2^15 bits); SMCLK = MCLK = DCOCLKDIV (DCO interno de 16 MHz)
    UCSCTL5 |= DIVA_0 | DIVS_0; // Divisor para SMCLK = f(SMCLK)/1; ACLK = f(ACLK)/1
    __bic_SR_register(SCG0); // Enable the FLL control loop
}

void init_pines_ultrasonidos(void)
{
    P1DIR &= ~BIT1; // Pin ECHO as input
    P1SEL |= BIT1; // mode Capture Input Signal (CCI0A)
    P3DIR |= BIT7; // Pin TRIGGER as output
    P3SEL &= ~BIT7; // Gpio pin
    P3SEL &= ~BIT7; // Pin TRIGGER low
}

void pin_buzzer(void) // pin P3.6 as TB0.6 --> P3DIR.6 = 1; P3SEL.6 = 1 (see Table 6-48 Datasheet MSP430F5529)
{
    P3SEL |= BIT6; // P3.6 as alternative function (para Timer TB0.6 seg��n datasheet MSP430F5529)
    P3DIR |= BIT6; // Pin as buzzer output
    P4SEL &= ~BIT7; // Pin GPIO
    P4DIR |= BIT7; // como salida (Led2, verde)
    P4OUT &= ~BIT7; // led inicialmente apagado
}

void delay_ms(uint16_t tiempo_milisegundos) // Function for delay of miliseconds --> SMCLK
{
    contador_milisegundos = 0;
    TA1CTL |= MC_1; // UP MODE
    TA1CCTL0 |= CCIE;
    while(contador_milisegundos < tiempo_milisegundos);
    TA1CTL &= ~MC_3; // stop the timer
}

void TimerA0_capture(void) // Timer for capturing ECHO signal of ultrasonic sensor --> TA0.0 en modo Capture Input (CCI0A)
{
    TA0CCTL0 |= CM_3 | SCS | CCIS_0 | CAP | CCIE; // CM_3: Capture both rising and falling edges of signal; CCIS_0: CCIxA --> Capture/Compare in TimerA; CAP: enable capture mode; CCIE: enable interrupciones
    TA0CTL |= TASSEL_2 | MC_2 | ID_0 | TACLR; // TASSEL_2: SMCLK; MC_2: Continuous Mode; ID_0: f(SMCLK)/1; TACLR: Reset Timer
}

void init_TimerA1_ms(void) // for delay
{
    TA1CCR0 = 16000-1; // pulses for 1 milisecond
    TA1CTL |= TASSEL_2 | MC_0; // SMCLK, Freq: 16 MHz; MC_1: Stop Mode
}

void TimerB0_PWM(void) // Timer for monitoring PWM of buzzer
{
    TB0CCR0 = 249; // Per��odo de PWM: 250 pulsos --> 4kHz (o 0,25 ms) --> 1000 pulses/ms �� 0,25 ms (inverse of 4kHz) = 250 - 1 = 249.
    TB0CCTL6 |= OUTMOD_7; // Modo CCR6: PWM Reset/Set --> increasing CCR6 value will also increase Duty Cycle --> more buzzer amplitude
    TB0CCR6 = 0; // Duty Cycle inicial: 0 %. TB0.6 --> TB0CCR6.
    TB0CTL |= TASSEL_2 | ID_0 | MC_1 | TACLR; // TASSEL_2: SMCLK; ID_2: Divisor freq --> f(SMCLK)/1; MC_1: Up Mode
}

void main()
{
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
    Inicializacion_Relojes();
    init_pines_ultrasonidos();
    //pin_buzzer_and_LED();
    TimerA0_capture();
    init_TimerA1_ms();
    TimerB0_PWM();
    __enable_interrupt();
    while(1){
        P3OUT |= BIT7; // generate pulse TRIGGER
        __delay_cycles(10*HZ/1000000UL); // pulse of 10 us in TRIGGER pin for obtaining the measures (see datasheet HC-SR04)
        P3OUT &= ~BIT7; // disable TRIGGER pulse
        delay_ms(100); // delay over 60 ms measurement cycle in order to prevent trigger signal to the echo signal (see Datasheet HC-SR04)
        if(distancia_cm < 20 && distancia_cm != 0){ // if distance between object and the sensor is lower than 20 cm and also different from zero..
            P4OUT |= BIT7; // LED green ON
            TB0CCR6 = 0; // Reset value of Timer that decides the final PWM value
            TB0CCR6 += 249 - (distancia_cm * 12); // Aumentamos la frecuencia del tono del buzzer conforme estamos m��s cerca del objeto (a menor distancia)
        }
    }
}


#pragma vector=TIMER1_A0_VECTOR
__interrupt void timer1_A0_ISR(void)
{
    contador_milisegundos++;
    TA1CCTL0 &= ~CCIFG;
}

#pragma vector=TIMER0_A0_VECTOR // ISR de Capture Signal TA0.0
__interrupt void Timer0_A0(void)
{
    if(TA0CCTL0 & CCI){ // Read the CCI bit (ECHO signal) --> CCI reflects the state of Captura pin --> ECHO signal
        start_time = TA0CCR0; //takes first time measure in us
    }
    else{
        end_time = TA0CCR0; //takes second time measure in us
        distancia_cm = (end_time - start_time)/58; // to obtain distance in centimeters: (time in us/58) --> (see Datasheet HC-SR04)
    }
    TA0CTL &= ~TAIFG;
}