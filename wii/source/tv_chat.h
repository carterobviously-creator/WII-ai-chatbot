#ifndef TV_CHAT_H
#define TV_CHAT_H

/* TV-based chat interface.
 * Displays chat history on the TV and accepts input via on-screen keyboard.
 * No network required — everything runs locally on the Wii.
 */

/* Initialize the TV chat system */
void tv_chat_init(void);

/* Run the main chat loop. Returns when HOME is pressed. */
void tv_chat_run(void);

#endif
