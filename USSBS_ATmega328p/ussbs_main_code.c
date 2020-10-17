/*
 * USSBS_v1.0.c
 *
 * Created: 28.01.2020 15:43:11
 * Author : Michal Lorek
 */ 

#define F_CPU 16000000UL //taktowanie 16MHz
#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcdpcf8574.h"
#include "i2cmaster.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ADCIN PC3  //wejście przetwornika ADC
#define VREF 5.05  //napięcie referencyjne
#define ISKRA_1 (1<<PB0)
#define ZAWOR_1 (1<<PB1)
#define ZAWOR_2 (1<<PB2)

volatile float adc; //wartość z przetwornika ADC

char wynik[4];

int opoz_strz;
float delay;
float sekwencja;
int strzal;

bool impuls=false;
bool impuls_pop=false;
bool impuls_nast=false;
int czas=0;

void impulsPWM()
{
	if(impuls_pop!=impuls_nast)
	{
		czas++;
	}
	impuls_pop=impuls_nast;
	impuls_nast=impuls;
}

void opoznienie(int ile_10ms)
{
	do
	{
		impulsPWM();
	}while (czas<ile_10ms);
	
	if (czas>=ile_10ms)
	{
		czas=0;
	}
}

void wystrzal()
{
	PORTB =ZAWOR_1|ZAWOR_2;
	opoznienie(18);
	PORTB = ZAWOR_1;
	opoznienie(2);
	PORTB = ISKRA_1;
	opoznienie(3);
	PORTB = 0x00;
	opoznienie(3);
}


int main(void)
{
	DDRD  = 0x00;  //wejscia
	PORTD = 0xff;  //podc do vcc
	DDRB |=(1<<0)|(1<<1)|(1<<2); //wyjscie
	
	int licznik=0;
	
	//OCR1A = 0x3D08; //15624 -> 1s
	//OCR1A = 0x1E85; //7813  ->0.5s
	//OCR1A = 0x1250; //4688  ->0.3s
	OCR1A = 0x009D;   //157   ->0.01s
	TCCR1B |= (1 << WGM12); // Tryb CTC - wyzerowanie po doliczeniu do OCR1A
	TIMSK1 |= (1 << OCIE1A); //Przerwanie po doliczeniu do OCR1A
	TCCR1B |= (1 << CS12) | (1 << CS10); // ustawienie prescalera na 1024 i uruchomienie timera
	
	ADCSRA = (1<<ADEN) //włączenie ADC
	|(1<<ADIE) //uruchomienie zgłaszania przerwań
	|(1<<ADPS0)  
	|(1<<ADPS1)
	|(1<<ADPS2)  //ustawienie preskalera na 128
	|(1<<ADATE); 
	DIDR0 = (1<<ADC0D);
	ADCSRA |=(1<<ADSC);
	ADMUX  =    (1<<REFS0) //VCC jako napięcie referencyjne
	|(1<<MUX1) | (1<<MUX0); //wybór kanału pomiarowego ADC3
	DDRC &=~ (1<<ADCIN); //ustawienie wejścia ADC
	
	sei(); //Globalne uruchomienie przerwań
	
	lcd_init(LCD_DISP_ON_CURSOR);
	lcd_led(0);
	

	
	while (1)
	{
	if(!(PIND & 0x00))
		{ }
	while(!(PIND & 0x01)) //dopóki układ włączony
		{
			//adc - zmienna, równa wartości odczytanej z PC3
		delay=adc*0.1699;
		opoz_strz=adc*0.1699; //przeskalowanie wartości odczytanej z ADC na opóźnienie
		sekwencja=(0.26+(delay*0.01)); //czas trwania jednej sekwencji z opóźnieniem
		strzal=60/sekwencja; //szybkostrzelność w strz/min
	
		if(!(PIND & 0x03)) //jesli selektor ogrnia ustawionon na pojedynczy
		{
		lcd_gotoxy(0,0);
		lcd_puts("single          ");
		lcd_gotoxy(0,1);
		lcd_puts("                ");
		opoznienie(20);
	
		if(!(PIND & 0x0b)) //jeśli spust jest wcisniety
			{
				
			if (licznik>0)  { licznik=0; PORTB = 0x00; } // zmienna licznik odpowiada na minimalizowanie drgania styków
				if (licznik==0)
	
				{
				wystrzal();
				}
	
				while(!(PIND & 0x0b)){} //0xb
				licznik++;
				}
			}
	
		if (!(PIND & 0x05)) //jesli selektor ognia ustawiono na ogien ciagly
			{
			lcd_gotoxy(0,0);        //Ustawienie kursora w pozycji (1,1)
			lcd_puts("auto        ");
	
			lcd_gotoxy(9,1);
			lcd_puts("st/min");
	
			lcd_gotoxy(0,1);
			itoa(strzal,wynik,10); //
			lcd_puts(wynik);   //Wyświetlenie wyniku
			opoznienie(50);
			lcd_puts("     ");
	
				while(!(PIND & 0x0c))  //jeśli spust wcisniety
					{
						wystrzal();
							for (int i=1; i<opoz_strz; i++)
								{
								opoznienie(1); //opoznienie wynikajace z ustawionej szybkostrzelnosci
								}
					}
				}
			}
		}
	
	}

	ISR(ADC_vect)//obsługa przerwania po zakończeniu konwersji ADC
	{
		adc=ADC;
		ADCSRA |= (1 << ADSC);
	}

	ISR (TIMER1_COMPA_vect)
	{
	//wykonywane co 10ms
		impuls= !impuls;
	}


