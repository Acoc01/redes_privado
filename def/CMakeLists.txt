add_library(tcp tcp.c tcp.h crypto.c crypto.h util.c util.h)
add_library(crypto crypto.c crypto.h)
add_library(util util.c util.h)

target_link_libraries(tcp PUBLIC sodium)

target_include_directories(tcp PUBLIC ".")
target_include_directories(crypto PUBLIC ".")
target_include_directories(util PUBLIC ".")
