#!/bin/bash
# ============================================================
# WII-AI Chatbot — Plug-n-Play ISO Builder
# ============================================================
# One command to build the app and create a burnable Wii ISO.
# Burn the output ISO to a DVD-R and boot on a softmodded Wii.
#
# Usage:
#   ./build_iso.sh
#
# Output:
#   wii-ai-chatbot.iso  (ready to burn)
# ============================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ISO_NAME="wii-ai-chatbot.iso"
DISC_DIR="/tmp/wii-ai-disc-$$"
GAME_ID="WAIC01"
GAME_TITLE="WII-AI Chatbot"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

info()  { echo -e "${GREEN}[INFO]${NC} $1"; }
warn()  { echo -e "${YELLOW}[WARN]${NC} $1"; }
error() { echo -e "${RED}[ERROR]${NC} $1"; exit 1; }

# -----------------------------------------------------------
# Step 0: Check prerequisites
# -----------------------------------------------------------
info "Checking prerequisites..."

if [ -z "${DEVKITPRO:-}" ]; then
    error "DEVKITPRO is not set. Install devkitPro: https://devkitpro.org/wiki/Getting_Started"
fi

if [ -z "${DEVKITPPC:-}" ]; then
    export DEVKITPPC="${DEVKITPRO}/devkitPPC"
    warn "DEVKITPPC not set, defaulting to ${DEVKITPPC}"
fi

if ! command -v wit &>/dev/null; then
    error "Wiimms ISO Tools (wit) not found. Install from: https://wit.wiimm.de/"
fi

info "All prerequisites found."

# -----------------------------------------------------------
# Step 1: Build the .dol
# -----------------------------------------------------------
info "Building app.dol..."
cd "${SCRIPT_DIR}/wii"
make clean 2>/dev/null || true
make
if [ ! -f "app.dol" ]; then
    error "Build failed — app.dol not created."
fi
info "app.dol built successfully."

# -----------------------------------------------------------
# Step 2: Create disc layout
# -----------------------------------------------------------
info "Creating Wii disc layout..."
rm -rf "${DISC_DIR}"
mkdir -p "${DISC_DIR}/sys"
mkdir -p "${DISC_DIR}/files"

# Copy the main executable
cp "${SCRIPT_DIR}/wii/app.dol" "${DISC_DIR}/sys/main.dol"

# Generate boot.bin (disc header — 0x440 bytes)
# Wii disc header format:
#   0x000: Game ID (6 bytes)
#   0x018: Magic word 0x5D1C9EA3 (Wii disc)
#   0x020: Game title (64 bytes, null-padded)
python3 - "${DISC_DIR}/sys/boot.bin" "${GAME_ID}" "${GAME_TITLE}" << 'PYEOF'
import sys, struct

outpath = sys.argv[1]
game_id = sys.argv[2].encode('ascii')
title = sys.argv[3].encode('ascii')

header = bytearray(0x440)

# Game ID at offset 0 (6 bytes)
header[0:len(game_id)] = game_id

# Disc number and version
header[0x06] = 0  # disc number
header[0x07] = 0  # revision

# Wii magic word at offset 0x18
struct.pack_into('>I', header, 0x18, 0x5D1C9EA3)

# Game title at offset 0x20 (64 bytes max)
title_trunc = title[:64]
header[0x20:0x20+len(title_trunc)] = title_trunc

print(f"Generated boot.bin ({len(header)} bytes)")
with open(outpath, 'wb') as f:
    f.write(header)
PYEOF

# Generate bi2.bin (boot info 2 — 0x2000 bytes)
python3 - "${DISC_DIR}/sys/bi2.bin" << 'PYEOF'
import sys, struct

outpath = sys.argv[1]
bi2 = bytearray(0x2000)

# Country code at offset 0x18 (1 = USA)
struct.pack_into('>I', bi2, 0x18, 1)

print(f"Generated bi2.bin ({len(bi2)} bytes)")
with open(outpath, 'wb') as f:
    f.write(bi2)
PYEOF

# Generate minimal apploader.img (required by disc format)
# For homebrew backup discs launched via backup loaders, the apploader
# is typically bypassed — but we need a valid placeholder.
python3 - "${DISC_DIR}/sys/apploader.img" << 'PYEOF'
import sys

outpath = sys.argv[1]
# Minimal apploader stub — 32 bytes header + minimal code
# Backup loaders skip this entirely; it's just for disc format validity.
apploader = bytearray(0x20)
# Date string (10 bytes at offset 0)
date = b'2026/06/01'
apploader[0:len(date)] = date

print(f"Generated apploader.img ({len(apploader)} bytes)")
with open(outpath, 'wb') as f:
    f.write(apploader)
PYEOF

info "Disc layout created."

# -----------------------------------------------------------
# Step 3: Build ISO with wit
# -----------------------------------------------------------
info "Building Wii ISO image..."
cd "${SCRIPT_DIR}"
rm -f "${ISO_NAME}"

wit copy "${DISC_DIR}" "${ISO_NAME}" --id "${GAME_ID}" --name "${GAME_TITLE}" 2>&1 | while IFS= read -r line; do
    echo "  wit: $line"
done

if [ ! -f "${ISO_NAME}" ]; then
    error "ISO creation failed."
fi

ISO_SIZE=$(du -h "${ISO_NAME}" | cut -f1)
info "ISO created: ${ISO_NAME} (${ISO_SIZE})"

# -----------------------------------------------------------
# Step 4: Cleanup
# -----------------------------------------------------------
rm -rf "${DISC_DIR}"

# -----------------------------------------------------------
# Done!
# -----------------------------------------------------------
echo ""
echo "============================================================"
echo -e "${GREEN} ✅  READY TO BURN${NC}"
echo "============================================================"
echo ""
echo "  Output file:  ${SCRIPT_DIR}/${ISO_NAME}"
echo "  Size:         ${ISO_SIZE}"
echo ""
echo "  BURN INSTRUCTIONS:"
echo "  1. Insert blank DVD-R into your PC drive"
echo "  2. Open ImgBurn (Windows) or Brasero (Linux)"
echo "  3. Select 'Write image file to disc'"
echo "  4. Choose: ${ISO_NAME}"
echo "  5. Set write speed to 2x–4x"
echo "  6. Burn and verify"
echo ""
echo "  BOOT ON WII:"
echo "  1. Insert burned disc into softmodded Wii"
echo "  2. Launch via Priiloader or backup disc loader"
echo "  3. Read the IP address shown on TV"
echo "  4. On iPad/phone, open: http://<wii-ip>/"
echo "  5. Start chatting!"
echo ""
echo "============================================================"
