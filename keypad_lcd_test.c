
#include <stdio.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "keypad.h"
#include "lcd.h"

// Keypad GPIO assignments
static const unsigned int row_pins[4] = {
    2, 3, 4, 5
};

static const unsigned int col_pins[4] = {
    6, 7, 8, 9
};

int main(void)
{
    stdio_init_all();

    // Initialize both hardware modules
    keypad_init(row_pins, col_pins);
    lcd_init();

    // Welcome message
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_write_text("KEYPAD + LCD");
    lcd_set_cursor(1, 0);
    lcd_write_text("PRESS ANY KEY");

    sleep_ms(2000);

    // Prepare display
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_write_text("LAST KEY: NONE");

    uint32_t press_count = 0;

    while (true)
    {
        // Read a newly pressed key
        char key = keypad_read();

        if (key != '\0')
        {
            press_count++;

            // Display the pressed key
            lcd_clear();

            lcd_set_cursor(0, 0);
            lcd_write_text("LAST KEY: ");
            lcd_write_char(key);

            // Display total press count
            char count_text[17];

            snprintf(
                count_text,
                sizeof(count_text),
                "COUNT: %lu",
                (unsigned long)press_count
            );

            lcd_set_cursor(1, 0);
            lcd_write_text(count_text);

            // Optional USB serial debugging
            printf(
                "Key: %c | Count: %lu\n",
                key,
                (unsigned long)press_count
            );
        }

        sleep_ms(10);
    }

    return 0;
}
