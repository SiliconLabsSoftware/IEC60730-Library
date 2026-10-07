#  Exports:
#    - SLT_INSTALL_DIR : directory that contains the `slt` binary
#    - JAVA_HOME       : SLT's Java 21 root (`$(slt where java21)/jre`)
#    - SDK_PATH        : active SDK root (SSDK from SLT, or GSDK via set_gsdk.sh)
#    - TOOL_DIRS       : arm-none-eabi gcc bin directory
#    - TOOL_CHAINS     : GCC
#    - FLASH_REGIONS_TEST : flash start for CRC tests (BG21/BG24)
#    - POST_BUILD_EXE  : Simplicity Commander binary (if installed)
#    - PATH            : prepended with SLT-managed tools
#
#  SDK profiles:
#    - ssdk_2026_6 : Simplicity SDK (SimSDK) 2026.6.0 from SLT (`slt where simplicity-sdk`)
#    - gecko_4_5   : Gecko SDK 4.5.0 maintained on GitHub; resolved via script/set_gsdk.sh
#
#  Required: slc-cli, java21, gcc-arm-none-eabi, commander, ninja, cmake
#  Also verifies: srecord (apt / host package)
#
# This script must be sourced, not executed.
(return 0 2>/dev/null) || {
    echo "Error: this script must be sourced, not executed."
    echo "Run:   source ./script/set_env.sh"
    exit 1
}

_set_env_repo_root="$(dirname "$(dirname "$(realpath "${BASH_SOURCE[0]}")")")"

# Note: Load SDK settings for the selected profile. When sdk.env is missing
# (clean checkout), only create the default marker — do not run switch_sdk.py,
# which overwrites tracked project/source files from profile snapshots.
# Explicit profile switches stay behind: make apply-sdk-profile PROFILE=...
if [ ! -f "${_set_env_repo_root}/sdk.env" ]; then
    echo "sdk.env not found; using default profile ssdk_2026_6."
    echo "Tracked sources are left unchanged. To switch SDK profiles, run:"
    echo "  make apply-sdk-profile PROFILE=<ssdk_2026_6|gecko_4_5>"

    cp \
        "${_set_env_repo_root}/sdk_profiles/ssdk_2026_6/sdk.env.patch" \
        "${_set_env_repo_root}/sdk.env" || return 1
fi

# Profiles often set SDK_PATH= (empty) meaning "resolve later". Preserve a
# non-empty caller/Compose override across sourcing sdk.env.
_set_env_sdk_path_preset="${SDK_PATH-}"
source "${_set_env_repo_root}/sdk.env"
if [ -n "${_set_env_sdk_path_preset}" ]; then
    SDK_PATH="${_set_env_sdk_path_preset}"
fi
unset _set_env_sdk_path_preset


_set_env_resolve_slt_dir()
{
    local search_root="${1:-${HOME}/.silabs}"

    #
    # Prefer an explicit install root (e.g. Compose SLT_INSTALL_DIR=/opt/silabs).
    # Require a regular file: ~/.silabs/slt is often an SLT data directory
    # (installs/engines), and -x alone is true for directories.
    #
    if [ -f "${search_root}/bin/slt" ] && [ -x "${search_root}/bin/slt" ]; then
        echo "${search_root}/bin"
        return 0
    fi

    if [ -f "${search_root}/slt" ] && [ -x "${search_root}/slt" ]; then
        echo "${search_root}"
        return 0
    fi

    #
    # New Simplicity Installer layout under HOME
    #
    if [ -f "${HOME}/.silabs/bin/slt" ] && [ -x "${HOME}/.silabs/bin/slt" ]; then
        echo "${HOME}/.silabs/bin"
        return 0
    fi

    #
    # Legacy layout
    #
    local slt_bin

    slt_bin="$(find "${HOME}/.silabs" \
        -type f \
        -path "*/slt-cli-*/slt" \
        2>/dev/null | head -n 1)"

    if [ -n "${slt_bin}" ]; then
        dirname "${slt_bin}"
        return 0
    fi

    return 1
}

# Honor a pre-set SLT_INSTALL_DIR (Compose/CI), otherwise discover under HOME.
if [ -n "${SLT_INSTALL_DIR:-}" ]; then
    _set_env_slt_hint="${SLT_INSTALL_DIR}"
    if ! SLT_INSTALL_DIR="$(_set_env_resolve_slt_dir "${_set_env_slt_hint}")"; then
        echo "Error: SLT_INSTALL_DIR is set but no slt binary was found under it."
        echo "  SLT_INSTALL_DIR was: ${_set_env_slt_hint}"
        unset _set_env_repo_root _set_env_slt_hint
        unset -f _set_env_resolve_slt_dir
        return 1
    fi
    unset _set_env_slt_hint
elif ! SLT_INSTALL_DIR="$(_set_env_resolve_slt_dir)"; then
    echo "Error: SLT is not installed."
    echo "Run:   ./script/bootstrap silabs"
    unset _set_env_repo_root
    unset -f _set_env_resolve_slt_dir
    return 1
fi
export SLT_INSTALL_DIR

# recipe.slconf is committed with the paths of the Docker/CI image. On a native
# host those directories are absent, and both `slt where` and `slc generate`
# fail on them, so the file is only used when it resolves on this machine.
_set_env_slconf_usable() {
    local file="$1"
    local path
    local count=0

    [ -f "${file}" ] || return 1

    while IFS= read -r path; do
        [ -n "${path}" ] || continue
        count=$((count + 1))
        [ -d "${path}" ] || return 1
    done < <(sed 's/#.*//' "${file}" | grep -oE '"[^"]+"' | tr -d '"')

    [ "${count}" -gt 0 ]
}

if _set_env_slconf_usable "${_set_env_repo_root}/recipe.slconf"; then
    _set_env_slconf="${_set_env_repo_root}/recipe.slconf"
else
    _set_env_slconf=""
fi

_set_env_slt_where() {
    if [ -n "${_set_env_slconf}" ]; then
        "${SLT_INSTALL_DIR}/slt" where "$1" --slconf "${_set_env_slconf}" 2>/dev/null
    else
        "${SLT_INSTALL_DIR}/slt" where "$1" --ignore-slconf 2>/dev/null
    fi
}

# The directory has to end up ahead of /usr/bin even when it is already on PATH:
# `slc` also ships with heimdal-multidev as /usr/bin/slc, and that binary fails
# `slc generate` with "No such file or directory".
_set_env_path_prepend() {
    local dir="$1"
    local kept=""
    local entry
    local IFS=:

    for entry in ${PATH}; do
        [ -n "${entry}" ] || continue
        [ "${entry}" = "${dir}" ] && continue
        kept="${kept:+${kept}:}${entry}"
    done

    PATH="${dir}${kept:+:${kept}}"
}

_set_env() {
    local name="$1"
    local subdir="$2"
    local dir
    dir="$(_set_env_slt_where "${name}")"
    if [ -z "${dir}" ] || [ ! -d "${dir}${subdir}" ]; then
        echo "  [FAIL] ${name}: not installed (run './script/bootstrap silabs')"
        _set_env_failed=1
        return 1
    fi
    _set_env_path_prepend "${dir}${subdir}"
    echo "  [ OK ] ${name}: ${dir}${subdir}"
    printf -v "_set_env_dir_${name//-/_}" '%s' "${dir}"
}

_set_env_failed=0

echo "Setting environment from SLT (${SLT_INSTALL_DIR}):"
_set_env "slc-cli"           ""
_set_env "java21"            "/jre/bin"
_set_env "gcc-arm-none-eabi" "/bin"
_set_env "commander"         ""
_set_env "ninja"             ""
_set_env "cmake"             "/bin"

if [ -n "${_set_env_dir_java21:-}" ]; then
    export JAVA_HOME="${_set_env_dir_java21}/jre"
fi

if command -v srec_cat >/dev/null 2>&1 || command -v srecord >/dev/null 2>&1; then
    echo "  [ OK ] srecord: $(command -v srec_cat 2>/dev/null || command -v srecord)"
else
    echo "  [FAIL] srecord: not found (apt install srecord)"
    _set_env_failed=1
fi

export PATH

if [ -n "${_set_env_dir_slc_cli:-}" ]; then
    _set_env_slc="$(command -v slc 2>/dev/null || true)"
    if [ "${_set_env_slc}" != "${_set_env_dir_slc_cli}/slc" ]; then
        echo "  [FAIL] slc resolves to ${_set_env_slc:-<none>}, expected ${_set_env_dir_slc_cli}/slc"
        _set_env_failed=1
    fi
    unset _set_env_slc
fi

if [ "${_set_env_failed}" -ne 0 ]; then
    echo "One or more required tools are missing. Run ./script/bootstrap silabs."
    unset _set_env_repo_root _set_env_failed _set_env_slconf
    unset _set_env_dir_slc_cli _set_env_dir_java21 _set_env_dir_gcc_arm_none_eabi
    unset _set_env_dir_commander _set_env_dir_ninja _set_env_dir_cmake
    unset -f _set_env_resolve_slt_dir _set_env_slt_where _set_env_path_prepend _set_env
    unset -f _set_env_slconf_usable
    return 1
fi

# SDK root. sdk.env (written by `make apply-sdk-profile`) selects the profile:
#   gecko_4_5   -> script/set_gsdk.sh (GSDK_PATH / mount / cache / download)
#   otherwise   -> explicit SDK_PATH, else SLT-managed simplicity-sdk
if [ "${SDK_PROFILE:-}" = "gecko_4_5" ]; then
    # shellcheck source=set_gsdk.sh
    if ! source "${_set_env_repo_root}/script/set_gsdk.sh"; then
        echo "Error: could not resolve Gecko SDK for profile gecko_4_5."
        echo "Set GSDK_PATH to an existing Gecko SDK 4.5.0 root, or allow the download into \${XDG_CACHE_HOME:-~/.cache}/iec60730."
        unset _set_env_repo_root _set_env_failed _set_env_slconf
        unset _set_env_dir_slc_cli _set_env_dir_java21 _set_env_dir_gcc_arm_none_eabi
        unset _set_env_dir_commander _set_env_dir_ninja _set_env_dir_cmake
        unset -f _set_env_resolve_slt_dir _set_env_slt_where _set_env_path_prepend _set_env
        unset -f _set_env_slconf_usable
        return 1
    fi
elif [ -z "${SDK_PATH:-}" ]; then
    SDK_PATH="$(_set_env_slt_where simplicity-sdk)"
fi
export SDK_PATH

# Note: Toolchain paths used by CMake and build scripts.
export TOOL_DIRS="${_set_env_dir_gcc_arm_none_eabi}/bin"
export TOOL_CHAINS="${TOOL_CHAINS:-GCC}"


# Preserve FLASH_REGIONS_TEST from the build environment. Keep it defined
# (possibly empty) so nounset-safe callers do not abort; CMake treats empty
# as "use board/default fallback".
export FLASH_REGIONS_TEST="${FLASH_REGIONS_TEST:-}"

# Note: Ensure Docker containers run with the same IDs
export DOCKER_UID=$(id -u)
export DOCKER_GID=$(id -g)

if [ -n "${_set_env_slconf}" ]; then
    export SLC_SLCONF="${_set_env_slconf}"
else
    # Let CMake generate a machine-local user.slconf from SDK_PATH instead.
    unset SLC_SLCONF
fi

if [ -n "${_set_env_dir_commander:-}" ]; then
    if [ -x "${_set_env_dir_commander}/commander" ]; then
        export POST_BUILD_EXE="${_set_env_dir_commander}/commander"
    elif [ -x "${_set_env_dir_commander}/Commander.bin/commander" ]; then
        export POST_BUILD_EXE="${_set_env_dir_commander}/Commander.bin/commander"
    fi
fi

echo "Environment ready."
echo "  SLT_INSTALL_DIR=${SLT_INSTALL_DIR}"
echo "  JAVA_HOME=${JAVA_HOME}"
echo "  SDK_PATH=${SDK_PATH}"
echo "  TOOL_DIRS=${TOOL_DIRS}"
echo "  TOOL_CHAINS=${TOOL_CHAINS}"
echo "  FLASH_REGIONS_TEST=${FLASH_REGIONS_TEST}"
if [ -n "${SLC_SLCONF:-}" ]; then
    echo "  SLC_SLCONF=${SLC_SLCONF}"
else
    echo "  SLC_SLCONF=<unset> (recipe.slconf is Docker-only here; CMake will generate user.slconf from SDK_PATH)"
fi
if [ -n "${POST_BUILD_EXE:-}" ]; then
    echo "  POST_BUILD_EXE=${POST_BUILD_EXE}"
fi

unset _set_env_repo_root _set_env_failed _set_env_slconf
unset _set_env_dir_slc_cli _set_env_dir_java21 _set_env_dir_gcc_arm_none_eabi
unset _set_env_dir_commander _set_env_dir_ninja _set_env_dir_cmake
unset -f _set_env_resolve_slt_dir _set_env_slt_where _set_env_path_prepend _set_env
unset -f _set_env_slconf_usable
