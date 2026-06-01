# WII-AI Chatbot

WII-AI Chatbot is a Nintendo Wii homebrew application that runs a lightweight rule-based chatbot engine locally on Wii hardware and exposes a browser chat UI over WiFi.

## What this project does
- Boots as a Wii homebrew app (`app.dol`)
- Initializes Wii WiFi and prints the Wii IP on TV
- Runs an HTTP server on port 80
- Serves a touch-friendly iPad chat UI at `/`
- Handles `/chat?msg=...` and replies from a rule-based AI brain in C

## Hardware requirements
- Nintendo Wii with softmod / modchip support for backup disc launching
- DVD-R media for burned disc workflow
- iPad or phone with modern browser (Safari recommended)
- WiFi router (Wii + iPad on same network)

## Software requirements
- [devkitPro + devkitPPC + libogc](https://devkitpro.org/)
- Wiimms ISO Tools (`wit`)
- ImgBurn (Windows) or Brasero (Linux) for disc burning

## Project layout

```
WII-ai-chatbot/
├── wii/
│   ├── source/
│   │   ├── main.c
│   │   ├── http_server.c
│   │   ├── http_server.h
│   │   ├── ai_brain.c
│   │   ├── ai_brain.h
│   │   └── chat_page.h
│   ├── Makefile
│   └── meta.xml
├── ipad/
│   └── index.html
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

## Burn to disc
See [burn/burn_instructions.md](burn/burn_instructions.md) for full workflow:
- Build `app.dol`
- Create Wii ISO with `wit`
- Burn to DVD-R at low speed (2x–4x)
- Launch with Priiloader / backup launcher

## Boot and use
1. Boot the app from your homebrew/backup-disc launcher.
2. Wait for TV output showing the Wii IP address.
3. Open `http://<wii-ip>/` on iPad Safari.
4. Start chatting.
5. Press Wii Remote HOME to exit.

## Troubleshooting
- **WiFi not connecting**: Reconfigure Wii internet settings and test connection in Wii System Settings.
- **IP not showing**: Ensure `if_config()` succeeds and Wii is in range of router.
- **Browser cannot reach Wii**: Confirm iPad and Wii are on same subnet, and router isolation is disabled.
- **Disc does not boot**: Stock Wii cannot boot burned discs without softmod/modchip + compatible loader.

## Expanding the AI
Edit `wii/source/ai_brain.c` and add new keyword/response rules in the `rules[]` table, or expand parsing logic for new commands.

## Technical notes
- The Wii has limited RAM/CPU compared to modern ML workloads, so this project uses a fast rule-based chatbot design.
- HTTP is intentionally simple (HTTP/1.0, close per request) for compatibility and memory safety.
- All server/AI logic runs directly on the Wii CPU—no cloud model required.
