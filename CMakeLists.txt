cmake_minimum_required(VERSION 3.28.3)
project(compiler_c C)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

add_executable(compiler_c
    src/main.c
    src/compiler.c
    src/tokenizer.c
    src/parse/parser.c
    src/parse/parse_util.c
    src/node.c
    src/util.c
    src/x86/x86_gen.c
    src/x86/x86_emit.c
    src/type.c
    src/ir/ir_gen.c
    src/ir/ir_util.c
    src/ir/ir_builder.c
    src/ir/ir_module.c
    src/sema.c
    src/analysis.c
)

target_include_directories(compiler_c
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#     message(STATUS "Enabling AddressSanitizer and UndefinedBehaviorSanitizer")
#     set(SANITIZE_FLAGS "-fsanitize=address,undefined -fno-omit-frame-pointer")
#     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SANITIZE_FLAGS}")
#     set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${SANITIZE_FLAGS}")
# endif()
