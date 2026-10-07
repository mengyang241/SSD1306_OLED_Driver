/**
  ******************************************************************************
  * @file    ssd1306.h
  * @brief   SSD1306 OLED display driver (I2C)
  *          Modular component - can be reused across projects
  * @author  MengYang
  *
  * ============================ Hardware connection ============================
  *   SDA ---> I2C data line   (any I2C-capable pin, default: I2C1)
  *   SCL ---> I2C clock line  (any I2C-capable pin, default: I2C1)
  *   VCC ---> 3.3V
  *   GND ---> GND
  *
  *   ★ Most SSD1306 modules have onboard I2C pull-up resistors. If yours is
  *     a bare board, add 4.7K ~ 10K pull-ups on both SDA and SCL to 3.3V.
  *
  * ==================== Configuration ====================
  *   The driver uses the handle `hi2c1` (declared extern below), configured
  *   in CubeMX. To use another I2C peripheral, change the `hi2c1` references
  *   in ssd1306.c (or make the handle a parameter).
  *
  *   I2C address used by the driver: 0x78 (7-bit address 0x3C shifted left
  *   by 1, which is what HAL_I2C_Master_Transmit expects).
  *
  * ==================== Display layout ====================
  *   The screen is 128 x 64 pixels, organized as 8 pages of 128 columns.
  *   One page = 8 vertical pixels, so an 8x16 font occupies 2 pages.
  *
  *   Coordinate system used by the API:
  *     x (column): 0 ~ 127, left to right
  *     y (page):   0 ~ 7,   top to bottom (each page is 8 pixels tall)
  *
  * ==================== Usage ====================
  *   ssd1306_Init();                          // once after power-up
  *   ssd1306_Clear();
  *   ssd1306_ShowString(0, 0, "Hello");       // column 0, page 0 (top-left)
  *   ssd1306_ShowNum(0, 2, 12345, 5);         // column 0, page 2
  *
  *   Note: a character string wraps automatically when it reaches the right
  *   edge (x > 120), moving down 2 pages (because the font is 16px tall).
  ******************************************************************************
  */

#ifndef SSD1306_H
#define SSD1306_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "font8x16.h"
/**
 * @brief  Set the cursor position in page addressing mode
 * @param  page: the screen is divided into 8 pages vertically, page = which page (0~7)
 * @param  col:  the screen has 128 columns horizontally, col = which column (0~127)
 */
void OLED_SetCursor(uint8_t page, uint8_t col);
/**
 * @brief  Write one command / one data byte to the SSD1306
 * @param  cmd: command byte (sent with the 0x00 control byte)
 * @param  dat: data byte    (sent with the 0x40 control byte)
 */
void ssd1306_WriteCommand(uint8_t cmd);
void ssd1306_WriteData(uint8_t dat);

void ssd1306_Init(void);
void ssd1306_Clear(void);
/**
 * @brief  Show one character at the specified position
 * @param  x:  column coordinate (0~127)
 * @param  y:  page coordinate (0~7), the 8x16 font occupies 2 pages
 * @param  ch: the ASCII character to display
 */
void ssd1306_ShowChar(uint8_t x , uint8_t y ,char ch);

/**
 * @brief  Show a string at the specified position
 * @param  x:  starting column coordinate (0~127)
 * @param  y:  starting page coordinate (0~7), the 8x16 font occupies 2 pages
 * @param  str: the string to display (terminated by '\0')
 */
void ssd1306_ShowString(uint8_t x, uint8_t y, char *str);
/**
 * @brief  Show a decimal number at the specified position
 * @param  x:   starting column coordinate (0~127)
 * @param  y:   starting page coordinate (0~7)
 * @param  num: the number to display
 * @param  len: number of digits to display (zero-padded on the left if
 *              num has fewer digits than len, max 10)
 */
void ssd1306_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len);

/* I2C handle used by this driver, defined in main.c and configured in CubeMX */
extern I2C_HandleTypeDef hi2c1;

#ifdef __cplusplus
}
#endif

#endif /* SSD1306_H */
