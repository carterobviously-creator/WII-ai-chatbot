#include "tv_chat.h"

#include "ai_brain.h"
#include "keyboard.h"

#include <gccore.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Chat history — keep last N messages visible on TV */
#define CHAT_HISTORY_MAX 8
#define CHAT_MSG_LEN 80

typedef struct {
    char text[CHAT_MSG_LEN];
    int is_user; /* 1 = user, 0 = AI */
} chat_entry_t;

static chat_entry_t history[CHAT_HISTORY_MAX];
static int history_count = 0;

static void add_to_history(const char* text, int is_user)
{
    /* Shift history up if full */
    if (history_count >= CHAT_HISTORY_MAX) {
        int i;
        for (i = 0; i < CHAT_HISTORY_MAX - 1; i++) {
            history[i] = history[i + 1];
        }
        history_count = CHAT_HISTORY_MAX - 1;
    }

    strncpy(history[history_count].text, text, CHAT_MSG_LEN - 1);
    history[history_count].text[CHAT_MSG_LEN - 1] = '\0';
    history[history_count].is_user = is_user;
    history_count++;
}

static void draw_screen(void)
{
    int i;

    /* Clear screen using ANSI escape */
    printf("\x1b[2J");

    /* Title */
    printf("\x1b[0;0H");
    printf("========= WII-AI CHATBOT =========\n");

    /* Chat history area (after keyboard space) */
    printf("\x1b[15;0H");
    printf("--- Chat ---\n");

    for (i = 0; i < history_count; i++) {
        if (history[i].is_user) {
            printf(" You: %s\n", history[i].text);
        } else {
            printf(" AI:  %s\n", history[i].text);
        }
    }

    /* Draw keyboard in upper area */
    keyboard_draw();
}

void tv_chat_init(void)
{
    history_count = 0;
    keyboard_init();

    /* Welcome message */
    add_to_history("Hello! I'm WII-AI. Type with D-pad + A, send with +", 0);
}

void tv_chat_run(void)
{
    int result;

    draw_screen();

    while (1) {
        VIDEO_WaitVSync();

        result = keyboard_update();

        if (result == -1) {
            /* HOME pressed — exit */
            break;
        }

        if (result == 1) {
            /* Message submitted */
            const char* msg = keyboard_get_message();
            const char* response;

            add_to_history(msg, 1);

            response = ai_brain_respond(msg);
            add_to_history(response, 0);
        }

        /* Redraw every frame */
        draw_screen();
    }
}
