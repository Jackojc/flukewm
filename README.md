<p align=center><img alt="logo" src="logos/fluke_transparent.png" width=30%/></p>

# fluke
A simple, fast and efficient window manager for X, made with C++ & XCB.

> note: this project is still under heavy development, so while suggestions and critisism are welcome, please bear in mind that the project is unfinished and not considered usable yet.

[![build](https://img.shields.io/travis/Jackojc/flukewm.svg?style=flat)](https://travis-ci.org/Jackojc/flukewm)
![c++](https://img.shields.io/badge/c%2B%2B-%3E%3D17-blue.svg?style=flat)
![code size](https://img.shields.io/github/languages/code-size/Jackojc/flukewm.svg)
[![license](https://img.shields.io/github/license/Jackojc/flukewm.svg?style=flat)](./LICENSE)
[![issues](https://img.shields.io/github/issues/Jackojc/flukewm.svg?style=flat)](https://github.com/Jackojc/flukewm/issues)
[![discord](https://img.shields.io/discord/537732103765229590.svg?label=discord&style=flat)](https://discord.gg/RmgjcES)
### what is fluke?
Fluke is a simple (but not extremely barebones) window manager for X which uses XCB for efficient asynchronous IO. Fluke is designed to be configured through source (á la dwm).

### rationale
I wanted a very minimal window manager that suited my workflow, had good hotplug multimonitor support and was written in something a little bit less cryptic than C. I never really found what I was looking for in other window managers and decided to give it a try myself.

### features
> The planned features list is by no means exhaustive, it's just what I have managed to think of right now.

#### current
- Focus windows using the mouse.
- Adopt orphaned windows by default (allows you to restart flukewm in the case of a crash and not lose any work.)
- Window borders
- Hooks which run every N ms

#### planned/ideas
- Move & resize windows
- Fullscreen windows
- Center windows
- Window snapping
- Workspaces (N per monitor)
- Groups/Scratchpad (seperate from workspaces in that you hide or show them regardless of what workspace you're on)
- _Basic_ EWMH support for docks, notifications, respectful closing of windows etc.
- Monitor hotplug support (automatically re-adjust windows & layout, maybe a built in autorandr functionality?)
- Keybindings

### prerequisites
- Any C++17 compliant compiler should work (tested with clang-9 and gcc-9)
- Libraries:
	- `xcb`
	- `xcb-util`
	- `xcb-randr`
	- `xcb-icccm`

### build & run
- Run `make` or `make debug=no symbols=no` for debug and release build respectively (uses gcc by default)
- Binary will be placed in `build/flukewm` or you can run using the symlink in the root of the project, `./flukewm`
- Note: fluke will not run if another window manager is currently active

### installation
> todo...

### license
This project uses the GPLv2 license. (check [LICENSE.md](LICENSE.md))

### acknowledgements
- Thanks to [@noxabellus](https://github.com/noxabellus) for the great logo!

- This project was inspired and helped by the source code of the following projects:
	- [bspwm](https://github.com/baskerville/bspwm)
	- [wmutils](https://github.com/wmutils/core)
	- [disputils](https://github.com/tudurom/disputils)
	- [windowchef](https://github.com/tudurom/windowchef)

### progress & discussion
You can join the discord server in order to follow progress and/or contribute to discussion of the project. (https://discord.gg/RmgjcES)


