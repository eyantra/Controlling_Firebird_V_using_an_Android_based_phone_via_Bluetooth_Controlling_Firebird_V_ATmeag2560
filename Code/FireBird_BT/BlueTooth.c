/**@mainpage package Bluetooth Communication
 @author Group 7: 	Jagbandhu		09307603 
 					Kanzaria Jatin	09307919
					K. L. Srinivas	09307051
 					Shah Rohan 		09307050

 AVR Studio Version 4.17, Build 666

 Date: 8th November 2010

 Bluetooth Connections:
 		Bluetooth	  POD Extension Connector Pin
 			  Rx  --> 1  (Txd)
			  Tx  --> 2  (Rxd)
			  VCC --> 21 (+5v)
			  GND --> 23 (GND)

 Note: 
 
 1. Make sure that in the Bluetooth Module is configured to work at baud rate
     of 9600bps, No Parity and No Flow Control

 2. Make sure that in the configuration options following settings are 
 	done for proper operation of the code

  	Microcontroller: atmega2560
 	Frequency: 11059200
 	Optimization: -O0 (For more information read section: Selecting proper opti-
                      mization options below figure 4.22 in the hardware manual)

 3. Jumper for USART2 is connected in POD direction and not in USB.

*********************************************************************************/

/********************************************************************************

   Copyright (c) 2010, ERTS Lab IIT Bombay erts@cse.iitb.ac.in        -*- c -*-
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
#include <avr/interrupt.h>

#include "BlueTooth.h"
#include "GlobalVar.h"

/*Function to Initialize USART2 for Communication with Bluetooth
	Baud Rate = 9600
	8 bit
	No Parity
	Transmit and Recieve interrupt Enable*/
void Init_USART2()
{
	UCSR2B = 0x00;
	UCSR2A = 0x00;
	UCSR2C  = 0x06;
	UBRR2L = 0x47;
	UBRR2H = 0x00;
	UCSR2B = 0xd8;
}

/*Recieve Interrupt Handler
	Returns Character String Arrived in main_buf array
	Indicates Main Routine after Whole String Has Arrived*/
SIGNAL(SIG_USART2_RECV)
{
 	unsigned char data = UDR2;
	if (data != 0x0a)
	{
		ser_buf[count] = data;
		count++;
	}
	else
	{
		int i;
		if ((ser_buf[0] == '\'') || (ser_buf[0] == 'C') || (ser_buf[0] == 'D'))
			count = 0;
		else
		{
			decode = 1;
			for (i=0;i<count;i++)
				main_buf[i] = ser_buf[i];
			maincnt = count;
			count = 0;
		}
	}
}

/*Transmit Interrupt Subroutine
	Transmit String in ser_trans_buf array
	No. of characters to be transmitted in trx_count*/
SIGNAL(SIG_USART2_TRANS)
{
	if(trx_count >= trx_curr_cnt)
		UDR2 = ser_trax_buf[trx_curr_cnt];
		trx_curr_cnt++;
}

/*Function to Inquire MAC Address of Bluetooth Module Connected on FB5*/
void BlueTooth_Add()
{
	ser_trax_buf[0] = 'A';
	ser_trax_buf[1] = 'T';
	ser_trax_buf[2] = 'B';
	ser_trax_buf[3] = '?';
	ser_trax_buf[4] = 0x0D;
	trx_count = 4;
	trx_curr_cnt = 1;
	UDR2 = ser_trax_buf[0];
	res_wait = 1;
}
