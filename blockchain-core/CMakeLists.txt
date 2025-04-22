cmake_minimum_required(VERSION 3.10)
project(decentweet LANGUAGES CXX)

find_package(OpenSSL REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(BLOCKCHAIN_CORE_DIR ${CMAKE_SOURCE_DIR})
include_directories(${BLOCKCHAIN_CORE_DIR}/include)

file(GLOB SOURCES
    ${BLOCKCHAIN_CORE_DIR}/src/*.cpp
)

message(STATUS "Source files: ${SOURCES}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(decentweet ${SOURCES})

target_include_directories(decentweet PRIVATE ${OpenSSL_INCLUDE_DIR})

target_link_libraries(decentweet OpenSSL::SSL OpenSSL::Crypto)

target_compile_options(decentweet PRIVATE -Wzero-as-null-pointer-constant)