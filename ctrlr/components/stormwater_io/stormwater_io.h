#ifndef STORMWATER_IO_H
#define STORMWATER_IO_H

#include "stormwater_config.h"
#include <stdint.h>
 
typedef struct stormwater_io_s {
    uint8_t spool;
    uint8_t pump;
    uint8_t data;
} stormwater_io_t;

extern stormwater_io_t stormwater_io;

void stormwater_io_init(void);

#endif
