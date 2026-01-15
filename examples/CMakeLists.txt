# Auto-discover all .cc files in examples/
file(GLOB EXAMPLE_SOURCES "*.cc")

foreach(example_source ${EXAMPLE_SOURCES})
    # Get filename without extension
    get_filename_component(example_name ${example_source} NAME_WE)
    
    # Create executable
    add_executable(${example_name} ${example_source})
    target_link_libraries(${example_name} PRIVATE gradual)
endforeach()
