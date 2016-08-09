cmake_minimum_required(VERSION 3.5)
project(server)

set(CMAKE_VERBOSE_MAKEFILE off)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

#set(wxWidgets_ROOT "C:\\wxWidgets-3.0.2")
#find_package(wxWidgets COMPONENTS core base REQUIRED)
#include( "${wxWidgets_USE_FILE}" )

include_directories("C:/Users/kobus/ClionProjects/sqlite")
set(CMAKE_EXE_LINKER_FLAGS "-LC:/Users/kobus/ClionProjects/sqlite/.libs")


#set(PostgreSQL_ROOT, "C:\\Program Files\\PostgreSQL\\9.5")
#find_package(PostgreSQL REQUIRED)
#if (PostgreSQL_FOUND)
#    include_directories (${PostgreSQL_INCLUDE_DIRS})
#    link_directories(${PostgreSQL_LIBRARY_DIRS})
#else()
#    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
#endif()

set(BOOST_ROOT "C:/MinGW_Boost/MinGW/include/")
set(BOOST_INCLUDEDIR "C:/MinGW_Boost/MinGW/include/")
set(BOOST_LIBRARYDIR "C:/MinGW_Boost/MinGW/lib/")
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

set(SOURCE_FILES main.cpp server.cpp server.h database.cpp database.h mailer.cpp mailer.h clientInfo.h)
add_executable(server ${SOURCE_FILES})
#target_link_libraries (server boost_log boost_system boost_filesystem boost_date_time boost_thread ws2_32 ${PostgreSQL_LIBRARIES} ${wxWidgets_LIBRARIES})
target_link_libraries (server boost_log boost_system boost_filesystem boost_date_time boost_thread ws2_32 libsqlite3.a)