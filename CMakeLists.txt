cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_FLAGS "-O3")
option(BUILD_COVERAGE "Build coverage" OFF)

set(
  HUNTER_CACHE_SERVERS
  "https://github.com/bmstu-iu8-cpp-sem-3/hunter-cache"
  CACHE STRING "Default cache server"
)

include("tools/gate/cmake/HunterGate.cmake")

HunterGate(
  URL "https://github.com/cpp-pm/hunter/archive/v0.23.218.tar.gz"
  SHA1 "9a3594a61227967fb92159339ba326701b287b19"
)


project(project)
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

# hunter_add_package(GTest)
hunter_add_package(yaml-cpp)
find_package(yaml-cpp CONFIG REQUIRED)

add_subdirectory(third_party/gtest)

add_library(research STATIC
  # enum your files and delete this comment
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/investigation.cpp
)
add_library(experiment STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/experiment.cpp)

target_include_directories(experiment PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(research PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(tests
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.cpp)

add_executable(main
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/main.cpp
        )
target_include_directories(main
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        )
target_link_libraries(main research)

target_include_directories(research
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(research yaml-cpp::yaml-cpp)
target_link_libraries(research experiment)
target_link_libraries(main research)
target_link_libraries(tests gtest_main research)


enable_testing()
add_test(NAME unit_tests COMMAND tests)

if(BUILD_COVERAGE)
  set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage build." FORCE)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  find_package(codecov)
  add_coverage(research)
  add_coverage(tests)
  add_coverage(main)
  list(APPEND LCOV_REMOVE_PATTERNS "'${PROJECT_SOURCE_DIR}/tests/*'")
  coverage_evaluate()
endif()
