# SLC SDK profiles for generating .slce / .slcp from *.in templates.
# Usage:
#   make apply-sdk-profile
#   make apply-sdk-profile PROFILE=gecko_4_5
#   make apply-sdk-profile SDK_ID=simplicity_sdk SDK_VERSION=2025.6.2 EXT_VERSION=2.0.0
#
# Default matches recipe.toml / docs (Simplicity SDK 2026.6.0).
# Command-line SDK_ID / SDK_VERSION / EXT_VERSION always win over PROFILE
# because ?= does not override variables already set on the command line.

PROFILE ?= simplicity_2026_6

ifeq ($(PROFILE),simplicity_2026_6)
  _PROFILE_SDK_ID      := simplicity_sdk
  _PROFILE_SDK_VERSION := 2026.6.0
  _PROFILE_EXT_VERSION := 2.0.0
else ifeq ($(PROFILE),gecko_4_5)
  _PROFILE_SDK_ID      := gecko_sdk
  _PROFILE_SDK_VERSION := 4.5.0
  _PROFILE_EXT_VERSION := 2.0.0
else ifeq ($(PROFILE),custom)
  # Caller must supply SDK_ID and SDK_VERSION on the command line.
else
  $(error Unknown PROFILE='$(PROFILE)'. Use simplicity_2026_6, gecko_4_5, or custom.)
endif

SDK_ID      ?= $(_PROFILE_SDK_ID)
SDK_VERSION ?= $(_PROFILE_SDK_VERSION)
EXT_VERSION ?= $(_PROFILE_EXT_VERSION)

ifeq ($(strip $(SDK_ID)),)
  $(error SDK_ID is empty. Pass SDK_ID=... or use PROFILE=simplicity_2026_6|gecko_4_5.)
endif
ifeq ($(strip $(SDK_VERSION)),)
  $(error SDK_VERSION is empty. Pass SDK_VERSION=... or use a known PROFILE.)
endif
ifeq ($(strip $(EXT_VERSION)),)
  EXT_VERSION := 2.0.0
endif
