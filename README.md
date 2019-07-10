```
  __ _       _
 / _| |_   _| | _______      ___ __ ___
| |_| | | | | |/ / _ \ \ /\ / | '_ ` _ \
|  _| | |_| |   |  __/\ V  V /| | | | | |
|_| |_|\__,_|_|\_\___| \_/\_/ |_| |_| |_|
```

# fluke window manager
A simple, fast and efficient window manager for X.

[![build](https://img.shields.io/travis/Jackojc/flukewm.svg?style=flat)](https://travis-ci.org/Jackojc/flukewm)
![c++](https://img.shields.io/badge/c%2B%2B-%3E%3D17-blue.svg?style=flat)
![code size](https://img.shields.io/github/languages/code-size/Jackojc/flukewm.svg)
[![license](https://img.shields.io/github/license/Jackojc/flukewm.svg?style=flat)](./LICENSE)
[![issues](https://img.shields.io/github/issues/Jackojc/flukewm.svg?style=flat)](https://github.com/Jackojc/flukewm/issues)
[![discord](https://img.shields.io/discord/537732103765229590.svg?label=discord&style=flat)](https://discord.gg/RmgjcES)
### what is fluke?
Fluke is designed to be a simple and fast controller for X. It can manipulate windows in various ways like resizing, moving, hiding etc. It is intended for use with a hotkey daemon like [sxhkd](https://github.com/baskerville/sxhkd). The hotkey daemon will call flukewm _as needed_ to perform various operations.

### rationale
I wanted a very minimal window manager that could be scripted with ease. I spent a couple of years using [bspwm](https://github.com/baskerville/bspwm) but it felt a bit too feature rich for my needs and was required to run in the background alongside X at all times.

bspwm makes uses of a server/client control model with bspc which is both good and bad. It's good in that it is very easy to script and bad in that it comes at the cost of some latency and extra resource usage.

### features
> The planned features list is by no means exhaustive, it's just what I have managed to think of right now.

#### current
- Move, resize windows
- Retrieve cursor position relative to root window
- Close/Kill windows

#### planned
- Focus windows (by id, direction, monitor)
- Toggle fullscreen
- Add some grids/layouts
- Grow/Shrink windows
- Snap windows left,right,top,bottom
- Center a window
- Resize window to a percentage of screen size
- Workspaces
- Window borders
- Focus on cursor hover
- Implement a way to update a statusbar using xsetroot (something like dwm)
- Move/resize windows using the mouse
- Groups (seperate from workspaces in that you hide or show them regardless of what workspace you're on)
- Respect EWMH for docks, notifications etc.
- Query information about various things (currently focused display, currently focused window, primary display etc.)

### prerequisites
- Any C++17 compliant compiler should work (tested with Clang and GCC)
- Libraries:
	- `xcb`
	- `xcb-util`
	- `xcb-randr`
	- `xcb-icccm`
- [hyperfine](https://github.com/sharkdp/hyperfine) (optional for benchmarking)
- A running X server for fluke to communicate with

### build & run
- Run `make debug=yes` or `make` for debug and release build respectively (uses GCC by default)
- Binary will be placed in `build/flukewm` or you can run using the symlink in the root of the project, `./flukewm`

### installation
> Optionally configure install directory with PREFIX variable.

To install flukewm, simply run `make install` (you might need root).
To uninstall, run `make uninstall`

You may also use `make dist` if you wish to package flukewm as an archive.

### license
This project uses the GPLv2 license. (check [LICENSE.md](LICENSE.md))

### acknowledgements
This project was inspired by:
- [bspwm](https://github.com/baskerville/bspwm)
- [wmutils](https://github.com/wmutils/core)
- [disputils](https://github.com/tudurom/disputils)
- [windowchef](https://github.com/tudurom/windowchef)

### progress & discussion
You can join the discord server in order to follow progress and/or contribute to discussion of the project. (https://discord.gg/RmgjcES)

