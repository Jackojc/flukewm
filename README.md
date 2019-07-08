# Fluke Window Manager
A simple, fast and efficient window manager for X.

[![build](https://img.shields.io/travis/Jackojc/flukewm.svg?style=flat)](https://travis-ci.org/Jackojc/flukewm)
![c++](https://img.shields.io/badge/c%2B%2B-%3E%3D17-blue.svg?style=flat)
![code size](https://img.shields.io/github/languages/code-size/Jackojc/flukewm.svg)
[![license](https://img.shields.io/github/license/Jackojc/flukewm.svg?style=flat)](./LICENSE)
[![issues](https://img.shields.io/github/issues/Jackojc/flukewm.svg?style=flat)](https://github.com/Jackojc/flukewm/issues)
[![discord](https://img.shields.io/discord/537732103765229590.svg?label=discord&style=flat)](https://discord.gg/RmgjcES)
### What is flukewm?
Fluke is designed to be a simple and fast controller for X. It can manipulate windows in various ways like resizing, moving, hiding etc. It is intended for use with a hotkey daemon like [sxhkd](https://github.com/baskerville/sxhkd). The hotkey daemon will call flukewm _as needed_ to perform various operations.

### Why is flukewm?
I wanted a very minimal window manager that could be scripted very easily. I spent a couple of years using [bspwm](https://github.com/baskerville/bspwm) but it felt a bit too feature rich for my needs and was required to run in the background alongside X at all times.

bspwm also makes uses of a server/client control model with bspc which is both good and bad. It's good in that it is very easy to script and bad in that it comes at the cost of some latency and extra resource usage.

### Build Requirements
- Any C++17 compliant compiler should work. (Tested with Clang and GCC)

### Other Requirements
- A running X server for fluke to communicate with.

### Build & Run
- Run `make` or `make rel` for debug and release build respectively. (uses GCC by default)
- Binary will be placed in `build/flukewm` or you can run using the symlink in the root of the project, `./flukewm`.

### Progress & Discussion
You can join the discord server in order to follow progress and/or contribute to discussion of the project. (https://discord.gg/RmgjcES)
