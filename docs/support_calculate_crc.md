# Support calculate CRC by Srecord

## Install

On Linux

```sh
sudo apt install srecord
```

On Window: Download Srecord to install

## Arguments to bash script

```sh
PROJ_NAME=$1
BUILD_DIR=$2
SREC_PATH=$3
TOOL_CHAINS=$4
START_ADDR=$6
```

For example:

```sh
$ bash sl_iec60730_cal_crc16.sh "${PROJ_NAME}" "${BUILD_DIR}" "C:\srecord\bin" GCC "0x8000000"
```

 Or if you want to calculate multiple regions:

```sh
$ bash sl_iec60730_cal_crc16.sh "${PROJ_NAME}" "${BUILD_DIR}" "C:\srecord\bin" GCC "0x8000000 0x8000050 0x80000a0 0x80000f0 0x8000140 0x8000190"
```

> [!NOTE]
> If you calculate multiple regions CRC, you need to enter the start and end addresses of each zone. For example, in the above command, srec will calculate a three-range CRC: 0x8000000 - 0x8000050; 0x80000a0 - 0x80000f0; 0x8000140 - 0x8000190.
>
> Path `${BUILD_DIR}` have the files `*.bin`, `*.hex`, and `*.s37`. A `*.map` file is also needed in this directory, so that bash can check to find the address of the `check_sum` variable to store the calculated CRC value.