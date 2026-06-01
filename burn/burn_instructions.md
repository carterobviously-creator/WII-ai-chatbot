# Burning and Booting WII-AI Chatbot from DVD-R

## Plug-n-Play (Recommended)

**One command does everything:**

```bash
./build_iso.sh
```

This will:
1. Build the Wii app (`app.dol`)
2. Generate all required disc metadata
3. Package everything into a burnable Wii ISO

Output: `wii-ai-chatbot.iso` — ready to burn.

### Burn to DVD-R

| OS | Tool | Steps |
|----|------|-------|
| Windows | ImgBurn | Write image → select ISO → speed 2x–4x → burn |
| Linux | Brasero | Burn Image → select ISO → speed 2x–4x → burn |
| macOS | Disk Utility or `hdiutil burn wii-ai-chatbot.iso` | |

### Boot on Wii

1. Insert burned DVD-R into softmodded Wii
2. Launch via **Priiloader** autoboot or **backup disc loader**
3. The chatbot appears on your TV immediately
4. Use the Wii Remote to chat:
   - **D-pad**: Navigate the on-screen keyboard
   - **A**: Type the selected letter
   - **B**: Backspace
   - **+**: Send message
   - **HOME**: Exit

**No WiFi. No internet. No phone. Just the disc and your TV.**

---

## Manual Steps (Advanced)

> Only needed if you want to customize the disc layout.

### Requirements
- Softmodded Nintendo Wii with **Homebrew Channel** and **BootMii** installed
- Backup-disc capable setup: **Priiloader + backup launcher** or compatible **modchip**
- Blank **DVD-R** disc (avoid DVD+R when possible)
- PC for building + burning
- Burning software: **ImgBurn** (Windows) or **Brasero** (Linux)
- Wiimms ISO Tools (`wit`)

### Build the `.dol`

```bash
cd wii
make
```

### Create ISO manually

```bash
mkdir -p /tmp/wii-ai-disc/sys
cp wii/app.dol /tmp/wii-ai-disc/sys/main.dol
# Add boot.bin, bi2.bin, apploader.img to /tmp/wii-ai-disc/sys/
wit copy /tmp/wii-ai-disc wii-ai-chatbot.iso
```

### Burn and boot

Same as the plug-n-play section above.

---

## Troubleshooting

- **Disc does not boot**: Stock Wii cannot boot burned discs without softmod/modchip + compatible loader.
- **No picture**: Check TV input/video cable. Try composite if component doesn't work.
- **Controls unresponsive**: Re-sync the Wii Remote (press SYNC on Wii + Remote).
