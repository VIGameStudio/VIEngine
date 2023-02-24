cmake_minimum_required (VERSION 3.1)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory (extern)

set (EDITOR_SRCS
	"src/Editor/EditorWindow.cpp"
)

add_executable (Editor "src/main.cpp" ${EDITOR_SRCS})
target_link_libraries (Editor Engine imgui)
target_include_directories (Editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)