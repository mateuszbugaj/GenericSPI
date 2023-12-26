#pragma once
#include <SPI_HAL.h>
#include <unordered_set>
#include <unordered_map>

typedef enum {
  MISO_IN,
  MISO_OUT,
  MOSI_IN,
  MOSI_OUT,
  SCK_IN,
  SCK_OUT,
  SS,
  MISO,
  MOSI,
  SCK
} SPI_HAL_PinRole;

struct SPI_HAL_Pin {
  uint8_t pinNumber;
  SPI_HAL_PinDirection direction;
  SPI_HAL_PinLevel level;
  SPI_HAL_PinRole pinRole;
  std::string pinRoleName;
};

class SPI_HAL_PinManager {
  private:
    int pinCounter = 0;
    std::unordered_map<SPI_HAL_PinRole, std::string> pinRoleStrings;
    std::unordered_set<SPI_HAL_Pin*> registeredPins;

  public:
    SPI_HAL_Pin* misoPin;
    SPI_HAL_Pin* mosiPin;
    SPI_HAL_Pin* sckPin;

    SPI_HAL_Pin* createPin(SPI_HAL_PinRole pinRole);
    SPI_HAL_PinManager();
    ~SPI_HAL_PinManager();
    void SPI_HAL_pinWrite(SPI_HAL_Pin* pin, SPI_HAL_PinLevel level);
    SPI_HAL_PinLevel HAL_pinRead(SPI_HAL_Pin* pin);
};

void SPI_HAL_setPinManager(SPI_HAL_PinManager* pinManager);
