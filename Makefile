SHELL := /bin/bash
.ONESHELL:

# Docker Compose configuration.
COMPOSE_FILE ?= $(CURDIR)/compose.yml

# Compose command with host UID/GID mapping.
DOCKER_COMPOSE ?= DOCKER_UID=$(shell id -u) \
                  DOCKER_GID=$(shell id -g) docker compose -f $(COMPOSE_FILE)

# Development container service name.
DEV_CONTAINER ?= iec60730-library

# Default target board.
BOARD_NAME ?= brd4264c

# Additional build arguments passed to the build command.
BUILD_ARGS ?=

# Build scope: all, app, lib, etc.
BUILD_MODE ?= all

# Execution backend: compose (Docker) or native (host).
RUNNER ?= compose

# Override per device family when the flash memory map differs
FLASH_REGIONS_TEST := 0x08000000
ifeq ($(BOARD_NAME),EFR32BG21A010F1024IM32)
FLASH_REGIONS_TEST := 0x00000000
endif
export FLASH_REGIONS_TEST

# Inside Compose/CI containers the repo is mounted at /workspace.
ifeq ($(RUNNER),compose)
WORKSPACE := /workspace
else
WORKSPACE := $(CURDIR)
endif

# Keep legacy prepare target used by test/execute_*.sh and CMake generate steps.
prepare:
	- @rm -rf build
	rm -rf build
	rm -rf log
	mkdir build
	mkdir log

# Enable strict error handling and ensure USER is defined.
define BOOTSTRAP_SCRIPT
set -euo pipefail
export USER=$${USER:-root}

echo '>>> Configuring Git safe directory...'
git config --global --add safe.directory $(WORKSPACE)

echo '>>> Running bootstrap for Silabs SDK...'
cd $(WORKSPACE)
source ./script/bootstrap silabs

echo '>>> Bootstrap complete!'
endef
export BOOTSTRAP_SCRIPT

define BUILD_SCRIPT
set -euo pipefail
export USER=$${USER:-root}

echo '>>> Configuring Git safe directory...'
git config --global --add safe.directory $(WORKSPACE)

echo '>>> Building ($(BUILD_MODE)) for $(BOARD_NAME)...'
cd $(WORKSPACE)
source ./script/set_env.sh
./script/build $(BUILD_MODE) $(BOARD_NAME) $(BUILD_ARGS)

echo '>>> Build complete!'
endef
export BUILD_SCRIPT

define CLEAN_SCRIPT
set -euo pipefail

echo '>>> Cleaning firmware build output...'
rm -rf $(WORKSPACE)/build $(WORKSPACE)/log $(WORKSPACE)/.docker-build-stash

echo '>>> Clean complete!'
endef
export CLEAN_SCRIPT

# RUNNER=compose: run inside the Docker container.
# RUNNER=native: run directly in the current environment.
define RUN_SCRIPT
	@if [ "$(RUNNER)" = "compose" ]; then \
		if [ -f /.dockerenv ]; then \
			echo "Error: RUNNER=compose cannot be used from inside a container."; \
			echo "Use: make RUNNER=native $(1)"; \
			exit 1; \
		fi; \
		printf '%s\n' "$$$(2)" | $(DOCKER_COMPOSE) run --rm -T $(DEV_CONTAINER) /bin/bash; \
	elif [ "$(RUNNER)" = "native" ]; then \
		printf '%s\n' "$$$(2)" | /bin/bash; \
	else \
		echo "Unsupported RUNNER: $(RUNNER). Use compose or native."; \
		exit 2; \
	fi
endef

# make all               : bootstrap + build
# make bootstrap         : initialize build environment
# make bootstrap-env     : run bootstrap script
# make build             : build all targets
# make build-unit        : build unit tests
# make build-integration : build integration tests
# make clean             : remove build artifacts
.PHONY: all bootstrap bootstrap-env build build-unit build-integration clean prepare apply-sdk-profile

# make apply-sdk-profile PROFILE=gecko_4_5
# make apply-sdk-profile PROFILE=ssdk_2026_6
apply-sdk-profile:
	@python3 "$(CURDIR)/script/switch_sdk.py" "$(PROFILE)"

# Run the default bootstrap and build workflow.
all: bootstrap build

# Initialize the SDK build environment.
bootstrap: bootstrap-env

# Run bootstrap setup scripts and environment preparation steps.
bootstrap-env:
	$(call RUN_SCRIPT,bootstrap-env,BOOTSTRAP_SCRIPT)

# Build all configured targets.
build: BUILD_MODE=all
build:
	$(call RUN_SCRIPT,build,BUILD_SCRIPT)

# Build unit test targets.
build-unit: BUILD_MODE=unit
build-unit:
	$(call RUN_SCRIPT,build-unit,BUILD_SCRIPT)

# Build integration test targets.
build-integration: BUILD_MODE=integration
build-integration:
	$(call RUN_SCRIPT,build-integration,BUILD_SCRIPT)

# Remove generated build artifacts and logs.
clean:
	$(call RUN_SCRIPT,clean,CLEAN_SCRIPT)
