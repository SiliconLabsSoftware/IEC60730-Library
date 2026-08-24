if(ENABLE_UNIT_TESTING)
    add_subdirectory(unit_test)
endif()

if(ENABLE_INTEGRATION_TESTING)
    add_subdirectory(integration_test)
endif()
