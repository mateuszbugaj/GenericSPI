cmake_minimum_required(VERSION 3.20)
project(GenericI2C DESCRIPTION "Software-based SPI implementation for AVR and STM32 platforms with desktop support for development.")

if(TARGET_PLATFORM STREQUAL "DESKTOP")
  add_library(GenericSPI SHARED src/SPI.c src/SPI_HAL_Desktop_Impl.cpp)
  target_compile_definitions(GenericSPI PUBLIC DESKTOP=1)
  target_include_directories(GenericSPI PUBLIC inc/)
else()
  message(FATAL_ERROR "Invalid TARGET_PLATFORM: ${TARGET_PLATFORM}")
endif()
