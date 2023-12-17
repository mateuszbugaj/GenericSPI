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
}