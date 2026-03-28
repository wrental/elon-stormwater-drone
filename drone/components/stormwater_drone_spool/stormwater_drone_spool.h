#ifndef STORMWATER_DRONE_SPOOL_H 
#define STORMWATER_DRONE_SPOOL_H 

#include "stormwater_config.h"

// a leads b for ccw (unspooling)

// PPR = 512 (?)
// x2 with ticks = 1024 ticks/rotationA
// 40cm circumference
// 3.5 full rotations for ~140cm

#define SPOOL_IN    0               // initial position, always start IN
#define SPOOL_OUT   (512 * 2 * 3.5) // ticks to extend fully

void stormwater_drone_spool_init(void);
int stormwater_drone_spool_get_pos_ticks(void);
void stormwater_drone_spool_out(void);
void stormwater_drone_spool_in(void);
void stormwater_drone_spool_stop(void);

#endif
