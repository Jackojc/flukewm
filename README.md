<p align=center><img alt="logo" src="logos/fluke_transparent.png" width=30%/></p>

# fluke
A simple, fast and efficient window manager for X, made with C++ & XCB.

> note: this project is still under heavy development, so do not expect a fully working experience out of the box. Feedback & critsism are welcome!

[![build](https://img.shields.io/travis/Jackojc/flukewm.svg?style=flat)](https://travis-ci.org/Jackojc/flukewm)
![c++](https://img.shields.io/badge/c%2B%2B-%3E%3D17-blue.svg?style=flat)
![code size](https://img.shields.io/github/languages/code-size/Jackojc/flukewm.svg)
[![license](https://img.shields.io/github/license/Jackojc/flukewm.svg?style=flat)](./LICENSE)
[![issues](https://img.shields.io/github/issues/Jackojc/flukewm.svg?style=flat)](https://github.com/Jackojc/flukewm/issues)
[![discord](https://img.shields.io/discord/537732103765229590.svg?label=discord&style=flat)](https://discord.gg/RmgjcES)
### what is fluke?
Fluke is a simple (but with batteries included) window manager for X which uses XCB for efficient asynchronous IO. Fluke is designed to be configured through source (á la dwm).

### why is fluke?
I wanted a window manager that was lightweight, hackable, well documented, included a decent amount of functionality out of the box (with sane defaults) and was written in a language I was comfortable with.

I never quite found what I was looking for in other window managers with similar goals for a number of reasons:

- lightweight usually implies C which is a little archaic for my liking, I wanted a more modern language to better express my ideas and which didn't comprimise on runtime performance.

- most window managers are sorely lacking in well documented code which makes it hard to understand what is happening under the hood. I wanted to address this by being very liberal in my usage of comments and logging.

- multi-head support in so-called "minimal" window managers is usually quite poor or non-existant. I use a laptop + dock setup which means I frequently hotplug monitors.

- the majority of minimal window managers are automatic tiling which I'm not the biggest fan of, with fluke I hope to solve this by making all window positioning algorithms on-demand(in response to a keybinding) so that my layout is not disturbed on opening/closing of windows.

- I wanted to make best possible use of the asynchronous capabilities of XCB by properly queing requests and fetching replies. So in essence, blocking for replies is done only when necessary and in batches.

### features
> These features constitute what I consider to be a usable base but I am open to [suggestions](https://github.com/Jackojc/flukewm/issues/new?assignees=Jackojc&labels=enhancement&template=feature_request.md&title=%5Bfeature%5D).

* [x] keybindings
* [x] window centering
* [x] window resizing & moving
* [x] tiling (on-demand with keybinding)
* [x] directional focusing, next/prev focusing, mouse focusing
* [x] adopt orphaned windows (allows you to restart flukewm in place)
* [x] configurable gutters to reserve space for status bars
* [x] configurable window gaps & borders
* [ ] fullscreen windows
* [ ] window snapping
* [ ] workspaces & scratchpads
* [ ] _basic_ EWMH support for docks, notifications, respectful closing of windows etc.
* [ ] monitor hotplug support

### prerequisites
- any c++17 compliant compiler should work (tested with clang-9)
- libraries/headers:
	- `xcb`
	- `xcb-util`
	- `xcb-randr`
	- `xcb-icccm`
	- `xcb-ewmh`
	- `xcb-keysyms`
	- `X11` (for key symbols)

### build & run
- run `make` or `make debug=no symbols=no` for debug and release build respectively
- binary will be placed at `build/fluke`
- note: fluke will not run if another window manager is currently active

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
	- [mcwm](https://github.com/bbidulock/mcwm)
	- [2bwm](https://github.com/venam/2bwm)
	- [sowm](https://github.com/dylanaraps/sowm)

### progress & discussion
You can join the discord server in order to follow progress and/or contribute to discussion of the project. (https://discord.gg/RmgjcES)



