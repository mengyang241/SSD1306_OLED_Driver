/**
  ******************************************************************************
  * @file    font8x16.h
  * @brief   8x16 ASCII font glyph table (space ' ' through '~')
  * @author  MengYang
  *
  * Each glyph is 16 bytes: bytes 0~7 draw the top 8 pixels (page 1),
  * bytes 8~15 draw the bottom 8 pixels (page 2).
  * Lookup: Font8x16[ch - 32], valid for ASCII 32 (space) ~ 126 (~).
  ******************************************************************************
  */

#ifndef FONT8x16_H
#define FONT8x16_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ssd1306.h"

extern const uint8_t Font8x16[][16];

#ifdef __cplusplus
}
#endif

#endif /* FONT8x16_H */
