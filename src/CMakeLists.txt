cmake_minimum_required(VERSION 3.3)
project(Poco2)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

    include_directories(/opt/local/include)
    link_directories(/opt/local/lib)
    target_link_libraries(vegvesen-weather-locations curl curlpp pqxx PocoFoundation PocoXML)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11 -Wall -O2 -Werror")

    include_directories(/usr/local/include)
    link_libraries(/usr/local/lib/libcurl.so)
    link_libraries(/usr/local/lib/libcurlpp.so)
    link_libraries(/usr/local/lib/libpqxx.so)
    link_libraries(/usr/local/lib/libPocoFoundation.so)
    link_libraries(/usr/local/lib/libPocoXML.so)
endif()

set(SOURCE_FILES Poco2.cpp MyHandler.cpp Location.cpp)
add_executable(Poco2 ${SOURCE_FILES})
