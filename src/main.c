#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL

void ADC_Init(void);

int main(void)
{
  while(0);
}

void ADC_Init(void)
{
  ADMUX |= (1 << REFS0) | (1 << MUX0);
  ADCSRA |= (1 << ADEN) | (1 << ADATE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  ADCSRB |= (1 << ADTS1) | (1 << ADTS0);
}

void PWM_Init(void)
{

}

void TIMER_Init(void)
{
  
}