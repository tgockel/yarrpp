cmake_minimum_required(VERSION 2.8)
project(yarrpp)

include_directories(src)

file(GLOB_RECURSE yarrpp_cpps RELATIVE_PATH "." "src/yarr/*.cpp")
add_library(yarrpp ${yarrpp_cpps})

find_package(GTest REQUIRED)

file(GLOB_RECURSE test_cpps RELATIVE_PATH "." "src/yarr_tests/*.cpp")
add_executable(yarrpp_tests ${test_cpps})
target_link_libraries(yarrpp_tests yarrpp GTest::GTest GTest::Main)

# CMake wants to reserve the "test" target for their awful CTest utility, so turn that off.
cmake_policy(SET CMP0037 OLD)
add_custom_target(test
                  COMMAND $<TARGET_FILE:yarrpp_tests>
                  DEPENDS yarrpp_tests
                  USES_TERMINAL
                 )
