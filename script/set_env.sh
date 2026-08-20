#  Exports:
#    - SLT_INSTALL_DIR : directory that contains the `slt` binary
#    - JAVA_HOME       : SLT's Java 21 root (`$(slt where java21)/jre`)
#    - SDK_PATH        : Simplicity SDK root from SLT
#    - TOOL_DIRS       : arm-none-eabi gcc bin directory
#    - TOOL_CHAINS     : GCC
#    - FLASH_REGIONS_TEST : flash start for brd4264c CRC tests
#    - POST_BUILD_EXE  : Simplicity Commander binary (if installed)
#    - PATH            : prepended with SLT-managed tools
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

_set_env_resolve_slt_dir() {
    local slt_location="${HOME}/.silabs/slt/slt.location"
    local candidate=""
    if [ -f "${slt_location}" ]; then
        candidate="$(dirname "$(cat "${slt_location}")")"
    fi
    if [ -n "${candidate}" ] && "${candidate}/slt" --version >/dev/null 2>&1; then
        echo "${candidate}"
        return 0
    fi
    if [ -n "${SLT_INSTALL_DIR:-}" ] && "${SLT_INSTALL_DIR}/slt" --version >/dev/null 2>&1; then
        echo "${SLT_INSTALL_DIR}"
        return 0
    fi
    # Common layout after install_slt: ${SLT_INSTALL_DIR}/bin/slt
    if [ -n "${SLT_INSTALL_DIR:-}" ] && "${SLT_INSTALL_DIR}/bin/slt" --version >/dev/null 2>&1; then
        echo "${SLT_INSTALL_DIR}/bin"
        return 0
    fi
    return 1
}

if ! SLT_INSTALL_DIR="$(_set_env_resolve_slt_dir)"; then
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

export SDK_PATH="$(_set_env_slt_where simplicity-sdk)"
export TOOL_DIRS="${_set_env_dir_gcc_arm_none_eabi}/bin"
export TOOL_CHAINS="${TOOL_CHAINS:-GCC}"
export FLASH_REGIONS_TEST="${FLASH_REGIONS_TEST:-0x8000000}"
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
