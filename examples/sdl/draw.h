#include <stdint.h>
#include "SDL3/SDL_render.h"
#include "world.h"
#include "utils/math/vec2.h"

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
