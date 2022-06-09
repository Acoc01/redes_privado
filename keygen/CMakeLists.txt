add_executable(keygen keygen.c)

target_link_libraries(keygen PRIVATE tcp)
target_link_libraries(keygen PUBLIC sodium)
target_link_libraries(keygen PRIVATE crypto)
target_link_libraries(keygen PRIVATE util)
