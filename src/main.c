#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "darkcore.h"
#include "generate.h"

void obj_on_key_press(struct dc_object* self, struct dc_keys_pressed keys) {
    if (keys.up == 1) {
        dc_object_set_y(self, dc_object_get_y(self) + 1);
    }
    if (keys.down == 1) {
        dc_object_set_y(self, dc_object_get_y(self) - 1);
    }
    if (keys.left == 1) {
        dc_object_set_x(self, dc_object_get_x(self) - 1);
    }
    if (keys.right == 1) {
        dc_object_set_x(self, dc_object_get_x(self) + 1);
    }
}

int main(int argc, char* argv[]) {
    // Start the Dark Core Reactor!    
    dc_init(640, 480);

    dc_world world = dc_world_create();
    generate_map(&world);
    
    dc_object obj = dc_object_create();
    dc_object_set_x(&obj, 10);
    dc_object_set_y(&obj, 1);
    dc_object_set_on_key_press(&obj, obj_on_key_press);
    
    dc_world_add_object(&world, &obj);
    
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
