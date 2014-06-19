/**@mainpage package FireBird-5 Control Through Bluetooth
 @author Group 7: 	Jagbandhu		09307603 
 					Kanzaria Jatin	09307919
					K. L. Srinivas	09307051
 					Shah Rohan 		09307050

 AVR Studio Version 4.17, Build 666

 Date: 8th November 2010
 
 This experiment demonstrates FireBird 5 control through Bluetooth Interface


 Bluetooth Connections:
 		Bluetooth	  POD Extension Connector Pin
 			  Rx  --> 1  (Txd)
			  Tx  --> 2  (Rxd)
			  VCC --> 21 (+5v)
			  GND --> 23 (GND)

 Note: 
 
 1. Make sure that in the Bluetooth Module is configured to work at baud rate of 9600
 	bps, No Parity and No Flow Control.

 2.	Make sure that in the configuration options following settings are 
 	done for proper operation of the code

  	Microcontroller: atmega2560
 	Frequency: 11059200
 	Optimization: -O0 (For more information read section: Selecting proper optimization options 
						below figure 4.22 in the hardware manual)

 3. Jumper for USART2 is connected in POD direction and not in USB.

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

#define F_CPU 11059200ul

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "BlueTooth.h"
#include "MotorControl.h"
#include "GlobalVar.h"
#include "LCD.h"

/*Function to Initialize the Peripheral Devices*/
void Init_Devices(void)
{
	Init_USART2();
	Init_Motor_Peri();
	LCD_Init();
}

/*Main Function*/
void main(void)
{
	cli();
	Init_Devices();
	sei();
	LCD_WR_Command(0x01);
	LCD_Cursor(1,1);
	LCD_String("ANDROID_FB5");
	while (1)
	{
		_delay_ms(100);
		if (decode == 1)
		{
			unsigned char speed;
			decode = 0;
			/*Speed on the scale of 1 to 8.
				1 => Minimum
				8 => Maximum*/
			switch (main_buf[1])
			{
				case '1':
					speed = 0x1F;
					break;
				case '2':
					speed = 0x3F;
					break;
				case '3':
					speed = 0x5F;
					break;
				case '4':
					speed = 0x7F;
					break;
				case '5':
					speed = 0x9F;
					break;
				case '6':
					speed = 0xBF;
					break;
				case '7':
					speed = 0xDF;
					break;
				case '8':
					speed = 0xFF;
					break;
				default :
					speed = 0x00;
					break;
			}
			/* Direction:
				F => Forward
				B => Reverse
				L => Left Turn
				R => Right Turn
				S => Stop*/
			switch (main_buf[0])
			{
				case 'F' :
					Forward(speed);
					LCD_WR_Command(0x01);
					LCD_Cursor(1,1);
					LCD_String("FORWARD:");
					LCD_Print(1,10,main_buf[1]-0x30,1);
					break;
				case 'B' :
					Reverse(speed);
					LCD_WR_Command(0x01);
					LCD_Cursor(1,1);
					LCD_String("REVERSE:");
					LCD_Print(1,10,main_buf[1]-0x30,1);
					break;
				case 'L' :
					Left_Turn(speed);
					LCD_WR_Command(0x01);
					LCD_Cursor(1,1);
					LCD_String("LEFT TURN:");
					LCD_Print(1,12,main_buf[1]-0x30,1);
					break;
				case 'R' :
					Right_Turn(speed);
					LCD_WR_Command(0x01);
					LCD_Cursor(1,1);
					LCD_String("RIGHT TURN:");
					LCD_Print(1,13,main_buf[1]-0x30,1);
					break;
				case 'S' :
					Stop();
					LCD_WR_Command(0x01);
					LCD_Cursor(1,1);
					LCD_String("STOP");
					break;
				default :
					Stop();
					LCD_WR_Command(0x01);
					LCD_Cursor(1,1);
					LCD_String("ANDROID_FB5");
					break;
			}
		}
	}
}


