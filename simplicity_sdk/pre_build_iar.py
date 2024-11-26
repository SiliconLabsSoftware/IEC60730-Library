import os
import sys
import fileinput

# Read in the file
full_path = os.path.realpath(__file__)
dir_path = os.path.dirname(full_path)
lib_iec60730_file = dir_path + "/src/lib_iec60730_cmake/lib_iec60730.cmake"
text_search = "sl_udelay_armv6m_gcc.S"
text_replace = "sl_udelay_armv6m_iar.s"
is_crc32 = False
is_coretex_m33 = False
is_secure_peripherals = False
checksum_flags = ""
build_option = ""
end_of_str = "\n\
)\n\
"
line_replace = 0



cortex_m33_option_str ="\n\
target_link_libraries(slc_lib_iec60730 PUBLIC\n\
    --cpu=Cortex-M33.fp\n\
    --fpu=VFPv5-SP\n\
    --error_limit=3\n\
    --use_optimized_variants=auto\n\
)\n\
target_compile_options(slc_lib_iec60730 PUBLIC\n\
    $<$<COMPILE_LANGUAGE:C>:--cpu=Cortex-M33.fp --fpu=VFPv5-SP --cmse --debug --cpu_mode=thumb -On -e --char_is_unsigned --header_context --error_limit=3>\n\
    $<$<COMPILE_LANGUAGE:CXX>:--cpu=Cortex-M33.fp --fpu=VFPv5-SP --cmse --debug --cpu_mode=thumb -On -e --char_is_unsigned --header_context --error_limit=3>\n\
    $<$<COMPILE_LANGUAGE:ASM>:--cpu Cortex-M33.fp --fpu VFPv5-SP -s+ -r >\n\
)\n\
\n\
set(post_build_command )\n\
set_property(TARGET slc_lib_iec60730 PROPERTY C_STANDARD 99)\n\
set_property(TARGET slc_lib_iec60730 PROPERTY CXX_STANDARD 11)\n\
set_property(TARGET slc_lib_iec60730 PROPERTY CXX_EXTENSIONS OFF)\n\
\n\
target_link_options(slc_lib_iec60730 INTERFACE\n\
    --cpu=Cortex-M33.fp\n\
    --fpu=VFPv5-SP\n\
    --error_limit=3\n\
    --use_optimized_variants=auto\n\
    --config ${CMAKE_CURRENT_LIST_DIR}/../autogen/linkerfile_iec60730.icf\n\
    --no_wrap_diagnostics --map ""$<TARGET_FILE_DIR:lib_iec60730>/lib_iec60730.map""\n"

cortex_m4_option_str ="\n\
target_link_libraries(slc_lib_iec60730 PUBLIC\n\
    --cpu=Cortex-M4\n\
    --fpu=VFPv4-SP\n\
    --error_limit=3\n\
    --use_optimized_variants=auto\n\
)\n\
target_compile_options(slc_lib_iec60730 PUBLIC\n\
    $<$<COMPILE_LANGUAGE:C>:--cpu=Cortex-M4 --fpu=VFPv4-SP --debug --cpu_mode=thumb -On -e --char_is_unsigned --header_context --error_limit=3>\n\
    $<$<COMPILE_LANGUAGE:CXX>:--cpu=Cortex-M4 --fpu=VFPv4-SP --debug --cpu_mode=thumb -On -e --char_is_unsigned --header_context --error_limit=3>\n\
    $<$<COMPILE_LANGUAGE:ASM>:--cpu Cortex-M4 --fpu VFPv4-SP -s+ -r >\n\
)\n\
\n\
set(post_build_command )\n\
set_property(TARGET slc_lib_iec60730 PROPERTY C_STANDARD 99)\n\
set_property(TARGET slc_lib_iec60730 PROPERTY CXX_STANDARD 11)\n\
set_property(TARGET slc_lib_iec60730 PROPERTY CXX_EXTENSIONS OFF)\n\
\n\
target_link_options(slc_lib_iec60730 INTERFACE\n\
    --cpu=Cortex-M4\n\
    --fpu=VFPv4-SP\n\
    --error_limit=3\n\
    --use_optimized_variants=auto\n\
    --config ${CMAKE_CURRENT_LIST_DIR}/../autogen/linkerfile_iec60730.icf\n\
    --no_wrap_diagnostics --map ""$<TARGET_FILE_DIR:lib_iec60730>/lib_iec60730.map""\n"

def find_line_build_option(file_open):
  with open(file_open, 'r') as file:
    # read all lines using readline()
    lines = file.readlines()
    for row in lines:
    # check target_link_libraries line
      word = "target_link_libraries"
      if row.find(word) != -1:
        file.close()
        return lines.index(row)

def delete_old_build_option(file_open,line_replace):
  if line_replace != 0 :
    with open(file_open, "r") as file:
      lines = file.readlines()
      # position
      position = 1
    with open(file_open, "w") as file:
        for row in lines:
            if position < line_replace:
                file.write(row)
            position +=1
    file.close()
  #print("-- [I] Deleted old build option done!\n")

def add_new_build_option(file_open, build_option):
  with open(file_open, 'a') as file:
    file.write(build_option)
  file.close()

def replace_name_file(file_open,text_search,text_replace):
  with fileinput.FileInput(file_open, "w") as file:
    for line in file:
        print(line.replace(text_search, text_replace), end='')
  file.close()

if __name__ == "__main__":

  # Setup

  while len(sys.argv) > 1:
    line = sys.argv.pop()

    if "CAL_CRC_32=enable" in line:
      is_crc32 = True
    if "CORE_TYPE=m33" in line:
      is_coretex_m33 = True
    if "SECURE_PERIPHERALS_ENABLE=enable" in line:
      is_secure_peripherals = True
  print("-- [I] Start run pre_build_iar!")

  #print("-- [I] CORE_TYPE=M33\n") if is_coretex_m33 else print("-- [I] CORE_TYPE=M4\n")
  #print("-- [I] CAL_CRC_32=enable\n") if is_crc32 else print("-- [I] CAL_CRC_32=disable\n")
  if is_crc32:
    checksum_flags = "--keep check_sum --place_holder check_sum,4,.checksum,64"
  else:
    checksum_flags = "--keep check_sum --place_holder check_sum,2,.checksum,64"

  if is_coretex_m33:
    build_option = cortex_m33_option_str + "    " + checksum_flags + end_of_str
  else:
    build_option = cortex_m4_option_str + "    " + checksum_flags + end_of_str
  #print(build_option)

  # Pre-build

  if is_secure_peripherals:
    lib_iec60730_vmc_support_cmake = dir_path + "/src/lib_iec60730_vmc_support_cmake/lib_iec60730_vmc_support.cmake"

    line_replace = find_line_build_option(lib_iec60730_vmc_support_cmake)
    if line_replace == None:
      print("-- [I] Could not found line with word: target_link_libraries in " + lib_iec60730_vmc_support_cmake)
    else:
      #print("-- [I] Found line with word: target_link_libraries in " + lib_iec60730_vmc_support_cmake)
      #print("-- [I] Found in line:",line_replace)
      delete_old_build_option(lib_iec60730_vmc_support_cmake, line_replace)

    add_new_build_option(lib_iec60730_vmc_support_cmake, build_option)
    replace_name_file(lib_iec60730_vmc_support_cmake,text_search,text_replace)
    replace_name_file(lib_iec60730_vmc_support_cmake, "slc_lib_iec60730_vmc_support", "slc_lib_iec60730")
    replace_name_file(lib_iec60730_vmc_support_cmake, "slc_lib_iec60730", "slc_lib_iec60730_vmc_support")

  line_replace = find_line_build_option(lib_iec60730_file)
  if line_replace == None:
    print("-- [I] Could not found line with word: target_link_libraries in " + lib_iec60730_file)
  else:
    #print("-- [I] Found line with word: target_link_libraries in " + lib_iec60730_file)
    #print("-- [I] Found in line:",line_replace)
    delete_old_build_option(lib_iec60730_file, line_replace)


  add_new_build_option(lib_iec60730_file, build_option)
  replace_name_file(lib_iec60730_file,text_search,text_replace)

  print("-- [I] Run pre_build_iar done!")