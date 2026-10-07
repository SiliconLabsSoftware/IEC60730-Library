# Support Calculating CRC with SRecord

## Install

**Linux:**

```sh
sudo apt install srecord
```

**Windows:** download and install SRecord from [https://srecord.sourceforge.net/](https://srecord.sourceforge.net/).

## Script arguments

CRC helper scripts (`sl_iec60730_cal_crc16.sh` / `sl_iec60730_cal_crc32.sh`) expect:

| Position | Argument | Description |
| --- | --- | --- |
| `$1` | `PROJ_NAME` | Project / artifact base name |
| `$2` | `BUILD_DIR` | Directory containing `*.bin`, `*.hex`, `*.s37`, and `*.map` |
| `$3` | `SREC_PATH` | Path to the SRecord `bin` directory (use `""` on Linux if already on `PATH`) |
| `$4` | `TOOL_CHAINS` | Toolchain identifier (`GCC`) |
| `$5` | address list | Flash start address, or start/end pairs for multiple regions |

### Examples

Single continuous region:

```sh
bash sl_iec60730_cal_crc16.sh "${PROJ_NAME}" "${BUILD_DIR}" "C:\srecord\bin" GCC "0x8000000"
```

Multiple regions (start/end pairs):

```sh
bash sl_iec60730_cal_crc16.sh "${PROJ_NAME}" "${BUILD_DIR}" "C:\srecord\bin" GCC "0x8000000 0x8000050 0x80000a0 0x80000f0 0x8000140 0x8000190"
```

> [!NOTE]
> For multiple regions, provide start and end addresses for each zone. The example above covers three ranges: `0x8000000–0x8000050`, `0x80000a0–0x80000f0`, and `0x8000140–0x8000190`.
>
> `${BUILD_DIR}` must contain `*.bin`, `*.hex`, and `*.s37`, plus a `*.map` file so the script can locate the `check_sum` symbol used to store the reference CRC.
>
> Keep the address list identical to the Flash IMC regions configured in OEM code. See [IEC60730 safety library integration to SDK](./iec60730_safety_library_integration_to_sdk.md).
