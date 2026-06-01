#include "ai_brain.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    const char* keyword;
    const char* response;
} rule_t;

static const rule_t rules[] = {
    {"hello", "Hey there! I'm WII-AI, running on a Nintendo Wii!"},
    {"hi", "Hi! Your Wii-powered AI is online and ready."},
    {"hey", "Hey! I booted from disc and I'm feeling chatty."},
    {"how are you", "I'm running at 729MHz and feeling great!"},
    {"who are you", "I'm WII-AI, a rule-based chatbot living inside your Nintendo Wii."},
    {"what are you", "I'm a chatbot running directly on Wii hardware with no cloud AI."},
    {"what can you do", "I can chat, answer simple math, and serve this page over Wii WiFi."},
    {"help", "Try: hello, joke, wii facts, math like 2+2, favorite game, or bye."},
    {"nintendo", "Nintendo means playfulness, and your Wii is proving it right now."},
    {"wii", "Fun fact: the Wii's Broadway CPU runs at 729MHz with 88MB total memory."},
    {"gamecube", "The Wii can play GameCube discs on early models. Retro power!"},
    {"motion", "Wii Remote motion controls changed gaming history."},
    {"sensor bar", "The Sensor Bar is actually two IR light clusters for Wiimote tracking."},
    {"mario", "Mario approves of this homebrew experiment. Wah-hoo!"},
    {"zelda", "It's dangerous to go alone—take this Wii chatbot."},
    {"joke", "Why did the Wii cross the network? To serve JSON to your iPad!"},
    {"another joke", "My favorite exercise is Wii-Fit-ing more rules into tiny RAM."},
    {"favorite game", "My favorite game is... running HTTP servers!"},
    {"time", "I don't have a clock, but the Wii launched November 19, 2006!"},
    {"date", "I don't track dates, but Wii history says 2006 was legendary."},
    {"weather", "I can't check weather, but I predict a high chance of gaming."},
    {"cpu", "The Wii CPU is codenamed Broadway, and I'm using it right now."},
    {"gpu", "The Wii GPU is Hollywood—great name for a tiny graphics powerhouse."},
    {"memory", "Wii memory is tight, so I'm compact and rule-based by design."},
    {"wifi", "I'm connected over Wii WiFi so your browser can chat with me."},
    {"disc", "Yep—this app is designed to boot from burned DVD-R on modded Wiis."},
    {"dvd", "DVD-R homebrew is possible with the right softmod or modchip setup."},
    {"boot", "Once booted, I host a chat server right from your Wii."},
    {"thanks", "You're welcome! Powered by vintage hardware and pure determination."},
    {"thank you", "Anytime! Wii-AI likes helpful conversations."},
    {"bye", "Goodbye! Press HOME on the Wii Remote when you want to exit."},
    {"goodbye", "See you next session—Wii-AI signing off."},
    {"see you", "See you later! Keep your Wii online and curious."},
    {"creator", "Built for carterobviously-creator with Wii homebrew style."},
    {"ipad", "Your iPad is the perfect chat terminal for this Wii server."},
    {"ai", "I'm a lightweight rule-based AI tuned for Wii-era hardware."},
};

static int contains_keyword(const char* haystack, const char* needle)
{
    return strstr(haystack, needle) != NULL;
}

const char* ai_brain_respond(const char* input)
{
    static char response[128];
    char lowered[256];
    size_t i;

    if (!input || !*input) {
        return "Say something and I'll reply from inside this Wii.";
    }

    for (i = 0; input[i] != '\0' && i < sizeof(lowered) - 1; ++i) {
        lowered[i] = (char)tolower((unsigned char)input[i]);
    }
    lowered[i] = '\0';

    {
        int a = 0;
        int b = 0;
        char op = 0;
        if ((sscanf(lowered, "what is %d %c %d", &a, &op, &b) == 3) ||
            (sscanf(lowered, "%d %c %d", &a, &op, &b) == 3)) {
            switch (op) {
                case '+':
                    snprintf(response, sizeof(response), "%d + %d = %d", a, b, a + b);
                    return response;
                case '-':
                    snprintf(response, sizeof(response), "%d - %d = %d", a, b, a - b);
                    return response;
                case '*':
                case 'x':
                    snprintf(response, sizeof(response), "%d * %d = %d", a, b, a * b);
                    return response;
                case '/':
                    if (b == 0) {
                        return "Division by zero? Even Wii hardware draws the line there.";
                    }
                    snprintf(response, sizeof(response), "%d / %d = %d", a, b, a / b);
                    return response;
                default:
                    break;
            }
        }
    }

    for (i = 0; i < sizeof(rules) / sizeof(rules[0]); ++i) {
        if (contains_keyword(lowered, rules[i].keyword)) {
            return rules[i].response;
        }
    }

    return "Hmm, I'm just a Wii, but that's an interesting thought!";
}
