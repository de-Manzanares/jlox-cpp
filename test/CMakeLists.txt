
add_executable(test_scanner)
target_sources(test_scanner PRIVATE scanner.cxx)
target_include_directories(test_scanner PRIVATE ../include)
target_link_libraries(test_scanner PRIVATE common-options)
# target_compile_options(test_scanner PRIVATE -DSCANNER_PRINT_TOKENS_)

add_executable(test_parser)
target_sources(test_parser PRIVATE parser.cxx)
target_include_directories(test_parser PRIVATE ../include)
target_link_libraries(test_parser PRIVATE common-options)
