cmake_minimum_required(VERSION 3.10.0)
project(SDLShooter VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(MSVC)
    add_compile_options(/W4)
else()
    # add_compile_options(-Wall -Wextra -Werror)
endif()

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

find_package(SDL2 REQUIRED)
# include_directories("/usr/include/SDL2")
# link_directories("/usr/lib/x86_64-linux-gnu")

set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

add_executable(${TARGET} main.cpp)

target_link_libraries(${TARGET}
    SDL2::SDL2
    SDL2::SDL2main)
