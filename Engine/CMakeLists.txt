cmake_minimum_required (VERSION 3.1)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set (ENGINE_SRCS
	"src/Engine/Application.cpp"
	"src/Engine/Audio.cpp"
	"src/Engine/Graphics.cpp"
	"src/Engine/Level.cpp"
	"src/Engine/Log.cpp"
	"src/Engine/Physics.cpp"
	"src/Engine/Profiler.cpp"
	"src/Engine/Resource.cpp"
	"src/Engine/Window.cpp"
)

add_library (Engine ${ENGINE_SRCS})
target_link_libraries (Engine cereal fmt stb glm PortAudio glad glfw imgui assimp box2d bullet3)
target_include_directories (Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)