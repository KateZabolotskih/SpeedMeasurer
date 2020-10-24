project(server)

set(SOURCE server.cpp)
set(HEADERS)

find_package(Boost 1.65.1 REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

add_executable(server server.cpp measurers/speedMeasurer.cpp measurers/speedMeasurer.h speedMeasurerManager.cpp speedMeasurerManager.h filter.cpp filter.h models/models.h)
target_link_libraries(server ${Boost_LIBRARIES})