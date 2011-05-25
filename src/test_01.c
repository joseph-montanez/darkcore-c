#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "darkcore.h"

void obj_on_event(struct dc_object this) {
    printf("%i\n", this.x);
}

int main(int argc, char* argv[]) {
    dc_world world;
    dc_world_create(&world);
    int map[5][5][2] = {
        {{0, -1}, {0, -1}, {0, -1}, {1, -1}, {0, -1}},
        {{0, -1}, {0, -1}, {0, -1}, {0, -1}, {0, -1}},
        {{0, -1}, {0, -1}, {1, -1}, {0, -1}, {0, -1}},
        {{0, -1}, {0, -1}, {0, -1}, {0, -1}, {0, -1}},
        {{0, -1}, {0, -1}, {0, -1}, {0, -1}, {0, -1}},
    };
    world.map = &map; 
    
    dc_object obj;
    obj.x = 10;
    obj.y = 1;
    obj.on_event = obj_on_event;
    
    dc_world_add_object(&world, &obj);
    obj.x = 11;
    world.objects[0].on_event(obj);
    printf("Number of objects %i\n", world.objects_size);
    
    struct dc_tile tile1;
    dc_tile_set_name(&tile1, "THis is a tile!?");
    dc_world_add_tile(&world, &tile1);
    
    
    printf("Tile: %s\n", world.tiles[0].name);
    
    return 0;
}
