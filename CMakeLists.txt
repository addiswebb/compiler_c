cmake_minimum_required(VERSION 4.1)
project(compiler_c C)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works

add_executable(compiler_c
    src/main.c
    src/compiler.c
    src/tokenizer.c
    src/parser.c
    src/node.c
    src/util.c
    src/x86/x86_gen.c
    src/x86/x86_emit.c
    src/type.c
    src/ir/ir_gen.c
    src/ir/ir_analysis.c
    src/ir/ir_util.c
    src/ir/ir_builder.c
    src/ir/ir_module.c
    src/sema.c
)

target_include_directories(compiler_c
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
