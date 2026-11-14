cmake_minimum_required(VERSION 3.20)

project(qgb VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Git QUIET)
set(APP_VERSION "0.0.0-unknown")
if(GIT_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} describe --tags --always --dirty
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_DESC OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
  if(GIT_DESC)
    set(APP_VERSION "${GIT_DESC}")
  endif()
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
add_subdirectory(external/SDL EXCLUDE_FROM_ALL)

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

configure_file(${PROJECT_SOURCE_DIR}/cmake/templates/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h @ONLY)
target_include_directories(qgb PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
# Prevent overriding parent project's compiler/linker settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(tests)

install(TARGETS qgb RUNTIME DESTINATION .)
install(FILES README.md DESTINATION .)

set(CPACK_GENERATOR "ZIP;TGZ")
string(TOLOWER "${CMAKE_CXX_COMPILER_ID}" QGB_COMPILER)   # gnu, clang, msvc
set(CPACK_PACKAGE_FILE_NAME
    "qgb-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${QGB_COMPILER}")
include(CPack)   # must come after these set() calls