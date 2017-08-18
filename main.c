#include <stdlib.h>
#include <avr/io.h>
#include "lcd.h"   //" = Lib in akt Projektverzeichnis
#include <avr/interrupt.h>


#ifndef F_CPU
#define F_CPU 8000000L
#endif
#include <util/delay.h>


uint16_t ms100;
uint16_t ISR_zaehler;
uint16_t sec;
uint8_t state;
uint8_t min;

ISR (TIMER0_OVF_vect)		//Overflow Interrupt Service Routine von Timer0
{		
		TCNT0 = 0;		// Startwert des Zaehlers nach Interrupt.
		ISR_zaehler++;
		if(ISR_zaehler==12)	//entspricht ca 100ms Sekunden (8M / 256 / 256 = 122/10)
		{
			ISR_zaehler=0;
			ms100++;
			
		}
		
		if(ms100==10)
		{
			ms100 = 0;
			sec++;
		}
		
		if(sec==60)
		{
			sec = 0;
			min++;
		}
} // end of ISR


int main(void)
{

		DDRB = 0x0F;
		PORTB = 0x07;
		
		DDRC = 0xFF;
		PORTC = 0x00;
		
		DDRD=0xFF;
		PORTD = 0xFF;
		
		int x;
		
		
		//Konfiguration Timer Overflow
		TCCR0A	= 0x00;			//normal mode
		TCCR0B	= 0x04;			//clk/256
		TIMSK0	|= (1<<TOV0);	//Time0 frei
		TIFR0	|= (1<<TOV0);	//Interrupt Timeroverflow einschalten
		sei();
		
		lcd_clrscr();
		state = 1;
		
		

		lcd_init(LCD_DISP_ON_CURSOR_BLINK);  // initialisieren		
		lcd_clrscr();
		lcd_gotoxy(0,2);
		
		while(1)
		{
			for(x=0;x<10000;x++);
			{
				lcd_gotoxy(7,2);
				lcd_count_16(min);
				
				lcd_gotoxy(15,2);
				lcd_count_16(sec);
			}
		}//end of while
return 0;
}