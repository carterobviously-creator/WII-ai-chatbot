# WII-AI Chatbot

A Nintendo Wii homebrew chatbot that runs directly on your TV. **No WiFi required.**

## Homebrew Channel setup (stock Wii)

A normal (unmodded) Wii cannot boot burned discs. Use the Homebrew Channel (HBC), which can be installed for free with common exploits:

- **str2hax**: https://wiibrew.org/wiki/Str2hax
- **Bluebomb**: https://wiibrew.org/wiki/Bluebomb

Follow one of those guides first to install HBC on your Wii.

## How to use

1. Download `wii-ai-chatbot-sd.zip` from the [Releases page](../../releases/latest)
2. Extract it to your SD card root (so you get `SD:/apps/wii-ai-chatbot/boot.dol`)
3. Insert the SD card into your Wii
4. Open the Homebrew Channel
5. Launch **WII-AI Chatbot**

## Controls (Wii Remote)

| Button | Action |
|--------|--------|
| D-pad | Navigate on-screen keyboard |
| A | Type the highlighted letter |
| B | Backspace / delete |
| + | Send your message |
| HOME | Exit |

## What this does

- Shows a chat interface on your TV
- You type with the on-screen keyboard using the Wii Remote D-pad
- AI responds instantly — everything runs on the Wii CPU
- Rule-based chatbot with jokes, math, Wii facts, and more

## Expanding the AI

Edit `wii/source/ai_brain.c` and add new keyword/response rules in the `rules[]` table. Push to main and GitHub Actions will auto-build a new Homebrew Channel package in Releases.

## Project layout

```
WII-ai-chatbot/
├── .github/workflows/    ← Auto-builds HBC SD package
├── wii/
│   ├── source/
│   │   ├── main.c       ← Entry point
│   │   ├── tv_chat.c    ← TV chat display
│   │   ├── keyboard.c   ← On-screen keyboard
│   │   ├── ai_brain.c   ← AI response engine
│   │   └── *.h          ← Headers
│   ├── Makefile
│   └── meta.xml
└── README.md
```

## Troubleshooting

- **Homebrew Channel not installed**: Install HBC first using str2hax or Bluebomb (links above).
- **App does not appear in HBC**: Confirm `SD:/apps/wii-ai-chatbot/boot.dol` and `SD:/apps/wii-ai-chatbot/meta.xml` exist.
- **No picture**: Check TV input and video cable.
- **Controls not working**: Re-sync Wii Remote (press SYNC on both Wii and Remote).
- **Package not in Releases**: Push to `main` branch or create a tag like `v1.0` to trigger a build.

## Technical notes

- Rule-based AI designed for Wii's limited 88MB RAM / 729MHz CPU
- All logic runs on the Wii — zero network, zero cloud
- GitHub Actions compiles and packages an SD-card-ready Homebrew Channel zip on every push
