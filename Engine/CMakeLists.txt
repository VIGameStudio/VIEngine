cmake_minimum_required (VERSION 3.1)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory (extern)

set (ENGINE_SRCS
	"src/Engine/Application.cpp"
	"src/Engine/Audio.cpp"
	"src/Engine/Graphics.cpp"
	"src/Engine/Log.cpp"
	"src/Engine/Physics.cpp"
	"src/Engine/Profiler.cpp"
	"src/Engine/Resource.cpp"
	"src/Engine/Script.cpp"
	"src/Engine/Window.cpp"

	"src/Engine/Resources/Shader.cpp"
	"src/Engine/Resources/Texture.cpp"
	"src/Engine/Resources/Model.cpp"
)

add_library (Engine STATIC ${ENGINE_SRCS})
target_link_libraries (Engine cereal fmt stb glm PortAudio glad glfw wren assimp box2d bullet3)
target_include_directories (Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)