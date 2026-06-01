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

CFILES := $(notdir $(wildcard $(SOURCES)/*.c))
OFILES := $(addprefix $(BUILD)/,$(CFILES:.c=.o))

LIBS := -lwiiuse -logi -lasnd -lbte -logc -lm
INCLUDE := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) -I$(LIBOGC_INC)

CFLAGS := -g -O2 -Wall $(MACHDEP) $(INCLUDE)
LDFLAGS := -g $(MACHDEP) $(LIBPATHS) $(LIBS)

.PHONY: all clean

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

# To wrap app.dol into a bootable ISO with Wiimms ISO Tools (wit),
# create a Wii disc layout and use: wit copy <disc-folder> wii-ai-chatbot.iso
