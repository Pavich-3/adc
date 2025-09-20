#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 8000000UL

void ADC_Init(void);
void PWM_Init(void);
void TIMER_Init(void);

int main(void)
{
  ADC_Init();
  PWM_Init();
  TIMER_Init();
  sei();

  while(1);
}

void ADC_Init(void)
{
  ADMUX |= (1 << REFS0) | (1 << MUX0);
  ADCSRA |= (1 << ADEN) | (1 << ADATE) | (1 << ADPS2) | (1 << ADPS1);
  ADCSRB |= (1 << ADTS1) | (1 << ADTS0);
  DIDR0 |= (1 << ADC1D);
  ADCSRA |= (1 << ADSC);
}

void PWM_Init(void)
{
  DDRB |= (1 << PB1);
  TCCR1A |= (1 << COM1A1) | (1 << WGM11) | (1 << WGM10);
  TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10);
}

void TIMER_Init(void)
{
  TCCR0A |= (1 << WGM01);
  TCCR0B |= (1 << CS02) | (1 << CS00);
  OCR0A = 156;
  TIMSK0 |= (1 << OCIE0A);
}

ISR(TIMER0_COMPA_vect)
{
  OCR1A = ADCW;
}