#ifndef DARKCORE_H
#define DARKCORE_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

#define map_max_x 5
#define map_max_y 5
#define map_max_z 2

/*
    case SDLK_ESCAPE:
        dc_quit(0);
        break;
    case SDLK_SPACE:
        break;
    case SDLK_UP:
        world->camera_y += 1.0f;
        break;
    case SDLK_DOWN:
        world->camera_y -= 1.0f;
        break;
    case SDLK_LEFT:
        world->camera_x += 1.0f;
        break;
    case SDLK_RIGHT:
        world->camera_x -= 1.0f;
        break;
*/
typedef struct dc_keys_pressed {
    int space;
    int up;
    int down;
    int left;
    int right;
} dc_keys_pressed;

typedef struct dc_texture {
    SDL_Surface *surface;
    GLuint texture;
    GLenum texture_format;
    GLint nOfColors;
    int texture_id;
    int loaded;
    int width;
    int height;
} dc_texture;

typedef struct dc_texture_tileset {
    struct dc_texture tex;
    int tile_width;
    int tile_height;
} dc_texture_tileset;

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
    void (*on_key_press)(struct dc_object *self, struct dc_keys_pressed keys);
} dc_object;

typedef struct dc_world {
    struct dc_keys_pressed keys_pressed;
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

// World
dc_world dc_world_create();
void dc_world_add_object(dc_world *world, dc_object *obj);
void dc_world_add_texture(dc_world *world, dc_texture *tex);
void dc_world_add_tile(dc_world *world, dc_tile *tile);

void dc_texture_create(dc_texture *tex, char *filename, char *image_type);
void dc_texture_destory(dc_texture *tex);
void dc_texture_map(dc_world* world, dc_tile *tile, char *name, int x, int y, int w, int h);
void dc_tile_set_name(dc_tile* tile, char *name);
void dc_tile_get_position(int *pos, int *tile_pos);

// Objects
dc_object dc_object_create();
int dc_object_get_x(dc_object* self);
void dc_object_set_x(dc_object* self,  int x);
int dc_object_get_y(dc_object* self);
void dc_object_set_y(dc_object* self, int y);
void dc_object_set_on_key_press(dc_object* self, void (*on_event)(struct dc_object* self, struct dc_keys_pressed keys));

void dc_setup(int width, int height);
void dc_init(int width, int height);
void dc_run(dc_world* world);
void dc_quit(int code);
void dc_handle_key_down(dc_world* world, SDL_keysym* keysym);
void dc_process_events(dc_world* world);
void dc_draw_screen(dc_world* world);
void dc_world_draw(dc_world* world);
#endif
