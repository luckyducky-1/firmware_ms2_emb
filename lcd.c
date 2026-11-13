
#include "lcd.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// I2C0 is available on GP12 (SDA) and GP13 (SCL)
#define LCD_I2C_PORT i2c0

// Common PCF8574-to-HD44780 backpack mapping
#define LCD_RS        0x01
#define LCD_RW        0x02
#define LCD_ENABLE    0x04
#define LCD_BACKLIGHT 0x08

// HD44780 commands
#define LCD_CLEAR_DISPLAY 0x01
#define LCD_ENTRY_MODE    0x06
#define LCD_DISPLAY_ON    0x0C
#define LCD_FUNCTION_SET  0x28
#define LCD_SET_DDRAM     0x80

// Send one byte to the I2C backpack

static bool lcd_communication_ok = true;

// Send one byte to the I2C backpack
static void lcd_write_i2c(uint8_t data)
{
    uint8_t output = data | LCD_BACKLIGHT;

    int result = i2c_write_blocking(
        LCD_I2C_PORT,
        LCD_I2C_ADDRESS,
        &output,
        1,
        false
    );

    if (result != 1)
    {
        lcd_communication_ok = false;
    }
}

// Pulse the LCD enable pin
static void lcd_pulse_enable(uint8_t data)
{
    lcd_write_i2c(data | LCD_ENABLE);
    sleep_us(1);

    lcd_write_i2c(data & ~LCD_ENABLE);
    sleep_us(50);
}

// Send the upper or lower four bits
static void lcd_send_nibble(uint8_t nibble,
                            uint8_t mode)
{
    uint8_t data = (nibble & 0xF0) | mode;

    lcd_write_i2c(data);
    lcd_pulse_enable(data);
}

// Send a complete byte as two nibbles
static void lcd_send_byte(uint8_t value,
                          uint8_t mode)
{
    lcd_send_nibble(value & 0xF0, mode);
    lcd_send_nibble((value << 4) & 0xF0, mode);
}

// Send an instruction to the LCD
static void lcd_command(uint8_t command)
{
    lcd_send_byte(command, 0);

    if (command == LCD_CLEAR_DISPLAY ||
        command == 0x02)
    {
        sleep_ms(2);
    }
}

// Initialize the LCD
void lcd_init(void)
{
    i2c_init(LCD_I2C_PORT, LCD_I2C_BAUDRATE);

    gpio_set_function(LCD_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(LCD_SCL_PIN, GPIO_FUNC_I2C);

    gpio_pull_up(LCD_SDA_PIN);
    gpio_pull_up(LCD_SCL_PIN);

    sleep_ms(50);

    // HD44780 initialization sequence
    lcd_send_nibble(0x30, 0);
    sleep_ms(5);

    lcd_send_nibble(0x30, 0);
    sleep_us(150);

    lcd_send_nibble(0x30, 0);
    sleep_us(150);

    // Switch to 4-bit mode
    lcd_send_nibble(0x20, 0);
    sleep_us(150);

    // Two lines, 5x8 character font
    lcd_command(LCD_FUNCTION_SET);

    // Turn display on, cursor off
    lcd_command(LCD_DISPLAY_ON);

    // Automatic cursor increment
    lcd_command(LCD_ENTRY_MODE);

    lcd_clear();
}

// Clear the display
void lcd_clear(void)
{
    lcd_command(LCD_CLEAR_DISPLAY);
}

// Position cursor on row 0 or 1
void lcd_set_cursor(uint8_t row, uint8_t col)
{
    if (row >= LCD_ROWS || col >= LCD_COLS)
    {
        return;
    }

    uint8_t address = (row == 0 ? 0x00 : 0x40)
                      + col;

    lcd_command(LCD_SET_DDRAM | address);
}

// Write a single character
void lcd_write_char(char character)
{
    lcd_send_byte((uint8_t)character, LCD_RS);
}

// Write a string
void lcd_write_text(const char *text)
{
    if (text == NULL)
    {
        return;
    }

    while (*text != '\0')
    {
        lcd_write_char(*text);
        text++;
    }
}
bool lcd_is_communication_ok(void)
{
    return lcd_communication_ok;
}