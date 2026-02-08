set(SOURCES
    src/Main.cpp
    
 "src/MenuLayer.h" "src/MenuLayer.cpp")

add_executable(App)

target_sources(App PRIVATE ${SOURCES} "src/MenuLayer.h" "src/MenuLayer.cpp"  )

# Match VS filters to directory structure on disk
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

target_link_libraries(App Core)

target_include_directories(App PRIVATE src)

# Set working directory for VS debugger
set_target_properties(App PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/App"
)