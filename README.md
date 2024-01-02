# GenericSPI

A software-based implementation of the SPI protocol. Doesn't directly use timers, ISRs or pins and instead uses one of the provided HAL libraries to work on the desktop, AVR or STM32F103.

Master starts the transmission by pulling the SS (Slave Select) pin assosiated with the target slave low. Each device has it's own SPIDR (SPI Data Register) that gets replaced bit by bit with the SPIDR in the counterpart. 

Pins MOSI, MISO and SCK are common for all devices.

Example:

```C
SPI_HAL_Pin* MISO;
SPI_HAL_Pin* MOSI;
SPI_HAL_Pin* SCK;
SPI_HAL_Pin* SS;

// Initialize pins correctly for given target platform

SPI_config = {
  .role = SPI_MASTER, // SPI_SLAVE
  .loggingLevel = SPI_LoggingLevel::SPI_BITS,
  .MOSI = MOSI,
  .MISO = MISO,
  .SS = SS,
  .SCK = SCK,
  .print_str = printStr,
  .print_num = printNum
};

SPI_init(&SPI_config);

// Slave needs to pool the bus pins
while(1){
  SPI_read(&SPI_config);
}

// When writing byte, master needs to pass the SS pin of the selected slave.
// Slave can just pass NULL.
uint8_t payload = 22;
SPI_send(cfg, payload, slaveSS_1);

```

```
SLAVE SPIDR = 0b00110111 (55)
MASTER SPIDR = 0b00010110 (22)

SPI_LSBFE = 1

Clock cycle | Master SPIDR | Slave SPIDR | MISO | MOSI |
------------+--------------+-------------+------+------+
0           |  00010110    |  00110111   | 1    | -    |
1           | 1 0010110    | 0 0011011   | 1    | 0    |
2           | 11 001011    | 10 001101   | 1    | 1    |
3           | 111 00101    | 110 00110   | 0    | 1    |
4           | 0111 0010    | 0110 0011   | 1    | 0    |
5           | 10111 001    | 10110 001   | 1    | 1    |
6           | 110111 00    | 010110 00   | 0    | 0    |
7           | 0110111 0    | 0010110 0   | 0    | 0    |
8           | 00110111     | 00010110    | -    | 0    |
```