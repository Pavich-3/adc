#include <avr/io.h>
#include <avr/interrupt.h>

// Define the microcontroller's clock frequency (8 MHz)
#define F_CPU 8000000UL

// Function prototypes for initialization
void ADC_Init(void);
void PWM_Init(void);
void TIMER_Init(void);

int main(void)
{
  // Call initialization functions for peripherals
  ADC_Init();   // Configure the Analog-to-Digital Converter
  PWM_Init();   // Configure PWM on Timer1
  TIMER_Init(); // Configure Timer0 for periodic interrupts

  // sei() - Enable global interrupts
  sei();

  // Infinite loop to keep the program running
  while(1);
}

/**
 * @brief Initializes the ADC (Analog-to-Digital Converter)
 */
void ADC_Init(void)
{
  // ADMUX: Set reference voltage and input channel
  // REFS0=1: Set reference to AVcc
  // MUX0=1:  Select ADC1 (pin PC1) as the input channel
  ADMUX |= (1 << REFS0) | (1 << MUX0);

  // ADCSRA: Main ADC Control and Status Register
  // ADEN=1:  Enable the ADC
  // ADATE=1: Enable Auto Trigger mode
  // ADPS2=1, ADPS1=1: Set prescaler to 64 (8MHz / 64 = 125kHz - optimal ADC clock)
  ADCSRA |= (1 << ADEN) | (1 << ADATE) | (1 << ADPS2) | (1 << ADPS1);

  // ADCSRB: Select the auto-trigger source
  // ADTS1=1, ADTS0=1: Set trigger source to Timer0 Compare Match A
  ADCSRB |= (1 << ADTS1) | (1 << ADTS0);

  // DIDR0: Disable digital input buffer on the ADC pin
  // ADC1D=1: Disable buffer on ADC1 to reduce noise and power consumption
  DIDR0 |= (1 << ADC1D);

  // ADSC=1: Start the first conversion manually. Subsequent conversions are auto-triggered.
  ADCSRA |= (1 << ADSC);
}

/**
 * @brief Initializes PWM (Pulse Width Modulation) on Timer1
 */
void PWM_Init(void)
{
  // DDRB: Data Direction Register for Port B
  // (1 << PB1): Set pin PB1 (which is OC1A) as an output
  DDRB |= (1 << PB1);

  // TCCR1A & TCCR1B: Timer/Counter1 Control Registers
  // COM1A1=1: Non-inverting PWM mode on OC1A output
  // WGM12, WGM11, WGM10 = 1: Set Fast PWM, 10-bit mode (TOP = 1023)
  TCCR1A |= (1 << COM1A1) | (1 << WGM11) | (1 << WGM10);
  TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10); // CS11, CS10 = 1: Set prescaler to 64, starts the timer
}

/**
 * @brief Initializes Timer0 for periodic interrupts
 */
void TIMER_Init(void)
{
  // TCCR0A: Timer/Counter0 Control Register A
  // WGM01=1: Set CTC (Clear Timer on Compare Match) mode
  TCCR0A |= (1 << WGM01);

  // TCCR0B: Timer/Counter0 Control Register B
  // CS02=1, CS00=1: Set prescaler to 1024
  TCCR0B |= (1 << CS02) | (1 << CS00);

  // OCR0A: Output Compare Register A. The timer will count up to this value.
  // 156: Value to get a frequency of ~50 Hz (8MHz / 1024 / (156+1) ≈ 49.76 Hz)
  OCR0A = 156;

  // TIMSK0: Timer/Counter0 Interrupt Mask Register
  // OCIE0A=1: Enable the interrupt on compare match with OCR0A
  TIMSK0 |= (1 << OCIE0A);
}

/**
 * @brief Interrupt Service Routine for Timer0 (runs at ~50 Hz)
 */
ISR(TIMER0_COMPA_vect)
{
  // Read the 10-bit ADC result (from the ADCW register)
  // and write it directly to the OCR1A compare register,
  // which updates the PWM duty cycle.
  OCR1A = ADCW;
}