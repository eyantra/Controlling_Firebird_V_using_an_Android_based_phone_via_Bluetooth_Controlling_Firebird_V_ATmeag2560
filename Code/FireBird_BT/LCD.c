/**@mainpage package lcd_interface
 @author Group 7: 	Jagbandhu		09307603 
 					Kanzaria Jatin	09307919
					K. L. Srinivas	09307051
 					Shah Rohan 		09307050
 
 AVR Studio Version 4.17, Build 666

 Date: 13th January 2010
 
 LCD Connections:
 			 LCD	  Microcontroller Pins
 			  RS  --> PC0
			  RW  --> PC1
			  EN  --> PC2
			  DB7 --> PC7
			  DB6 --> PC6
			  DB5 --> PC5
			  DB4 --> PC4

 Note: 
 
 1. Make sure that in the configuration options following settings are 
 	done for proper operation of the code

 	Microcontroller: atmega2560
 	Frequency: 11059200
 	Optimization: -O0 (For more information read section: Selecting proper optimization options 
						below figure 4.22 in the hardware manual)

*********************************************************************************/

/********************************************************************************

   Copyright (c) 2010, ERTS Lab IIT Bombay erts@cse.iitb.ac.in               -*- c -*-
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.

   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

   * Source code can be used for academic purpose. 
	 For commercial use permission form the author needs to be taken.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. 

  Software released under Creative Commence cc by-nc-sa licence.
  For legal information refer to: 
  http://creativecommons.org/licenses/by-nc-sa/3.0/legalcode

********************************************************************************/

#ifndef F_CPU
#define F_CPU 11059200UL
#endif
#include <avr/io.h>
#include <util/delay.h>

#include "LCD.h"
#include "GlobalVar.h"

/*****Function to Reset LCD*****/
void LCD_Reset_4bit()
{
	_delay_ms(1);

	cbit(LCD_PORT,RS);				//RS=0 --- Command Input
	cbit(LCD_PORT,RW);				//RW=0 --- Writing to LCD
	LCD_PORT = 0x30;				//Sending 3
	sbit(LCD_PORT,EN);				//Set Enable Pin
	_delay_ms(5);					//Delay
	cbit(LCD_PORT,EN);				//Clear Enable Pin

	_delay_ms(1);

	cbit(LCD_PORT,RS);				//RS=0 --- Command Input
	cbit(LCD_PORT,RW);				//RW=0 --- Writing to LCD
	LCD_PORT = 0x30;				//Sending 3
	sbit(LCD_PORT,EN);				//Set Enable Pin
	_delay_ms(5);					//Delay
	cbit(LCD_PORT,EN);				//Clear Enable Pin

	_delay_ms(1);

	cbit(LCD_PORT,RS);				//RS=0 --- Command Input
	cbit(LCD_PORT,RW);				//RW=0 --- Writing to LCD
	LCD_PORT = 0x30;				//Sending 3
	sbit(LCD_PORT,EN);				//Set Enable Pin
	_delay_ms(5);					//Delay
	cbit(LCD_PORT,EN);				//Clear Enable Pin

	_delay_ms(1);

	cbit(LCD_PORT,RS);				//RS=0 --- Command Input
	cbit(LCD_PORT,RW);				//RW=0 --- Writing to LCD
	LCD_PORT = 0x20;				//Sending 2 to initialise LCD 4-bit mode
	sbit(LCD_PORT,EN);				//Set Enable Pin
	_delay_ms(5);					//Delay
	cbit(LCD_PORT,EN);				//Clear Enable Pin

	
}

/*Function to Initialize Port For LCD connection*/
void LCD_Init_Ports (void)
{
	DDRC = DDRC | 0xF7; 	//all the LCD pin's direction set as output
	PORTC = PORTC & 0x80; 	// all the LCD pins are set to logic 0 except PORTC 7
}


/*****Function to Initialize LCD*****/
void LCD_Init()
{
	LCD_Init_Ports();
	LCD_Reset_4bit();
	_delay_ms(1);
	LCD_WR_Command(0x28);			//LCD 4-bit mode and 2 lines.
	LCD_WR_Command(0x01);
	LCD_WR_Command(0x06);
	LCD_WR_Command(0x0E);
	LCD_WR_Command(0x80);
}

	 
/*****Function to Write Command on LCD*****/
void LCD_WR_Command(unsigned char cmd)
{
	unsigned char temp;
	temp = cmd;
	temp = temp & 0xF0;
	LCD_PORT &= 0x0F;
	LCD_PORT |= temp;
	cbit(LCD_PORT,RS);
	cbit(LCD_PORT,RW);
	sbit(LCD_PORT,EN);
	_delay_ms(5);
	cbit(LCD_PORT,EN);
	
	cmd = cmd & 0x0F;
	cmd = cmd<<4;
	LCD_PORT &= 0x0F;
	LCD_PORT |= cmd;
	cbit(LCD_PORT,RS);
	cbit(LCD_PORT,RW);
	sbit(LCD_PORT,EN);
	_delay_ms(5);
	cbit(LCD_PORT,EN);
}

/*****Function to Write Data on LCD*****/
void LCD_WR_Char(char letter)
{
	char temp;
	temp = letter;
	temp = (temp & 0xF0);
	LCD_PORT &= 0x0F;
	LCD_PORT |= temp;
	sbit(LCD_PORT,RS);
	cbit(LCD_PORT,RW);
	sbit(LCD_PORT,EN);
	_delay_ms(5);
	cbit(LCD_PORT,EN);

	letter = letter & 0x0F;
	letter = letter<<4;
	LCD_PORT &= 0x0F;
	LCD_PORT |= letter;
	sbit(LCD_PORT,RS);
	cbit(LCD_PORT,RW);
	sbit(LCD_PORT,EN);
	_delay_ms(5);
	cbit(LCD_PORT,EN);
}


void LCD_Home()
{
	LCD_WR_Command(0x80);
}


/*****Function to Print String on LCD*****/
void LCD_String(char *str)
{
	while(*str != '\0')
	{
		LCD_WR_Char(*str);
		str++;
	}
}

/*** Position the LCD cursor at "row", "column". ***/

void LCD_Cursor (char row, char column)
{
	switch (row) {
		case 1: LCD_WR_Command (0x80 + column - 1); break;
		case 2: LCD_WR_Command (0xc0 + column - 1); break;
		case 3: LCD_WR_Command (0x94 + column - 1); break;
		case 4: LCD_WR_Command (0xd4 + column - 1); break;
		default: break;
	}
}

/***** Function To Print Any input value upto the desired digit on LCD *****/
void LCD_Print (char row, char coloumn, unsigned int value, int digits)
{
	flag = 0;
	if(row==0||coloumn==0)
	{
		LCD_Home();
	}
	else
	{
		LCD_Cursor(row,coloumn);
	}
	if(digits==5 || flag==1)
	{
		million=value/10000+48;
		LCD_WR_Char(million);
		flag=1;
	}
	if(digits==4 || flag==1)
	{
		temp = value/1000;
		thousand = temp%10 + 48;
		LCD_WR_Char(thousand);
		flag=1;
	}
	if(digits==3 || flag==1)
	{
		temp = value/100;
		hundred = temp%10 + 48;
		LCD_WR_Char(hundred);
		flag=1;
	}
	if(digits==2 || flag==1)
	{
		temp = value/10;
		tens = temp%10 + 48;
		LCD_WR_Char(tens);
		flag=1;
	}
	if(digits==1 || flag==1)
	{
		unit = value%10 + 48;
		LCD_WR_Char(unit);
	}
	if(digits>5)
	{
		LCD_WR_Char('E');
	}
	
}
		
