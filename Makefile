CXX=g++
STD=c++17

INCLUDE=-Isrc -I. -Imodules/tinge/
LIBS=-lxcb -lxcb-util -lxcb-randr -lxcb-icccm -static-libstdc++ -static-libgcc -lstdc++fs

BUILD_DIR=build
TARGET=flukewm



CXXWARN=-Wall -Wextra -Wcast-align -Wcast-qual -Wnon-virtual-dtor -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2  -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=4 -Wundef -Wno-unused -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wuseless-cast -Wdouble-promotion


GENERAL_FLAGS=$(CXXFLAGS) -msse2 -march=native -mtune=native -m64
RELEASE_FLAGS=$(GENERAL_FLAGS) -Ofast -finline-limit=200 -fipa-pta -fwhole-program -fsplit-loops -funswitch-loops -DNDEBUG
DEBUG_FLAGS=$(GENERAL_FLAGS) -O2 -g


LFLAGS=-flto


# PROFILE_GENERAL_FLAGS=-fvpt
PROFILE_GEN_FLAGS=$(PROFILE_GENERAL_FLAGS) -fprofile-generate
PROFILE_USE_FLAGS=$(PROFILE_GENERAL_FLAGS) -fprofile-use





COMMAND=$(CXX) --std=$(STD) $(CXXWARN) $(INCLUDE)


all:
	mkdir -p $(BUILD_DIR)
	$(COMMAND) $(DEBUG_FLAGS) -c main.cpp -o $(BUILD_DIR)/main.o
	$(COMMAND) $(DEBUG_FLAGS) build/*.o $(LIBS) -o $(BUILD_DIR)/$(TARGET)


unop:
	mkdir -p $(BUILD_DIR)
	$(COMMAND) -c main.cpp -o $(BUILD_DIR)/main.o
	$(COMMAND) $(LFLAGS) build/*.o $(LIBS) -o $(BUILD_DIR)/$(TARGET)


rel:
	mkdir -p $(BUILD_DIR)
	$(COMMAND) $(RELEASE_FLAGS) $(PROFILE_GEN_FLAGS) -c main.cpp -o $(BUILD_DIR)/main.o
	$(COMMAND) $(RELEASE_FLAGS) $(PROFILE_GEN_FLAGS) $(LFLAGS) build/*.o $(LIBS) -o $(BUILD_DIR)/$(TARGET)

	./bench

	$(COMMAND) $(RELEASE_FLAGS) $(PROFILE_USE_FLAGS) -c main.cpp -o $(BUILD_DIR)/main.o
	$(COMMAND) $(RELEASE_FLAGS) $(PROFILE_USE_FLAGS) $(LFLAGS) build/*.o $(LIBS) -o $(BUILD_DIR)/$(TARGET)


# With symbols
relsym:
	mkdir -p $(BUILD_DIR)
	$(COMMAND) $(RELEASE_FLAGS) -g $(PROFILE_GEN_FLAGS) -c main.cpp -o $(BUILD_DIR)/main.o
	$(COMMAND) $(RELEASE_FLAGS) -g $(PROFILE_GEN_FLAGS) $(LFLAGS) build/*.o $(LIBS) -o $(BUILD_DIR)/$(TARGET)

	./bench

	$(COMMAND) $(RELEASE_FLAGS) -g $(PROFILE_USE_FLAGS) -c main.cpp -o $(BUILD_DIR)/main.o
	$(COMMAND) $(RELEASE_FLAGS) -g $(PROFILE_USE_FLAGS) $(LFLAGS) build/*.o $(LIBS) -o $(BUILD_DIR)/$(TARGET)


