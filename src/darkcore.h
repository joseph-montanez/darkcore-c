#ifndef DARKCORE_H
#define DARKCORE_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

#define map_max_x 5
#define map_max_y 5
#define map_max_z 2

typedef struct dc_texture {
    SDL_Surface *surface;
    GLuint texture;
    GLenum texture_format;
    GLint nOfColors;
    int texture_id;
    int loaded;
} dc_texture;

typedef struct dc_tile {
    char name[32];
    double coord_0[2];
    double coord_1[2];
    double coord_2[2];
    double coord_3[2];
} dc_tile;

typedef struct dc_object {
    int x;
    int y;
    void (*on_event)(struct dc_object obj);
} dc_object;

typedef struct dc_world {
    int map[map_max_x][map_max_y][map_max_z];
    dc_texture *textures;
    dc_object *objects;
    dc_tile *tiles;
    int objects_size;
    int textures_size;
    int tiles_size;
    float camera_y;
    float camera_x;
} dc_world;

//void dc_object_create();
void dc_world_create(dc_world *world);
void dc_world_add_object(dc_world *world, dc_object *obj);
void dc_world_add_texture(dc_world *world, dc_texture *tex);
void dc_world_add_tile(dc_world *world, dc_tile *tile);
void dc_texture_create(dc_texture *tex, char *filename, char *image_type);
void dc_texture_destory(dc_texture *tex);
void dc_texture_map(dc_world* world, dc_tile *tile, char *name, int x, int y, int w, int h);
void dc_tile_set_name(dc_tile* tile, char *name);
void dc_tile_get_position(int *pos, int *tile_pos);
void dc_setup(int width, int height);
void dc_init(int width, int height);
void dc_run(dc_world* world);
void dc_quit(int code);
void dc_handle_key_down(dc_world* world, SDL_keysym* keysym);
void dc_process_events(dc_world* world);
void dc_draw_screen(dc_world* world);
void dc_draw_textured_quad(dc_world* world);
#endif
