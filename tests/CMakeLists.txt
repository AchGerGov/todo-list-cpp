include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(todo-tests
    test_task.cpp
    test_todolist.cpp
)
target_link_libraries(todo-tests PRIVATE todolib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(todo-tests)
