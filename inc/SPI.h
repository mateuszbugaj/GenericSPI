#pragma once

#include <SPI_HAL.h>

#define SPI_WORD_SIZE 8

typedef enum {
  SPI_MASTER,
  SPI_SLAVE
} SPI_Role;

typedef enum {
  SPI_NOTHING,
  SPI_BYTES,
  SPI_STATES,
  SPI_BITS
} SPI_LoggingLevel;

typedef struct {
  SPI_Role role;
  SPI_LoggingLevel loggingLevel;
  SPI_HAL_Pin* MOSI;  // Master out/slave in
  SPI_HAL_Pin* MISO;  // Master in/slave out
  SPI_HAL_Pin* SS;    // Slave Select signal
  SPI_HAL_Pin* SCK;   // Serial clock
  uint8_t SPICR1;     // SPI Control Register 1
  uint8_t SPICR2;     // SPI Control Register 2
  uint8_t SPIBR;      // SPI Baud Rate Register
  uint8_t SPISR;      // SPI Status Register
  uint8_t SPIDR;      // SPI Data Register
  void (*print_str)(SPI_Role, char[]);
  void (*print_num)(SPI_Role, uint16_t);
  SPI_HAL_PinLevel sckLevel;
  SPI_HAL_PinLevel ssLevel;
} SPI_Config;

void SPI_init(SPI_Config* cfg);
void SPI_send(SPI_Config* cfg, uint8_t payload, SPI_HAL_Pin* ss);
void SPI_read(SPI_Config* cfg);
