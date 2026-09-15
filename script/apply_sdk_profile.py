import argparse
import re
from pathlib import Path

# Project files updated by the SDK profile migration.
FILES = [
    "iec60730.slce",
    "sample/iec60730_demo/iec60730_demo.slcp",
    "sample/iec60730_demo_cpp/iec60730_demo_cpp.slcp",
    "simplicity_sdk/lib_iec60730.slcp",
    "simplicity_sdk/lib_iec60730_secure.slcp",
    "simplicity_sdk/lib_iec60730_coding_convention.slcp",
]

# BG21/BG24 config_file overrides for library projects.
LIB_CONFIG_BLOCK = """
config_file:
  - path: ../sample/iec60730_demo/config/xg21/sl_board_control_config.h
    override:
      component: board_control
      file_id: board_control_config
    condition:
      - device_family_efr32bg21
    unless:
      - hardware_board

  - path: ../sample/iec60730_demo/config/xg21/sl_iostream_usart_vcom_config.h
    override:
      component: iostream_usart
      file_id: iostream_usart_config
      instance: vcom
    condition:
      - device_family_efr32bg21
    unless:
      - hardware_board

  - path: ../sample/iec60730_demo/config/xg24/sl_board_control_config.h
    override:
      component: board_control
      file_id: board_control_config
    condition:
      - device_family_efr32bg24
    unless:
      - hardware_board

  - path: ../sample/iec60730_demo/config/xg24/sl_iostream_usart_vcom_config.h
    override:
      component: iostream_usart
      file_id: iostream_usart_config
      instance: vcom
    condition:
      - device_family_efr32bg24
    unless:
      - hardware_board
""".strip()

def update_lib_project(text: str, sdk_id: str) -> str: 
    """Apply SDK-specific updates to IEC60730 library projects."""
    m = re.search(
        r"^project_name:\s*(\S+)\s*$",
        text,
        re.MULTILINE,
    )

    if not m:
        return text

    if m.group(1) not in {
        "lib_iec60730",
        "lib_iec60730_secure",
        "lib_iec60730_coding_convention",
    }:
        return text

    if sdk_id == "simplicity_sdk":
        text = re.sub(
            r"^(\s*)-\s*id:\s*device_init_hfrco\s*$",
            r"\1- id: clock_manager",
            text,
            flags=re.MULTILINE,
        )
 
    if sdk_id == "gecko_sdk":

        # Add board_control if missing
        if not re.search(
            r"^\s*-\s*id:\s*board_control\s*$",
            text,
            re.MULTILINE,
        ):
            text = re.sub(
                r"(^\s*-\s*id:\s*udelay\s*$)",
                (
                    r"\1\n"
                    "  # Required explicitly when generating for a device without a board BSP.\n"
                    "  - id: board_control"
                ),
                text,
                count=1,
                flags=re.MULTILINE,
            )

    if "SL_IEC60730_CRC_DEBUG_ENABLE" in text:
        text = re.sub(
            r"(- name:\s*SL_IEC60730_CRC_DEBUG_ENABLE\s*\n\s*value:\s*)\d+",
            r"\g<1>0",
            text,
        )

    # Insert config_file block before configuration
    if not re.search(
        r"^config_file:\s*$",
        text,
        re.MULTILINE,
    ):
        text = re.sub(
            r"^configuration:\s*$",
            LIB_CONFIG_BLOCK + "\n\nconfiguration:",
            text,
            count=1,
            flags=re.MULTILINE,
        )

    return text

def rewrite_file(
    text: str,
    sdk_id: str,
    sdk_version: str,
    ext_version: str,
) -> str:
    """Rewrite a project file for the selected SDK profile."""

    # Replace placeholder
    text = text.replace("@EXT_VERSION@", ext_version)

    # Apply SDK-specific vendor metadata updates.
    text = update_slce_vendor(text, sdk_id)

    # Apply project-specific migrations.
    text = update_lib_project(text, sdk_id)
    text = update_demo_project(text)
    text = update_demo_cpp_project(text)

    # Add project metadata sections.
    text = add_readme(text)
    text = add_ui_hints(text)

    # EXTRA_CONFIG_TEXT = build_extra_config_text()

    # Force iec60730 version
    if sdk_id == "simplicity_sdk":
        text = re.sub(
            r"\{id:\s*iec60730[^}]*\}",
            f"{{id: iec60730, version: {ext_version}, vendor: silabs}}",
            text,
        )
    else:
        text = re.sub(
            r"\{id:\s*iec60730[^}]*\}",
            f"{{id: iec60730, version: {ext_version}}}",
            text,
        )

    # sdk inline format
    if sdk_id == "simplicity_sdk":
        sdk_inline = (
            f"sdk: {{id: {sdk_id}, "
            f"version: {sdk_version}, "
            f"vendor: silabs}}"
        )
    else:
        sdk_inline = (
            f"sdk: {{id: {sdk_id}, "
            f"version: {sdk_version}}}"
        )

    text = re.sub(
        r"sdk:\s*\{id:\s*[^}]+\}",
        sdk_inline,
        text,
    )

    # sdk multiline format
    text = re.sub(
        r"sdk:\s*\n\s*id:\s*[^\n]+\n\s*version:\s*[^\n]+",
        f"sdk:\n  id: {sdk_id}\n  version: {sdk_version}",
        text,
        flags=re.MULTILINE,
    )

    if sdk_id == "gecko_sdk":

        text = re.sub(
            r"^(\s*)-\s*id:\s*clock_manager\s*$",
            r"\1- id: device_init_hfrco",
            text,
            flags=re.MULTILINE,
        )

        text = re.sub(
            r",\s*vendor:\s*silabs",
            "",
            text,
        )

        text = re.sub(
            r"\n\s*vendor:\s*silabs",
            "",
            text,
        )

    # Remove readme block
    text = re.sub(
        r"\nreadme:.*?(?=\nui_hints:|\ntoolchain_settings:)",
        "\n",
        text,
        flags=re.DOTALL,
    )

    # Remove ui_hints block
    text = re.sub(
        r"\nui_hints:.*?(?=\ntoolchain_settings:)",
        "\n",
        text,
        flags=re.DOTALL,
    )

    # Remove clock_manager comment
    text = re.sub(
        r"\n\s*# Prefer LFRCO for LF / WDOG \(no device_init_lfxo with clock_manager\)\.",
        "",
        text,
    )

    # Collapse multiple blank lines
    text = re.sub(
        r"\n{3,}",
        "\n\n",
        text,
    )

    # Remove trailing spaces
    text = "\n".join(
        line.rstrip()
        for line in text.splitlines()
    )

    # Ensure single newline at EOF
    text = text.rstrip() + "\n"

    return text

def update_slce_vendor(text: str, sdk_id: str) -> str:
    """Update SLCE vendor metadata for the target SDK."""
    if (
        'id: "iec60730"' not in text
        or "component_path:" not in text
    ):
        return text

    if sdk_id == "simplicity_sdk":

        if "vendor: silabs" not in text:
            text = re.sub(
                r'^(version:\s*[^\n]+)',
                r'\1\nvendor: silabs',
                text,
                count=1,
                flags=re.MULTILINE,
            )

        # Remove duplicate vendor entries in sdk block.
        text = re.sub(
            r'(\n\s*vendor:\s*silabs)\n\s*vendor:\s*silabs',
            r'\1',
            text,
        )

    elif sdk_id == "gecko_sdk":

        text = re.sub(
            r'^vendor:\s*silabs\s*\n',
            '',
            text,
            flags=re.MULTILINE,
        )

        text = re.sub(
            r'\n\s*vendor:\s*silabs',
            '',
            text,
        )

    return text

def update_demo_project(text: str) -> str:
    """Apply required updates to the IEC60730 demo project."""
    if not re.search(
        r"^project_name:\s*iec60730_example_demo\s*$",
        text,
        re.MULTILINE,
    ):
        return text

    # board_control
    if not re.search(
        r"^\s*-\s*id:\s*board_control\s*$",
        text,
        re.MULTILINE,
    ):
        text = re.sub(
            r"(^\s*-\s*id:\s*udelay\s*$)",
            r"\1\n"
            r"\n"
            r"  # oem_hal_config_board.h includes sl_board_control_config.h, which only\n"
            r"  # exists when board_control is part of the project.\n"
            r"  - id: board_control",
            text,
            count=1,
            flags=re.MULTILINE,
        )

    # config_file block
    if not re.search(
        r"^config_file:\s*$",
        text,
        re.MULTILINE,
    ):
        demo_config_block = """
config_file:
  - path: ./config/xg21/sl_board_control_config.h
    override:
      component: board_control
      file_id: board_control_config
    condition:
      - device_family_efr32bg21
    unless:
      - hardware_board

  - path: ./config/xg21/sl_iostream_usart_vcom_config.h
    override:
      component: iostream_usart
      file_id: iostream_usart_config
      instance: vcom
    condition:
      - device_family_efr32bg21
    unless:
      - hardware_board

  - path: ./config/xg24/sl_board_control_config.h
    override:
      component: board_control
      file_id: board_control_config
    condition:
      - device_family_efr32bg24
    unless:
      - hardware_board

  - path: ./config/xg24/sl_iostream_usart_vcom_config.h
    override:
      component: iostream_usart
      file_id: iostream_usart_config
      instance: vcom
    condition:
      - device_family_efr32bg24
    unless:
      - hardware_board
""".strip()

        text = re.sub(
            r"^configuration:\s*$",
            demo_config_block + "\n\nconfiguration:",
            text,
            count=1,
            flags=re.MULTILINE,
        )

    # remove invalid vendor under sdk
    text = re.sub(
        r"sdk:\s*\{id:\s*simplicity_sdk,\s*vendor:\s*silabs,\s*version:\s*([^}]+)\}",
        r"sdk: {id: simplicity_sdk, version: \1}",
        text,
    )

    # add SL_DEVICE_INIT_HFRCO_BAND if missing
    if "SL_DEVICE_INIT_HFRCO_BAND" not in text:
        text = re.sub(
            r"(- name:\s*SL_BOARD_ENABLE_VCOM\s*\n\s*value:\s*0)",
            r"\1\n"
            r"  - name: SL_DEVICE_INIT_HFRCO_BAND\n"
            r"    value: cmuHFRCOFreq_19M0Hz\n"
            r"    condition:\n"
            r"      - device_series_1",
            text,
            count=1,
        )

    return text

def update_demo_cpp_project(text: str) -> str:
    """Apply required updates to the IEC60730 C++ demo project."""
    if not re.search(
        r"^project_name:\s*iec60730_example_demo_cpp\s*$",
        text,
        re.MULTILINE,
    ):
        return text

    if "iec60730_stub_functions.c" not in text:
        text = text.replace(
            "- path: ./oem/src/oem_iec60730_timer.c",
            "- path: ./oem/src/iec60730_stub_functions.c\n"
            "  - path: ./oem/src/oem_iec60730_timer.c",
        )

    return text

def add_readme(text: str) -> str:
    """Add readme metadata if missing."""
    if "readme:" in text:
        return text

    text = re.sub(
        r"(^toolchain_settings:)",
        "readme:\n"
        "  - path: ./readme.html\n\n"
        r"\1",
        text,
        count=1,
        flags=re.MULTILINE,
    )

    return text

def add_ui_hints(text: str) -> str:
    """Add ui_hints metadata if missing."""
    if "ui_hints:" in text:
        return text

    text = re.sub(
        r"(^toolchain_settings:)",
        "ui_hints:\n"
        "  highlight:\n"
        "    - path: readme.html\n"
        "      focus: false\n\n"
        r"\1",
        text,
        count=1,
        flags=re.MULTILINE,
    )

    return text

# def build_extra_config_text() -> str:
#     """Build additional SDK configuration entries."""

#     return """
#   # CTS/RTS blocks USART_Tx when VCOM/board controller is not driven (COS stub).
#   - name: SL_IOSTREAM_USART_VCOM_FLOW_CONTROL_TYPE
#     value: usartHwFlowControlNone
#   - name: SL_CLOCK_MANAGER_DEFAULT_LF_CLOCK_SOURCE
#     value: SL_CLOCK_MANAGER_DEFAULT_LF_CLOCK_SOURCE_LFRCO
#   - name: SL_CLOCK_MANAGER_WDOG0CLK_SOURCE
#     value: CMU_WDOG0CLKCTRL_CLKSEL_LFRCO
#   - name: SL_CLOCK_MANAGER_WDOG1CLK_SOURCE
#     value: CMU_WDOG1CLKCTRL_CLKSEL_LFRCO
# """.strip()

def main():
    """Apply the selected SDK profile to all project files."""

    parser = argparse.ArgumentParser()
    parser.add_argument("--sdk-id", required=True)
    parser.add_argument("--sdk-version", required=True)
    parser.add_argument("--ext-version", required=True)
    args = parser.parse_args()
    root = Path.cwd()

    for rel_path in FILES:

        path = root / rel_path

        if not path.exists():
            print(f"[SKIP] {rel_path}")
            continue

        original = path.read_text(encoding="utf-8")

        updated = rewrite_file(
            original,
            args.sdk_id,
            args.sdk_version,
            args.ext_version,
        )

        if updated != original:
            path.write_text(updated, encoding="utf-8")
            print(f"[UPDATED] {rel_path}")
        else:
            print(f"[UNCHANGED] {rel_path}")

    print(
        f"Applied profile: "
        f"sdk_id={args.sdk_id}, "
        f"sdk_version={args.sdk_version}, "
        f"ext_version={args.ext_version}"
    )


if __name__ == "__main__":
    main()
