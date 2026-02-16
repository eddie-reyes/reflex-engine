set(SOURCES "src/Application.h" "src/Application.cpp" "src/Layer.h" "src/Layer.cpp" "src/Event.h" "src/InputEvents.h"  "src/Button.h" "src/Button.cpp" "src/AssetManager.h" "src/JSONParser.h" "src/Engine/Math.h" "src/Engine/Body.h"  "src/Engine/Collision.h" "src/Engine/Impulse.h" "src/Engine/Engine.h" "src/Engine/Engine.cpp" )

add_library(Core STATIC)

target_sources(Core PRIVATE ${SOURCES})

# Match VS filters to directory structure on disk
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

target_link_libraries(Core raylib nlohmann_json::nlohmann_json)

target_include_directories(Core PUBLIC "src") 	
