# Game Boy Demo

[![License: GPLv3](https://img.shields.io/badge/License-GPLv3-blue.svg)](LICENSE)

A demo repo for me to explore [Game Boy][gb] programming.

## Features

After booting up the game boy, you will see a block appearing in the center of
the screen. The block can be repositioned and, with a proper use of keys, it
can be put into motion.

### Direct positioning mode

This is the default mode after startup. Pressing arrow keys moves the block.
Hitting `B` recenters it.

### Velocity configuration mode

When in direct positioning mode, pressing `SELECT` enters the velocity
configuration mode. In this mode, instead of moving the block, the arrow keys
adjust its velocity. If unconfigured, the default velocity is zero.

By pressing and releasing an arrow key, the corresponding velocity gets updated
by one pixel per frame: `LEFT`/`RIGHT` decreases/increases horizontal velocity,
and `UP`/`DOWN` decreases/increases vertical velocity. The block stays in
place. Press `B` to zero both velocity components.

In this mode, hitting `SELECT` saves the velocity, exits the configuration
mode, and returns to the direct positioning mode.

### Motion mode

In direct positioning or velocity configuration mode, press `START` to launch
the block with the stored velocity. Press `START` again to stop. During motion,
arrow keys, `SELECT`, and `B` are ignored. The block rebounds once it hits a
boundary

### Color themes

The key `A` loops through four different color themes for the display.

## Build

This project requires [GBDK-2020](https://gbdk.org/docs/api/). After having it
installed, ensure its `lcc` is discoverable on your `PATH` or pass
`CC=/path/to/gbdk/bin/lcc` to `make`.

Run `make` to build the entire project and `make clean` to remove build
outputs.

Load `rom.gbc` in a Game Boy Color emulator, or write it to a compatible
cartridge to run it on a Chromatic.

## Preview

Game preview requires Python 3, available as `python3` on your `PATH`.

The command `make preview` runs the ROM in your browser. The browser preview
uses [EmulatorJS](https://emulatorjs.org/) and requires internet access for its
emulator code. Keep the terminal open while playing; press Ctrl-C to stop the
local server.

The browser emulator uses these default keyboard bindings:

| Key | Game Boy button |
| --- | --- |
| Arrow keys | D-pad |
| `Z` | A |
| `X` | B |
| `Enter` | START |
| `V` | SELECT |

You can change them in the emulator's control settings.

## Installing to Chromatic

**This installation procedure overrides existing data on the cartridge.**

[ModRetro Chromatic][mc] is a handheld console capable of running games created
for Nintendo's Game Boy and [Game Boy Color][gbc].

To install the ROM to Chromatic, first install the [Chromatic CLI][ccli]:

```sh
npm install --global @modretro/chromatic-cli
```

Insert a writable ModRetro cartridge. Switch on and connect the Chromatic to
your computer by USB-C so that it can be discovered via

```sh
chromatic-cli list-devices
```

For example, a device with player number `1` appears as:

```text
Chromatic Player 01 at 03.1
  GWU2X  33aa:0120 GWU2X
  Player 374e:0101 Chromatic - Player 01
```

Run the following command to install to player `1`, the default. For another
player, pass `PLAYER=<number>` to `make`.

```sh
make install
```

If the CLI is installed at a custom location, pass
`CHROMATIC_CLI=/path/to/chromatic-cli` to `make`.

[mc]: https://modretro.com/collections/chromatic-consoles
[gb]: https://en.wikipedia.org/wiki/Game_Boy
[gbc]: https://en.wikipedia.org/wiki/Game_Boy_Color
[ccli]: https://www.npmjs.com/package/@modretro/chromatic-cli
