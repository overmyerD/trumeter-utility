cmake_minimum_required(VERSION 3.0.0)
project(onion-cmake-template VERSION 0.1.0)

add_subdirectory(${CMAKE_SOURCE_DIR}/libs)

set(TRUMETER_UTIL_SRC
  ${CMAKE_SOURCE_DIR}/src/trumeter-utility.cpp
)

add_executable(trumeter-utility ${TRUMETER_UTIL_SRC})
target_include_directories(trumeter-utility PUBLIC ${LIBMODBUS_SRC_DIR})
target_link_libraries(trumeter-utility modbus)

INSTALL(TARGETS trumeter-utility
	RUNTIME DESTINATION sbin
)