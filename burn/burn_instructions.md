# Burning and Booting WII-AI Chatbot from DVD-R

## 1) Requirements
- Softmodded Nintendo Wii with **Homebrew Channel** and **BootMii** installed
- Backup-disc capable setup: **Priiloader + backup launcher** or compatible **modchip**
- Blank **DVD-R** disc (avoid DVD+R when possible)
- PC for building + burning
- Burning software: **ImgBurn** (Windows) or **Brasero** (Linux)
- Wiimms ISO Tools (`wit`)

> Important: a stock unmodified Wii cannot boot burned backup discs directly.

## 2) Build the `.dol`
From the repository root:

```bash
cd wii
make
```

This creates `wii/app.dol`.

## 3) Create a bootable Wii ISO with `wit`
Create a temporary disc layout:

```bash
mkdir -p /tmp/wii-ai-disc/sys
cp wii/app.dol /tmp/wii-ai-disc/sys/main.dol
```

Add valid disc metadata files (`boot.bin`, `bi2.bin`, `apploader.img`) from a legal homebrew-compatible disc template, then build ISO:

```bash
wit copy /tmp/wii-ai-disc wii-ai-chatbot.iso --dest .
```

Optional verify:

```bash
wit verify wii-ai-chatbot.iso
```

## 4) Burn ISO to DVD-R (ImgBurn)
1. Open **ImgBurn** → *Write image file to disc*
2. Source: `wii-ai-chatbot.iso`
3. Speed: **2x to 4x**
4. Enable verify after burn
5. Start burn

## 5) Boot on softmodded Wii
- Insert burned DVD-R
- Launch through **Priiloader** autoboot path or your installed **backup disc launcher**
- If your setup supports it, the DOL boots and starts the chat server

## 6) Network setup
Before launching, configure Wii WiFi in:
- Wii Options → Wii Settings → Internet

## 7) Connect from iPad
- Read the Wii IP printed on TV (example `192.168.1.45`)
- On iPad Safari open: `http://<wii-ip>/`
- Chat with Wii AI from the browser page
