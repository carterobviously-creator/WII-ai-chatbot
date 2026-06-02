# Wii homebrew Makefile for devkitPPC/libogc
# NOTE: GitHub Actions builds the Homebrew Channel package automatically.

DEVKITPRO ?= /opt/devkitpro
DEVKITPPC ?= $(DEVKITPRO)/devkitPPC
LIBOGC ?= $(DEVKITPRO)/libogc
LIBOGC_INC ?= $(LIBOGC)/include
LIBOGC_LIB ?= $(LIBOGC)/lib/wii

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

CFILES := main.c ai_brain.c keyboard.c tv_chat.c
OFILES := $(addprefix $(BUILD)/,$(CFILES:.c=.o))

LIBS := -lwiiuse -lbte -logc -lm
INCLUDE := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) -I$(LIBOGC_INC)
LIBOGC_LIBPATH := -L$(LIBOGC_LIB)

CFLAGS := -g -O2 -Wall $(MACHDEP) $(INCLUDE)
LDFLAGS := -g $(MACHDEP) $(LIBOGC_LIBPATH) $(LIBPATHS) $(LIBS)

.PHONY: all clean

all: $(OUTPUT).dol

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.o: $(SOURCES)/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTPUT).elf: $(OFILES)
	$(CC) $(OFILES) $(LDFLAGS) -o $@

$(OUTPUT).dol: $(OUTPUT).elf
	elf2dol $< $@

clean:
	rm -rf $(BUILD) $(OUTPUT).elf $(OUTPUT).dol
