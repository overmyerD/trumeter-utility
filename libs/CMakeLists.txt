set(LIBMODBUS_SRC_DIR 
  ${CMAKE_SOURCE_DIR}/libs/libmodbus/src
)
set(LIBMODBUS_SRC
  ${LIBMODBUS_SRC_DIR}/modbus.c
  ${LIBMODBUS_SRC_DIR}/modbus-rtu.c
  ${LIBMODBUS_SRC_DIR}/modbus-tcp.c
  ${LIBMODBUS_SRC_DIR}/modbus-data.c
)

add_library(modbus STATIC ${LIBMODBUS_SRC})
target_include_directories(modbus PUBLIC ${LIBMODBUS_SRC_DIR})