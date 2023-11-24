# Skeleton from : https://github.com/ttroy50/cmake-examples/blob/host/01-basic/B-hello-headers/CMakeLists.txt
# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.2)

# Set the project name
project (i2c)

add_compile_options(-g)

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
    src/main.c
    src/clock/clock.c
    src/components/component.c
    src/host/host.c
    src/client/client.c
)

# Add an executable with the above sources
add_executable(i2c ${SOURCES})

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(i2c
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)
enable_testing()
include(GoogleTest)

add_executable(
  components_test
  src/components/components_test.cc
  #dependencies
  src/components/component.c
)
target_link_libraries(
  components_test
  GTest::gtest_main
)

gtest_discover_tests(components_test)

add_executable(
  client_test
  src/client/client_test.cc
  #dependencies
  src/client/client.c
  src/components/component.c
)
target_link_libraries(
  client_test
  GTest::gtest_main
)

gtest_discover_tests(client_test)

add_executable(
  host_test
  src/host/host_test.cc
  #dependencies
  src/host/host.c
  src/client/client.c
  src/components/component.c
)
target_link_libraries(
  host_test
  GTest::gtest_main
)

gtest_discover_tests(host_test)
