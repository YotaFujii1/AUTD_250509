cmake_minimum_required(VERSION 3.21)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
endif()

project(autd3-test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(RUN_LINK_SOEM "RUN_LINK_SOEM" OFF)
option(RUN_LINK_REMOTE_SOEM "RUN_LINK_REMOTE_SOEM" OFF)
option(RUN_LINK_TWINCAT "RUN_LINK_TWINCAT" OFF)
option(RUN_LINK_REMOTE_TWINCAT "RUN_LINK_REMOTE_TWINCAT" OFF)
option(RUN_LINK_SIMULATOR "RUN_LINK_SIMULATOR" OFF)
option(COVERAGE "Take coverage" OFF)

if(COVERAGE)
  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O0 -g --coverage -fno-inline -fno-inline-small-functions -fno-default-inline -fprofile-arcs -ftest-coverage")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage -fprofile-arcs -fprofile-generate")
endif()

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()
include(${PROJECT_SOURCE_DIR}/gtest.cmake)

enable_testing()

include(FetchContent)
FetchContent_Declare(
  autd3
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/..
)
set(AUTD3_ASYNC_API ON)
if(AUTD3_ASYNC_API AND APPLE)
  if(CMAKE_CXX_COMPILER_ID MATCHES "AppleClang|Clang")
    if (CMAKE_CXX_COMPILER_VERSION LESS 16.0.0)
        set(AUTD3_ASYNC_API OFF)
        message(STATUS "Disable async APIs on macOS because Clang version ${CMAKE_CXX_COMPILER_VERSION} is not supported. Use 16.0.0 or higher version.")
    endif()
  else()
    set(AUTD3_ASYNC_API OFF)
    message(STATUS "Disable async APIs on macOS.")
  endif()
endif()
FetchContent_MakeAvailable(autd3)

add_executable(test_autd3 main.cpp)

target_include_directories(test_autd3 PRIVATE .)
target_link_libraries(test_autd3 PRIVATE gtest_main)
target_link_libraries(test_autd3 PRIVATE autd3::autd3 autd3::gain::holo autd3::modulation::audio_file)
target_link_libraries(test_autd3 PRIVATE autd3::link::twincat)
target_link_libraries(test_autd3 PRIVATE autd3::link::simulator)

add_subdirectory(controller)
add_subdirectory(gain)
add_subdirectory(driver)
add_subdirectory(link)
add_subdirectory(modulation)

add_test(NAME test_autd3 COMMAND test_autd3)

include(GoogleTest)
gtest_discover_tests(test_autd3)

if(WIN32)
  target_link_options(test_autd3 PRIVATE /NODEFAULTLIB:libcmt.lib /NODEFAULTLIB:libcmtd.lib /NODEFAULTLIB:msvcrtd.lib)
endif()

if(COVERAGE)
  target_compile_definitions(test_autd3 PRIVATE COVERAGE)
endif()
if(RUN_LINK_SOEM)
  target_compile_definitions(test_autd3 PRIVATE RUN_LINK_SOEM)
endif()
if(RUN_LINK_REMOTE_SOEM)
  target_compile_definitions(test_autd3 PRIVATE RUN_LINK_REMOTE_SOEM)
endif()
if(RUN_LINK_TWINCAT)
  target_compile_definitions(test_autd3 PRIVATE RUN_LINK_TWINCAT)
endif()
if(RUN_LINK_REMOTE_TWINCAT)
  target_compile_definitions(test_autd3 PRIVATE RUN_LINK_REMOTE_TWINCAT)
endif()
if(RUN_LINK_SIMULATOR)
  target_compile_definitions(test_autd3 PRIVATE RUN_LINK_SIMULATOR)
endif()
target_compile_definitions(test_autd3 PRIVATE AUTD3_RESOURCE_PATH="${PROJECT_SOURCE_DIR}/../examples/resources")
