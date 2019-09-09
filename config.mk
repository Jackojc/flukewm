# options
PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

XCBINC = /usr/include/xcb/
XCBLIB = /usr/lib/

PKG_CONFIG = pkg-config

# includes and libs
INCS = -I$(XCBINC) -Isrc/ -I. -Imodules/tinge/
LIBS = -L$(XCBLIB) -lxcb -lxcb-util -lxcb-randr -lxcb-icccm $(LDLIBS)


# preprocessor flags
FLUKE_CPPFLAGS = $(CPPFLAGS)

# linker flags
FLUKE_LDFLAGS = $(LIBS) $(LDFLAGS) -static-libstdc++ -static-libgcc

# warnings
FLUKE_CXXWARN = $(CXXWARN) -Wall -Wextra -Wcast-align -Wcast-qual -Wnon-virtual-dtor -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=4 -Wundef -Wno-unused


logging =

# optimisation flags
FLUKE_CXXFLAGS = $(INCS) $(FLUKE_CPPFLAGS) $(CXXFLAGS) -m64

debug ?= no

ifeq ($(debug),no)
	FLUKE_CXXFLAGS += -Ofast -march=native
	FLUKE_LDFLAGS += -flto

	ifeq ($(CXX),g++)
		FLUKE_CXXFLAGS += -finline-limit=200 -fipa-pta -fsplit-loops -funswitch-loops -fwhole-program

	else ifeq ($(CXX),clang++)
		FLUKE_CXXFLAGS += -fwhole-program-vtables -fforce-emit-vtables
	endif

else ifeq ($(debug),yes)
	FLUKE_CXXFLAGS += $(INCS) $(FLUKE_CPPFLAGS) $(CXXFLAGS) -Og -march=native
	symbols ?= yes
else
$(error debug should be either yes or no)
endif



# debugging symbols
symbols ?= no
ifeq ($(symbols),yes)
	FLUKE_CXXFLAGS += -g
else ifneq ($(symbols),no)
$(error symbols should be either yes or no)
endif
