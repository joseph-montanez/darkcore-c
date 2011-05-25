#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "darkcore.h"

void obj_on_event(struct dc_object this) {
    printf("%i\n", this.x);
}

void generate_map(dc_world *world) {
    world.map[0][0][0] = 0;
    world.map[1][0][0] = 1;
    world.map[2][0][0] = 1;
    world.map[3][0][0] = 0;
    world.map[4][0][0] = 0;
    
    world.map[0][1][0] = 0;
    world.map[1][1][0] = 0;
    world.map[2][1][0] = 1;
    world.map[3][1][0] = 0;
    world.map[4][1][0] = 0;
    
    world.map[0][2][0] = 0;
    world.map[1][2][0] = 0;
    world.map[2][2][0] = 0;
    world.map[3][2][0] = 0;
    world.map[4][2][0] = 0;
    
    world.map[0][3][0] = 0;
    world.map[1][3][0] = 0;
    world.map[2][3][0] = 0;
    world.map[3][3][0] = 0;
    world.map[4][3][0] = 0;
    
    world.map[0][4][0] = 0;
    world.map[1][4][0] = 0;
    world.map[2][4][0] = 0;
    world.map[3][4][0] = 0;
    world.map[4][4][0] = 0;
}

int main(int argc, char* argv[]) {
    // Start the Dark Core Reactor!    
    dc_init(640, 480);

    dc_world world;
    dc_world_create(&world);
    
    dc_object obj;
    obj.x = 10;
    obj.y = 1;
    obj.on_event = obj_on_event;
    
    dc_world_add_object(&world, &obj);
    obj.x = 11;
    world.objects[0].on_event(obj);
    printf("Number of objects %i\n", world.objects_size);
    
    
    // Load the texture
    struct dc_texture tex;
    dc_texture_create(&tex, "resources/grass-tiles-2-small.png", "png");
    dc_world_add_texture(&world, &tex);
    
    // Map tiles to texture 
    dc_tile t1, t2;
    dc_texture_map(&world, &t1, "Grass", 0, 0, 6, 2);
    dc_world_add_tile(&world, &t1);
    dc_texture_map(&world, &t2, "Grass", 5, 0, 6, 2);
    dc_world_add_tile(&world, &t2);
    
    dc_run(&world);
            
    dc_texture_destory(&tex);

    return 0;
}
