# flukewm

.POSIX:

include config.mk

SRC=$(wildcard utils/*.cpp)

all: options flukewm

config:
	mkdir -p build/

options:
	@echo " \033[32;1m=>\033[39m Debug    = $(debug)\033[0m"
	@echo " \033[32;1m=>\033[39m Symbols  = $(symbols)\033[0m"

	@echo "\n \033[32m=>\033[39m CXX    = \033[0m$(CXX)"
	@echo " \033[32m=>\033[39m CXXWARN  = \033[0m$(FLUKE_CXXWARN)"
	@echo " \033[32m=>\033[39m CXXFLAGS = \033[0m$(FLUKE_CXXFLAGS)"
	@echo " \033[32m=>\033[39m LDFLAGS  = \033[0m$(FLUKE_LDFLAGS)"

flukewm: config $(SRC)
	$(CXX) -std=c++17 $(FLUKE_CXXWARN) $(FLUKE_CXXFLAGS) -o build/$@ $(SRC) $(FLUKE_LDFLAGS)
	@echo "$(logging)"

clean:
	rm -rf build/ flukewm.tar.gz *.gcda

dist: clean
	mkdir -p flukewm-tmp/
	cp -Rf LICENSE README.md Makefile utils/*.cpp fluke/ modules/ docs/ flukewm-tmp/
	tar -cf - flukewm-tmp/ | gzip > flukewm.tar.gz
	rm -rf flukewm-tmp/

	@echo "\n \033[32;1m=>\033[39m Created flukewm.tar.gz\033[0m\n"

# install: flukewm
# 	mkdir -p $(DESTDIR)$(PREFIX)/bin
# 	cp -f ./build/* $(DESTDIR)$(PREFIX)/bin
# 	chmod 755 $(DESTDIR)$(PREFIX)/bin/*

# 	@echo "\n \033[32;1m=>\033[39m Installed flukewm to $(DESTDIR)$(PREFIX)/bin/flukewm\033[0m\n"

# uninstall:
# 	rm -f $(DESTDIR)$(PREFIX)/bin/flukewm

# 	@echo "\n \033[32;1m=>\033[39m Removed flukewm\033[0m\n"

.PHONY: all options clean dist install uninstall

