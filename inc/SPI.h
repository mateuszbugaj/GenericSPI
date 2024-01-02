#pragma once

#include <SPI_HAL.h>

#define SPI_DEFAULT_BIT_INTERVAL 200
#define SPI_WORD_SIZE 8
#define SPI_MAX_SS_PINS 4

/* SPICR1 */
#define SPI_LSBFE 0   // LSB-First Enable
#define SPI_SSOE 1    // Slave Select Output (SSOE = 0 / MODF feature not implemented)
#define SPI_CPHA 2    // Clock Phase Bit
#define SPI_CPOL 3    // Clock Polarity Bit
#define SPI_MSTR 4    // Msater/Slave Mode Select Bit
#define SPI_SPE 6     // System Enable Bit

/* SPICR2 */
#define SPI_MODFEN 4  // Mode Fault Enable Bit

/* SPISR */
#define SPI_MODF 4    // Mode Fault Flag
#define SPI_SPIF 7    // Interrupt Flag

/* Clock polarity and phase
  SPI_CPHA = 0: Sampling of data occurs at odd edges (1,3,5,...,15) of the SCK clock
  SPI_CPHA = 1: Sampling of data occurs at even edges (2,4,6,...,16) of the SCK clock

  SPI_CPOL = 0: Active-high clocks selected. In idle state SCK is low
  SPI_CPOL = 1: Active-low clocks selected. In idle state SCK is high

         | CPHA = 0 | CPHA = 1 |
       --+----------+----------+
         |  __    __|  __    __|
CPOL = 0 |_/  \__/  |_/  \__/  |
         | ^     ^  |    ^     |
       --+----------+----------+
         |_    __   |_    __   |
CPOL = 1 | \__/  \__| \__/  \__|
         | ^     ^  |    ^     |
       --+----------+----------+

  Default: CPHA = 1, CPOL = 0
*/


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
  uint16_t SPIBR;     // SPI Baud Rate Register (just the ms interval between bits)
  uint8_t SPISR;      // SPI Status Register
  uint8_t SPIDR;      // SPI Data Register
  void (*print_str)(SPI_Role, char[]);
  void (*print_num)(SPI_Role, uint16_t);
  SPI_HAL_PinLevel sckLevel;
  SPI_HAL_PinLevel ssLevel;
  uint8_t bitNumber;
  SPI_HAL_Pin* ssPins[SPI_MAX_SS_PINS];
  uint8_t ssPinCount;
} SPI_Config;

void SPI_init(SPI_Config* cfg);
void SPI_send(SPI_Config* cfg, uint8_t payload, SPI_HAL_Pin* ss);
void SPI_read(SPI_Config* cfg);
void SPI_addSSPin(SPI_Config* cfg, SPI_HAL_Pin* ss);
SPI_HAL_Pin* SPI_getSSPin(SPI_Config* cfg, uint8_t index);
