#include <stdio.h>
#include "Fonts.h"

//define touchscreen
#define Touchscreen_Status		(*(volatile unsigned char *)(0x84000230))
#define Touchscreen_Control 		(*(volatile unsigned char *)(0x84000230))
#define Touchscreen_TxData 			    (*(volatile unsigned char *)(0x84000232))
#define Touchscreen_RxData 			    		(*(volatile unsigned char *)(0x84000232))
#define Touchscreen_Baud    					(*(volatile unsigned char *)(0x84000234))




//define RS232
#define RS232_Control (*(volatile unsigned char *)(0x84000200))
#define RS232_Status (*(volatile unsigned char *)(0x84000200))
#define RS232_TxData (*(volatile unsigned char *)(0x84000202))
#define RS232_RxData (*(volatile unsigned char *)(0x84000202))
#define RS232_Baud (*(volatile unsigned char *)(0x84000204))


//define GPS
#define GPS_Control (*(volatile unsigned char *)(0x84000210))
#define GPS_Status (*(volatile unsigned char *)(0x84000210))
#define GPS_TxData (*(volatile unsigned char *)(0x84000212))
#define GPS_RxData (*(volatile unsigned char *)(0x84000212))
#define GPS_Baud (*(volatile unsigned char *)(0x84000214))
#define arrayLength 100


#define hex0_1 (char *) 0x0002030
#define hex2_3 (char *) 0x0002040
#define hex4_5 (char *) 0x0002050

int arr[arrayLength];
char timeArray[7];
//int minutes[3] = {};
//int seconds[2] = {};

int i = 0;

/**
 *
 *
 * graphics starts
 *
 *
 *
 *
 *
 */
// #defined constants representing values we write to the graphics 'command' register to get
// it to do something. You will add more values as you add hardware to the graphics chip
// Note DrawHLine, DrawVLine and DrawLine at the moment do nothing - you will modify these

#define DrawHLine  	     1
#define DrawVLine		2
#define DrawLine			3
#define PutAPixel				0xA
#define GetAPixel					0xB
#define ProgramPaletteColour   				0x10

#define switches    (volatile int *)0x00002000
#define leds        (volatile int *)0x00002010
#define hex0        (volatile int *)0x00002030
#define hex1        (volatile int *)0x00002040
#define hex2        (volatile int *)0x00002050
#define buttons     (volatile int *)0x00002060

#define FONT2_XPIXELS 10  // width of Font2 characters in pixels (no spacing)
#define FONT2_YPIXELS 14  // height of Font2 characters in pixels (no spacing)
#define FONT3_XPIXELS 16  // width of Font3 characters in pixels (no spacing)
#define FONT3_YPIXELS 27  // height of Font3 characters in pixels (no spacing)
#define FONT4_XPIXELS 22  // width of Font4 characters in pixels (no spacing)
#define FONT4_YPIXELS 40  // height of Font4 characters in pixels (no spacing)
#define FONT5_XPIXELS 38  // width of Font5 characters in pixels (no spacing)
#define FONT5_YPIXELS 59  // height of Font5 characters in pixels (no spacing)

#define XRES 800  // Width of display in pixels
#define YRES 480  // Height of display in pixels

#define CLEAR_SCREEN SolidRectangle(0, 800, 0, 480, BLACK)  // Clear screen

#define WAIT_FOR_GRAPHICS	       	    while((GraphicsStatusReg & 0x0001) != 0x0001)

/*********************************************************************************************
 * This macro pauses until the graphics chip status register indicates that it is idle
*******************************************************************************************/

//Predefined Colour Values
//Use the symbolic constants below as the values to write to the Colour Register
//When you ask the graphics chip to draw something. These values are also returned
//By the read pixel command
// the header file "Colours.h" contains symbolic names for all 256 colours e.g. RED
// while the source file ColourPaletteData.c contains the 24 bit RGB data
// that is pre-programmed into each of the 256 palettes
#define	BLACK		       0
#define	WHITE			1
#define	RED					2
#define	LIME						3
#define	BLUE							4
#define	YELLOW								5
#define	CYAN									6
#define	MAGENTA										7

#define GraphicsCommandReg									(*(volatile unsigned short int *)(0x84000000))
#define GraphicsStatusReg   										    (*(volatile unsigned short int *)(0x84000000))
#define GraphicsX1Reg   										    		    (*(volatile unsigned short int *)(0x84000002))
#define GraphicsY1Reg   												    		    (*(volatile unsigned short int *)(0x84000004))
#define GraphicsX2Reg   														    		    (*(volatile unsigned short int *)(0x84000006))
#define GraphicsY2Reg																	    			     (*(volatile unsigned short int *)(0x84000008))
#define GraphicsColourReg																			     		     (*(volatile unsigned short int *)(0x8400000E))
#define GraphicsBackGroundColourReg (*(volatile unsigned short int *)(0x84000010))

/**********************************************************************
 * This function writes a single pixel to the x,y coords specified in the specified colour
 * Note colour is a palette number (0-255) not a 24 bit RGB value
 **********************************************************************/
void WriteAPixel(int x, int y, int Colour) {
    WAIT_FOR_GRAPHICS;	       // is graphics ready for new command

    GraphicsX1Reg = x;// write coords to x1, y1
    GraphicsY1Reg = y;
    GraphicsColourReg = Colour;// set pixel colour with a palette number
    GraphicsCommandReg = PutAPixel;// give graphics a "write pixel" command
}

/*****************************************************************************************
 * This function read a single pixel from x,y coords specified and returns its colour
 * Note returned colour is a palette number (0-255) not a 24 bit RGB value
 ******************************************************************************************/
int ReadAPixel(int x, int y) {
    WAIT_FOR_GRAPHICS;		// is graphics ready for new command

    GraphicsX1Reg = x;		      // write coords to x1, y1
    GraphicsY1Reg = y;
    GraphicsCommandReg = GetAPixel;		// give graphics a "get pixel" command

    WAIT_FOR_GRAPHICS
    ;			// is graphics done reading pixel
    return (int) (GraphicsColourReg);  // return the palette number (colour)
}

/*****************************************************************************************
 * This function writes pixels across a line from x1 to x2 (or vice-versa)
 ******************************************************************************************/
void HLine(int x1, int x2, int y, int colour) {
    WAIT_FOR_GRAPHICS;      // is graphics ready for new command

    // x1 should be the lesser of the two
    if (x1 < x2) {
        GraphicsX1Reg = x2;
        GraphicsX2Reg = x1;
    } else {
        GraphicsX1Reg = x1;
        GraphicsX2Reg = x2;
    }

    GraphicsY1Reg = y;
    GraphicsColourReg = colour;
    GraphicsCommandReg = DrawHLine;
}

/*****************************************************************************************
 * This function writes pixels across a line from y1 to y2 (or vice-versa)
 ******************************************************************************************/
void VLine(int y1, int y2, int x, int colour) {
    WAIT_FOR_GRAPHICS;      // is graphics ready for new command

    // y1 should be the lesser of the two
    if (y1 < y2) {
        GraphicsY1Reg = y2;
        GraphicsY2Reg = y1;
    } else {
        GraphicsY1Reg = y1;
        GraphicsY2Reg = y2;
    }

    GraphicsX1Reg = x;
    GraphicsColourReg = colour;
    GraphicsCommandReg = DrawVLine;
}

/*****************************************************************************************
 * This function writes pixels across a line from x1, y1 to x2, y2 (or vice-versa)
 ******************************************************************************************/
/*
 void Line (int x1, int x2, int y1, int y2, int colour)
 {
 WAIT_FOR_GRAPHICS;      // is graphics ready for new command

 // y1 should be the lesser of the two
 if (y1 < y2) {
 GraphicsY1Reg = y1;
 GraphicsY2Reg = y2;
 } else {
 GraphicsY1Reg = y2;
 GraphicsY2Reg = y1;
 }

 // x1 should be the lesser of the two
 if (x1 < x2) {
 GraphicsX1Reg = x1;
 GraphicsX2Reg = x2;
 } else {
 GraphicsX1Reg = x2;
 GraphicsX2Reg = x1;
 }

 GraphicsColourReg = colour;
 GraphicsCommandReg = DrawLine;
 }

 /*******************************************************************************
 ** Implementation of Bresenhams line drawing algorithm
 *******************************************************************************/

int abs(int a) {
    if (a < 0)
        return -a;
    else
        return a;
}

int sign(int a) {
    if (a < 0)
        return -1;
    else if (a == 0)
        return 0;
    else
        return 1;
}

void Line(int x1, int y1, int x2, int y2, int Colour) {
    int x = x1;
    int y = y1;
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int s1 = sign(x2 - x1);
    int s2 = sign(y2 - y1);
    int i, temp, interchange = 0, error;

// if x1=x2 and y1=y2 then it is a line of zero length

    if (dx == 0 && dy == 0)
        return;

    // must be a complex line so use bresenhams algorithm
    else {

// swap delta x and delta y depending upon slop of line

        if (dy > dx) {
            temp = dx;
            dx = dy;
            dy = temp;
            interchange = 1;
        }

// initialise the error term to compensate for non-zero intercept

        error = (dy << 1) - dx;    // (2 * dy) - dx

// main loop
        for (i = 1; i <= dx; i++) {
            WriteAPixel(x, y, Colour);

            while (error >= 0) {
                if (interchange == 1)
                    x += s1;
                else
                    y += s2;

                error -= (dx << 1);    // times 2
            }

            if (interchange == 1)
                y += s2;
            else
                x += s1;

            error += (dy << 1);    // times 2
        }
    }
}

/*****************************************************************************************
 * This function draws a hollow triangle using the line function
 ******************************************************************************************/
void Triangle(int x1, int x2, int x3, int y1, int y2, int y3, int colour) {
    Line(x1, y1, x2, y2, colour);
    Line(x3, y3, x2, y2, colour);
    Line(x1, y1, x3, y3, colour);
}

/*****************************************************************************************
 * This function draws a hollow rectangle using the horizontal and vertical line functions
 ******************************************************************************************/
void HollowRectangle(int x1, int x2, int y1, int y2, int colour) {
    HLine(x1, x2, y1, colour);
    HLine(x1, x2, y2, colour);
    VLine(y1, y2, x1, colour);
    VLine(y1, y2, x2, colour);
}

/*****************************************************************************************
 * This function draws a solid rectangle using the horizontal line functions
 * Horizontal lines are twice as fast to draw as vertical lines
 ******************************************************************************************/
void SolidRectangle(int x1, int x2, int y1, int y2, int colour) {
    if (y1 < y2)
        while (y1 <= y2) {
            HLine(x1, x2, y1, colour);
            y1++;
        }
    else
        while (y1 >= y2) {
            HLine(x1, x2, y1, colour);
            y1--;
        }
}

/*****************************************************************************************
 * This function draws a solid rectangle
 * It also draws a border around it with a 1 pixel buffer
 ******************************************************************************************/
void BorderedRectangle(int x1, int x2, int y1, int y2, int colour) {
    // Draw inner rectangle
    SolidRectangle(x1, x2, y1, y2, colour);

    if (x1 < x2) {
        x1 -= 2;
        x2 += 2;
    } else {
        x1 += 2;
        x2 -= 2;
    }

    if (y1 < y2) {
        y1 -= 2;
        y2 += 2;
    } else {
        y1 += 2;
        y2 -= 2;
    }

    // Draw border
    HollowRectangle(x1, x2, y1, y2, colour);
}

/*
 * Draws an octant of a circle with origin x, y, radius r and colour
 * The final argument oct defines which arc is being drawn:
 *
 *    2 1
 *  3     0
 *  4     7
 *    5 6
 *
 * The Circle argument draws all 8 arcs at the same time, much more quickly
 * than calling this function 8 times will do
 *
 */
void Arc(int x, int y, int r, int colour, int oct) {
    int dx = r, dy = 0, err = 0;

    while (dx >= dy) {
        switch (oct) {
        case 0:
            WriteAPixel(x + dx, y + dy, colour);
            break;
        case 1:
            WriteAPixel(x + dy, y + dx, colour);
            break;
        case 2:
            WriteAPixel(x - dy, y + dx, colour);
            break;
        case 3:
            WriteAPixel(x - dx, y + dy, colour);
            break;
        case 4:
            WriteAPixel(x - dx, y - dy, colour);
            break;
        case 5:
            WriteAPixel(x - dy, y - dx, colour);
            break;
        case 6:
            WriteAPixel(x + dy, y - dx, colour);
            break;
        case 7:
            WriteAPixel(x + dx, y - dy, colour);
            break;
        }

        if (err <= 0)
            err += ++dy * 2 + 1;
        if (err > 0)
            err -= --dx * 2 + 1;
    }
}

/*
 * Draws a circle with origin x, y, radius r and colour
 */
void Circle(int x, int y, int r, int colour) {
    int dx = r, dy = 0, err = 0;

    while (dx >= dy) {
        WriteAPixel(x + dx, y + dy, colour);
        WriteAPixel(x + dy, y + dx, colour);
        WriteAPixel(x - dy, y + dx, colour);
        WriteAPixel(x - dx, y + dy, colour);
        WriteAPixel(x - dx, y - dy, colour);
        WriteAPixel(x - dy, y - dx, colour);
        WriteAPixel(x + dy, y - dx, colour);
        WriteAPixel(x + dx, y - dy, colour);

        if (err <= 0)
            err += ++dy * 2 + 1;
        if (err > 0)
            err -= --dx * 2 + 1;
    }
}

/*************************************************************************************************
 ** This function draws a single ASCII character at the coord and colour specified
 ** it optionally ERASES the background colour pixels to the background colour
 ** This means you can use this to erase characters
 **
 ** e.g. writing a space character with Erase set to true will set all pixels in the
 ** character to the background colour
 **
 *************************************************************************************************/
void OutGraphicsCharFont1(int x, int y, int fontcolour, int backgroundcolour,
        int c, int Erase) {
    // using register variables (as opposed to stack based ones) may make execution faster
    // depends on compiler and CPU

    register int row, column, theX = x, theY = y;
    register int pixels;
    register char theColour = fontcolour;
    register int BitMask, theC = c;

    // if x,y coord off edge of screen don't bother
    // XRES and YRES are #defined to be 800 and 480 respectively
    if (((short) (x) > (short) (XRES - 1))
            || ((short) (y) > (short) (YRES - 1)))
        return;

    // if printable character subtract hex 20
    if (((short) (theC) >= (short) (' '))
            && ((short) (theC) <= (short) ('~'))) {
        theC = theC - 0x20;
        for (row = 0; (char) (row) < (char) (7); row++) {

            // get the bit pattern for row 0 of the character from the software font
            pixels = Font5x7[theC][row];
            BitMask = 16;

            for (column = 0; (char) (column) < (char) (5); column++) {

                // if a pixel in the character display it
                if ((pixels & BitMask))
                    WriteAPixel(theX + column, theY + row, theColour);
                else {
                    if (Erase)

                        // if pixel is part of background (not part of character)
                        // erase the background to value of variable BackGroundColour

                        WriteAPixel(theX + column, theY + row,
                                backgroundcolour);
                }
                BitMask = BitMask >> 1;
            }
        }
    }
}

/******************************************************************************************************************************
 ** This function draws a single ASCII character at the coord specified using the colour specified
 ** OutGraphicsCharFont2(100,100, RED, 'A', TRUE, FALSE, 1, 1) ;	// display upper case 'A' in RED at coords 100,100, erase background
 ** no scroll, scale x,y= 1,1
 **
 ******************************************************************************************************************************/
void OutGraphicsCharFont2(int x, int y, int colour, int backgroundcolour, int c,
        int Erase) {
    register int row, column, theX = x, theY = y;
    register int pixels;
    register char theColour = colour;
    register int BitMask, theCharacter = c, j, theRow, theColumn;

    if (((short) (x) > (short) (XRES - 1))
            || ((short) (y) > (short) (YRES - 1))) // if start off edge of screen don't bother
        return;

    if (((short) (theCharacter) >= (short) (' '))
            && ((short) (theCharacter) <= (short) ('~'))) {	// if printable character
        theCharacter -= 0x20;// subtract hex 20 to get index of first printable character (the space character)
        theRow = FONT2_YPIXELS;
        theColumn = FONT2_XPIXELS;

        for (row = 0; row < theRow; row++) {
            pixels = Font10x14[theCharacter][row];// get the pixels for row 0 of the character to be displayed
            BitMask = 512;			     	 // set of hex 200 i.e. bit 7-0 = 0010 0000 0000
            for (column = 0; column < theColumn;) {
                if ((pixels & BitMask))				// if valid pixel, then write it
                    WriteAPixel(theX + column, theY + row, theColour);
                else {// if not a valid pixel, do we erase or leave it along (no erase)
                    if (Erase)
                        WriteAPixel(theX + column, theY + row,
                                backgroundcolour);
                    // else leave it alone
                }
                column++;
                BitMask = BitMask >> 1;
            }
        }
    }
}

/******************************************************************************************************************************
 ** This function draws a single ASCII character at the coord specified using the colour specified
 ** OutGraphicsCharFont3(100,100, RED, 'A', TRUE, FALSE, 1, 1) ;	// display upper case 'A' in RED at coords 100,100, erase background
 ** no scroll, scale x,y= 1,1
 **
 ******************************************************************************************************************************/
void OutGraphicsCharFont3(int x, int y, int colour, int backgroundcolour, int c,
        int Erase) {
    register int column, theX = x, theY = y;
    register int pixels;
    register char theColour = colour;
    register int BitMask, theCharacter = c, j, theRow, theColumn;

    // If starting off the edge of the screen, don't bother
    if (((short) (x) > (short) (XRES - 1))
            || ((short) (y) > (short) (YRES - 1)))
        return;

    if (((short) (theCharacter) >= (short) (' '))
            && ((short) (theCharacter) <= (short) ('~'))) { // if printable character
        theCharacter -= 0x20; // subtract hex 20 to get index of first printable character (the space character)
        theCharacter *= 54; // Multiply by 54 to get first value pertaining to the character
        theRow = FONT3_YPIXELS;
        theColumn = FONT3_XPIXELS;

        // TODO: This can just be the defined constant, right?
        int i;
        for (i = 0; i < 54; i++) {
            pixels = Font16x27[theCharacter + i]; // get the pixels for half of the row of the character to be displayed
            // 8 bits per element
            j = 0;
            for (BitMask = 128; BitMask != 0; BitMask = BitMask >> 1) {
                if (pixels & BitMask)
                    WriteAPixel(theX + (8 * (i % 2)) + j, theY + (i / 2),
                            theColour);
                else if (Erase)
                    WriteAPixel(theX + (8 * (i % 2)) + j, theY + (i / 2),
                            backgroundcolour);
                j++;
            }
        }
    }
}

/******************************************************************************************************************************
 ** This function draws a single ASCII character at the coord specified using the colour specified
 ** OutGraphicsCharFont4(100,100, RED, 'A', TRUE, FALSE, 1, 1) ;	// display upper case 'A' in RED at coords 100,100, erase background
 ** no scroll, scale x,y= 1,1
 **
 ******************************************************************************************************************************/
void OutGraphicsCharFont4(int x, int y, int colour, int backgroundcolour, int c,
        int Erase) {
    register int column, theX = x, theY = y;
    register int pixels;
    register char theColour = colour;
    register int BitMask, theCharacter = c, j, theRow, theColumn;

    // If starting off the edge of the screen, don't bother
    if (((short) (x) > (short) (XRES - 1))
            || ((short) (y) > (short) (YRES - 1)))
        return;

    if (((short) (theCharacter) >= (short) (' '))
            && ((short) (theCharacter) <= (short) ('~'))) { // if printable character
        theCharacter -= 0x20; // subtract hex 20 to get index of first printable character (the space character)
        theCharacter *= 120; // Multiply by 120 to get first value pertaining to the character
        theRow = FONT4_YPIXELS;
        theColumn = FONT4_XPIXELS;

        // TODO: This can just be the defined constant, right?
        int i;

        for (i = 0; i < 120; i++) {
            pixels = Font22x40[theCharacter + i]; // get the pixels for a third of the row of the character to be displayed
            BitMask = 128;  // 8 bits per element
            j = 0;
            for (BitMask = 128; BitMask != 0; BitMask = BitMask >> 1) {
                if (pixels & BitMask)
                    WriteAPixel(theX + (8 * (i % 3)) + j, theY + (i / 3),
                            theColour);
                else if (Erase)
                    WriteAPixel(theX + (8 * (i % 3)) + j, theY + (i / 3),
                            backgroundcolour);
                j++;
            }
        }
    }
}

/******************************************************************************************************************************
 ** This function draws a single ASCII character at the coord specified using the colour specified
 ** OutGraphicsCharFont5(100,100, RED, 'A', TRUE, FALSE, 1, 1) ;	// display upper case 'A' in RED at coords 100,100, erase background
 ** no scroll, scale x,y= 1,1
 **
 ******************************************************************************************************************************/
void OutGraphicsCharFont5(int x, int y, int colour, int backgroundcolour, int c,
        int Erase) {
    register int column, theX = x, theY = y;
    register int pixels;
    register char theColour = colour;
    register int BitMask, theCharacter = c, j, theRow, theColumn;

    // If starting off the edge of the screen, don't bother
    if (((short) (x) > (short) (XRES - 1))
            || ((short) (y) > (short) (YRES - 1)))
        return;

    if (((short) (theCharacter) >= (short) (' '))
            && ((short) (theCharacter) <= (short) ('~'))) { // if printable character
        theCharacter -= 0x20; // subtract hex 20 to get index of first printable character (the space character)
        theCharacter *= 236; // Multiply by 236 to get first value pertaining to the character
        theRow = FONT5_YPIXELS;
        theColumn = FONT5_XPIXELS;

        // TODO: This can just be the defined constant, right?
        int i;
        for (i = 0; i < 236; i++) {
            pixels = Font38x59[theCharacter + i]; // get the pixels for a third of the row of the character to be displayed
            BitMask = 128;  // 8 bits per element
            j = 0;
            for (BitMask = 128; BitMask != 0; BitMask = BitMask >> 1) {
                if (pixels & BitMask)
                    WriteAPixel(theX + (8 * (i % 4)) + j, theY + (i / 4),
                            theColour);
                else if (Erase)
                    WriteAPixel(theX + (8 * (i % 4)) + j, theY + (i / 4),
                            backgroundcolour);
                j++;
            }
        }
    }
}

/*****************************************************************************************
 * This function draws the string provided
 ******************************************************************************************/
void DisplayString(int x, int y, int fg, int bg, char* phrase, int overwrite,
        int size) {
    int i = 0;
    switch (size) {
    case 1:
        while (phrase[i] != '\0') {
            WAIT_FOR_GRAPHICS
            ; // is graphics ready for new command
            OutGraphicsCharFont1(x, y, fg, bg, (int) phrase[i], overwrite);
            i++;
            x += 5;    //width of character
        }
        break;
    case 2:
        while (phrase[i] != '\0') {
            WAIT_FOR_GRAPHICS
            ; // is graphics ready for new command
            OutGraphicsCharFont2(x, y, fg, bg, (int) phrase[i], overwrite);
            i++;
            x += 10;    //width of character
        }
        break;
    case 3:
        while (phrase[i] != '\0') {
            WAIT_FOR_GRAPHICS
            ; // is graphics ready for new command
            OutGraphicsCharFont3(x, y, fg, bg, (int) phrase[i], overwrite);
            i++;
            x += 16;    //width of character
        }
        break;
    case 4:
        while (phrase[i] != '\0') {
            WAIT_FOR_GRAPHICS
            ; // is graphics ready for new command
            OutGraphicsCharFont4(x, y, fg, bg, (int) phrase[i], overwrite);
            i++;
            x += 22;    //width of character
        }
        break;
    case 5:
        while (phrase[i] != '\0') {
            WAIT_FOR_GRAPHICS
            ; // is graphics ready for new command
            OutGraphicsCharFont5(x, y, fg, bg, (int) phrase[i], overwrite);
            i++;
            x += 38;    //width of character
        }
        break;
    }
}

/****************************************************************************************************
 * subroutine to program a hardware (graphics chip) palette number with an RGB value
 ** e.g. ProgramPalette(RED, 0x00FF0000) ;
 ****************************************************************************************************/
void ProgramPalette(int PaletteNumber, int RGB) {
    WAIT_FOR_GRAPHICS
    ;
    GraphicsColourReg = PaletteNumber;
    GraphicsX1Reg = RGB >> 16;        // program red value in ls.8 bit of X1 reg
    GraphicsY1Reg = RGB;         // program green and blue into 16 bit of Y1 reg
    GraphicsCommandReg = ProgramPaletteColour;	  // issue command
}

/****************************************************************
 * GRAPHICS LIBRARIES ABOVE THIS COMMENT! DIYA LOOK NO FURTHER! *
 ***************************************************************/

int answers[25];        // History of user's answers

/* Hilights the answer selected by the user */
void ShowAnswer(int choice) {
    // 1 = yes, 2 = no, 0 = undecided
    switch (choice) {
    case 0:
        BorderedRectangle(150, 349, 260, 380, WHITE);
        BorderedRectangle(451, 649, 260, 380, WHITE);
        DisplayString(230, 310, WHITE, BLUE, "YES", 1, 3);
        DisplayString(535, 310, WHITE, BLUE, "NO", 1, 3);
        break;
    case 1:
        BorderedRectangle(150, 349, 260, 380, BLUE);
        BorderedRectangle(451, 649, 260, 380, WHITE);
        DisplayString(230, 310, WHITE, BLUE, "YES", 1, 3);
        DisplayString(535, 310, WHITE, BLUE, "NO", 1, 3);
        break;
    case 2:
        BorderedRectangle(150, 349, 260, 380, WHITE);
        BorderedRectangle(451, 649, 260, 380, BLUE);
        DisplayString(230, 310, WHITE, BLUE, "YES", 1, 3);
        DisplayString(535, 310, WHITE, BLUE, "NO", 1, 3);
        break;
    }
}

/* Displays desired questions and options */
void Question(int stage) {
    CLEAR_SCREEN;
    // Set up options for answers
    ShowAnswer(answers[stage]);

    // Set up questions
    switch (stage) {
    case 0:
        DisplayString(150, 100, WHITE, BLACK, " I LIKE TO HELP PEOPLE ", 0, 4);
        break;
    case 1:
        DisplayString(150, 100, WHITE, BLACK, "I ENJOY PROBLEM SOLVING", 0, 4);
        break;
    case 2:
        DisplayString(150, 100, WHITE, BLACK, "I LIKE KNOWING WHAT TO ", 0, 4);
        DisplayString(150, 130, WHITE, BLACK, "  DO AND HOW TO DO IT  ", 0, 4);
        break;
    case 3:
        DisplayString(150, 100, WHITE, BLACK, "I LIKE WORKING WITH MY", 0, 4);
        DisplayString(150, 130, WHITE, BLACK, "         HANDS        ", 0, 4);
        break;
    case 4:
        DisplayString(150, 100, WHITE, BLACK, "I AM A CREATIVE PERSON", 0, 4);
        break;
    case 5:
        DisplayString(150, 100, WHITE, BLACK, "I LIKE TO BE IN CHARGE", 0, 4);
        DisplayString(130, 130, WHITE, BLACK, "AND HOLD RESPONSIBILITIES", 0, 4);
        break;
    case 6:
        DisplayString(150, 100, WHITE, BLACK, "  I ENJOY ADRENALINE  ", 0, 4);
        break;
    case 7:
        DisplayString(150, 100, WHITE, BLACK, "  I LIKE THE OUTDOORS ", 0, 4);
        break;
    case 8:
        DisplayString(150, 100, WHITE, BLACK, "I EXPRESS MYSELF WELL ", 0, 4);
        break;
    case 9:
        DisplayString(150, 100, WHITE, BLACK, "   I'M GOOD AT MATH   ", 1, 4);
        break;
    case 10:
        DisplayString(150, 100, WHITE, BLACK, "      I DRAW WELL     ", 1, 4);
        break;
    case 11:
        DisplayString(150, 100, WHITE, BLACK, "   I LIKE THINKING    ", 0, 4);
        DisplayString(150, 130, WHITE, BLACK, "      CRITICALLY      ", 0, 4);
        break;
    case 12:
        DisplayString(150, 100, WHITE, BLACK, "  I AM NOT AFRAID OF  ", 0, 4);
        DisplayString(140, 130, WHITE, BLACK, "SITUATIONS SOME CONSIDER", 0, 4);
        DisplayString(150, 160, WHITE, BLACK, "      DANGEROUS       ", 0, 4);
        break;
    case 13:
        DisplayString(150, 100, WHITE, BLACK, " I MUST BE SURROUNDED ", 0, 4);
        DisplayString(150, 130, WHITE, BLACK, "       BY BEAUTY      ", 0, 4);
        break;
    case 14:
        DisplayString(150, 100, WHITE, BLACK, "I NEED MENTAL STIMULATION", 0,
                4);
        break;
    case 15:
        DisplayString(150, 100, WHITE, BLACK, " I MUST SEE CONCRETE  ", 0, 4);
        DisplayString(150, 130, WHITE, BLACK, "  RESULTS OF MY WORK  ", 0, 4);
        DisplayString(150, 160, WHITE, BLACK, "      RIGHT AWAY      ", 0, 4);
        break;
    case 16:
        DisplayString(150, 100, WHITE, BLACK, " I AM METICULOUS AND  ", 0, 4);
        DisplayString(150, 130, WHITE, BLACK, "       PRECISE        ", 0, 4);
        break;
    case 17:
        DisplayString(150, 100, WHITE, BLACK, "I'D RATHER WORK OUTDOORS", 0, 4);
        DisplayString(150, 130, WHITE, BLACK, "      THAN INDOORS      ", 0, 4);
        break;
    case 18:
        DisplayString(150, 100, WHITE, BLACK, "I'D LIKE TO WORK IN A", 0, 4);
        DisplayString(150, 130, WHITE, BLACK, "  HOSPITAL OR CLINIC ", 0, 4);
        break;
    case 19:
        DisplayString(150, 100, WHITE, BLACK, "I'D LIKE TO WORK IN A", 0, 4);
        DisplayString(150, 130, WHITE, BLACK, " BIG FACTORY OR PLANT", 0, 4);
        break;
    case 20:
        DisplayString(150, 100, WHITE, BLACK, "I'D LIKE TO WORK IN A", 0, 4);
        DisplayString(150, 130, WHITE, BLACK, "      LABORATORY     ", 0, 4);
        break;
    case 21:
        DisplayString(150, 100, WHITE, BLACK, "I'D LIKE TO WORK WHERE", 0, 4);
        DisplayString(150, 130, WHITE, BLACK, "THERE IS A LOT OF ACTION", 0, 4);
        break;
    case 22:
        DisplayString(150, 100, WHITE, BLACK, "I LIKE OPERATING VEHICLES", 0,
                4);
        DisplayString(150, 130, WHITE, BLACK, "   OR HEAVY MACHINERY    ", 0,
                4);
        break;
    case 23:
        DisplayString(150, 100, WHITE, BLACK, "I'D LIKE TO WORK IN PUBLIC", 0,
                4);
        DisplayString(150, 130, WHITE, BLACK, "PLACES WITH LOTS OF PEOPLE", 0,
                4);
        DisplayString(150, 160, WHITE, BLACK, "     TO INTERACT WITH     ", 0,
                4);
        break;
    case 24:
        DisplayString(150, 100, WHITE, BLACK, "SMOKE, DUST, AND DIRT", 0, 4);
        DisplayString(150, 130, WHITE, BLACK, "   DON'T BOTHER ME   ", 0, 4);
        break;
    case 25:
        ;   // This is to address a stupid quirk with gcc
        int i = 0;
        for(i = 0; i < 25; i++)
            if(answers[i] == 2) answers[i] = 0;
        assignScore();
        break;
    }
    SolidRectangle(720, 800, 400, 480, RED);
    OutGraphicsCharFont5(740, 420, WHITE, BLACK, '<', 0);
    // getInfo();
     printGPS();
    DisplayString(720, 12, BLUE, BLACK, timeArray, 0, 2);
}

/*
 * Used for printing the number pad to screen. Does not need to be called more than the one time!
 * This function is boring, do not look at it Nav and Diya!
 */
void ShowNumPad() {
    CLEAR_SCREEN;
    SolidRectangle(400,479,60,140,BLUE);
    OutGraphicsCharFont5(420, 80, WHITE, BLACK, (int)'1', 0);
    SolidRectangle(500,579,60,140,BLUE);
    OutGraphicsCharFont5(520, 80, WHITE, BLACK, (int)'2', 0);
    SolidRectangle(600,679,60,140,BLUE);
    OutGraphicsCharFont5(620, 80, WHITE, BLACK, (int)'3', 0);

    SolidRectangle(400,479,160,240,BLUE);
    OutGraphicsCharFont5(420, 180, WHITE, BLACK, (int)'4', 0);
    SolidRectangle(500,579,160,240,BLUE);
    OutGraphicsCharFont5(520, 180, WHITE, BLACK, (int)'5', 0);
    SolidRectangle(600,679,160,240,BLUE);
    OutGraphicsCharFont5(620, 180, WHITE, BLACK, (int)'6', 0);

    SolidRectangle(400,479,260,340,BLUE);
    OutGraphicsCharFont5(420, 280, WHITE, BLACK, (int)'7', 0);
    SolidRectangle(500,579,260,340,BLUE);
    OutGraphicsCharFont5(520, 280, WHITE, BLACK, (int)'8', 0);
    SolidRectangle(600,679,260,340,BLUE);
    OutGraphicsCharFont5(620, 280, WHITE, BLACK, (int)'9', 0);

    SolidRectangle(400,579,360,440,BLUE);
    OutGraphicsCharFont5(470, 380, WHITE, BLACK, (int)'0', 0);
    SolidRectangle(600,679,360,440,BLUE);
    OutGraphicsCharFont5(620, 380, WHITE, BLACK, (int)'X', 0);

}



/**
 *
 * touchscreen functions start here
 *
 *
 */


int putCharTouch(char c){
    // Wait for TX bit in status register to turn 1 (means transmit is empty)
    while((Touchscreen_Status & 0x02) != 0x02);
    // Send the data to TX (reduce to 8 bits)
    return c & 0xFF;
}

int getCharTouch(void)
{
	// poll Rx bit in 6850 status register. Wait for it to become '1'
	// read received character from 6850 RxData register.
	while ((Touchscreen_Status & 0x01) != 0x01);
	return Touchscreen_RxData;
}

/*****************************************************************************
** Initialise touch screen controller
*****************************************************************************/
void Init_Touch(void)
{
	 // Program 6850 and baud rate generator to communicate with touchscreen
	  // send touchscreen controller an "enable touch" command

	  // Divide by 16 clock, RTS Low, 8 bits of data, no parity,
	  // 1 stop bit, transmitter interrupt disabled
	  Touchscreen_Control = 0x15;
	  // 9600 BAUD
	  Touchscreen_Baud = 0x05;

	  // slight delay to process
	  usleep(10000);

	  // Send TOUCH_ENABLE command
	  putCharTouch(0x55);
	  putCharTouch(0x01);
	  putCharTouch(0x12);

	  usleep(10000);
}

/*****************************************************************************
** test if screen touched
*****************************************************************************/
int ScreenTouched( void )
{
	 // return TRUE if any data received from 6850 connected to touchscreen
	  // or FALSE otherwise

	  return (Touchscreen_RxData == 0x80);
}

/*****************************************************************************
** wait for screen to be touched
*****************************************************************************/
int WaitForTouch()
{
	while(!ScreenTouched())
		;
		return 1;
}

/* a data type to hold a point/coord */
typedef struct { int x, y; } Point ;

Point GetPen(void){
      Point p1;
      int packets[4];
      // wait for a pen down command then return the X,Y coord of the point
      // calibrated correctly so that it maps to a pixel on screen

      // Wait for first packet of touch
      while(WaitForTouch()!=1);

      int i;
      for(i = 0; i < 4; i++){
      	    packets[i] = getCharTouch();
	    }

	    // Get x11 : x7 from 2nd packet, and concatenate to x6 : x0 from 1st packet
	    p1.x = (packets[1] << 7) | packets[0];
	    p1.y = (packets[3] << 7) | packets[2];

	    // Map from controller resolution to screen pixel
	    p1.x = p1.x * 799 / 4095;
	    p1.y = p1.y  * 479 / 4095;

	    printf("x = %d ", p1.x);
	    printf("y = %d\n", p1.y);

	    return p1;
}


/**
 *
 *
 * GPS starts here
 *
 *
 *
 */
void Init_GPS(void){
	GPS_Control = 0x55;
		    GPS_Baud = 0x05;
}


//Receiving from GPS
char getcharGPS(){

while((GPS_Status & 0x01) != 1){
}

return GPS_RxData;
}

void intialiseArray(){
     int j;
     for(j = 0; j < 100; j++){
     	   arr[j] = '-';
	   }
}

void getInfo(){
     char count;
     while(1){

	count = getcharGPS();
	      if(count == '$'){
	      	       	  if(getcharGPS() == 'G')
						if(getcharGPS() == 'P')
										if(getcharGPS() == 'G')
															if(getcharGPS() == 'G')
																	       if(getcharGPS() == 'A'){
																	       		       	       count = getcharGPS();
																				       	            while(count != '\n'){
																						    		         arr[i] =  count;
																									 	         count = getcharGPS();
																											       	       i++;
																												              }
																													               break;
																														             }
																															      }

																															      }

}


void printGPS(){

    int i = 1;

    while(1){

        int k;
        //Extract hours
        for(k = 0; k < 6; k++){
            timeArray[k] = arr[i];
            i++;
        }


        break;
    }

    return;
}


void printOnHex( int val){
    *hex0_1 = val % 100;
    *hex2_3 = (val % 10000)/100;
    *hex4_5 = val / 10000;
    return;
}

/*
 * Determines your aptitude for each career and prints results to the screen
 */
void assignScore()
{
    // Aptitude for each profession
    int engineeringScore = 0;
    int artistScore = 0;
    int environmentalistScore = 0;
    int teacherScore = 0;
    int musicianScore = 0;
    int athleteScore = 0;
    int constructionWorkerScore = 0;
    int policeScore = 0;
    int politicianScore = 0;
    int doctorScore = 0;
    int lawyerScore = 0;
    int scientistScore = 0;

    //Sample QuestionNo.  {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5};
    int engineering[25] = {0,1,1,1,1,1,0,0,0,1,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0};
    int artist[25] =      {0,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int environmentalist[25] = {0,0,0,1,0,0,0,1,1,0,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0};
    int teacher[25] =     {0,1,1,0,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int musician[25] =    {0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int athlete[25] =     {0,0,0,0,0,0,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0};
    int construct[25] =   {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1};
    int police[25] =      {1,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0};
    int politician[25] =  {1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int doctor[25] =      {1,1,1,1,0,0,0,0,0,0,0,1,0,0,1,0,1,0,1,0,1,0,0,0,0};
    int lawyer[25] =      {1,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0};
    int scientist[25] =   {0,1,1,0,1,0,0,0,0,0,0,1,0,0,1,0,1,0,0,0,1,0,0,0,0};

    int i;
    int row = 0;
    char* buf[3];

    // Assign score to all the professions based on the input by the user
    // Adds 1 for each match
    for( i = 0; i < 25; i++ ) {
        engineeringScore += (engineering[i] == answers[i]);
        artistScore += (artist[i] == answers[i]);
        environmentalistScore += (environmentalist[i] == answers[i]);
        teacherScore += (teacher[i] == answers[i]);
        musicianScore += (musician[i] == answers[i]);
        athleteScore +=  (athlete[i] == answers[i]);
        constructionWorkerScore += (construct[i] == answers[i]);
        policeScore += (police[i] == answers[i]);
        politicianScore += (politician[i] == answers[i]);
        doctorScore += (doctor[i] == answers[i]);
        lawyerScore += (lawyer[i] == answers[i]);
        scientistScore += (scientist[i] == answers[i]);
    }

    CLEAR_SCREEN;

    DisplayString(180, 25, BLUE, BLACK, "Your career affinities:", 0, 4);
    // Prints all professions and your match % in order
    for(i = 25; i > 0; i--){
        if(engineeringScore == i) {
            DisplayString(80, 80+(30*row), BLUE, BLACK, "Engineering:", 0, 3);
            sprintf(buf,"%d", engineeringScore*4);
            DisplayString(380, 80+(30*row), BLUE, BLACK, buf, 0, 4);
            row++;
        }

        if(artistScore == i) {
            DisplayString(80, 80+(30*row), BLUE, BLACK, "Artist:", 0, 3);
            sprintf(buf,"%d", artistScore*4);
            DisplayString(380, 80+(30*row), BLUE, BLACK, buf, 0, 4);
            row++;
        }

        if(environmentalistScore == i) {
            DisplayString(80, 80+(30*row), BLUE, BLACK, "Environmentalist:", 0, 3);
            sprintf(buf,"%d", environmentalistScore*4);
            DisplayString(380, 80+(30*row), BLUE, BLACK, buf, 0, 4);
            row++;
        }

        if(teacherScore == i) {
            DisplayString(80, 80+(30*row), BLUE, BLACK, "Teacher:", 0, 3);
            sprintf(buf,"%d", teacherScore*4);
            DisplayString(380, 80+(30*row), BLUE, BLACK, buf, 0, 4);
            row++;
        }

        if(musicianScore == i) {
            DisplayString(80, 80+(30*row), BLUE, BLACK, "Musician:", 0, 3);
            sprintf(buf,"%d", musicianScore*4);
            DisplayString(380, 80+(30*row), BLUE, BLACK, buf, 0, 4);
            row++;
        }

        if(athleteScore == i) {
            DisplayString(80, 80+(30*row), BLUE, BLACK, "Athlete:", 0, 3);
            sprintf(buf,"%d", athleteScore*4);
            DisplayString(380, 80+(30*row), BLUE, BLACK, buf, 0, 4);
            row++;
        }

        if(constructionWorkerScore == i) {
            DisplayString(80, 80+(30*row), BLUE, BLACK, "Engineering:", 0, 3);
            sprintf(buf,"%d", engineeringScore*4);
            DisplayString(380, 80+(30*row), BLUE, BLACK, buf, 0, 4);
            row++;
        }

        if(policeScore == i) {
            DisplayString(80, 80+(30*row), BLUE, BLACK, "Police:", 0, 3);
            sprintf(buf,"%d", policeScore*4);
            DisplayString(380, 80+(30*row), BLUE, BLACK, buf, 0, 4);
            row++;
        }

        if(politicianScore == i) {
            DisplayString(80, 80+(30*row), BLUE, BLACK, "Politician:", 0, 3);
            sprintf(buf,"%d", politicianScore*4);
            DisplayString(380, 80+(30*row), BLUE, BLACK, buf, 0, 4);
            row++;
        }

        if(doctorScore == i) {
            DisplayString(80, 80+(30*row), BLUE, BLACK, "Doctor:", 0, 3);
            sprintf(buf,"%d", doctorScore*4);
            DisplayString(380, 80+(30*row), BLUE, BLACK, buf, 0, 4);
            row++;
    }

        if(lawyerScore == i) {
            DisplayString(80, 80+(40*row), BLUE, BLACK, "Lawyer:", 0, 3);
            sprintf(buf,"%d", lawyerScore*4);
            DisplayString(380, 80+(30*row), BLUE, BLACK, buf, 0, 4);
            row++;
    }

        if(scientistScore == i) {
            DisplayString(80, 80+(30*row), BLUE, BLACK, "Scientist:", 0, 3);
            sprintf(buf,"%d", scientistScore*4);
            DisplayString(380, 80+(30*row), BLUE, BLACK, buf, 0, 4);
            row++;
        }
    }
}

int main() {

    Init_Touch();
    printf("Done with Touchscreen initialization\r\n");
    Init_GPS();
    intialiseArray();
    getInfo();
    printf("Done with GPS initialization\r\n");

    int i, j;               // Used in for loops
    int sel, count;         // Recording button and switch input
    int lastChoice, choice; // User's current answers
    int stage, setup;       // Keep track of quiz state
    int pos;   		       // Used for number pad entry
    char* num = "_         \0";          // You can later use 'atoi(num);' to use this number as an integer instead of string
    int numpad = 1;


    // Clear number after reset
    for (i = 1; i < 10; i++) num[i] = ' ';
    num[0] = '_';

    // Initialize the answer array to all unanswered
    for (i = 0; i < 25; i++) answers[i] = 0;

    choice = 0; // Does not default to any specific answer
    stage = 0;  // Start at question 0
    setup = 1;  // Flag that allows the intro to print

    // Print the welcome screen //
    CLEAR_SCREEN;

    DisplayString(150, 130, BLUE, BLACK, "APTITUDE TEST", 0, 5);
    DisplayString(180, 300, WHITE, BLACK, "PRESS ANYWHERE TO CONTINUE.", 0, 3);


    /*
     *
     * TODO: Make this loop until the user touches the screen
     * Right now it loops until a button is pressed + depressed
     * Be sure to wait for the user to both lower and lift their finger from the screen,
     * So that they don't accidentally enter the first number on the numpad as it loads!
     *
     * THIS CODE (Or something like it) SHOULD BE USED IN SOME FASHION TO CONFIRM YOUR TOUCH SCREEN RESPONSES
     * In that case the while loops should have large bodies, to allow the user to still change
     * their selection or go back via touchscreen
     */
    // count = 7;
    //while (count == 7) count = *buttons;
    //while (count != 7) count = *buttons;

    while(WaitForTouch()!=1);
    // TODO: Consider making this 'while (stage != 25), that way it will exit once the last question has been answered!
    // You should probably make it do something at the end, in that case!
    while (1) {

        // TODO: Replace switches with touch screen entirely
        // sel = *switches;            // Read switches
        //count = *buttons;           // Read buttons
        // TODO: Set choice via touch screen coordinates. If choice != 0, then check for count != 7 to confirm
        if(ScreenTouched()){

            Point p1 = GetPen();
            int y1 = p1.y;
            int x1 = p1.x;

            lastChoice = choice;

            if ((440<x1 && x1<663)&&(80<y1 && y1<215)) choice = 1;
            else if ((440<x1 && x1<663)&&(253<y1 && y1<387)) choice = 2;
            else choice = 3;

            if (!numpad) ShowAnswer(choice);    // Update the selected response if you're past the number entry screen
            while(ScreenTouched());
            //while (count != 7) count = *buttons;    // Wait for buttons to be depressed
        }

        // Number entry! //

        if (numpad) {
            // Displays the numpad the first time, never again called
            if(setup) {
                ShowNumPad();
                setup = 0;
                i = 0;
                DisplayString(100, 200, BLACK, WHITE, num, 1, 4);
            }
            // TODO: Choice and sel should be replaced by touch screen input!

            if(ScreenTouched()){
                while(ScreenTouched());
                Point p1 = GetPen();
                int y = p1.y;
                int x = p1.x;

                if((583<x && x<739)&&(236<y && y<359)){

                    if (i != 10){
                        num[i] = '0';
                        i++;
                    }
                    if(i != 10) num[i] = '_';
                }else if((165<x && x<320)&&(236<y && y<290)){
                    if (i != 10){
                        num[i] = '1';
                        i++;
                    }
                    if(i != 10) num[i] = '_';

                }else if ((165<x && x<320)&&(290<y && y<359)){

                    if (i != 10){
                        num[i] = '2';
                        i++;
                    }
                    if(i != 10) num[i] = '_';
                }else if((165<x && x<320)&&(359<y && y<420)){


                    if (i != 10){
                        num[i] = '3';
                        i++;
                    }
                    if(i != 10) num[i] = '_';
                }else if((320<x && x<460)&&(236<y && y<290)){

                    if (i != 10){
                        num[i] = '4';
                        i++;
                    }
                    if(i != 10) num[i] = '_';
                }else if((320<x && x<460)&&(290<y && y<359)){

                    if (i != 10){
                        num[i] = '5';
                        i++;
                    }
                    if(i != 10) num[i] = '_';
                }else if((320<x && x<460)&&(359<y && y<420)){


                    if (i != 10){
                        num[i] = '6';
                        i++;
                    }
                    if(i != 10) num[i] = '_';
                }else if((460<x && x<583)&&(236<y && y<290)){

                    if (i != 10){
                        num[i] = '7';
                        i++;
                    }
                    if(i != 10) num[i] = '_';
                }else if((460<x && x<583)&&(290<y && y<359)){

                    if (i != 10){
                        num[i] = '8';
                        i++;
                    }
                    if(i != 10) num[i] = '_';
                }else if((460<x && x<583)&&(359<y && y<420)){

                    if (i != 10){
                        num[i] = '9';
                        i++;
                    }
                    if(i != 10) num[i] = '_';
                }else if((583<x && x<739)&&(359<y && y<420)){
                    if(i > 0){
                        if (i != 10) num[i] = ' ';
                        i--;
                        num[i] = '_';
                    }
                }else{

                    if(i == 10) {
                        numpad = 0;
                        setup = 1;
                    }




                }
                DisplayString(100, 200, BLUE, WHITE, num, 1, 4);   // Display phone #
                choice = 0;     // Allow this if statement to be called again

                /*
                 * TODO: THIS IS WHERE THE USER CAN ENTER THEIR NUMBER!
                 * DIYA, YOU SHOULD PROBABLY USE THIS SECTION TO CHECK IF THEY PRESS THE ENTER BUTTON
                 * MAKE SURE THEY CAN'T PRESS IT WHEN I != 10!
                 */
                if(i == 10) {
                    SolidRectangle(80, 349, 280, 340, BLUE);
                    DisplayString(100, 300, WHITE, WHITE, "ENTER", 0, 4);
                } else SolidRectangle(80, 349, 280, 420, BLACK);
            }

            // Question Answering Part //

        } else {
            // Redraw page if new question is being asked
            if (setup) {

                Question(stage);
                setup = 0;
            }

            // TODO: This case handles the back button!
            if(ScreenTouched()){


                Point p1 = GetPen();
                while(ScreenTouched());
                int b = p1.y;
                int a = p1.x;

                if ((675<a && a<760)&&(400<b && b<460)) {

                    if (stage > 0) {

                        stage--;
                        setup = 1;
                        choice = answers[stage];
                        lastChoice = -1;    // Prevents lastChoice == choice from consecutive back entries!
                    }
                    // Record answer and move to next question
                    // if the user has locked in their selection
                    // TODO: SHOULD BE 'if (choice && (buttons != 7))' or something like that!
                } else if ((440<a && a<663)&&(80<b && b<215)&&(choice && lastChoice == choice)) {
                    answers[stage] = choice;
                    stage++;
                    setup = 1;
                    choice = answers[stage];
                    lastChoice = 0;


                }else if ((440<a && a<663)&&(253<b && b<387)&&(choice && lastChoice == choice)){
                    answers[stage] = choice;
                    stage++;
                    setup = 1;
                    choice = answers[stage];
                    lastChoice=0;
                }

            }
        }
    }

    // YOU MADE IT GUYS! WELL DONE!
    return 0;
}
