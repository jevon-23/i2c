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
