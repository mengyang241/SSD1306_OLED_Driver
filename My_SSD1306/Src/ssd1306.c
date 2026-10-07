/**
  ******************************************************************************
  * @file    ssd1306.c
  * @brief   SSD1306 OLED display driver (I2C) implementation
  * @author  MengYang
  *
  * See ssd1306.h for wiring, configuration and usage notes.
  *
  * I2C protocol used by this driver:
  *   [Start] [0x78] [control byte] [data byte] [Stop]
  *   control byte 0x00 = following byte is a command
  *   control byte 0x40 = following byte is display data
  ******************************************************************************
  */

#include "ssd1306.h"
#include "main.h"

// Send one command to the SSD1306 (control byte 0x00)
void ssd1306_WriteCommand(uint8_t cmd){
	uint8_t buf[2] = {0x00,cmd};
	HAL_I2C_Master_Transmit(&hi2c1,0x78,buf,2,HAL_MAX_DELAY);
}

// Send one data byte to the SSD1306 (control byte 0x40)
void ssd1306_WriteData(uint8_t dat){
	uint8_t buf[2] = {0x40,dat};
	HAL_I2C_Master_Transmit(&hi2c1,0x78,buf,2,HAL_MAX_DELAY);
}

// SSD1306 initialization
void ssd1306_Init(void){
	ssd1306_WriteCommand(0xae);// display OFF
	ssd1306_WriteCommand(0x8d);// set charge pump
	ssd1306_WriteCommand(0x14);// charge pump ON (required for OLED panel)
	ssd1306_WriteCommand(0x20);// set addressing mode
  ssd1306_WriteCommand(0x02);// page addressing mode
	ssd1306_WriteCommand(0xa4);// output follows RAM contents
	ssd1306_WriteCommand(0xaf);// display ON
	
}

// Move the cursor to the given page and column (page addressing mode)
void OLED_SetCursor(uint8_t page, uint8_t col){
    ssd1306_WriteCommand(0xB0 + page);        // set page address (0xB0 ~ 0xB7)
    ssd1306_WriteCommand(0x00 + (col & 0x0F));// set lower nibble of column address
    ssd1306_WriteCommand(0x10 + (col >> 4));  // set upper nibble of column address
}

// Clear the whole screen (write 0x00 to every page/column)
void ssd1306_Clear(void){
    uint8_t i, j;
    for(j = 0; j < 8; j++){          // 8 pages
        OLED_SetCursor(j, 0);         // move to page j, column 0
        for(i = 0; i < 128; i++){     // 128 columns per page
            ssd1306_WriteData(0x00);  // write 0, pixel off
        }
    }
}

/**
 * @brief  Show one character at the specified position
 * @param  x:  column coordinate (0~127)
 * @param  y:  page coordinate (0~7), the 8x16 font occupies 2 pages
 * @param  ch: the ASCII character to display
 * @note   Requires the Font8x16 glyph table (defined in font8x16.c)
 */

void ssd1306_ShowChar(uint8_t x , uint8_t y ,char ch){
	uint8_t idx = ch - ' ';
	uint8_t i;
	
	// ===== page 1: first 8 bytes of the glyph (top 8 pixels) =====
    OLED_SetCursor(y, x);
    for(i = 0; i < 8; i++){
        ssd1306_WriteData(Font8x16[idx][i]);
    }

    // ===== page 2: last 8 bytes of the glyph (bottom 8 pixels) =====
    OLED_SetCursor(y + 1, x);
    for(i = 8; i < 16; i++){
        ssd1306_WriteData(Font8x16[idx][i]);
    }
}

/**
 * @brief  Show a string at the specified position
 * @param  x:  starting column coordinate (0~127)
 * @param  y:  starting page coordinate (0~7), the 8x16 font occupies 2 pages
 * @param  str: the string to display (terminated by '\0')
 */
void ssd1306_ShowString(uint8_t x, uint8_t y, char *str){
    while(*str != '\0'){          // walk through each character until the end
        ssd1306_ShowChar(x, y, *str); // show the current character
        x += 8;                    // each char is 8 columns wide, move cursor right
        if(x > 120){               // line is full (128 - 8 = 120)
            x = 0;                 // wrap back to the leftmost column
            y += 2;                // the 8x16 font spans 2 pages, so page += 2
        }
        str++;                     // advance to the next character
    }
}

/**
 * @brief  Show a decimal number at the specified position
 * @param  x:   starting column coordinate (0~127)
 * @param  y:   starting page coordinate (0~7)
 * @param  num: the number to display
 * @param  len: number of digits to display (max 10)
 * @note   Digits are extracted from the lowest one and placed from the right,
 *         so if num has fewer digits than len the result is zero-padded on
 *         the left (e.g. num=7, len=3 shows "007").
 */
void ssd1306_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len){
    char buf[11];
    uint8_t i;

    if(len > 10) len = 10;       // at most 10 digits (uint32_t max is 4294967295)
    buf[len] = '\0';             // place the string terminator at the end

    for(i = len; i > 0; i--){
        buf[i - 1] = (char)('0' + num % 10);  // take the lowest digit, convert to char
        num /= 10;                             // drop the lowest digit
    }

    ssd1306_ShowString(x, y, buf);  // display the resulting string
}
