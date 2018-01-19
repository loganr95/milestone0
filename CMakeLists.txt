cmake_minimum_required(VERSION 3.5)
project(M0 CXX)

# try to prevent accidental in-source builds, these cause lots of problems
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Remove any files created thus far and use a different directory for the build.")
endif()

# require a C++11 compiler for all targets
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX AND STRICT)
  message("-- Enabling strict compilation mode")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()

set(SOURCE
  token.hpp token.cpp
  lexer.hpp lexer.cpp)

set(TESTS
  lexer_test.cpp)

add_executable(unit_tests unit_tests.cpp ${TESTS} ${SOURCE})

enable_testing()
add_test(unit_tests unit_tests)
