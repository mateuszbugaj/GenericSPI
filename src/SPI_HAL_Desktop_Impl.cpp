#include <stdio.h>
#include <string>
#include <vector>

extern "C" {
#include "SPI_HAL.h"
#include "SPI_HAL_DESKTOP.h"
}

volatile uint8_t pinCounter = 0;
std::vector<SPI_HAL_Pin*> registeredPins;

void SPI_HAL_init(){
  printf("Initializing SPI HAL\n");
}

SPI_HAL_Pin* SPI_HAL_pinSetup(SPI_HAL_PinRole pinRole){
  std::string roleString;
  switch (pinRole){
    case MISO:
      roleString = "MISO";
      break;
    case MOSI:
      roleString = "MOSI";
      break;
    case SCK:
      roleString = "SCK";
      break;
    case SS:
      roleString = "SS";
      break;
    case MISO_IN:
      roleString = "MISO_IN";
      break;
    case MISO_OUT:
      roleString = "MISO_OUT";
      break;
    case MOSI_IN:
      roleString = "MOSI_IN";
      break;
    case MOSI_OUT:
      roleString = "MOSI_OUT";
      break;
    case SCK_IN:
      roleString = "SCK_IN";
      break;
    case SCK_OUT:
      roleString = "SCK_OUT";
      break;
    case SS_IN:
      roleString = "SS_IN";
      break;
    case SS_OUT:
      roleString = "SS_OUT";
      break;

    default:
      roleString = "UNKNOWN";
      break;
  };

  printf("Creating pin %d: %s\n", pinCounter, roleString.c_str());
  SPI_HAL_Pin* pin = new SPI_HAL_Pin;
  pin->pinNumber = pinCounter++;
  pin->pinRole = pinRole;

  if(pinRole == MISO || pinRole == MOSI || pinRole == SCK || pinRole == SS){
    pin->level = SPI_HIGH;
    pin->direction = SPI_INPUT_OUTPUT;
  } else if(pinRole == MISO_IN || pinRole == MOSI_IN || pinRole == SCK_IN || pinRole == SS_IN){
    pin->direction = SPI_INPUT;
  } else if(pinRole == MISO_OUT || pinRole == MOSI_OUT || pinRole == SCK_OUT || pinRole == SS_OUT){
    pin->direction = SPI_OUTPUT;
  }

  registeredPins.push_back(pin);
  return pin;
}