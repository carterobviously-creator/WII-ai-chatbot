# Wii homebrew Makefile for devkitPPC/libogc

ifneq ($(strip $(DEVKITPPC)),)
include $(DEVKITPPC)/wii_rules
else
$(error "Please set DEVKITPRO and DEVKITPPC in your environment.")
endif

TARGET := app
BUILD := build
SOURCES := source
INCLUDES := source

OUTPUT := $(CURDIR)/$(TARGET)

# Only compile the offline TV chat sources (no network code)
CFILES := main.c ai_brain.c keyboard.c tv_chat.c
OFILES := $(addprefix $(BUILD)/,$(CFILES:.c=.o))

LIBS := -lwiiuse -lbte -logc -lm
INCLUDE := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) -I$(LIBOGC_INC)

CFLAGS := -g -O2 -Wall $(MACHDEP) $(INCLUDE)
LDFLAGS := -g $(MACHDEP) $(LIBPATHS) $(LIBS)

.PHONY: all clean iso

all: $(OUTPUT).dol

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.o: $(SOURCES)/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTPUT).elf: $(OFILES)
	$(CC) $(OFILES) $(LDFLAGS) -o $@

$(OUTPUT).dol: $(OUTPUT).elf
	$(ELF2DOL) $< $@

clean:
	rm -rf $(BUILD) $(OUTPUT).elf $(OUTPUT).dol

iso: all
	@echo "Building plug-n-play ISO..."
	cd $(CURDIR)/.. && bash build_iso.sh
