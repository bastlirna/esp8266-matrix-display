#include <Arduino.h>
#include <SPI.h>
#include <MD_MAX72xx.h>

#define	PRINT(s, x)	{ Serial.print(F(s)); Serial.print(x); }
#define	PRINTS(x)	Serial.print(F(x))
#define	PRINTD(x)	Serial.println(x, DEC)

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define	MAX_DEVICES	10

#define	CLK_PIN		13  // or SCK
#define	DATA_PIN	11  // or MOSI
#define	CS_PIN		4  // or SS

// SPI hardware interface
MD_MAX72XX mx = MD_MAX72XX(CS_PIN, MAX_DEVICES);
// Arbitrary pins
//MD_MAX72XX mx = MD_MAX72XX(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// We always wait a bit between updates of the display
#define  DELAYTIME  10  // in milliseconds

// Text parameters
#define	CHAR_SPACING	1	// pixels between characters

void scrollText(char *p, unsigned int length)
{
  uint8_t	charWidth;
  uint8_t	cBuf[8];	// this should be ok for all built-in fonts

  PRINTS("\nScrolling text");
  //mx.clear();

  while (*p != '\0' & length!=0)
  {length--;
    charWidth = mx.getChar(*p++, sizeof(cBuf)/sizeof(cBuf[0]), cBuf);

    for (uint8_t i=0; i<charWidth + 1; i++)	// allow space between characters
    {
      mx.transform(MD_MAX72XX::TSL);
      if (i < charWidth)
        mx.setColumn(0, cBuf[i]);
      delay(DELAYTIME);
    }
  }
}

void printText(uint8_t modStart, uint8_t modEnd, char *pMsg, unsigned int length)
// Print the text string to the LED matrix modules specified.
// Message area is padded with blank columns after printing.
{
  uint8_t   state = 0;
  uint8_t	  curLen;
  uint16_t  showLen;
  uint8_t	  cBuf[8];
  int16_t   col = ((modEnd + 1) * COL_SIZE) - 1;

  mx.control(modStart, modEnd, MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);

  do     // finite state machine to print the characters in the space available
  {
    switch(state)
    {
      case 0:	// Load the next character from the font table
        // if we reached end of message, reset the message pointer
        if (length == 0)
        {
          showLen = col - (modEnd * COL_SIZE);  // padding characters
          state = 2;
          break;
        }

        // retrieve the next character form the font file
        showLen = mx.getChar(*pMsg++, sizeof(cBuf)/sizeof(cBuf[0]), cBuf);
        curLen = 0;
        state++;
        // !! deliberately fall through to next state to start displaying
        length--;
        

      case 1:	// display the next part of the character
        mx.setColumn(col--, cBuf[curLen++]);

        // done with font character, now display the space between chars
        if (curLen == showLen)
        {
          showLen = CHAR_SPACING;
          state = 2;
        }
        break;

      case 2: // initialize state for displaying empty columns
        curLen = 0;
        state++;
        // fall through

      case 3:	// display inter-character spacing or end of message padding (blank columns)
        mx.setColumn(col--, 0);
        curLen++;
        if (curLen == showLen)
          state = 0;
        break;

      default:
        col = -1;   // this definitely ends the do loop
    }
  } while (col >= (modStart * COL_SIZE));

  mx.control(modStart, modEnd, MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
}

int display_init(){

    mx.begin();
    delay(1000);
    PRINTS("\n[MD_MAX72XX Test & Demo]");
      mx.control(MD_MAX72XX::INTENSITY, 0);
  //  scrollText("MD_MAX72xx Test  ");

}
