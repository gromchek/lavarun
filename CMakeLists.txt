cmake_minimum_required(VERSION 2.6)

project(lavarun)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -O2 -s")

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

if( WIN32 )
    set(CMAKE_MODULE_PATH "${SFMLDIR}/cmake/Modules")
    set(SFML_ROOT "${SFMLDIR}")
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(SFML 2.4 REQUIRED graphics window system)

if( SFML_FOUND )
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
endif()