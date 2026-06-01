#include "keyboard.h"

#include <stdio.h>
#include <string.h>
#include <wiiuse/wpad.h>

#define INPUT_MAX 128

/* Keyboard layout: 4 rows x 10 columns */
static const char kb_layout[KB_ROWS][KB_COLS] = {
    {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'},
    {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p'},
    {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', '?'},
    {'z', 'x', 'c', 'v', 'b', 'n', 'm', ' ', '.', '!'},
};

static int cursor_row = 1;
static int cursor_col = 0;
static char input_buf[INPUT_MAX];
static int input_len = 0;
static char message_buf[INPUT_MAX];

void keyboard_init(void)
{
    cursor_row = 1;
    cursor_col = 0;
    input_buf[0] = '\0';
    input_len = 0;
    message_buf[0] = '\0';
}

void keyboard_draw(void)
{
    int r, c;

    /* Draw input line */
    printf("\x1b[2;0H"); /* row 2 (below title) */
    printf("+-----------------------------------------+\n");
    printf("| > %-37.37s |\n", input_buf);
    printf("+-----------------------------------------+\n");

    /* Draw keyboard grid */
    printf("\n  Use D-pad to move, A=type, B=delete, +=send\n\n");

    for (r = 0; r < KB_ROWS; r++) {
        printf("   ");
        for (c = 0; c < KB_COLS; c++) {
            char ch = kb_layout[r][c];
            if (r == cursor_row && c == cursor_col) {
                /* Highlight selected key */
                if (ch == ' ') {
                    printf("[_] ");
                } else {
                    printf("[%c] ", ch);
                }
            } else {
                if (ch == ' ') {
                    printf(" _  ");
                } else {
                    printf(" %c  ", ch);
                }
            }
        }
        printf("\n");
    }

    printf("\n");
}

int keyboard_update(void)
{
    u32 pressed;

    WPAD_ScanPads();
    pressed = WPAD_ButtonsDown(0);

    if (pressed & WPAD_BUTTON_HOME) {
        return -1;
    }

    /* Navigation */
    if (pressed & WPAD_BUTTON_RIGHT) {
        cursor_col = (cursor_col + 1) % KB_COLS;
    }
    if (pressed & WPAD_BUTTON_LEFT) {
        cursor_col = (cursor_col - 1 + KB_COLS) % KB_COLS;
    }
    if (pressed & WPAD_BUTTON_DOWN) {
        cursor_row = (cursor_row + 1) % KB_ROWS;
    }
    if (pressed & WPAD_BUTTON_UP) {
        cursor_row = (cursor_row - 1 + KB_ROWS) % KB_ROWS;
    }

    /* Type character (A button) */
    if (pressed & WPAD_BUTTON_A) {
        if (input_len < INPUT_MAX - 1) {
            input_buf[input_len++] = kb_layout[cursor_row][cursor_col];
            input_buf[input_len] = '\0';
        }
    }

    /* Backspace (B button) */
    if (pressed & WPAD_BUTTON_B) {
        if (input_len > 0) {
            input_buf[--input_len] = '\0';
        }
    }

    /* Send message (+ button) */
    if (pressed & WPAD_BUTTON_PLUS) {
        if (input_len > 0) {
            strncpy(message_buf, input_buf, INPUT_MAX - 1);
            message_buf[INPUT_MAX - 1] = '\0';
            input_buf[0] = '\0';
            input_len = 0;
            return 1;
        }
    }

    return 0;
}

const char* keyboard_get_input(void)
{
    return input_buf;
}

const char* keyboard_get_message(void)
{
    return message_buf;
}

void keyboard_clear(void)
{
    input_buf[0] = '\0';
    input_len = 0;
}
