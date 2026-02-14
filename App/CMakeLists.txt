set(SOURCES "src/Main.cpp" "src/MenuLayer.h" "src/MenuLayer.cpp" "src/SimLayer.h" "src/SimLayer.cpp")
    
add_executable(App)

target_sources(App PRIVATE ${SOURCES})

# Match VS filters to directory structure on disk
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

target_link_libraries(App Core)

target_include_directories(App PRIVATE src)

# Set working directory for VS debugger
set_target_properties(App PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/App"
)

#set asset directory
set(ASSET_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")

# set output directory for App
set(TARGET_OUTPUT_DIR $<TARGET_FILE_DIR:App> )

# Add a post-build command to copy assets directory
#add_custom_command(
#    TARGET App
#    POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#    "${ASSET_DIR}"
#    "${TARGET_OUTPUT_DIR}/assets"
#    COMMENT "Copying assets to the output directory"
#)