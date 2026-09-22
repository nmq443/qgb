cmake_minimum_required(VERSION 4.0.0)

project(qgb VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

file(COPY "${PROJECT_SOURCE_DIR}/roms"
     DESTINATION "${CMAKE_BINARY_DIR}"
)

add_subdirectory(core)
add_subdirectory(qgb)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
# Prevent overriding parent project's compiler/linker settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(tests)