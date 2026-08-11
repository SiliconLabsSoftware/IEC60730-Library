prepare:
	@rm -rf ./src
	@mkdir src
	@touch ./src/CMakeLists.txt
ifeq ($(TOOL_CHAINS),IAR)
	@echo "add_subdirectory(cmake_iar)" >> ./src/CMakeLists.txt
else
	@echo "add_subdirectory(cmake_gcc)" >> ./src/CMakeLists.txt
endif
vmc_support:
	@echo "add_subdirectory(lib_iec60730_vmc_support_cmake)" >> ./src/CMakeLists.txt
