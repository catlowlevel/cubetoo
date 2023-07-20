cmake_minimum_required(VERSION 3.0.0)
project(imgui VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32") #generate 32bit
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



add_library(imgui STATIC 
            imgui.cpp 
            imgui_demo.cpp
            imgui_draw.cpp
            imgui_tables.cpp
            imgui_widgets.cpp
            imgui_impl_win32.cpp
            imgui_impl_opengl2.cpp
            )
