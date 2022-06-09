add_executable(cliente client.c)

target_link_libraries(cliente PRIVATE tcp)
