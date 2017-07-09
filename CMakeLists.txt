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
elseif(EXISTS "/usr/src/gtest/src/")
  # CMake+Ubuntu have taken years off my life. This is just not a reasonable thing to do.
  message(STATUS "GTest is not installed, but the sources were found...adding them to the build")
  file(GLOB gtest_cpps "/usr/src/gtest/src/*.cc")
  message(STATUS "gtest_cpps=${gtest_cpps}")
  # The Oh So Wise Creators of GTest don't understand the concept of relative imports, so make sure to add it to the
  # include path, which is totally reasonable.
  include_directories("/usr/src/gtest")
  add_library(gtest_all ${gtest_cpps})
  target_link_libraries(yarrpp_tests gtest_all -pthread)
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
