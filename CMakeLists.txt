cmake_minimum_required(VERSION 3.10)

project(jlox-cpp VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CompilerOptions -Wall -Wextra -Wpedantic
        #-Werror
)
set(DEBUGCompilerOptions -g ${CompilerOptions})
set(COVCompilerOptions --coverage ${CompilerOptions})
set(COVLinkerOptions --coverage)
set(SANCompilerOptions -fsanitize=address,undefined ${CompilerOptions})
set(SANLinkerOptions -fsanitize=address,undefined)
set(LIBCXXCompileOptions -stdlib=libc++)
set(LIBCXXLinkerOptions -stdlib=libc++ -lc++abi)

add_library(
        common-options
        INTERFACE
)
target_compile_options(
        common-options
        INTERFACE
        $<$<CONFIG:Debug>:$<$<CXX_COMPILER_ID:GNU,Clang>:${DEBUGCompilerOptions}>>
        $<$<CONFIG:Coverage>:$<$<CXX_COMPILER_ID:GNU,Clang>:${COVCompilerOptions}>>
        $<$<CONFIG:Sanitizer>:$<$<CXX_COMPILER_ID:GNU,Clang>:${SANCompilerOptions}>>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
target_link_options(
        common-options
        INTERFACE
        $<$<CONFIG:Coverage>:$<$<CXX_COMPILER_ID:GNU,Clang>:${COVLinkerOptions}>>
        $<$<CONFIG:Sanitizer>:$<$<CXX_COMPILER_ID:GNU,Clang>:${SANLinkerOptions}>>
)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(common-options INTERFACE ${LIBCXXCompileOptions})
    target_link_options(common-options INTERFACE ${LIBCXXLinkerOptions})
endif ()

add_executable(jlox-cpp)
target_sources(jlox-cpp PRIVATE src/main.cpp)
target_include_directories(jlox-cpp PRIVATE include)
target_link_libraries(jlox-cpp PRIVATE common-options)

add_executable(test_scanner)
target_sources(test_scanner PRIVATE test/scanner.cxx)
# target_compile_options(test_scanner PRIVATE -DSCANNER_PRINT_TOKENS_)
target_include_directories(test_scanner PRIVATE include)
target_link_libraries(test_scanner PRIVATE common-options)
