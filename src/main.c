#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "darkcore.h"
#include "generate.h"

dc_world world;

void obj_on_key_press(struct dc_object *self) {
    int mx, my;
    mx = 0;
    my = 0;
    printf("Before: %ix%i\n", mx, my);
    
    if (world.keys_pressed.up == 1) {
        my = 8;
    }
    if (world.keys_pressed.down == 1) {
        my = -8;
    }
    if (world.keys_pressed.left == 1) {
        mx = -8;
    }
    if (world.keys_pressed.right == 1) {
        mx = 8;
    }
    
        int x = mx;
        int y = my; //<-- CRASH OMFG!!!!
        printf("After: %ix%i\n", mx, my);
        
        int player_pos[2] = {self->x, self->y};
        dc_int_2 player_tile_pos = dc_tile_get_position(player_pos);
        int px = player_tile_pos._0;
        int py = player_tile_pos._1;
        
        printf("After 2: %ix%i\n", mx, my);
        
        //-- Top Row
        int tmptiles[2][9];
        tmptiles[0][0] = px - 1;
        tmptiles[0][1] = py - 1;
        tmptiles[1][0] = px;
        tmptiles[1][1] = py - 1;
        tmptiles[2][0] = px + 1;
        tmptiles[2][1] = py - 1;
        //-- Center Row
        tmptiles[3][0] = px - 1;
        tmptiles[3][1] = py;
        tmptiles[4][0] = px;
        tmptiles[4][1] = py;
        tmptiles[5][0] = px + 1;
        tmptiles[5][1] = py;
        //-- Bottom Row
        tmptiles[6][0] = px - 1;
        tmptiles[6][1] = py + 1;
        tmptiles[7][0] = px;
        tmptiles[7][1] = py + 1;
        tmptiles[8][0] = px + 1;
        tmptiles[8][1] = py + 1;

        printf("After 3: %ix%i\n", mx, my);
        //mx = x;
        //my = y;
        
        int tid;
        dc_bounding_box player_box = dc_get_bounding_box(player_pos, 16);
        printf("Pre-Pre-Move: %ix%i\n", mx, my);
        for (tid = 0; tid < 9; tid++) {
            int tile[2];
            tile[0] = tmptiles[tid][0];
            tile[1] = tmptiles[tid][1];
            
            if (tile[0] < 0 || tile[1] < 0) {
                printf("Tile out of bounds\n");
                continue;
            }
            
            int tile_id = world.map[tile[0]][tile[1]][0];
            
            printf("Tile Position: %ix%i\n", tile[0], tile[1]);
            
            // half since it based on the center of the tile
            dc_bounding_box tile_box = dc_get_bounding_box(tile, 16); 
            
            int is_hit = dc_collision_box_in_box(player_box, tile_box);
            if (is_hit == 1) {
                dc_tile *world_tiles = world.tiles;
                
                if (tile_id < 0 || tile_id >= world.tiles_size) {
                    printf("Tile out of bounds\n");
                    continue;
                }
                printf("tileid: %i\n", tile_id);
                if (world_tiles[tile_id].blocked == 1) {
                    printf("Tile is blocked\n");
                    mx = 0;
                    my = 0;
                    break;
                }
            }
            
            printf("Pre-Move: %ix%i\n", mx, my);
        }
        
        printf("Move: %ix%i\n", mx, my);
        if (my != 0) {
            dc_object_set_y(self, dc_object_get_y(self) + my);
        }
        
        if (mx != 0) {
            dc_object_set_x(self, dc_object_get_x(self) + mx);
        }
}

int main(int argc, char* argv[]) {
    // Start the Dark Core Reactor!    
    dc_init(640, 480);

    world = dc_world_create();
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
    dc_tile t1, t2, t3;
    dc_texture_map(&world, &t1, "Grass", 0, 0, 6, 2, 0);
    dc_world_add_tile(&world, &t1);
    dc_texture_map(&world, &t2, "Grass", 5, 0, 6, 2, 0);
    dc_world_add_tile(&world, &t2);
    dc_texture_map(&world, &t3, "Dirt", 4, 0, 6, 2, 1);
    dc_world_add_tile(&world, &t3);
    
    dc_run(&world);
            
    dc_texture_destory(&tex);

    return 0;
}
