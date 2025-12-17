cmake_minimum_required(VERSION 3.10.0)
project(SDLShooter VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(MSVC)
    add_compile_options(/W4)
else()
    # add_compile_options(-Wall -Wextra -Werror)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
# include_directories("/usr/include/SDL2")
# link_directories("/usr/lib/x86_64-linux-gnu")

set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

get_target_property(_inc SDL2::SDL2 INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "SDL2 include dirs = ${_inc}")

add_executable(${TARGET} "src/main.cpp" "src/Game.cpp" "src/Scene.cpp" "src/SceneMain.cpp" "src/SceneTitle.cpp")

target_link_libraries(${TARGET}
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf)
