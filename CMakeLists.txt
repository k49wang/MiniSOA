cmake_minimum_required(VERSION 3.20)

project(MiniSOA VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(MiniSOAObjects OBJECT
    src/subscription-tree/treeWrapper.cpp
    src/subscription-tree/subscriptionTree.cpp
)

target_include_directories(MiniSOAObjects
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

add_executable(MiniSOA
    src/main.cpp
    src/parser/fileParser.cpp
    src/subscription-tree/subscriptionTree.cpp
    src/subscription-tree/treeWrapper.cpp
)

target_include_directories(MiniSOA
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(MiniSOAObjects
    PRIVATE
        -Wall
        -Wextra)
