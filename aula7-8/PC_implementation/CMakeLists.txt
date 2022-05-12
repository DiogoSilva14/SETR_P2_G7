cmake_minimum_required(VERSION 3.10)
project(vending_machine)

set(CMAKE_BUILD_TYPE Debug)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
)

file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/src/*.c"
        )

add_compile_options(-pthread)

add_executable(vending_machine ${all_SRCS})

target_link_libraries(vending_machine PRIVATE Threads::Threads)