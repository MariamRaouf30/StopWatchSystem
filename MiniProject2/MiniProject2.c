/*
 * MiniProject2.c
 *
 *  Created on: Sep 16, 2022
 *      Author: dell
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

typedef unsigned char uint8;

uint8 flag = 0;

uint8 sec1 = 0;
uint8 sec2 = 0;
uint8 min1 = 0;
uint8 min2 = 0;
uint8 hr1 = 0;
uint8 hr2 = 0;

void Timer1_init(void){
	TCCR1A=(1<<FOC1A);//non pwm
	TCCR1B =(1<<WGM12)|(1<<CS10)|(1<<CS12);// CTC and prescalar 1024
	TCNT1 = 0;
	OCR1A = 1000;
	SREG|=(1<<7);
	TIMSK |= (1<<OCIE1A);
}


ISR(TIMER1_COMPA_vect){
	sec1++;
	if(sec1 == 10){
		sec1 = 0;
		sec2++;
	}
	if (sec2 == 6){
		sec2 = 0;
		min1++;
	}
	if(min1 == 10){
		min1 = 0;
		min2++;
	}
	if(min2 == 6){
		min2 = 0;
		hr1++;
	}
	if(hr1 == 10){
		hr1 = 0;
		hr2++;
	}

}

void INT0_init(void){
	DDRD &= ~(1<<PD2);
	MCUCR |=(1<<ISC01) &~(1<<ISC00);
	GICR |= (1<<INT0);
}


ISR(INT0_vect){
	sec1 = 0;
	sec2 = 0;
	min1 = 0;
	min2 = 0;
	hr1 = 0;
	hr2 = 0;

}

void INT1_init(void){
	DDRD &= ~(1<<PD3);
	MCUCR |=(1<<ISC11) | (1<<ISC10);
	GICR |= (1<<INT1);
}


ISR(INT1_vect){
	TCCR1B &=~(1<<CS10)&~(1<<CS11)&~(1<<CS12);

}

void INT2_init(void){
	DDRB &= ~(1<<PB2);
	PORTB |=(1<<PB2);
	MCUCSR &= ~(1<<ISC2);
	GICR |= (1<<INT2);
}

ISR(INT2_vect){
	TCCR1B |=	(1<<CS10)|(1<<CS12);
}





int main(void){
	SREG  |= (1<<7);
	DDRC |= 0x0F;
	PORTC &= ~(0X0F);
	DDRA |= 0x3F;
	PORTA |= 0x3F;
	INT0_init();
 	INT1_init();
	INT2_init();
	Timer1_init();
	while(1){
		PORTA =(PORTA & 0xC0) | (1<<PA5);
		PORTC = (PORTC & 0xF0) | (sec1 & 0x0F);
		_delay_ms(2);
		PORTA =(PORTA & 0xC0) | (1<<PA4);
		PORTC = (PORTC & 0xF0) | (sec2 & 0x0F);
		_delay_ms(2);
		PORTA =(PORTA & 0xC0) | (1<<PA3);
		PORTC = (PORTC & 0xF0) | (min1 & 0x0F);
		_delay_ms(2);
		PORTA =(PORTA & 0xC0) | (1<<PA2);
		PORTC = (PORTC & 0xF0) | (min2 & 0x0F);
		_delay_ms(2);
		PORTA =(PORTA & 0xC0) | (1<<PA1);
		PORTC = (PORTC & 0xF0) & ~(hr1 & 0x0F);
		_delay_ms(2);
		PORTA =(PORTA & 0xC0) | (1<<PA0);
		PORTC = (PORTC & 0xF0) & ~(hr2 & 0x0F);
		_delay_ms(2);
	}
}
