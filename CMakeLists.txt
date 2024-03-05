cmake_minimum_required(VERSION 3.0)
project(sed)

set(CMAKE_CXX_STANDARD 17)

# llvm config
set(LLVM_DIR "/usr/lib/llvm-14/cmake")
find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
include_directories("/usr/lib/llvm-14/include")
separate_arguments(LLVM_DEFINITIONS NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS})

# flex and bison config
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
BISON_TARGET(parser src/core/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.hpp)
FLEX_TARGET(scanner src/core/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(scanner parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/core)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-but-set-variable -Wno-unused-value -Wno-unused-label -Wno-unused-local-typedefs -Wno-unused-private-field")

add_executable(sed main.cpp
        ${llvm_libs}
        src/AST.cpp
        src/AST.hpp
        ${BISON_parser_OUTPUTS}
        ${FLEX_scanner_OUTPUTS}
        src/core/Driver.cpp
        src/core/Driver.hpp

)


add_custom_target(
        run
        COMMAND sed -
        DEPENDS sed
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
        clean-all
        COMMAND ${CMAKE_BUILD_TOOL} clean
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_clean.cmake
)

llvm_map_components_to_libnames(llvm_libs support core irreader)

target_link_libraries(sed
        LLVM
)

set(OUTPUT_PRODUCT_NAME "mermaid" CACHE STRING "Output product name")

enable_testing()

file(GLOB TEST_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/*)

foreach (TEST_DIR ${TEST_DIRS})
    set(TEST_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/tests/${TEST_DIR}/in.sy)

    if (OUTPUT_PRODUCT_NAME STREQUAL "mermaid")
        set(TEST_OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/tests/${TEST_DIR}/out.md)

    elseif (OUTPUT_PRODUCT_NAME STREQUAL "llvm")
        set(TEST_OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/tests/${TEST_DIR}/out.ll)
    endif ()

    set(TEST_ERROR ${CMAKE_CURRENT_SOURCE_DIR}/tests/${TEST_DIR}/error.txt)
    add_test(
            NAME ${TEST_DIR}
            COMMAND sed ${OUTPUT_PRODUCT_NAME} ${TEST_INPUT} ${TEST_OUTPUT} ${TEST_ERROR}
    )
endforeach ()