#!/usr/bin/env bash
#
# Resolve the Gecko SDK for the gecko_4_5 profile and export SDK_PATH.
#
# Resolution order:
#   1. GSDK_PATH            explicit override (host install, Studio tree, ...)
#   2. GSDK_MOUNT_DIR       pre-mounted SDK (Docker: ${GSDK_PATH}:/opt/gecko_sdk)
#   3. cache                ${XDG_CACHE_HOME:-~/.cache}/iec60730/gecko-sdk/<ver>
#   4. download             gecko-sdk.zip from GSDK_URL into the cache
#
# This script must be sourced; it sets SDK_PATH in the caller and returns
# non-zero on failure. It must not use `set -e`/`exit`, since those would
# affect the sourcing shell (set_env.sh, bootstrap_silabs).
(return 0 2>/dev/null) || {
    echo "Error: this script must be sourced, not executed."
    echo "Run:   source ./script/set_gsdk.sh"
    exit 1
}

_gsdk_repo_root="$(dirname "$(dirname "$(realpath "${BASH_SOURCE[0]}")")")"

# shellcheck source=../sdk_profiles/gecko_4_5/source_gsdk.path
source "${_gsdk_repo_root}/sdk_profiles/gecko_4_5/source_gsdk.path"

_gsdk_cache_root="${IEC60730_CACHE_DIR:-${XDG_CACHE_HOME:-${HOME}/.cache}/iec60730}"
_gsdk_cache_dir="${_gsdk_cache_root}/${GSDK_CACHE_SUBDIR}/${GSDK_VERSION}"
_gsdk_zip="${_gsdk_cache_root}/downloads/gecko-sdk-${GSDK_VERSION}.zip"

_gsdk_is_sdk() {
    [ -n "${1:-}" ] && [ -f "${1}/${GSDK_MARKER}" ]
}

_gsdk_download() {
    local part="${_gsdk_zip}.part"

    mkdir -p "$(dirname "${_gsdk_zip}")" || return 1

    if [ -f "${_gsdk_zip}" ]; then
        if unzip -tq "${_gsdk_zip}" >/dev/null 2>&1; then
            return 0
        fi
        echo "Cached archive is corrupt, re-downloading: ${_gsdk_zip}"
        rm -f "${_gsdk_zip}"
    fi

    echo "Downloading Gecko SDK ${GSDK_VERSION} (~1.2 GB):"
    echo "  ${GSDK_URL}"
    echo "  -> ${_gsdk_zip}"

    # -c resumes a previous partial download.
    if ! wget -c -O "${part}" "${GSDK_URL}"; then
        echo "Error: download failed. Retry, or set GSDK_PATH to an existing Gecko SDK ${GSDK_VERSION}."
        return 1
    fi
    mv "${part}" "${_gsdk_zip}" || return 1

    local sum
    sum="$(sha256sum "${_gsdk_zip}" | cut -d' ' -f1)"
    if [ -n "${GSDK_SHA256}" ]; then
        if [ "${sum}" != "${GSDK_SHA256}" ]; then
            echo "Error: sha256 mismatch for ${_gsdk_zip}"
            echo "  expected ${GSDK_SHA256}"
            echo "  actual   ${sum}"
            rm -f "${_gsdk_zip}"
            return 1
        fi
        echo "  sha256 OK"
    else
        echo "  sha256 ${sum}  (pin it as GSDK_SHA256 in sdk_profiles/gecko_4_5/source_gsdk.path)"
    fi
}

_gsdk_extract() {
    local tmp="${_gsdk_cache_dir}.tmp"

    rm -rf "${tmp}" "${_gsdk_cache_dir}"
    mkdir -p "${tmp}" || return 1

    echo "Extracting to ${_gsdk_cache_dir} ..."
    if ! unzip -q "${_gsdk_zip}" -d "${tmp}"; then
        rm -rf "${tmp}"
        echo "Error: failed to extract ${_gsdk_zip}"
        return 1
    fi

    # Accept either files at the archive root or a single top-level folder.
    if _gsdk_is_sdk "${tmp}"; then
        mv "${tmp}" "${_gsdk_cache_dir}" || return 1
    else
        local sub
        sub="$(find "${tmp}" -mindepth 1 -maxdepth 1 -type d | head -n 1)"
        if _gsdk_is_sdk "${sub}"; then
            mv "${sub}" "${_gsdk_cache_dir}" && rm -rf "${tmp}" || return 1
        else
            rm -rf "${tmp}"
            echo "Error: ${GSDK_MARKER} not found in archive; not a Gecko SDK."
            return 1
        fi
    fi
}

_gsdk_resolve() {
    if [ -n "${GSDK_PATH:-}" ]; then
        if _gsdk_is_sdk "${GSDK_PATH}"; then
            SDK_PATH="${GSDK_PATH}"
            echo "  [ OK ] gecko-sdk: ${SDK_PATH} (GSDK_PATH)"
            return 0
        fi
        echo "  [WARN] GSDK_PATH=${GSDK_PATH} has no ${GSDK_MARKER}; ignoring."
    fi

    if _gsdk_is_sdk "${GSDK_MOUNT_DIR}"; then
        SDK_PATH="${GSDK_MOUNT_DIR}"
        echo "  [ OK ] gecko-sdk: ${SDK_PATH} (mounted)"
        return 0
    fi

    if _gsdk_is_sdk "${_gsdk_cache_dir}"; then
        SDK_PATH="${_gsdk_cache_dir}"
        echo "  [ OK ] gecko-sdk: ${SDK_PATH} (cache)"
        return 0
    fi

    echo "Gecko SDK ${GSDK_VERSION} not found (GSDK_PATH unset, no cache at ${_gsdk_cache_dir})."
    _gsdk_download || return 1
    _gsdk_extract || return 1

    SDK_PATH="${_gsdk_cache_dir}"
    echo "  [ OK ] gecko-sdk: ${SDK_PATH} (downloaded)"
}

_gsdk_resolve
_gsdk_rc=$?
[ "${_gsdk_rc}" -eq 0 ] && export SDK_PATH

unset _gsdk_repo_root _gsdk_cache_root _gsdk_cache_dir _gsdk_zip
unset -f _gsdk_is_sdk _gsdk_download _gsdk_extract _gsdk_resolve

# Keep the pin variables out of the caller's environment.
unset GSDK_VERSION GSDK_URL GSDK_SHA256 GSDK_CACHE_SUBDIR GSDK_MARKER GSDK_MOUNT_DIR

if [ "${_gsdk_rc}" -ne 0 ]; then
    unset _gsdk_rc
    return 1
fi
unset _gsdk_rc
