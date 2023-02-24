cmake_minimum_required (VERSION 3.1)

# ImGui lib
add_library (imgui STATIC "imgui/imgui.cpp" "imgui/imgui_demo.cpp" "imgui/imgui_draw.cpp" "imgui/imgui_tables.cpp" "imgui/imgui_widgets.cpp" "imgui/backends/imgui_impl_opengl3.cpp" "imgui/backends/imgui_impl_glfw.cpp")
target_link_libraries (imgui glfw)
target_include_directories (imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui)