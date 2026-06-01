# WII-AI Chatbot

WII-AI Chatbot is a Nintendo Wii homebrew application that runs a lightweight rule-based chatbot directly on your TV. **No WiFi, no internet, no phone required** — just burn the disc and go.

## What this project does
- Boots as a Wii homebrew app (`app.dol`) from a burned DVD-R
- Displays a chat interface directly on your TV
- Uses an on-screen keyboard controlled by the Wii Remote D-pad
- AI chatbot responds instantly — everything runs locally on the Wii CPU
- **Plug and play**: burn disc → insert → boot → chat

## How it works
1. Burn the ISO to a DVD-R
2. Put the disc in your softmodded Wii
3. Boot it
4. Chat on your TV using the Wii Remote:
   - **D-pad**: Move cursor on the on-screen keyboard
   - **A button**: Type the highlighted letter
   - **B button**: Backspace/delete
   - **+ button**: Send your message
   - **HOME**: Exit

## Hardware requirements
- Nintendo Wii with softmod / modchip for backup disc launching
- DVD-R media for burned disc
- Wii Remote
- TV (any TV the Wii supports)

## Software requirements (for building)
- [devkitPro + devkitPPC + libogc](https://devkitpro.org/)
- Wiimms ISO Tools (`wit`)
- ImgBurn (Windows) or Brasero (Linux) for disc burning

## Project layout

```
WII-ai-chatbot/
├── build_iso.sh          ← ONE-COMMAND plug-n-play builder
├── wii/
│   ├── source/
│   │   ├── main.c           ← Entry point (no network)
│   │   ├── tv_chat.c        ← TV chat display loop
│   │   ├── tv_chat.h
│   │   ├── keyboard.c       ← On-screen keyboard (D-pad)
│   │   ├── keyboard.h
│   │   ├── ai_brain.c       ← Rule-based AI engine
│   │   └── ai_brain.h
│   ├── Makefile
│   └── meta.xml
├── burn/
│   └── burn_instructions.md
└── README.md
```

## Build instructions
1. Install devkitPro and Wii packages.
2. Set environment:
   - `DEVKITPRO`
   - `DEVKITPPC`
3. Build:

```bash
cd wii
make
```

Output: `wii/app.dol`

## Plug-n-Play: Build & Burn (one command)

The easiest way to go from source to a bootable disc:

```bash
./build_iso.sh
```

This single script:
1. Builds the Wii app
2. Generates all required disc metadata (boot.bin, bi2.bin, apploader)
3. Packages into a ready-to-burn Wii ISO

Then just burn `wii-ai-chatbot.iso` to a DVD-R at 2x–4x speed and boot on your softmodded Wii.

See [burn/burn_instructions.md](burn/burn_instructions.md) for full details.

## Burn to disc (manual)
See [burn/burn_instructions.md](burn/burn_instructions.md) for the manual workflow:
- Build `app.dol`
- Create Wii ISO with `wit`
- Burn to DVD-R at low speed (2x–4x)
- Launch with Priiloader / backup launcher

## Boot and use
1. Burn the ISO to DVD-R (see build instructions above)
2. Insert disc into softmodded Wii
3. Boot via Priiloader or backup disc launcher
4. Use D-pad to navigate keyboard, A to type, + to send
5. Chat with the AI on your TV!
6. Press HOME to exit.

## Troubleshooting
- **Disc does not boot**: Stock Wii cannot boot burned discs without softmod/modchip + compatible loader.
- **No picture**: Check TV input and video cable connection.
- **Controls not working**: Make sure Wii Remote is synced (press SYNC button on both Wii and Remote).

## Expanding the AI
Edit `wii/source/ai_brain.c` and add new keyword/response rules in the `rules[]` table, or expand parsing logic for new commands.

## Technical notes
- The Wii has limited RAM/CPU compared to modern ML workloads, so this project uses a fast rule-based chatbot design.
- All logic runs directly on the Wii CPU — no cloud, no internet, no network required.
- On-screen keyboard uses D-pad navigation for simplicity and reliability.
- USB keyboard support could be added via libogc USB HID in the future.
