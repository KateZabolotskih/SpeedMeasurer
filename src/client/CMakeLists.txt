project(client)

add_executable(client client.cpp client.h)

find_package(Boost COMPONENTS thread system REQUIRED)

include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(client ${Boost_LIBRARIES})