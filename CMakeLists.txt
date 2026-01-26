# CMakeList.txt : CMake project for reflex-engine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.11)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project (reflex-engine)

include (Dependencies.cmake)

add_subdirectory(Core)
add_subdirectory(App)

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html") # Tell Emscripten to build an example.html file.
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

