#include <GL/gl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <math.h>
#include <time.h>

#include "darkcore.h"

int dc_texture_inc = 0;

dc_world dc_world_create() {
    dc_world world;
    
    world.objects = (dc_object*) malloc(sizeof(dc_object) * 1);
    world.objects_size = 0;
    world.textures = (dc_texture*) malloc(sizeof(dc_texture) * 1);
    world.textures_size = 0;
    world.tiles = (dc_tile*) malloc(sizeof(dc_tile) * 1);
    world.tiles_size = 0;
    
    return world;
}

dc_object dc_object_create() {
    dc_object obj;
    return  obj;
}

int dc_object_get_x(dc_object* self) {
    return self->x;
}
void dc_object_set_x(dc_object* self, int x) {
    self->x = x;
}
void dc_object_set_y(dc_object* self, int y) {
    self->y = y;
}
int dc_object_get_y(dc_object* self) {
    return self->y;
}
void dc_object_set_on_key_press(struct dc_object* self, void (*on_key_press)(dc_object* obj, struct dc_keys_pressed keys)) {
    self->on_key_press = on_key_press;
}

void dc_world_add_object(dc_world *world, dc_object *obj) {
    world->objects_size += 1;
    world->objects = (dc_object*) realloc(world->objects, sizeof(dc_object) * world->objects_size);
    world->objects[world->objects_size - 1] = *obj;
}

void dc_world_add_texture(dc_world *world, dc_texture *tex) {
    world->textures_size += 1;
    world->textures = (dc_texture*) realloc(world->textures, sizeof(dc_texture) * world->textures_size);
    world->textures[world->textures_size - 1] = *tex;
}

void dc_world_add_tile(dc_world *world, dc_tile *tile) {
    world->tiles_size += 1;
    world->tiles = (dc_tile*) realloc(world->tiles, sizeof(dc_tile) * world->tiles_size);
    world->tiles[world->tiles_size - 1] = *tile;
}

void dc_texture_create(dc_texture *tex, char *filename, char *image_type) {
    dc_texture_inc += 1;
    tex->texture_id = dc_texture_inc;
    tex->loaded = 0;
    
    tex->surface = IMG_Load(filename);
    
    if(!tex->surface) {
        printf("IMG_LoadPNG_RW: '%s'\n", IMG_GetError());
        printf("SDL Error: '%s'\n", SDL_GetError());
        // handle error
    } else {
        SDL_SetAlpha(tex->surface, 0, 0);
    	// Check that the image's width is a power of 2
    	tex->width = tex->surface->w;
	    if ( (tex->surface->w & (tex->surface->w - 1)) != 0 ) {
		    printf("warning: image.bmp's width is not a power of 2\n");
	    }
     
	    // Also check if the height is a power of 2
    	tex->height = tex->surface->h;
	    if ( (tex->surface->h & (tex->surface->h - 1)) != 0 ) {
		    printf("warning: image.bmp's height is not a power of 2\n");
	    }
 
        // get the number of channels in the SDL surface
        tex->nOfColors = tex->surface->format->BytesPerPixel;
        if (tex->nOfColors == 4) { // contains an alpha channel
            if (tex->surface->format->Rmask == 0x000000ff) {
                tex->texture_format = GL_RGBA;
            } else {
                tex->texture_format = GL_BGRA;
            }
        } else if (tex->nOfColors == 3) {    // no alpha channel
            if (tex->surface->format->Rmask == 0x000000ff) {
                tex->texture_format = GL_RGB;
            } else {
                tex->texture_format = GL_BGR;
            }
        } else {
            printf("warning: the image is not truecolor..  this will probably break\n");
            // this error should not go unhandled
        }

        glGenTextures(tex->texture_id, &tex->texture);
 
        glBindTexture(GL_TEXTURE_2D, tex->texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

        // Edit the texture object's image data using the information SDL_Surface gives us
        glTexImage2D(
            GL_TEXTURE_2D, 0, tex->nOfColors, tex->surface->w, tex->surface->h,
            0, tex->texture_format, GL_UNSIGNED_BYTE, tex->surface->pixels
        );
            
        tex->loaded = 1;
    }
    
    return;
}

void dc_texture_destory(dc_texture *tex) {
    if (tex->loaded != 0) {
        glDeleteTextures(tex->texture_id, &tex->texture);
        SDL_FreeSurface(tex->surface);
    }
}

void dc_texture_map(dc_world* world, dc_tile *tile, char *name, int x, int y, int w, int h) {
    double tile_width = 1.00 / (double) w;
    double tile_height = 1.00 / (double) h;
    
    dc_tile_set_name(tile, name);
    tile->coord_0[0] = 0.00 + (x * tile_width);
    tile->coord_0[1] = 0.00 + (y * tile_height);
    tile->coord_1[0] = tile_width + (x * tile_width);
    tile->coord_1[1] = 0.00 + (y * tile_height);
    tile->coord_2[0] = tile_width + (x * tile_width);
    tile->coord_2[1] = tile_height + (y * tile_height);
    tile->coord_3[0] = 0.00 + (x * tile_width);
    tile->coord_3[1] = tile_height + (y * tile_height);
}

void dc_tile_set_name(dc_tile* tile, char *name) {
    strncpy(tile->name, name, (size_t) 32);
}

void dc_tile_get_position(int *pos, int *tile_pos) {
    //-- We use a 32x32 tile for now
    int tileLength = 32;
    int tileHalf = tileLength / 2;
    
    tile_pos[0] = round((pos[0] - tileHalf) / tileLength);
    tile_pos[1] = round((pos[1] - tileHalf) / tileLength);
}


void dc_setup(int width, int height) {
    glShadeModel(GL_SMOOTH);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glClearColor(0, 0, 0, 0);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float w = 0.5f * 4.0f * 5.0f;
    float h = 0.5f * 3.0f * 5.0f;
    glOrtho(-1 * w, w, -1 * h, h, 0.1f, 5.f);
}

void dc_init(int width, int height) {
    const SDL_VideoInfo* info = NULL;
    int bpp = 0;
    int flags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
        dc_quit(1);
    }

    info = SDL_GetVideoInfo();

    if (!info) {
        fprintf(stderr, "Video query failed: %s\n", SDL_GetError());
        dc_quit(1);
    }
    
    bpp = info->vfmt->BitsPerPixel;

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    flags = SDL_OPENGL;

    if(SDL_SetVideoMode(width, height, bpp, flags) == 0) {
        fprintf(stderr, "Video mode set failed: %s\n", SDL_GetError());
        dc_quit(1);
    }
    
    dc_setup(width, height);
}

void dc_run(dc_world *world) {
    int OldTime = SDL_GetTicks();
    int LimitedOldTime = SDL_GetTicks();
    int fps = 0;
    int minticks = 1000 / (30 / 2); // ticks for 30 frames per second
    while(1) {
        /* Process incoming events. */
        dc_process_events(world);
        printf("tick!\n");
        int NewTime = SDL_GetTicks();  
        int LimitedNewTime = SDL_GetTicks();    
        int TimeSinceLastFrame = NewTime - OldTime;
        
        dc_draw_screen(world);
        fps++;
            
        int sleep_for = minticks - (LimitedNewTime - LimitedOldTime);
        if (sleep_for < minticks && sleep_for > 0) {
            struct timespec req = {0};
            req.tv_sec = 0;
            req.tv_nsec = sleep_for * 1000000L;
            nanosleep(&req, (struct timespec *) NULL);
        }
        LimitedOldTime = LimitedNewTime; 
        
        // If all works well you should get about 30 frames a second
        if (TimeSinceLastFrame > 1000) {
            printf("FPS: %i\n", fps);
            OldTime = NewTime;
            fps = 0;
        }
    } 
}

void dc_quit(int code) {
    SDL_Quit();
    exit(code);
}



void dc_handle_key_down(dc_world* world, SDL_keysym* keysym) {
    switch (keysym->sym) {
        case SDLK_ESCAPE:
            dc_quit(0);
            break;
        case SDLK_SPACE:
            world->keys_pressed.space = 1;
            break;
        case SDLK_UP:
            world->keys_pressed.up = 1;
            world->camera_y -= 1.0f;
            break;
        case SDLK_DOWN:
            world->keys_pressed.down = 1;
            world->camera_y += 1.0f;
            break;
        case SDLK_LEFT:
            world->keys_pressed.left = 1;
            world->camera_x += 1.0f;
            break;
        case SDLK_RIGHT:
            world->keys_pressed.right = 1;
            world->camera_x -= 1.0f;
            break;
        default:
            break;
    }
}

void dc_handle_key_up(dc_world* world, SDL_keysym* keysym) {
    switch (keysym->sym) {
        case SDLK_SPACE:
            world->keys_pressed.space = 0;
            break;
        case SDLK_UP:
            world->keys_pressed.up = 0;
            break;
        case SDLK_DOWN:
            world->keys_pressed.down = 0;
            break;
        case SDLK_LEFT:
            world->keys_pressed.left = 0;
            break;
        case SDLK_RIGHT:
            world->keys_pressed.right = 0;
            break;
        default:
            break;
    }
}

void dc_process_events(dc_world *world) {
    SDL_Event event;
    
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_KEYDOWN:
                dc_handle_key_down(world, &event.key.keysym);
                break;
            case SDL_KEYUP:
                dc_handle_key_up(world, &event.key.keysym);
                break;
            case SDL_QUIT:
                dc_quit(0);
                break;
        }
    }
}

void dc_world_draw(dc_world* world) {
    static GLubyte white[] = { 255, 255, 255, 255 };
    
    int x, y;
    
    glTranslatef(world->camera_x, world->camera_y, 0.0f);
    

    for (x = 0; x < map_max_x; x++) { 
        for (y = 0; y < map_max_y; y++) { 
            int* layers = (int *) world->map[x][y];
            int  tileId = layers[0];
            
            // Check to see if the tile
            if (tileId < 0 || tileId >= world->tiles_size) {
                continue;
            }
            
            dc_texture tex = world->textures[0];
            
            glColor4ubv(white);
            
            glEnable(GL_TEXTURE_2D);
            if (tex.loaded > 0) {
                glBindTexture(GL_TEXTURE_2D, tex.texture);
            }
            
            glBegin(GL_QUADS);
                // TODO: make sure the double are not null :(
                double *coord_0 = world->tiles[tileId].coord_0;
                double *coord_1 = world->tiles[tileId].coord_1;
                double *coord_2 = world->tiles[tileId].coord_2;
                double *coord_3 = world->tiles[tileId].coord_3;
                
                glTexCoord2d(coord_0[0], coord_0[1]);
                glVertex3f(-0.5f + (float) x, -0.5f + (float) y, 0.5f);
             
                glTexCoord2d(coord_1[0], coord_1[1]);
                glVertex3f(0.5f + (float) x, -0.5f + (float) y, 0.5f);
             
                glTexCoord2d(coord_2[0], coord_2[1]);
                glVertex3f(0.5f + (float) x, 0.5f + (float) y, 0.5f);
             
                glTexCoord2d(coord_3[0], coord_3[1]);
                glVertex3f(-0.5f + (float) x, 0.5f + (float) y, 0.5f);
            glEnd();
            
            
            if (tex.loaded > 0) {
                glBindTexture(GL_TEXTURE_2D, (GLuint) 0);
            }
            glDisable(GL_TEXTURE_2D);
        }
    }
    
    int i;
    for (i = 0; i < world->objects_size; i++) {
        struct dc_object *obj = &world->objects[i];
        obj->on_key_press(obj, world->keys_pressed);
        
        // TODO: Move this to a callback to allow custom rendering
        glBegin(GL_QUADS);
            glVertex3f(-0.5f + ((float) obj->x * 0.5f), -0.5f + ((float) obj->y * 0.5f), 1.0f);
            glVertex3f(0.5f + ((float) obj->x * 0.5f), -0.5f + ((float) obj->y * 0.5f), 1.0f);
            glVertex3f(0.5f + ((float) obj->x * 0.5f), 0.5f + ((float) obj->y * 0.5f), 1.0f);
            glVertex3f(-0.5f + ((float) obj->x * 0.5f), 0.5f + ((float) obj->y * 0.5f), 1.0f);
        glEnd();
    }
}

void dc_draw_screen(dc_world* world) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* Move down the z-axis. */
    glTranslatef( 0.0, 0.0, -5.0 );
    
    dc_world_draw(world);

    SDL_GL_SwapBuffers();
}
