cmake_minimum_required(VERSION 3.15)
project(todo-list-cpp VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

option(BUILD_TESTS "Build unit tests" ON)

add_library(todolib
    src/Task.cpp
    src/TodoList.cpp
    src/ConsoleUI.cpp
)
target_include_directories(todolib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(todolib PRIVATE -Wall -Wextra -Wpedantic)

add_executable(todo-app src/main.cpp)
target_link_libraries(todo-app PRIVATE todolib)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
