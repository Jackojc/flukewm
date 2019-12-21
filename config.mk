SRC=main.cpp
STD=c++17

BUILD_DIR=build
TARGET=flukewm


# Include & Link
INCS=-I. -Isrc/ -Imodules/tinge/
LIBS=-lxcb -lxcb-util -lxcb-randr -lxcb-icccm $(LDLIBS)


# Options
PROGRAM_CPPFLAGS=$(CPPFLAGS)
PROGRAM_LDFLAGS=$(LIBS) $(LDFLAGS)

PROGRAM_WARNINGS=-Wall -Wextra -Wmissing-include-dirs -Wsign-conversion -Wshadow -Wundef -Wno-unused $(CXXWARN)


# Make Flags
debug ?= yes
symbols ?= yes


# Debugging
ifeq ($(debug),no)
	symbols ?= no
	PROGRAM_CXXFLAGS=-O3 -march=native -flto $(CXXFLAGS)

else ifeq ($(debug),yes)
	symbols ?= yes
	PROGRAM_CXXFLAGS=-Og -march=native

else
$(error debug should be either yes or no)
endif


# Debugging Symbols
ifeq ($(symbols),yes)
	PROGRAM_CXXFLAGS += -g

else ifneq ($(symbols),no)
$(error symbols should be either yes or no)
endif


# Accumulate all flags
COMPILE_COMMAND=$(CXX) -std=$(STD) $(PROGRAM_WARNINGS) $(PROGRAM_CXXFLAGS) $(INCS) $(PROGRAM_CPPFLAGS) $(PROGRAM_LDFLAGS) -o $(BUILD_DIR)/$(TARGET) $(SRC)

