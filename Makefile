PROJECT_NAME ?= try_box2d
CONFIG ?= Debug
BIN_DIR := ./build/bin

.PHONY: deps build run clean clean_app

deps:
	cmake -S . -B build

build:
	cmake --build build --config $(CONFIG)

run:
	 $(BIN_DIR)/$(CONFIG)/$(PROJECT_NAME)

clean:
	rm -rf build

clean_app:
	rm -rf build/bin/$(CONFIG)