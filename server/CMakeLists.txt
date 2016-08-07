cmake_minimum_required(VERSION 3.5)
project(server)

set(CMAKE_VERBOSE_MAKEFILE off)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

SET (BOOST_ROOT "C:/MinGW/include/")
SET (BOOST_INCLUDEDIR "C:/MinGW/include/")
SET (BOOST_LIBRARYDIR "C:/MinGW/lib/")

find_package(Boost COMPONENTS system filesystem REQUIRED)
if (Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${BOOST_INCLUDEDIR})
    set (Boost_USE_STATIC_LIBS on) # enable static linking
else()
    message(FATAL_ERROR "Boost not found")
endif()

#set(CMAKE_EXE_LINKER_FLAGS "-lboost_log -lboost_system -lboost_filesystem -lboost_date_time -lboost_thread -L${Boost_ROOT}/boost/")
set(SOURCE_FILES main.cpp tcpIpServer.cpp tcpIpServer.h)
add_executable(server ${SOURCE_FILES})
target_link_libraries (server boost_log boost_system boost_filesystem boost_date_time boost_thread ws2_32)