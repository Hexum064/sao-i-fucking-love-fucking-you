/*
 * PoC.c
 *
 * Created: 2022-04-25 19:06:16
 * Author : Branden
 */ 

#define F_CPU 1000000UL //1MHz
#include <avr/io.h>
#include <util/delay.h>

//#define _DEBUG

#define INTERVAL 1000
#define FLASH_OFF 10
#define FLASH_ON 50
#define PRIME 919

void init_adc()
{
	// Initialize ADC
	ADMUX = 0; //(1 << MUX1); //PB0
	ADCSRA = (1<<ADEN)|(1<<ADPS1)|(1<<ADPS1)|(1<<ADPS0);	
}

void flash_pin_on(uint8_t pin)
{
	uint8_t mask = 1 << pin;
	
	if ((PORTB & (1 << pin))) //If port b1 is on, turn it off
	{
		PORTB ^= mask;
	}
	else
	{
		
		PORTB |= mask;
		_delay_ms(FLASH_OFF);
		PORTB ^= mask;
		_delay_ms(FLASH_ON);
		PORTB |= mask;
		_delay_ms(FLASH_OFF);
		PORTB ^= mask;
		_delay_ms(FLASH_ON);
		PORTB |= mask;
	}		
}

int main(void)
{
	DDRB = (1 << PORTB1) | (1 << PORTB2); //Pin 1 and 2 as outputs
	
	init_adc();
	
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	
	uint32_t rndVal = ADCL; //PRIME;
	
#ifdef _DEBUG
	
	for (uint8_t i = 0; i < 8; i++)
	{
		PORTB |= 1 << PORTB1;
		
		if ((rndVal & (1 << i)))
		{
			PORTB |= 1 << PORTB2;
			
		}
		
		_delay_ms(1000);
		
		PORTB = 0;
		_delay_ms(1000);
	}
	
#endif // _DEBUG
	
	
	uint8_t target0 = 0;
	uint8_t target0_count = 0;
	uint8_t target1 = 0;
	uint8_t target1_count = 0;

    while (1) 
    {	
		//Sudo random time generator using a prime number. It's just a very long pattern.
		target0 = (rndVal % 5) + 1; //random-ish number between 1 and 6
		rndVal += PRIME;
		target1 = (rndVal % 4) + 1; //random-ish number between 1 and 5
		rndVal += PRIME;
		
		target0_count = 0;
		target1_count = 0;
		
		while(target0_count <= target0 || target1_count <= target1)
		{
			if (target0_count == target0)
			{
				flash_pin_on(PORTB1);				
			}
			
			if (target1_count == target1)
			{
				flash_pin_on(PORTB2);							
			}
			
			target0_count++;
			target1_count++;
			//Random timing will be based on 1000ms (1 second) intervals
			_delay_ms(INTERVAL);
			
		}
		
    }
}

