/**
 * @file    main.c
 * @brief   CSEN701 - Project Milestone 1
 *          Two-button LED control on a Raspberry Pi Pico (RP2040).
 *
 * Behaviour
 * ---------
 *   - Button A (POSITIVE logic, active-high) turns the LED ON.
 *   - Button B (NEGATIVE logic, active-low)  turns the LED OFF.
 *   - If both buttons are held at the same time, OFF wins (safe default).
 *
 * Wiring (numbers in brackets are physical header pins)
 * -----------------------------------------------------
 *   Button A : 3V3(OUT) [36] <--> GP28 [34]   internal pull-DOWN -> pressed = 1
 *   Button B : GND      [33] <--> GP26 [31]   internal pull-UP   -> pressed = 0
 *   LED      : GP22 [29] --> 220 ohm resistor --> LED anode (+)
 *              LED cathode (-) --> GND [28]
 *
 * Program structure (input - processing - output loop)
 * ----------------------------------------------------
 *   1. INPUT      : read the two button pins.
 *   2. PROCESSING : decide the new LED state from the button states.
 *   3. OUTPUT     : drive the LED pin.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "keypad.h"
/* ------------------------------------------------------------------------- */
/* Pin assignment (GPIO numbers, NOT physical pin numbers)                   */
/* ------------------------------------------------------------------------- */
#define BTN_ON_PIN    28u   /* Button A: turns the LED on  (positive logic) */
#define BTN_OFF_PIN   26u   /* Button B: turns the LED off (negative logic) */
#define LED_PIN       22u   /* External LED (through a series resistor)     */

/* ------------------------------------------------------------------------- */
/* Logic levels                                                              */
/* ------------------------------------------------------------------------- */
/* Level read on the pin while the button is PRESSED. */
#define BTN_ON_PRESSED_LEVEL    1   /* positive logic: pressed = HIGH */
#define BTN_OFF_PRESSED_LEVEL   0   /* negative logic: pressed = LOW  */

/* Level written to the LED pin. */
#define LED_ON_LEVEL    1           /* GPIO HIGH lights the LED */
#define LED_OFF_LEVEL   0           /* GPIO LOW  turns it off   */

/* ------------------------------------------------------------------------- */
/* Timing                                                                    */
/* ------------------------------------------------------------------------- */
/* Polling period. Also acts as a simple debounce: contact bounce lasts a few
 * milliseconds, and since setting the LED is idempotent (writing the same
 * level twice changes nothing) any bounce is harmless.                       */
#define POLL_PERIOD_MS  10u

/* ------------------------------------------------------------------------- */
/* Function prototypes                                                       */
/* ------------------------------------------------------------------------- */
static void gpio_setup(void);
static bool is_btn_on_pressed(void);
static bool is_btn_off_pressed(void);
static void led_set(bool turn_on);

/* ------------------------------------------------------------------------- */
/* Main                                                                      */
/* ------------------------------------------------------------------------- */
int main(void)
{
    gpio_setup();
const unsigned int row_pins[4] = {2, 3, 4, 5};  // R1–R4
const unsigned int col_pins[4] = {6, 7, 8, 9};  // C1–C4
keypad_init(row_pins, col_pins);
    while (true)
    {
        /* 1. INPUT: sample both buttons. */
        const bool btn_on_pressed  = is_btn_on_pressed();
        const bool btn_off_pressed = is_btn_off_pressed();

        /* 2. PROCESSING + 3. OUTPUT:
         * OFF is checked first so it takes priority if both are pressed.
         * If neither is pressed, the LED simply keeps its last state.       */
        if (btn_off_pressed)
        {
            led_set(false);
        }
        else if (btn_on_pressed)
        {
            led_set(true);
        }
        else
        {
            /* No button pressed: do nothing, LED holds its state. */
        }
char key = keypad_read();
if (key != '\0')
{
    printf("Key pressed: %c\n", key);
}
        sleep_ms(POLL_PERIOD_MS);
    }

    return 0; /* Never reached; kept for standards compliance. */
}

/* ------------------------------------------------------------------------- */
/* Function definitions                                                      */
/* ------------------------------------------------------------------------- */

/**
 * @brief Configure all GPIO pins used by this program.
 *
 * - Button A pin: input with internal pull-DOWN  (idle = 0, pressed = 1)
 * - Button B pin: input with internal pull-UP    (idle = 1, pressed = 0)
 * - LED pin     : output, initially OFF
 */
static void gpio_setup(void)
{
    /* Button A: positive logic. */
    gpio_init(BTN_ON_PIN);
    gpio_set_dir(BTN_ON_PIN, GPIO_IN);
    gpio_pull_down(BTN_ON_PIN);

    /* Button B: negative logic. */
    gpio_init(BTN_OFF_PIN);
    gpio_set_dir(BTN_OFF_PIN, GPIO_IN);
    gpio_pull_up(BTN_OFF_PIN);

    /* LED: output, start in a known (off) state. */
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, LED_OFF_LEVEL);
}

/**
 * @brief  Check Button A (positive logic).
 * @return true while the button is pressed (pin reads HIGH).
 */
static bool is_btn_on_pressed(void)
{
    return (gpio_get(BTN_ON_PIN) == BTN_ON_PRESSED_LEVEL);
}

/**
 * @brief  Check Button B (negative logic).
 * @return true while the button is pressed (pin reads LOW).
 */
static bool is_btn_off_pressed(void)
{
    return (gpio_get(BTN_OFF_PIN) == BTN_OFF_PRESSED_LEVEL);
}

/**
 * @brief Drive the LED.
 * @param turn_on true to light the LED, false to turn it off.
 */
static void led_set(bool turn_on)
{
    gpio_put(LED_PIN, turn_on ? LED_ON_LEVEL : LED_OFF_LEVEL);
}
