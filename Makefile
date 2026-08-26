SHELL := /bin/bash
.ONESHELL:

COMPOSE_FILE ?= $(CURDIR)/compose.yml
DOCKER_COMPOSE ?= docker compose -f $(COMPOSE_FILE)
DEV_CONTAINER ?= iec60730-library
BOARD_NAME ?= brd4264c
BUILD_ARGS ?=
BUILD_MODE ?= all
RUNNER ?= compose

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

.PHONY: all bootstrap bootstrap-env build build-unit build-integration clean prepare

all: bootstrap build

bootstrap: bootstrap-env

bootstrap-env:
	$(call RUN_SCRIPT,bootstrap-env,BOOTSTRAP_SCRIPT)

build: BUILD_MODE=all
build:
	$(call RUN_SCRIPT,build,BUILD_SCRIPT)

build-unit: BUILD_MODE=unit
build-unit:
	$(call RUN_SCRIPT,build-unit,BUILD_SCRIPT)

build-integration: BUILD_MODE=integration
build-integration:
	$(call RUN_SCRIPT,build-integration,BUILD_SCRIPT)

clean:
	$(call RUN_SCRIPT,clean,CLEAN_SCRIPT)
