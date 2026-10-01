ROM:=rom.gbc
C_SOURCES:=$(wildcard *.c)
OBJ:=$(C_SOURCES:.c=.o)
BUILD_OUTPUTS:=$(OBJ) $(ROM)
BUILD_OUTPUTS+=$(C_SOURCES:.c=.asm) $(C_SOURCES:.c=.lst) $(C_SOURCES:.c=.sym)
BUILD_OUTPUTS+=$(ROM:.gbc=.ihx) $(ROM:.gbc=.map)
BUILD_OUTPUTS+=$(ROM:.gbc=.noi) $(ROM:.gbc=.cdb)
CC:=lcc
# Only needed for `install'.
CHROMATIC_CLI:=chromatic-cli
PLAYER:=1

# Store the game title in the cartridge header.
ROMFLAGS+=-Wm-ynGAMEBOYDEMO
# Select the cartridge type (0x00 means ROM only).
ROMFLAGS+=-Wm-yt0x00
# Set the number of cartridge RAM banks (0 means none).
ROMFLAGS+=-Wm-ya0
# Set Game Boy Color support (-Wm-yC means Color only).
ROMFLAGS+=-Wm-yC

.PHONY: all clean preview install

all: $(ROM)

%.o: %.c
	"$(CC)" $(CFLAGS) -c -o "$@" "$<"

$(ROM): $(OBJ) Makefile
	"$(CC)" $(ROMFLAGS) -o "$@" $(OBJ)

preview: $(ROM)
	python3 preview.py

install: $(ROM)
	# Leave the CLI confirmation prompt enabled before writing the cartridge.
	"$(CHROMATIC_CLI)" write-homebrew --player "$(PLAYER)" "$(ROM)"

clean:
	rm -f $(BUILD_OUTPUTS)

### Dependencies
main.o: main.c game.h
square.o: square.c game.h
