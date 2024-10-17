#include <stdint.h>
#include "SDL3/SDL_render.h"
#include "world.h"
#include "utils/math/vec2.h"

// A transform component which allows us to specify the position of an entity
#define TRANSFORM_COMPONENT_TYPE "component::transform"
typedef struct {
    Vec2 position;
} CTransform;

// A sprite component that will hold the rect data for rendering to SDL
#define SPRITE_COMPONENT_TYPE "component::sprite"
typedef struct {
    SDL_FRect *rect;
} CSprite;

void render_sprites_sys(World *world);
void update_sprite_with_transform(World *world);
