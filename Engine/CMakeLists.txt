cmake_minimum_required (VERSION 3.1)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

project (Engine)

add_compile_definitions (ENGINE_ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/Assets")
include_directories (${ENGINE_ASSETS_PATH})

add_subdirectory (extern)

set (ENGINE_SRCS
	"src/Engine/Application.cpp"
	"src/Engine/Graphics.cpp"
	"src/Engine/Log.cpp"
	"src/Engine/Profiler.cpp"
	"src/Engine/Resource.cpp"
	"src/Engine/Script.cpp"

	"src/Engine/Backend/Audio_Pa.cpp"
	"src/Engine/Backend/Graphics_Gl.cpp"
	"src/Engine/Backend/Physics_Bullet.cpp"
	"src/Engine/Backend/Script_Wren.cpp"
	"src/Engine/Backend/Window_Glfw.cpp"
)

add_library (Engine STATIC ${ENGINE_SRCS})
target_link_libraries (Engine cereal fmt stb glm PortAudio glad glfw wren assimp box2d bullet3)
target_include_directories (Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)