#ifndef GENERATE_C
#define GENERATE_C
#include "darkcore.h"

void generate_map(dc_world *world) {
    world->map[0][0][0] = 0;
    world->map[1][0][0] = 1;
    world->map[2][0][0] = 1;
    world->map[3][0][0] = 0;
    world->map[4][0][0] = 0;
    
    world->map[0][1][0] = 0;
    world->map[1][1][0] = 0;
    world->map[2][1][0] = 1;
    world->map[3][1][0] = 0;
    world->map[4][1][0] = 0;
    
    world->map[0][2][0] = 0;
    world->map[1][2][0] = 0;
    world->map[2][2][0] = 0;
    world->map[3][2][0] = 0;
    world->map[4][2][0] = 0;
    
    world->map[0][3][0] = 0;
    world->map[1][3][0] = 0;
    world->map[2][3][0] = 0;
    world->map[3][3][0] = 0;
    world->map[4][3][0] = 0;
    
    world->map[0][4][0] = 0;
    world->map[1][4][0] = 0;
    world->map[2][4][0] = 0;
    world->map[3][4][0] = 0;
    world->map[4][4][0] = 0;
}
#endif
