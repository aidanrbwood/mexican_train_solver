cmake_minimum_required(VERSION 3.10)

project(mexican_train_solver)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(default_build_type "RelWithDebInfo")

add_executable(mexican_train_solver ui.cpp DominoInput.cpp DominoSolver.cpp glad.c imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_impl_glfw.cpp imgui/imgui_impl_opengl3.cpp imgui/imgui_widgets.cpp)

target_link_libraries(mexican_train_solver GL GLU glfw X11 Xrandr Xi dl)
