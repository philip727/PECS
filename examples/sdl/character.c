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
#include "app.h"
#include "SDL3/SDL_rect.h"
#include <stdlib.h>
#include <string.h>

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
}

void move_character_sys(World *world) {
    ResourceData *sdlCtxResource =
        world_get_resource(world, SDL_CTX_RESOURCE_TYPE);
    RSDLContext *sdlCtx = (RSDLContext *)sdlCtxResource->data;

    // Marked with local character component, so we dont move other transforms.
    char *types[] = {LOCAL_CHARACTER_COMPONENT_TYPE, TRANSFORM_COMPONENT_TYPE};
    Query query = {
        .amountOfComponents = 2,
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

    const bool *keyboardState = SDL_GetKeyboardState(NULL);

    if (keyboardState[SDL_SCANCODE_W]) {
        transform->position.y -= 1;
    }

    if (keyboardState[SDL_SCANCODE_S]) {
        transform->position.y += 1;
    }

    if (keyboardState[SDL_SCANCODE_D]) {
        transform->position.x += 1;
    }

    if (keyboardState[SDL_SCANCODE_A]) {
        transform->position.x -= 1;
    }
}
