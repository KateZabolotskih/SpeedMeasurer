project(server)

set(SOURCE server.cpp)
set(HEADERS)

find_package(Boost 1.65.1 REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

add_executable(server server.cpp server.h speedMeasurer.cpp speedMeasurer.h speedMeasurerManager.cpp speedMeasurerManager.h)
target_link_libraries(server ${Boost_LIBRARIES})