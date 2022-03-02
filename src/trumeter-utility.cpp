#include <errno.h>
#include <modbus.h>
#include <stdio.h>

#define UART_PATH           "/dev/ttyS1"
#define TRUMETER_SLAVE_ADDR 0x0A
#define TRUMETER_BUAD_RATE  9600

#define TRUMETER_NUM_OF_INPUT_REGISTERS 44
#define TRUMETER_INPUT_REGISTER_SIZE    TRUMETER_NUM_OF_INPUT_REGISTERS * 2

#define TRUMETER_NUM_OF_COIL_REGISTERS 10
#define TRUMETER_COIL_REGISTER_SIZE    TRUMETER_NUM_OF_COIL_REGISTERS

const char* coil_register_table[] = {
    "Alarm 1 status",
    "Alarm 2 status",
    "Alarm 3 status",
    "Alarm 4 status",
    "Alarm 5 status",
    "Alarm 6 status",
    "Alarm 7 status",
    "Alarm 8 status",
    "Alarm 9 status",
    "Alarm 10 status",
};

const char* input_register_table[] = {
    "Frequency",
    "L1 to L2 Voltage",
    "L1 to N Voltage",
    "L1 Current",
    "L1 Active Power",
    "L1 Apparent Power",
    "L1 Reactive Power",
    "L1 Current Phase Angle",
    "L1 Power Factor",
    "L1 Voltage THD",
    "L1 Current THD",
    "L1 Max Load",
    "L1 to L2 Voltage Phase Angle",
    "L2 to L3 Voltage",
    "L2 to N Voltage",
    "L2 Current",
    "L2 Active Power",
    "L2 Apparent Power",
    "L2 Reactive Power",
    "L2 Current Phase Angle",
    "L2 Power Factor",
    "L2 Voltage THD",
    "L2 Current THD",
    "L2 Max Load",
    "L1 to L3 Voltage Phase Angle",
    "L3 to L1 Voltage",
    "L3 to N Voltage",
    "L3 Current",
    "L3 Active Power",
    "L3 Apparent Power",
    "L3 Reactive Power",
    "L3 Current Phase Angle",
    "L3 Power Factor",
    "L3 Voltage THD",
    "L3 Current THD",
    "L3 Max Load",
    "Internal Temperature",
    "Active Energy",
    "Apparent Energy",
    "Reactive Energy",
    "Total Active Power",
    "Total Apparent Power",
    "Total reactive power",
    "All phase power factor",
};

int main(void) {
  modbus_t* ctx;

  ctx = modbus_new_rtu(UART_PATH, TRUMETER_BUAD_RATE, 'N', 8, 2);
  if (ctx == NULL) {
    fprintf(stderr, "Unable to create context: %s\n", modbus_strerror(errno));
    return 1;
  }

  // modbus_set_debug(ctx, 1);

  int ret = modbus_connect(ctx);

  if (ret < 0) {
    fprintf(stderr, "Unable to connect with context: %s\n", modbus_strerror(errno));
    return 1;
  }

  ret = modbus_set_slave(ctx, TRUMETER_SLAVE_ADDR);

  if (ret < 0) {
    fprintf(stderr, "Failed to set slave address to %d: %s\n", TRUMETER_SLAVE_ADDR, modbus_strerror(errno));
    return 1;
  }

  uint16_t input_register_data[TRUMETER_INPUT_REGISTER_SIZE];

  int nread = modbus_read_input_registers(ctx, 0x00, TRUMETER_INPUT_REGISTER_SIZE, input_register_data);

  if (nread < 0) {
    fprintf(stderr, "modbus_read_input_registers failed: %s\n", modbus_strerror(errno));
    return 1;
  }

  if (nread != TRUMETER_INPUT_REGISTER_SIZE) {
    fprintf(stderr, "nread != TRUMETER_INPUT_REGISTER_SIZE");
    return 1;
  }

  printf("Input Registers\n");
  for (int i = 0; i < nread; i += 2) {
    float f = modbus_get_float_dcba(input_register_data + i);
    printf("%-32s\t%.1f\n", input_register_table[i / 2], f);
  }
  printf("\n");

  uint8_t coil_register_data[TRUMETER_COIL_REGISTER_SIZE];

  nread = modbus_read_bits(ctx, 0x00, TRUMETER_COIL_REGISTER_SIZE, coil_register_data);

  if (nread < 0) {
    fprintf(stderr, "modbus_read_bits failed: %s\n", modbus_strerror(errno));
    return 1;
  }

  if (nread != TRUMETER_COIL_REGISTER_SIZE) {
    fprintf(stderr, "nread != TRUMETER_COIL_REGISTER_SIZE");
    return 1;
  }

  printf("Coil Registers\n");
  for (int i = 0; i < nread; i++) {
    printf("%-32s\t%d\n", coil_register_table[i], coil_register_data[i]);
  }

  modbus_close(ctx);
  modbus_free(ctx);

  return 0;
}
