#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_scancode.h"
#include "entity.h"
#include "query.h"
#include "world.h"
#include "character.h"
#include "world.h"
#include "draw.h"
#include "time.h"
#include "app.h"
#include "SDL3/SDL_rect.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void spawn_character_sys(World *world) {
    size_t id = world_create_empty_entity(world);

    // Marker component to query just the local character
    ComponentData localComponent = {};
    localComponent.data = NULL;
    localComponent.type = LOCAL_CHARACTER_COMPONENT_TYPE;
    world_insert_component(world, id, localComponent);

    SDL_FRect rect = {
        .x = 0,
        .y = 0,
        .w = 128,
        .h = 128,
    };
    ComponentData spriteComponent = {};
    CSprite *sprite = (CSprite *)malloc(sizeof(CSprite));
    sprite->rect = (SDL_FRect *)malloc(sizeof(SDL_FRect));
    memcpy_s(sprite->rect, sizeof(SDL_FRect), &rect, sizeof(SDL_FRect));

    spriteComponent.type = SPRITE_COMPONENT_TYPE;
    spriteComponent.data = (void *)sprite;
    world_insert_component(world, id, spriteComponent);

    ComponentData transformComponent = {};
    CTransform *transform = (CTransform *)malloc(sizeof(CTransform));
    transform->position.x = 50;
    transform->position.y = 50;
    transformComponent.type = TRANSFORM_COMPONENT_TYPE;
    transformComponent.data = (void *)transform;
    world_insert_component(world, id, transformComponent);

    ComponentData speedComponent = {};
    speedComponent.type = SPEED_COMPONENT_TYPE;
    CSpeed *speed = (CSpeed *)malloc(sizeof(CSpeed));
    speed->value = 500;
    speedComponent.data = (void *)speed;
    world_insert_component(world, id, speedComponent);
}

void move_character_sys(World *world) {
    ResourceData *sdlCtxResource =
        world_get_resource(world, SDL_CTX_RESOURCE_TYPE);
    RSDLContext *sdlCtx = (RSDLContext *)sdlCtxResource->data;

    ResourceData *timeResource = world_get_resource(world, TIME_RESOURCE_TYPE);
    RTime *time = (RTime *)timeResource->data;

    // Marked with local character component, so we dont move other transforms.
    char *types[] = {LOCAL_CHARACTER_COMPONENT_TYPE, TRANSFORM_COMPONENT_TYPE,
                     SPEED_COMPONENT_TYPE};
    Query query = {
        .amountOfComponents = 3,
        .componentTypes = types,
    };

    QueryResult result = world_query(world, query);
    if (query_result_is_empty(result)) {
        query_result_cleanup(&result);
        return;
    }

    // Should only be one in reality
    QueryResultEntry entry = result.entries[0];
    // Should be the second component as local character is first in query
    CTransform *transform = (CTransform *)entry.components[1]->data;
    // Get the speed set in the speed component
    CSpeed *speed = (CSpeed *)entry.components[2]->data;

    double deltaSeconds = time_get_delta_seconds(time);
    const bool *keyboardState = SDL_GetKeyboardState(NULL);
    Vec2 movementDir = {.x = 0, .y = 0};
    if (keyboardState[SDL_SCANCODE_W]) {
        movementDir.y -= 1;
    }

    if (keyboardState[SDL_SCANCODE_S]) {
        movementDir.y += 1;
    }

    if (keyboardState[SDL_SCANCODE_D]) {
        movementDir.x += 1;
    }

    if (keyboardState[SDL_SCANCODE_A]) {
        movementDir.x -= 1;
    }

    transform->position =
        vec2_add(transform->position,
                 vec2_mul_double(
                     vec2_mul_double(vec2_normalize(movementDir), speed->value),
                     deltaSeconds));

        query_result_cleanup(&result);
}
