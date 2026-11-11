#include "keypad.h"
#include "pico/stdlib.h"

#define DEBOUNCE_MS 25
#define MULTIPLE_KEYS '\x01'

static unsigned int rows[4];
static unsigned int cols[4];

static char candidate_key;
static uint32_t candidate_since_ms;
static bool waiting_for_release;

static const char keymap[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

void keypad_init(const unsigned int row_pins[4],
                 const unsigned int col_pins[4])
{
    for (int i = 0; i < 4; i++)
    {
        rows[i] = row_pins[i];
        cols[i] = col_pins[i];

        gpio_init(rows[i]);
        gpio_set_dir(rows[i], GPIO_IN);
        gpio_put(rows[i], 0);  // Drive low when this row is selected.

        gpio_init(cols[i]);
        gpio_set_dir(cols[i], GPIO_IN);
        gpio_pull_up(cols[i]);
    }

    candidate_key = '\0';
    candidate_since_ms = to_ms_since_boot(get_absolute_time());
    waiting_for_release = false;
}

static char scan_keypad(void)
{
    char detected = '\0';

    for (int row = 0; row < 4; row++)
    {
        gpio_set_dir(rows[row], GPIO_OUT);
        sleep_us(5);

        for (int col = 0; col < 4; col++)
        {
            if (!gpio_get(cols[col]))
            {
                detected = detected == '\0' ? keymap[row][col] : MULTIPLE_KEYS;
            }
        }

        gpio_set_dir(rows[row], GPIO_IN);
    }

    return detected;
}

char keypad_read(void)
{
    char raw_key = scan_keypad();
    uint32_t now = to_ms_since_boot(get_absolute_time());

    if (raw_key != candidate_key)
    {
        candidate_key = raw_key;
        candidate_since_ms = now;
        return '\0';
    }

    if (now - candidate_since_ms < DEBOUNCE_MS)
        return '\0';

    if (candidate_key == '\0')
    {
        waiting_for_release = false;
        return '\0';
    }

    if (candidate_key == MULTIPLE_KEYS || waiting_for_release)
        return '\0';

    waiting_for_release = true;
    return candidate_key;
}