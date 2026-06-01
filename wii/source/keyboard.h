#ifndef KEYBOARD_H
#define KEYBOARD_H

/* On-screen keyboard for Wii Remote D-pad input.
 * The keyboard is a grid of characters. The user navigates with D-pad
 * and selects characters with A. B = backspace, + = send message.
 */

#define KB_COLS 10
#define KB_ROWS 4

/* Initialize keyboard state */
void keyboard_init(void);

/* Draw the on-screen keyboard to the console at the current cursor position */
void keyboard_draw(void);

/* Process Wii Remote input. Returns:
 *  0 = normal (character added or navigation)
 *  1 = message submitted (call keyboard_get_message)
 * -1 = HOME pressed (exit)
 */
int keyboard_update(void);

/* Get the current input buffer contents */
const char* keyboard_get_input(void);

/* Get the submitted message and clear input buffer */
const char* keyboard_get_message(void);

/* Clear the input buffer */
void keyboard_clear(void);

#endif
