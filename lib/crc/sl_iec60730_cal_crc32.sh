#!/bin/bash

PROJ_NAME=$1
BUILD_DIR=$2
SREC_PATH=$3
TOOL_CHAINS=$4
START_ADDR=0
SUFFIX=_crc32
NON_SECURE=0

START_ADDR=${5//"%20"/" "}
NUMBER_ELEMENT=0


#printf "$START_ADDR\n"

START_ARR_ADDR=()
END_ARR_ADDR=()
for END_ARR_ADDR in ${START_ADDR[@]}; do
  #echo $END_ARR_ADDR ;
  if [ "$NUMBER_ELEMENT" -eq 0 ];then
    START_ARR_ADDR=$END_ARR_ADDR;
  fi
  NUMBER_ELEMENT=$((NUMBER_ELEMENT+1));
done

#printf "ARR_ADDR: $ARR_ADDR\n"
#printf "START_ARR_ADDR: $START_ARR_ADDR\n"
#printf "NUMBER_ELEMENT: $NUMBER_ELEMENT\n"

# SUFFIX_SECURE=""
# if [ ! -z $6 ]; then
#   NON_SECURE=$6
#   echo NonSecure: ${NON_SECURE}
#   if [ "$NON_SECURE" = "true" ]; then
#     SUFFIX_SECURE="_NS"
#   fi
# fi

case $(uname | tr '[:upper:]' '[:lower:]') in
  linux*)
    OS_NAME="linux"
    ;;
  msys*)
    OS_NAME="windows"
    ;;
  *)
    OS_NAME="notset"
    ;;
esac

if [ -z ${BUILD_DIR} ]; then
  MAPFILE=${PROJ_NAME}.map
  INPUT_HEXFILE=${PROJ_NAME}.hex
  OUTPUT_HEXFILE=${PROJ_NAME}${SUFFIX}.hex
  OUTPUT_BINFILE=${PROJ_NAME}${SUFFIX}.bin
  OUTPUT_SRECFILE=${PROJ_NAME}${SUFFIX}.s37
  TEMP_FILE=tmpfile.txt
else
  MAPFILE=${BUILD_DIR}/${PROJ_NAME}.map
  INPUT_HEXFILE=${BUILD_DIR}/${PROJ_NAME}.hex
  OUTPUT_HEXFILE=${BUILD_DIR}/${PROJ_NAME}${SUFFIX}.hex
  OUTPUT_BINFILE=${BUILD_DIR}/${PROJ_NAME}${SUFFIX}.bin
  OUTPUT_SRECFILE=${BUILD_DIR}/${PROJ_NAME}${SUFFIX}.s37
  TEMP_FILE=${BUILD_DIR}/tmpfile.txt
fi

echo Proj: ${PROJ_NAME}
echo BuildDir: ${BUILD_DIR}
echo TempFile: ${TEMP_FILE}
echo MapFile: ${MAPFILE}
echo InputHex: ${INPUT_HEXFILE}
echo InputHex: ${OUTPUT_HEXFILE}
echo OutputBin: ${OUTPUT_BINFILE}
echo OutputSrec: ${OUTPUT_SRECFILE}
echo OS: ${OS_NAME}

if [ "${OS_NAME}" = "windows" ]; then
  SREC_CAT=${SREC_PATH}/srec_cat.exe
  SREC_CMP=${SREC_PATH}/srec_cmp.exe
elif [ "${OS_NAME}" = "linux" ]; then
  SREC_CAT=srec_cat
  SREC_CMP=srec_cmp
fi

if [ -e ${TEMP_FILE} ];then
  rm -rf ${TEMP_FILE}
fi
touch ${TEMP_FILE}

if [ "${TOOL_CHAINS}" = "IAR" ]; then
  grep -n "CHECKSUM" ${MAPFILE}>${TEMP_FILE}
  while IFS=' ' read -r a1 a2 a3 remainder; do
    if [ $a2 = "CHECKSUM" ]; then
      LINE_NO=${a1%%:*}
      # CRC_ADDR=$a3
      CRC_ADDR=$( echo "$a3" | sed s/\'//g)
    fi
  done < ${TEMP_FILE}
else # GCC
  grep -n "PROVIDE (check_sum = .)" ${MAPFILE}>${TEMP_FILE}
  # Find address of check_sum
  while IFS=' ' read -r a1 a2 remainder; do
    LINE_NO=${a1%%:*}
    CRC_ADDR=$a2
  done < ${TEMP_FILE}
fi

echo Start address: ${START_ADDR}
echo CRC32 address: ${CRC_ADDR} at LineNo: ${LINE_NO} of ${MAPFILE}

if [ "$(( $NUMBER_ELEMENT % 2 ))" -eq 0 ]; then

#printf "=== Calculate multiple regions: $START_ADDR and $END_ARR_ADDR to ${CRC_ADDR}\n"

${SREC_CAT} \
  ${INPUT_HEXFILE} -intel \
  -crop ${START_ADDR} \
  -crc32-le ${CRC_ADDR} \
  -o ${TEMP_FILE} -intel >/dev/null 2>&1

${SREC_CAT} \
  ${INPUT_HEXFILE} -intel \
  -exclude -within ${TEMP_FILE} -intel \
  ${TEMP_FILE} -intel \
  -o ${OUTPUT_HEXFILE} -intel

else

#printf "=== Calculate from ${START_ADDR} to ${CRC_ADDR}\n"

${SREC_CAT} \
  ${INPUT_HEXFILE} -intel \
  -crop ${START_ADDR} ${CRC_ADDR}\
  -fill 0xFF ${START_ADDR} ${CRC_ADDR}\
  -crc32-le ${CRC_ADDR} \
  -o ${TEMP_FILE} -intel

${SREC_CAT} \
  ${INPUT_HEXFILE} -intel \
  -exclude -within ${TEMP_FILE} -intel \
  ${TEMP_FILE} -intel \
  -o ${OUTPUT_HEXFILE} -intel

fi

rm -rf ${TEMP_FILE}

echo Comparing ...
${SREC_CMP} \
  ${INPUT_HEXFILE} -intel ${OUTPUT_HEXFILE} -intel -v

${SREC_CAT} \
  ${OUTPUT_HEXFILE} -intel \
  -offset -${START_ARR_ADDR} \
  -o ${OUTPUT_BINFILE} -binary

${SREC_CAT} \
  ${OUTPUT_HEXFILE} -intel \
  -o ${OUTPUT_SRECFILE} -address-length=4

echo Build done!
