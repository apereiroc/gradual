find_package(Catch2 3 REQUIRED)
include(Catch)

# Auto-discover all .cc files in tests/
file(GLOB TEST_SOURCES "*.cc")

foreach(test_source ${TEST_SOURCES})
    # Get filename without extension
    get_filename_component(test_name ${test_source} NAME_WE)
    
    # Create test executable
    add_executable(${test_name} ${test_source})
    target_link_libraries(${test_name} PRIVATE gradual Catch2::Catch2WithMain)
    
    # Register with CTest
    catch_discover_tests(${test_name})
endforeach()
