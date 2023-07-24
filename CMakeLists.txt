cmake_minimum_required(VERSION 3.0.0)
project(cubetoo VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_definitions(-DUNICODE -D_UNICODE)

include_directories(include)
add_subdirectory(include/minhook)
add_subdirectory(include/imgui)

include(FetchContent)

# Set the path where the downloaded content will be stored
set(FETCHCONTENT_BASE_DIR ${CMAKE_BINARY_DIR}/external)

# Add the GLM library using FetchContent
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        0.9.9.8 # Replace with the desired version or commit hash
)

FetchContent_MakeAvailable(glm)

add_library(cubetoo SHARED
                    src/DllMain.cpp
                    src/Util.cpp
                    src/Memory.cpp
                    src/Functions.cpp
                    src/Game.cpp
                    src/Hooks.cpp
                    src/Draw.cpp
                    src/Ui.cpp
                    "include/Structs.cpp"
                    "include/Cheat/Visuals.cpp"
                    "include/Cheat/Aim.cpp"
)

target_link_libraries(cubetoo PRIVATE minhook imgui glm)