

#set(SOURCE_FILES
#        Socket.cpp Socket.h
#        Udp.cpp Udp.h)
#add_executable(client ${SOURCE_FILES} mainClient.cpp)
#add_executable(server ${SOURCE_FILES} mainServer.cpp)
cmake_minimum_required(VERSION 3.5.1)
project(bezalelNEW)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
set(BOOST_ROOT CACHE PATH "/usr/include/boost/" -lboost_serialization)
find_package(Boost 1.58.0 COMPONENTS serialization system)
include_directories(${Boost_INCLUDE_DIR})


set(SOURCE_FILES Socket.h Socket.cpp Udp.h Udp.cpp
        Driver.cpp Driver.h DriverSatisfaction.cpp DriverSatisfaction.h
        Node.h TaxiCab.cpp TaxiCab.h TripInfo.cpp TripInfo.h Grid.cpp Grid.h GridNode.cpp GridNode.h LuxuryCab.cpp LuxuryCab.h
        Passenger.cpp Passenger.h Point.cpp Point.h StandardCab.cpp StandardCab.h TaxiCenter.cpp TaxiCenter.h
        BFS.cpp BFS.h LuxuryCabFactory.cpp LuxuryCabFactory.h StandardCabFactory.cpp StandardCabFactory.h TaxiFactory.cpp
        TaxiFactory.h TaxiFac.cpp TaxiFac.h)


add_executable(server  server.cpp ${SOURCE_FILES})
add_executable(client client.cpp ${SOURCE_FILES})
target_link_libraries(server ${Boost_LIBRARIES})
target_link_libraries(client ${Boost_LIBRARIES})