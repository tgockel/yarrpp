cmake_minimum_required(VERSION 3.0)
project(yarrpp)

include_directories(src)

file(GLOB_RECURSE yarrpp_cpps RELATIVE_PATH "." "src/yarr/*.cpp")
add_library(yarrpp ${yarrpp_cpps})

find_package(GTest)

file(GLOB_RECURSE test_cpps RELATIVE_PATH "." "src/yarr_tests/*.cpp")
add_executable(yarrpp_tests ${test_cpps})
target_link_libraries(yarrpp_tests yarrpp)

if(GTest_FOUND)
  target_link_libraries(yarrpp_tests GTest::GTest GTest::Main)
elseif(EXISTS "/usr/src/gtest/CMakeLists.txt")
  message(STATUS "GTest is not installed, but the sources were found...installing from CMakeLists")
  list(APPEND CMAKE_MODULE_PATH "/usr/src/gtest/")
  include("/usr/src/gtest/CMakeLists.txt")
  target_link_libraries(yarrpp_tests gtest gtest_main)
else()
  message(SEND_ERROR "GTest was not found")
endif()

# CMake wants to reserve the "test" target for their awful CTest utility, so turn that off.
cmake_policy(SET CMP0037 OLD)
add_custom_target(test
                  COMMAND $<TARGET_FILE:yarrpp_tests>
                  DEPENDS yarrpp_tests
                  USES_TERMINAL
                 )
