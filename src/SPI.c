#include <SPI.h>
#include <stdio.h>

void SPI_log(SPI_Config* cfg, char* content, SPI_LoggingLevel level){
  if(cfg->loggingLevel >= level){
    cfg->print_str(cfg->role, content);
    cfg->print_str(cfg->role, "\n\r");
  }
}

void SPI_logNum(SPI_Config* cfg, char* name, uint16_t value, SPI_LoggingLevel level){
  if(cfg->loggingLevel >= level){
    cfg->print_str(cfg->role, name);
    cfg->print_str(cfg->role, ": ");
    cfg->print_num(cfg->role, value);
    cfg->print_str(cfg->role, "\n\r");
  }
}

void SPI_init(SPI_Config* cfg){
  SPI_log(cfg, "SPI Start", SPI_BYTES);
  SPI_log(cfg, cfg->role == SPI_MASTER ? "MASTER" : "SLAVE", SPI_BYTES);

  cfg->ssLevel = SPI_HIGH;
}

/*
SLAVE SPIDR = 0b00110111 (55)
MASTER SPIDR = 0b00010110 (22)

Clock cycle | Master SPIDR | Slave SPIDR | MISO | MOSI | Note
------------+--------------+-------------+------+------+-----
0           |  00010110    |  00110111   | 1    | -    | Slave sets MISO to first bit of Slave SPIDR after SS goes LOW
1           | 1 0010110    | 0 0011011   | 1    | 0    | Master sets SCK HIGH, reads MISO, sets MOSI to fist bit of Master SPIDR
2           | 11 001011    | 10 001101   | 1    | 1    |
3           | 111 00101    | 110 00110   | 0    | 1    |
4           | 0111 0010    | 0110 0011   | 1    | 0    |
5           | 10111 001    | 10110 001   | 1    | 1    |
6           | 110111 00    | 010110 00   | 0    | 0    |
7           | 0110111 0    | 0010110 0   | 0    | 0    |
8           | 00110111     | 00010110    | -    | 0    | Now MASTER SPIDR = 0b00110111 (55) and SLAVE SPIDR = 0b00010110 (22)

*/

void SPI_send(SPI_Config* cfg, uint8_t payload, SPI_HAL_Pin* ss){
  SPI_logNum(cfg, "Sending", payload, SPI_BYTES);
  cfg->SPIDR = payload;

  SPI_log(cfg, "Set SS LOW", SPI_BITS);
  SPI_HAL_pinWrite(ss, SPI_LOW);
  SPI_HAL_sleep(100);

  // Transmission
  if(cfg->role == SPI_MASTER){
    SPI_log(cfg, "LSB", SPI_BITS);
    for(int bitNumber = 0; bitNumber < SPI_WORD_SIZE; bitNumber++){
      // Send
      SPI_logNum(cfg, "SPIDR", cfg->SPIDR, SPI_BITS);
      SPI_logNum(cfg, "Bit", (cfg->SPIDR & 1), SPI_BITS);
      SPI_HAL_pinWrite(cfg->MOSI, (cfg->SPIDR & 1) == 0 ? SPI_LOW : SPI_HIGH);
      cfg->SPIDR >>= 1;

      // Read
      SPI_HAL_PinLevel inputBit = SPI_HAL_pinRead(cfg->MISO);
      if(inputBit == SPI_HIGH){
        cfg->SPIDR |= 0b10000000;
      }

      // Clock cycle
      SPI_HAL_pinWrite(cfg->SCK, SPI_HIGH);
      SPI_HAL_sleep(100);
      SPI_HAL_pinWrite(cfg->SCK, SPI_LOW);
      SPI_HAL_sleep(100);
    }
    SPI_log(cfg, "MSB", SPI_BITS);
  }
  SPI_logNum(cfg, "Finished transmission. Received", cfg->SPIDR, SPI_BYTES);

  SPI_log(cfg, "Set SS HIGH", SPI_BITS);
  SPI_HAL_pinWrite(ss, SPI_HIGH);
  SPI_HAL_pinWrite(cfg->MOSI, SPI_LOW);
  SPI_HAL_sleep(50);
}

void SPI_read(SPI_Config* cfg){
  SPI_HAL_PinLevel newSckLevel = SPI_HAL_pinRead(cfg->SCK);
  SPI_HAL_PinLevel newSsLevel = SPI_HAL_pinRead(cfg->SS);

  if(cfg->ssLevel == SPI_HIGH && newSsLevel == SPI_LOW){
    SPI_log(cfg, "Slave addressed", SPI_STATES);

    // Send first bit of SPIDR
    SPI_logNum(cfg, "SPIDR", cfg->SPIDR, SPI_BITS);
    SPI_logNum(cfg, "Bit", (cfg->SPIDR & 1), SPI_BITS);
    SPI_HAL_pinWrite(cfg->MISO, (cfg->SPIDR & 1) == 0 ? SPI_LOW : SPI_HIGH);
    cfg->SPIDR >>= 1;
  }

  if(cfg->ssLevel == SPI_LOW && newSsLevel == SPI_HIGH){
    SPI_log(cfg, "Slave not addressed", SPI_STATES);
  }

  if(cfg->sckLevel == SPI_LOW && newSckLevel == SPI_HIGH){
    // Read
    SPI_HAL_PinLevel inputBit = SPI_HAL_pinRead(cfg->MOSI);
    if(inputBit == SPI_HIGH){
      cfg->SPIDR |= 0b10000000;
    }

    // Send
    SPI_logNum(cfg, "SPIDR", cfg->SPIDR, SPI_BITS);
    SPI_logNum(cfg, "Bit", (cfg->SPIDR & 1), SPI_BITS);
    SPI_HAL_pinWrite(cfg->MISO, (cfg->SPIDR & 1) == 0 ? SPI_LOW : SPI_HIGH);
    cfg->SPIDR >>= 1;
  }

  cfg->sckLevel = newSckLevel;
  cfg->ssLevel = newSsLevel;
}