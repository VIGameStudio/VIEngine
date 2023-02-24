cmake_minimum_required (VERSION 3.1)

# Ctti lib
#add_subdirectory (ctti)

# Cereal lib
set (BUILD_DOC OFF)
set (BUILD_SANDBOX OFF)
set (SKIP_PERFORMANCE_COMPARISON ON)

add_subdirectory (cereal)

# FMT lib
set(FMT_MODULE ON)

add_subdirectory (fmt)

# STB lib
add_library (stb INTERFACE)
set_property(TARGET stb APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/stb)

# GLM lib
add_subdirectory (glm)

# Portaudio lib
add_subdirectory (portaudio)

# GLAD lib
add_subdirectory (glad)

# GLFW lib
add_subdirectory (glfw)

# ImGui lib
#add_library (imgui STATIC "imgui/imgui.cpp" "imgui/imgui_demo.cpp" "imgui/imgui_draw.cpp" #"imgui/imgui_tables.cpp" "imgui/imgui_widgets.cpp" "imgui/backends/imgui_impl_opengl3.cpp" #"imgui/backends/imgui_impl_glfw.cpp")
#target_link_libraries (imgui glfw)
#target_include_directories (imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

# Assimp
set (BUILD_SHARED_LIBS OFF)
set (ASSIMP_BUILD_ZLIB ON)

add_subdirectory (assimp)

# Box2D lib
set (BOX2D_BUILD_UNIT_TESTS OFF)
set (BOX2D_BUILD_TESTBED OFF)
set (BOX2D_BUILD_DOCS OFF)
set (BOX2D_USER_SETTINGS OFF)
set (BUILD_SHARED_LIBS OFF)

add_subdirectory (box2d)

# Bullet3 lib
set (USE_DOUBLE_PRECISION OFF)
set (USE_GRAPHICAL_BENCHMARK OFF)
set (USE_SOFT_BODY_MULTI_BODY_DYNAMICS_WORLD ON)
set (USE_OPENVR OFF)
set (BUILD_SHARED_LIBS OFF)
set (ENABLE_VHACD OFF)
set (BULLET2_MULTITHREADING OFF)
set (BUILD_CPU_DEMOS OFF)
set (USE_GLUT OFF)
set (BUILD_BULLET3 ON)
set (BUILD_PYBULLET OFF)
set (BUILD_ENET ON)
set (BUILD_CLSOCKET ON)
set (BUILD_EGL OFF)
set (BUILD_OPENGL3_DEMOS OFF)
set (BUILD_BULLET2_DEMOS OFF)
set (BUILD_EXTRAS OFF)
set (INSTALL_LIBS OFF)
set (INSTALL_EXTRA_LIBS OFF)
set (BUILD_UNIT_TESTS OFF)
set (INSTALL_CMAKE_FILES OFF)

add_subdirectory (bullet3)
add_library (bullet3 STATIC "bullet3/src/btBulletCollisionAll.cpp" "bullet3/src/btBulletDynamicsAll.cpp")# "bullet3/src/btLinearMathAll.cpp")
target_link_libraries(bullet3 Bullet3Collision Bullet3Dynamics LinearMath)
target_include_directories (bullet3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/bullet3/src)