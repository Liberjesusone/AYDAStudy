cmake_minimum_required(VERSION 3.16.3)
project(DiscreteProblems CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB PROBLEM_FILES "AYDAConcepts/*.cpp" "Problems/*.cpp")

foreach(file ${PROBLEM_FILES})
    get_filename_component(name ${file} NAME_WE)
    add_executable(${name} ${file})
    target_include_directories(${name} PRIVATE ${CMAKE_SOURCE_DIR})
endforeach()
