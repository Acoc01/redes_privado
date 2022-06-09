add_executable(servidor server.c)

target_link_libraries(servidor PRIVATE tcp)
