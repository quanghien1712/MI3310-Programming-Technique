cmake_minimum_required(VERSION 3.10)
project(PersonalExpenseManager LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tương đương cờ -pthread
find_package(Threads REQUIRED)

file(GLOB ALL_SRC "src/*.cpp")

set(SRC_FILES ${ALL_SRC})
list(REMOVE_ITEM SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# Chỉ định thư mục chứa header
include_directories(src)

# Tạo file thực thi expense_manager
add_executable(expense_manager src/main.cpp ${SRC_FILES})
target_link_libraries(expense_manager PRIVATE Threads::Threads)

#GTest
set(GTEST_DIR "libs/googletest")

include_directories(SYSTEM "${GTEST_DIR}/include")
include_directories("${GTEST_DIR}")

set(GTEST_SRC 
    "${GTEST_DIR}/src/gtest-all.cc" 
    "${GTEST_DIR}/src/gtest_main.cc"
)

file(GLOB TEST_FILES "tests/*.cpp")

add_executable(run_tests EXCLUDE_FROM_ALL ${TEST_FILES} ${SRC_FILES} ${GTEST_SRC})
target_link_libraries(run_tests PRIVATE Threads::Threads)

add_custom_target(build_test DEPENDS run_tests)