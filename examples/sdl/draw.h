#include <stdint.h>
#include "SDL3/SDL_render.h"
#include "world.h"

typedef struct {
    int64_t x;
    int64_t y;
} Vec2;

#define TRANSFORM_COMPONENT_TYPE "component::transform"
typedef struct {
    Vec2 position;
} CTransform;

#define SPRITE_COMPONENT_TYPE "component::sprite"
typedef struct {
    SDL_FRect *rect;
} CSprite;

void render_sprites_sys(World *world);
void update_sprite_with_transform(World *world);
