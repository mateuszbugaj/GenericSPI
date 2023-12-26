#include <stdio.h>
#include <string>
#include <vector>
#include <set>
#include <chrono>
#include <thread>

extern "C" {
  #include "SPI_HAL.h"
}

#include "SPI_HAL_DESKTOP.hpp"

SPI_HAL_PinManager* pinManagerInstance;

SPI_HAL_PinManager::SPI_HAL_PinManager(){
  printf("Created SPI_HAL_PinManager\n");
  pinRoleStrings[SPI_HAL_PinRole::MISO] = "MISO";
  pinRoleStrings[SPI_HAL_PinRole::MISO_IN] = "MISO_IN";
  pinRoleStrings[SPI_HAL_PinRole::MISO_OUT] = "MISO_OUT";
  pinRoleStrings[SPI_HAL_PinRole::MOSI] = "MOSI";
  pinRoleStrings[SPI_HAL_PinRole::MOSI_IN] = "MOSI_IN";
  pinRoleStrings[SPI_HAL_PinRole::MOSI_OUT] = "MOSI_OUT";
  pinRoleStrings[SPI_HAL_PinRole::SCK] = "SCK";
  pinRoleStrings[SPI_HAL_PinRole::SCK_IN] = "SCK_IN";
  pinRoleStrings[SPI_HAL_PinRole::SCK_OUT] = "SCK_OUT";
  pinRoleStrings[SPI_HAL_PinRole::SS] = "SS";

  misoPin = createPin(MISO);
  mosiPin = createPin(MOSI);
  sckPin = createPin(SCK);
}

SPI_HAL_Pin* SPI_HAL_PinManager::createPin(SPI_HAL_PinRole pinRole){
  printf("Creating pin %d: %s\n", pinCounter, pinRoleStrings[pinRole].c_str());
  SPI_HAL_Pin* pin = new SPI_HAL_Pin;

  pin->pinNumber = pinCounter++;
  pin->pinRole = pinRole;
  pin->pinRoleName = pinRoleStrings[pinRole];

  if(pinRole == MISO || pinRole == MOSI || pinRole == SCK){
    pin->level = SPI_LOW;
    pin->direction = SPI_INPUT_OUTPUT;
  } else if(pinRole == SS){
    pin->level = SPI_HIGH;
    pin->direction = SPI_INPUT_OUTPUT;
  } else if(pinRole == MISO_IN || pinRole == MOSI_IN || pinRole == SCK_IN){
    pin->direction = SPI_INPUT;
  } else if(pinRole == MISO_OUT || pinRole == MOSI_OUT || pinRole == SCK_OUT){
    pin->direction = SPI_OUTPUT;
  }

  registeredPins.insert(pin);
  return pin;
}

SPI_HAL_PinManager::~SPI_HAL_PinManager(){
  printf("Deleting SPI_HAL_PinManager\n");
  for(SPI_HAL_Pin* pin : registeredPins){
    delete pin;
  }
}

void SPI_HAL_setPinManager(SPI_HAL_PinManager* pinManager){
  pinManagerInstance = pinManager;
}

void SPI_HAL_pinWrite(SPI_HAL_Pin* pin, SPI_HAL_PinLevel level){
  if(pin->direction == SPI_INPUT){
    printf("WARNING: Writing to an input pin. Returning.\n");
    return;
  }
  // printf("Writing %d to %s\n", level, pin->pinRoleName.c_str());

  pin->level = level;
  if(pin->pinRole == MISO_OUT) pinManagerInstance->misoPin->level = pin->level;
  if(pin->pinRole == MOSI_OUT) pinManagerInstance->mosiPin->level = pin->level;
  if(pin->pinRole == SCK_OUT) pinManagerInstance->sckPin->level = pin->level;
}

SPI_HAL_PinLevel SPI_HAL_pinRead(SPI_HAL_Pin* pin){
  if(pin->pinRole == MISO_IN) pin->level = pinManagerInstance->misoPin->level;
  if(pin->pinRole == MOSI_IN) pin->level = pinManagerInstance->mosiPin->level;
  if(pin->pinRole == SCK_IN) pin->level = pinManagerInstance->sckPin->level;

  return pin->level;
}

void SPI_HAL_sleep(uint16_t ms){
  std::chrono::milliseconds sleepTimespan(ms);
  std::this_thread::sleep_for(sleepTimespan);
}