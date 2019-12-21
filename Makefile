# flukewm

.POSIX:

include config.mk

all: options flukewm

config:
	@mkdir -p $(BUILD_DIR)

options:
	@echo " ┌\033[32m target\033[30m····\033[92;1m$(TARGET)\033[0m"
	@echo " ├\033[34m debug\033[30m·····\033[94;1m$(debug)\033[0m"
	@echo " ├\033[34m symbols\033[30m···\033[94;1m$(debug)\033[0m"
	@echo " ├\033[31m compiler\033[30m··\033[91m$(CXX)\033[0m"
	@echo " ├\033[31m warnings\033[30m··\033[91m$(PROGRAM_WARNINGS)\033[0m"
	@echo " └\033[31m flags\033[30m·····\033[91m$(PROGRAM_CXXFLAGS) $(PROGRAM_LDFLAGS)\033[0m"

flukewm: config
	@$(COMPILE_COMMAND)

clean:
	rm -rf $(BUILD_DIR)/ *.gcda

.PHONY: all options clean


