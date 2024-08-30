/*--------------------------------------------------------
GEORGE MASON UNIVERSITY
  Routines for the built in LCD.

Date:   Spring 2016
Author: Jens-Peter Kaps
--------------------------------------------------------*/
#include "lcd.h"

// LCD memory map for numeric digits
const char digit[10][2] =
{
    {0xFC, 0x28},  /* "0" LCD segments a+b+c+d+e+f+k+q */
    {0x60, 0x20},  /* "1" */
    {0xDB, 0x00},  /* "2" */
    {0xF3, 0x00},  /* "3" */
    {0x67, 0x00},  /* "4" */
    {0xB7, 0x00},  /* "5" */
    {0xBF, 0x00},  /* "6" */
    {0xE4, 0x00},  /* "7" */
    {0xFF, 0x00},  /* "8" */
    {0xF7, 0x00}   /* "9" */
};

const char alphabet[26][2] =
{
    {0xEF, 0x00},  /* "A" */
    {0xF1, 0x50},  /* "B" */
    {0x9C, 0x00},  /* "C" */
    {0xF0, 0x50},  /* "D" */
    {0x9F, 0x00},  /* "E" */
    {0x8F, 0x00},  /* "F" */
    {0xBD, 0x00},  /* "G" */
    {0x6F, 0x00},  /* "H" */
    {0x90, 0x50},  /* "I" */
    {0x78, 0x00},  /* "J" */
    {0x0E, 0x28},  /* "K" */
    {0x1C, 0x00},  /* "L" */
    {0x6C, 0xA0},  /* "M" */
    {0x6C, 0x82},  /* "N" */
    {0xFC, 0x00},  /* "O" */
    {0xCF, 0x00},  /* "P" */
    {0xFC, 0x02},  /* "Q" */
    {0xCF, 0x02},  /* "R" */
    {0xB7, 0x00},  /* "S" */
    {0x80, 0x50},  /* "T" */
    {0x7C, 0x00},  /* "U" */
    {0x2C, 0x28},  /* "V" */
    {0x6C, 0x0A},  /* "W" */
    {0x00, 0xAA},  /* "X" */
    {0x00, 0xB0},  /* "Y" */
    {0x90, 0x28},  /* "Z" */
};

// Memory positions of the letters in LCD memory, adjust as per your LCD configuration
const unsigned int letterpos[6] =  {pos5, pos6, pos4, pos3, pos2, pos1};

void showLetter(char letter, unsigned int position)
{
    if (letter >= 'A' && letter <= 'Z')
    {
        unsigned int index = letter - 'A';

        // Set the letter segment pattern in the LCD memory
        LCDMEM[letterpos[position]] = alphabet[index][0];
        LCDMEM[letterpos[position] + 1] = alphabet[index][1];
    }
}

void displayMessage(const char *message)
{
    unsigned int i;
    for (i = 0; message[i] != '\0'; i++)
    {
        showLetter(message[i], i);  // Display each letter in the message
    }
}

const char digitpos[6] = {pos4, pos3, pos2, pos1};

void displayNum(unsigned int num, unsigned int digits)
{
	short i;
	for(i=digits; i!=0; i--){
		showDigit(num%10,digits-i);
		num=num/10;
	}
}

void showDigit(char c, unsigned int position)
{
	unsigned int mydigit;

    LCDMEM[digitpos[position]] = digit[c][0];
    mydigit = digit[c][1];
    LCDMEM[digitpos[position]+1] = mydigit;
}

int lcd_init()
{
    PJSEL0 = BIT4 | BIT5;                   // For LFXT

    LCDCPCTL0 = 0xFFF0;		// Init. LCD segments 4, 6-15
    LCDCPCTL1 = 0xF83F;		// Init. LCD segments 16-21, 27-31
    LCDCPCTL2 = 0x00F8; 	// Init. LCD segments 35-39

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Configure LFXT 32kHz crystal
    CSCTL0_H = CSKEY >> 8;                  // Unlock CS registers
    CSCTL4 &= ~LFXTOFF;                     // Enable LFXT
    do
    {
      CSCTL5 &= ~LFXTOFFG;                  // Clear LFXT fault flag
      SFRIFG1 &= ~OFIFG;
    }while (SFRIFG1 & OFIFG);               // Test oscillator fault flag
    CSCTL0_H = 0;                           // Lock CS registers

    // Initialize LCD_C
    // ACLK, Divider = 1, Pre-divider = 16; 4-pin MUX
    LCDCCTL0 = LCDDIV__1 | LCDPRE__16 | LCD4MUX | LCDLP | LCDSON;

    // VLCD generated internally,
    // V2-V4 generated internally, v5 to ground
    // Set VLCD voltage to 2.60v
    // Enable charge pump and select internal reference for it
    LCDCVCTL = VLCD_1 | VLCDREF_0 | LCDCPEN;

    LCDCCPCTL = LCDCPCLKSYNC;               // Clock synchronization enabled

    LCDCMEMCTL = LCDCLRM;                   // Clear LCD memory

    LCDCCTL0 |= LCDON;

	return 0;
}

void lcd_clear()
{
	// Initially, clear all displays.
	LCDCMEMCTL |= LCDCLRM;
}


