
#ifndef LCD_H
#define LCD_H

#include <stdint.h>

// LCD dimensions
#define LCD_ROWS 2
#define LCD_COLS 16

// I2C configuration
#define LCD_I2C_ADDRESS 0x27
#define LCD_SDA_PIN 12
#define LCD_SCL_PIN 13
#define LCD_I2C_BAUDRATE 100000

// Public LCD functions
void lcd_init(void);
void lcd_clear(void);
void lcd_set_cursor(uint8_t row, uint8_t col);
void lcd_write_text(const char *text);
void lcd_write_char(char character);

#include <stdbool.h>

bool lcd_is_communication_ok(void);
#endif
