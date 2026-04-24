PROJECT_NAME ?= try_box2d
CONFIG ?= Debug
BIN_DIR := ./build/bin

.PHONY: build_cmake build_app run clean clean_app

build_cmake:
	cmake -S . -B build

build_app:
	cmake --build build --config $(CONFIG)

run:
	 $(BIN_DIR)/$(CONFIG)/$(PROJECT_NAME)

clean:
	rm -rf build

clean_app:
	rm -rf build/bin/$(CONFIG)