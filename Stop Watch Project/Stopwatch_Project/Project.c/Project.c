/*
 * Project.c
 *
 *  Created on: Sep 12, 2024
 *      Author: MOH
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
void Timer1_CTC_Init(void)
{
	TCCR1B= (1<<CS10)|(1<<CS12)|(1<<WGM12);
	TCCR1A = (1<<FOC1A)|(1<<FOC1B);
	OCR1A = 15624;
	TIMSK |=  (1<<OCIE1A);
	TCNT1 = 0;
	SREG |= (1<<7);
}
char sec1=0,sec2=0,min1=0,min2=0,hr1=0,hr2=0;
char mode =0;
/*ISR(TIMER1_COMPA_vect)
{

	if(sec1 == 9) {
		sec1 = 0;
		if(sec2 == 5) {
			sec2 = 0;
			if(min1 == 9) {
				min1 = 0;
				if(min2 == 5) {
					min2 = 0;
					if(hr1 == 9 && hr2 == 9) {
						hr1 = 0;
						hr2 = 0;
					} else if(hr1 == 9) {
						hr1 = 0;
						hr2++;
					} else {
						hr1++;
					}
				} else {
					min2++;
				}
			} else {
				min1++;
			}
		} else {
			sec2++;
		}
	} else {
		sec1++;
	}
}*/
ISR(TIMER1_COMPA_vect)
{
    if (mode == 0) {
        if (sec1 == 9) {
            sec1 = 0;
            if (sec2 == 5) {
                sec2 = 0;
                if (min1 == 9) {
                    min1 = 0;
                    if (min2 == 5) {
                        min2 = 0;
                        if (hr1 == 9 && hr2 == 9) {
                            hr1 = 0;
                            hr2 = 0;
                        } else if (hr1 == 9) {
                            hr1 = 0;
                            hr2++;
                        } else {
                            hr1++;
                        }
                    } else {
                        min2++;
                    }
                } else {
                    min1++;
                }
            } else {
                sec2++;
            }
        } else {
            sec1++;
        }
    }
    else if (mode == 1) {
        if (sec1 == 0) {
            sec1 = 9;
            if (sec2 == 0) {
                sec2 = 5;
                if (min1 == 0) {
                    min1 = 9;
                    if (min2 == 0) {
                        min2 = 5;
                        if (hr1 == 0 && hr2 == 0) {
                            hr1 = 0;
                            hr2 = 0;
                            min1 = 0;
                            min2 = 0;
                            sec1 = 0;
                            sec2 = 0;
                        } else if (hr1 == 0) {
                            hr1 = 9;
                            hr2--;
                        } else {
                            hr1--;
                        }
                    } else {
                        min2--;
                    }
                } else {
                    min1--;
                }
            } else {
                sec2--;
            }
        } else {
            sec1--;
        }
    }
}


void display(void){
	PORTA = (1<<0);
	PORTC = (PORTC & 0xF0) | sec1 ;

	_delay_ms(1);
	PORTA = (1<<1);
	PORTC = (PORTC & 0xF0) | sec2 ;

	_delay_ms(1);
	PORTA = (1<<2);
	PORTC = (PORTC & 0xF0) | min1 ;

	_delay_ms(1);
	PORTA = (1<<3);
	PORTC = (PORTC & 0xF0) | (min2 & 0x0F);

	_delay_ms(1);
	PORTA = (1<<4);
	PORTC = (PORTC & 0xF0) | (hr1 & 0x0F);

	_delay_ms(1);
	PORTA = (1<<5);
	PORTC = (PORTC & 0xF0) | (hr2 & 0x0F);

	_delay_ms(1);
}
void INT1_Init(void)
{
	DDRD |= (1<<3);
	MCUCR |= (1 << ISC11) | (1 << ISC10);
	GICR |= (1<<INT1);
	SREG |= (1<<7);
}
ISR(INT1_vect)
{
	TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}
ISR(INT0_vect)
{
	sec1=0,sec2=0,min1=0,min2=0,hr1=0,hr2=0;
}

void INT2_Init(void)
{
	DDRD  &= (~(1<<PB2));
	GICR  |= (1<<INT2);
	SREG  |= (1<<7);
}
ISR(INT2_vect){
	TCCR1B= (1<<CS10)|(1<<CS12)|(1<<WGM12);
}

void INT0_Init(void)
{
	DDRD  &= (~(1<<PD2));
    MCUCR |= (1 << ISC01);
    MCUCR &= ~(1 << ISC00);
	GICR  |= (1<<INT0);
	SREG  |= (1<<7);
}
void count_mode(void){
    if(!(PINB & (1<<PB7))) {
        while(!(PINB & (1<<PB7))) {
       	 display();
        }
        if(mode==0){
        	mode=1;
        }

        else{
        	mode=0;
        }


    }
}
void change(void){
    display();
    if(!(PINB & (1<<PB1))) {
        while(!(PINB & (1<<PB1))) {
       	 display();
        }
        if(hr1 != 9)
            hr1++;
        else if(hr1 == 9 && hr2 != 9){
       	 hr2++;
       	 hr1=0;
        }
    }

    if(!(PINB & (1<<PB4))) {
        while(!(PINB & (1<<PB4))) {
       	 display();
        }
        if(min1 != 9)
            min1++;
        else if(min1 == 9 && min2 != 5){
       	 min2++;
       	 min1=0;
        }
    }

    if(!(PINB & (1<<PB6))) {
        while(!(PINB & (1<<PB6))) {
       	 display();
        }
        if(sec1 != 9)
            sec1++;
        else if(sec1 == 9 && sec2 != 5){
       	 sec2++;
       	 sec1=0;
        }
    }

    if(!(PINB & (1<<PB5))) {
        while(!(PINB & (1<<PB5))) {
       	 display();
        }
        if(sec1 != 0)
            sec1--;
        else if(sec1 == 0 && sec2 != 0){
       	 sec2--;
       	 sec1=9;
        }
    }

    if(!(PINB & (1<<PB3))) {
        while(!(PINB & (1<<PB3))) {
       	 display();
        }
        if(min1 != 0)
            min1--;
        else if(min1 == 0 && min2 != 0){
       	 min2--;
       	 min1=9;
        }
    }
    if(!(PINB & (1<<PB0))) {
        while(!(PINB & (1<<PB0))) {
       	 display();
        }
        if(hr1 != 0)
            hr1--;
        else if(hr1 == 0 && min2 != 0){
       	 hr2--;
       	 hr1=9;
        }
    }

}
void led(void){
    if(mode==0){
    	PORTD |=(1<<PD4);
    	PORTD &=~(1<<PD5);
    }

    else{
    	PORTD |=(1<<PD5);
    	PORTD &=~(1<<PD4);
    }
}
void buzzer(void){
	if(mode==1&&sec1==0 && sec2==0 && min1==0 && min2==0 && hr1==0 && hr2==0){
		PORTD|= (1<<PD0);
	}
	else
		PORTD &= ~(1<<PD0);


}
int main(void)
{
	 DDRC |= 0x0F;
	 DDRA |= 0x3F;
	 PORTA &= ~0x3F;
	 PORTC &= 0xF0;
	 DDRB &= ~0xFF;
	 PORTB |= 0xFF;
	 DDRD |= (1<<PD4)|(1<<PD5);
	 DDRD |= (1<<PD0);
	 PORTD =(1<<PD4);
	 Timer1_CTC_Init();
	 INT1_Init();
	 INT0_Init();
	 INT2_Init();
	 while(1) {
		 count_mode();
		 change();
		 led();
		 buzzer();


	 }










	 }

