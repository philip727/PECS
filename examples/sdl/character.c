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

// The system for spawning the character. We must remember this doesn't directly
// interact with SDL. This simply just creates the data that SDL uses to render
// the character to the screen.
void spawn_character_sys(World *world) {
    // Create an entity and get it's id
    size_t id = world_create_empty_entity(world);

    // Marker component so we can query just the local player.
    ComponentData localComponent = {};
    localComponent.data = NULL;
    localComponent.type = LOCAL_CHARACTER_COMPONENT_TYPE;
    world_insert_component(world, id, localComponent);

    // The rect required so we can render a rectangle to the SDL_Renderer
    SDL_FRect rect = {
        .x = 0,
        .y = 0,
        .w = 128,
        .h = 128,
    };
    ComponentData spriteComponent = {};
    CSprite *sprite = (CSprite *)malloc(sizeof(CSprite));
    sprite->rect = (SDL_FRect *)malloc(sizeof(SDL_FRect));
    // Copy the rect into the allocated space for the rect
    memcpy_s(sprite->rect, sizeof(SDL_FRect), &rect, sizeof(SDL_FRect));

    // We MUST declare the component type or else we can't query for it
    spriteComponent.type = SPRITE_COMPONENT_TYPE;
    spriteComponent.data = (void *)sprite;
    // Insert the sprite component on to the entity
    world_insert_component(world, id, spriteComponent);

    // The transform component so we can modify the position of the sprite
    ComponentData transformComponent = {};
    CTransform *transform = (CTransform *)malloc(sizeof(CTransform));
    transform->position.x = 50;
    transform->position.y = 50;
    transformComponent.type = TRANSFORM_COMPONENT_TYPE;
    transformComponent.data = (void *)transform;
    // Insert the transform component on to the entity
    world_insert_component(world, id, transformComponent);

    // The speed component so we can change the speed of the player
    ComponentData speedComponent = {};
    speedComponent.type = SPEED_COMPONENT_TYPE;
    CSpeed *speed = (CSpeed *)malloc(sizeof(CSpeed));
    speed->value = 500;
    speedComponent.data = (void *)speed;
    // Insert the speed component on to the entity
    world_insert_component(world, id, speedComponent);
}

// Move the character using SDL keys
void move_character_sys(World *world) {
    // Gets the time resource so we can access delta time
    ResourceData *timeResource = world_get_resource(world, TIME_RESOURCE_TYPE);
    RTime *time = (RTime *)timeResource->data;

    // Marked with local character component, so we dont move other transforms.
    char *types[] = {LOCAL_CHARACTER_COMPONENT_TYPE, TRANSFORM_COMPONENT_TYPE,
                     SPEED_COMPONENT_TYPE};
    Query query = {
        .amountOfComponents = 3,
        .componentTypes = types,
    };

    // Get the results of the query
    QueryResult result = world_query(world, query);
    // Ensure the query isn't empty (Has 0 results)
    if (query_result_is_empty(result)) {
        query_result_cleanup(&result);
        return;
    }

    // As we know there's only one local player. there only should be a single query result
    QueryResultEntry entry = result.entries[0];
    // Get the transform component. Should be the second component as local character is first in query
    CTransform *transform = (CTransform *)entry.components[1]->data;
    // Get the speed component
    CSpeed *speed = (CSpeed *)entry.components[2]->data;
    // 

    // Gets the delta time in seconds from the time resource
    // 
    double deltaSeconds = time_get_delta_seconds(time);
    // Gets the keyboard state from SDL
    const bool *keyboardState = SDL_GetKeyboardState(NULL);
    // The initial movement dir. (Vec2 is literally just an x and y double).
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

    // Add the calculated distance travelled to the transform position.
    // We multiply the normalized movement dir by the speed, and then multiply by delta time(to ensure we move at the same
    // speed no matter the frame rate). We then add that final value to the transform position.
    transform->position =
        vec2_add(transform->position,
                 vec2_mul_double(
                     vec2_mul_double(vec2_normalize(movementDir), speed->value),
                     deltaSeconds));

    // Ensure we clean up the query result.
    query_result_cleanup(&result);
}


    
