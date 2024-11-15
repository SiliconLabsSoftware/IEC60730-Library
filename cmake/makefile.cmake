
set(MAKEFILE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Make")
option(NON_SECURE_EN "NON_SECURE_EN"   FALSE)
option(APP_SECURE    "APP_SECURE"      FALSE)

if(DEFINED ENV{COMPONENT})
  set(COMPONENT "$ENV{COMPONENT}")
else()
  set(COMPONENT "cpu_registers")
  message("-- [I] Run cmake build . --target make_info for know componnents support. By default component is cpu_registers")
endif()

if(DEFINED ENV{COMPILER})
  set(COMPILER "$ENV{COMPILER}")
else()
  set(COMPILER "GCC")
  message("-- [I] Export COMPILER=<YOUR_COMPILERP> to choose compiler make build. By default is GCC")
endif()

if(DEFINED ENV{TOOL_DIRS})
  set(TOOL_DIRS "$ENV{TOOL_DIRS}")
else()
  set(TOOL_DIRS "${TOOLCHAIN_DIR}")
endif()

# run make info
add_custom_target(make_info
                  COMMAND make info
                  WORKING_DIRECTORY ${MAKEFILE_DIR}/
)

# run make build
if(DEFINED ENV{DERIVATIVE})
  set(DERIVATIVE "$ENV{DERIVATIVE}")
else()
  set(DERIVATIVE "efr32mg12p132f1024gl125")
  message("-- [I] Export DERIVATIVE=<NAME_YOUR_DEVICE_CHIP> to choose device make build. By default is efr32mg12p132f1024gl125")
endif()

add_custom_target(make_build
                  COMMAND make build COMPONENT=${COMPONENT} DERIVATIVE=${DERIVATIVE} NON_SECURE_EN=${NON_SECURE_EN} APP_SECURE=${APP_SECURE}
                  WORKING_DIRECTORY ${MAKEFILE_DIR}/
)



