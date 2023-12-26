#pragma once
#include <stdint.h>

typedef enum {
    SPI_OUTPUT = 0,
    SPI_INPUT = 1,
    SPI_INPUT_OUTPUT = 2
} SPI_HAL_PinDirection;

typedef enum {
    SPI_LOW = 0,
    SPI_HIGH = 1
} SPI_HAL_PinLevel;

typedef struct SPI_HAL_Pin SPI_HAL_Pin; // Forward declaration
void SPI_HAL_pinWrite(SPI_HAL_Pin* pin, SPI_HAL_PinLevel level);
SPI_HAL_PinLevel SPI_HAL_pinRead(SPI_HAL_Pin* pin);
void SPI_HAL_sleep(uint16_t ms);