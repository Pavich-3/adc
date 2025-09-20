#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 8000000UL

void ADC_Init(void);
void TIMER_Init(void);

int main(void)
{
  while(0);
}

void ADC_Init(void)
{
  ADMUX |= (1 << REFS0) | (1 << MUX0);
  ADCSRA |= (1 << ADEN) | (1 << ADATE) | (1 << ADPS2) | (1 << ADPS1);
  ADCSRB |= (1 << ADTS1) | (1 << ADTS0);
  DIDR0 |= (1 << ADC1D);
}

void PWM_Init(void)
{

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

}