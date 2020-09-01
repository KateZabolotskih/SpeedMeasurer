project(server)

set(SOURCE server.cpp)
set(HEADERS)

find_package(Boost 1.65.1 REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

add_executable(server server.cpp server.h)
target_link_libraries(server ${Boost_LIBRARIES})