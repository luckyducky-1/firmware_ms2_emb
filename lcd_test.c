
#include "pico/stdlib.h"
#include "lcd.h"

int main(void)
{
    // Initialize the LCD hardware
    lcd_init();

    while (true)
    {
        // Test 1: Display text on both lines
        lcd_clear();

        lcd_set_cursor(0, 0);
        lcd_write_text("ROBOTIC ARM");

        lcd_set_cursor(1, 0);
        lcd_write_text("SYSTEM READY");

        sleep_ms(3000);

        // Test 2: Clear and display new text
        lcd_clear();

        lcd_set_cursor(0, 0);
        lcd_write_text("LCD DRIVER");

        lcd_set_cursor(1, 0);
        lcd_write_text("TEST SUCCESS");

        sleep_ms(3000);
    }

    return 0;
}
