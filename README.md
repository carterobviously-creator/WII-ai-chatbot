# WII-AI Chatbot

A Nintendo Wii homebrew chatbot that runs directly on your TV. **No WiFi, no internet, no building, no commands.** Just download the ISO, burn it to a DVD-R, and play.

## How to use (Windows)

1. **Download** `wii-ai-chatbot.iso` from the [Releases page](../../releases/latest)
2. **Open ImgBurn** (free: [imgburn.com](https://www.imgburn.com/))
3. **Select** "Write image file to disc"
4. **Choose** the downloaded `wii-ai-chatbot.iso`
5. **Set write speed** to 2x–4x
6. **Insert** a blank DVD-R and burn
7. **Put the disc** in your softmodded Wii
8. **Boot** via Priiloader or backup disc loader
9. **Chat!** The AI appears on your TV immediately

## Controls (Wii Remote)

| Button | Action |
|--------|--------|
| D-pad | Navigate on-screen keyboard |
| A | Type the highlighted letter |
| B | Backspace / delete |
| + | Send your message |
| HOME | Exit |

## What you need

- Nintendo Wii with softmod or modchip (to boot burned discs)
- Blank DVD-R disc
- Wii Remote
- TV
- [ImgBurn](https://www.imgburn.com/) on Windows (to burn the ISO)

## What this does

- Boots from the burned disc — no setup, no WiFi
- Shows a chat interface on your TV
- You type with the on-screen keyboard using the Wii Remote D-pad
- AI responds instantly — everything runs on the Wii CPU
- Rule-based chatbot with jokes, math, Wii facts, and more

## Expanding the AI

Edit `wii/source/ai_brain.c` and add new keyword/response rules in the `rules[]` table. Push to main and GitHub Actions will auto-build a new ISO in Releases.

## Project layout

```
WII-ai-chatbot/
├── .github/workflows/    ← Auto-builds the ISO (you never run this)
├── wii/
│   ├── source/
│   │   ├── main.c       ← Entry point
│   │   ├── tv_chat.c    ← TV chat display
│   │   ├── keyboard.c   ← On-screen keyboard
│   │   ├── ai_brain.c   ← AI response engine
│   │   └── *.h          ← Headers
│   ├── Makefile
│   └── meta.xml
├── burn/
│   └── burn_instructions.md
└── README.md
```

## Troubleshooting

- **Disc does not boot**: Stock Wii cannot boot burned discs — you need a softmod or modchip + backup loader.
- **No picture**: Check TV input and video cable.
- **Controls not working**: Re-sync Wii Remote (press SYNC on both Wii and Remote).
- **ISO not in Releases**: Push code to `main` branch or create a tag like `v1.0` to trigger a build.

## Technical notes

- Rule-based AI designed for Wii's limited 88MB RAM / 729MHz CPU
- All logic runs on the Wii — zero network, zero cloud
- GitHub Actions automatically compiles and packages the ISO on every push
- You never need to install any dev tools — just download and burn
