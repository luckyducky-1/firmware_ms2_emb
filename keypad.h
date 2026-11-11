#ifndef KEYPAD_H
#define KEYPAD_H

// Set up the four row pins and four column pins.
void keypad_init(const unsigned int row_pins[4],
                 const unsigned int col_pins[4]);

// Return a newly pressed key, or '\0' if there is no new press.
char keypad_read(void);

#endif