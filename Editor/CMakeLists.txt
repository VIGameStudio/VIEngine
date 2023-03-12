cmake_minimum_required (VERSION 3.1)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

project (Editor)

add_compile_definitions (EDITOR_ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/Assets")
include_directories (${EDITOR_ASSETS_PATH})

add_subdirectory (extern)

set (EDITOR_SRCS
	"src/Editor/ImGui.cpp"
	"src/Editor/EditorScript.cpp"
	"src/Editor/Test.cpp"
)

add_executable (Editor "src/main.cpp" ${EDITOR_SRCS})
target_link_libraries (Editor Engine imgui)
target_include_directories (Editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)