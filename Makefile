PROJECT_NAME ?= try_box2d
CONFIG ?= Debug
BUILD_DIR := build
BIN_DIR := $(BUILD_DIR)/bin

.PHONY: deps build run clean clean_app

deps:
	cmake -S . -B $(BUILD_DIR)

build:
	cmake --build $(BUILD_DIR) --config $(CONFIG)

run:
	 $(BIN_DIR)/$(CONFIG)/$(PROJECT_NAME)

clean:
	rm -rf $(BUILD_DIR)

clean_app:
	rm -rf $(BIN_DIR)/$(CONFIG)