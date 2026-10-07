# SSD1306 STM32 Driver (I2C)

> I am also a beginner learning STM32. This driver was written while learning,
> with comments kept as detailed as possible. I hope it helps others who are
> at the same stage — corrections and suggestions are welcome.

A HAL-based SSD1306 OLED display driver over I2C for the STM32F1 series,
with an 8x16 ASCII font. Small, polled, no RTOS or interrupt dependency.

## Features

- Pure HAL implementation over I2C, no interrupt or RTOS dependency
- Page addressing mode, full-screen clear, character / string / number output
- 8x16 ASCII font covering space (0x20) through tilde (0x7E)
- Automatic line wrap when text reaches the right edge
- Comments in English, configuration notes in the header, I2C protocol notes
  in the source file

## Hardware connection

| OLED pin | Connect to | Notes |
|----------|-----------|-------|
| SDA      | I2C data line  | any I2C-capable pin (default: I2C1) |
| SCL      | I2C clock line | any I2C-capable pin (default: I2C1) |
| VCC      | 3.3V       | check your module, some accept 5V |
| GND      | GND        | must share ground with the MCU |

★ Most SSD1306 modules have onboard I2C pull-up resistors. If yours is a
bare board, add 4.7K ~ 10K pull-ups on both SDA and SCL to 3.3V.

## Configuration

- **I2C peripheral**: the driver uses the handle `hi2c1`, configured in CubeMX.
  To use another I2C peripheral, change the `hi2c1` references in `ssd1306.c`.
- **I2C address**: `0x78` (7-bit address `0x3C` shifted left by 1, which is
  what `HAL_I2C_Master_Transmit` expects).
- **CubeMX setup**: enable the I2C peripheral in standard mode (100kHz) or
  fast mode (400kHz); no pin remapping needed beyond selecting I2C1.

## Display layout

The screen is 128 × 64 pixels, organized as **8 pages × 128 columns**.
One page = 8 vertical pixels, so the 8x16 font occupies 2 pages.

```
        column x (0 ~ 127)
   ┌─────────────────────────┐
   │  page 0   (y = 0)       │  ← top
   │  page 1   (y = 1)       │
   │  page 2   (y = 2)       │
   │  ...                     │
   │  page 7   (y = 7)       │  ← bottom
   └─────────────────────────┘
```

## Directory structure

```
My_SSD1306/
├─ Inc/
│  ├─ ssd1306.h       driver header (wiring, configuration, usage — in the file header comment)
│  └─ font8x16.h      font table declaration
├─ Src/
│  ├─ ssd1306.c       driver implementation (I2C protocol notes — in the file header comment)
│  └─ font8x16.c      8x16 ASCII glyph data (95 characters)
└─ README.md
```

## Usage

```c
ssd1306_Init();                          // once after power-up
ssd1306_Clear();

ssd1306_ShowString(0, 0, "Hello");       // column 0, page 0 (top-left)
ssd1306_ShowChar(64, 0, 'A');            // single character at column 64
ssd1306_ShowNum(0, 2, 12345, 5);         // number with 5 digits, page 2
```

- Strings wrap automatically when `x > 120` (128 - 8), moving down 2 pages
  because the font is 16 pixels tall
- `ssd1306_ShowNum` zero-pads on the left: `ShowNum(0, 0, 7, 3)` displays `007`

## API

| Function | Description |
|----------|-------------|
| `ssd1306_Init()`                     | initialize the display (charge pump, page addressing, display on) |
| `ssd1306_Clear()`                    | clear the whole screen |
| `ssd1306_ShowChar(x, y, ch)`         | show one ASCII character |
| `ssd1306_ShowString(x, y, str)`      | show a string (auto line wrap) |
| `ssd1306_ShowNum(x, y, num, len)`    | show a decimal number with `len` digits |
| `OLED_SetCursor(page, col)`          | move the cursor (page addressing mode) |
| `ssd1306_WriteCommand(cmd)`          | send a raw command byte |
| `ssd1306_WriteData(dat)`             | send a raw data byte |

## Notes

- Call `ssd1306_Init()` once after power-up, before any drawing
- The coordinate `y` is a **page index** (0~7), not a pixel row: to draw at
  pixel row 16, use page `y = 2`
- The font table covers printable ASCII only (space through `~`); characters
  outside that range will show garbage
