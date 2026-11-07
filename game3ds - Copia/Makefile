# Quero Acabar Logo Com Isso — Nintendo 3DS (.3dsx e .cia)
# Requer devkitPro com devkitARM, libctru, citro2d, citro3d e as ferramentas
# 3dstools (smdhtool, 3dsxtool), bannertool e makerom.
#   make        -> gera .elf, .smdh e .3dsx
#   make cia    -> gera tambem o .cia (precisa de bannertool e makerom no PATH)

ifeq ($(strip $(DEVKITARM)),)
$(error "Defina DEVKITARM, por exemplo: export DEVKITARM=/opt/devkitpro/devkitARM")
endif
DEVKITPRO ?= /opt/devkitpro

include $(DEVKITARM)/3ds_rules

TARGET      := qalci
APP_TITLE   := Quero Acabar Logo Com Isso
APP_DESC    := Terror em 3 amigos e 1 porao
APP_AUTHOR  := Arthur Abreu
SOURCES     := source
BUILD       := build
CTRULIB     := $(DEVKITPRO)/libctru
PORTLIBS    := $(DEVKITPRO)/portlibs/3ds

ARCH     := -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft
CFLAGS   := -g -Wall -O2 -mword-relocations -ffunction-sections -fdata-sections $(ARCH) \
            -I$(CTRULIB)/include -I$(PORTLIBS)/include -D__3DS__ -std=gnu11
LDFLAGS  := -specs=3dsx.specs -g $(ARCH) -Wl,--gc-sections
LIBDIRS  := -L$(CTRULIB)/lib -L$(PORTLIBS)/lib
LIBS     := -lcitro2d -lcitro3d -lctru -lm

CFILES   := $(wildcard $(SOURCES)/*.c)
OFILES   := $(patsubst $(SOURCES)/%.c,$(BUILD)/%.o,$(CFILES))

.PHONY: all cia clean
all: $(TARGET).3dsx

$(BUILD):
	@mkdir -p $(BUILD)

$(BUILD)/%.o: $(SOURCES)/%.c $(wildcard $(SOURCES)/*.h) | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OFILES)
	$(CC) $(LDFLAGS) $(OFILES) $(LIBDIRS) $(LIBS) -o $@

$(TARGET).smdh: icon.png
	smdhtool --create "$(APP_TITLE)" "$(APP_DESC)" "$(APP_AUTHOR)" icon.png $@

$(TARGET).3dsx: $(TARGET).elf $(TARGET).smdh
	3dsxtool $(TARGET).elf $@ --smdh=$(TARGET).smdh

cia: $(TARGET).elf
	bannertool makebanner -i banner.png -a audio.wav -o $(BUILD)/banner.bnr
	bannertool makesmdh -s "$(APP_TITLE)" -l "$(APP_DESC)" -p "$(APP_AUTHOR)" -i icon.png -o $(BUILD)/icon.icn
	makerom -f cia -o $(TARGET).cia -elf $(TARGET).elf -rsf app.rsf -icon $(BUILD)/icon.icn -banner $(BUILD)/banner.bnr -exefslogo -target t

clean:
	rm -rf $(BUILD) $(TARGET).elf $(TARGET).3dsx $(TARGET).smdh $(TARGET).cia $(TARGET).map
