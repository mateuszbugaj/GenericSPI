#pragma once
#include <SPI_HAL.h>

typedef enum {
  MISO_IN,
  MISO_OUT,
  MOSI_IN,
  MOSI_OUT,
  SCK_IN,
  SCK_OUT,
  SS_IN,
  SS_OUT,
  MISO,
  MOSI,
  SCK,
  SS
} SPI_HAL_PinRole;

struct SPI_HAL_Pin {
  uint8_t pinNumber;
  SPI_HAL_PinDirection direction;
  SPI_HAL_PinLevel level;
  SPI_HAL_PinRole pinRole;
};

void SPI_HAL_init();
SPI_HAL_Pin* SPI_HAL_pinSetup(SPI_HAL_PinRole pinRole);
