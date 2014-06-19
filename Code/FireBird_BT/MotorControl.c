/**@mainpage package Motor Control
 @author Group 7: 	Jagbandhu 09307603 
 					Shah Rohan 09307050
 
 AVR Studio Version 4.17, Build 666

 Date: 13th January 2010
 
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

#include "MotorControl.h"

/*Functio to initialize Peripherals required for Motor Speed Control*/
void Init_Motor_Peri()
{
	Init_PWM_Ports();
	Init_Motor_Timer();
	Init_Motor_Port();
}

/*Function to initialize Ports For PWM*/
void Init_PWM_Ports()
{
	DDRL=0x18;
	PORTL = 0x18;
}

/*Function to initialize Ports For Motor*/
void Init_Motor_Port()
{
	DDRA=0x0F;
	PORTA = 0x00;
}

/*Function to initialize Timer for PWM*/
void Init_Motor_Timer()
{
	TCCR5B = 0x00;
	TCCR5A = 0xA9;
	TCCR5B = 0x0B;
}

/*Function to Set the Velocity for Motor Speed
	Left Motor Speed in lm
	Right Motor Speed in rm*/
void Set_Velocity(unsigned char lm,unsigned char rm)
{
	OCR5AL = (unsigned char)lm;
	OCR5BL = (unsigned char)rm;
}

/*Function to Set the Motion of Motor in Forward Direction
	Speed of Motion in lm*/
void Forward(unsigned char lm)
{
	Set_Velocity(lm, lm);
	PORTA = 0x06;
}

/*Function to Set the Motion of Motor in Reverse Direction
	Speed of Motion in lm*/
void Reverse(unsigned char lm)
{
	Set_Velocity(lm, lm);
	PORTA = 0x09;
}

/*Function to Rotate FB5 in Left Direction
	Speed of Motion in lm*/
void Left_Turn(unsigned char lm)
{
	Set_Velocity(lm, lm);
	PORTA = 0x05;
}

/*Function to Rotate FB5 in Right Direction
	Speed of Motion in lm*/
void Right_Turn(unsigned char lm)
{
	Set_Velocity(lm, lm);
	PORTA = 0x0A;
}

/*Function to Stop Motor*/
void Stop()
{
	PORTA = 0x00;
}
