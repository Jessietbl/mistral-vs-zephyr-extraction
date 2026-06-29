cmake_minimum_required(VERSION 3.16)

project(factuality_freshness_validator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(validator
    src/validator.cpp
)

add_executable(test_validator
    tests/test_validator.cpp
)

target_link_libraries(test_validator validator)
