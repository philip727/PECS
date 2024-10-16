#include "SDL3/SDL_render.h"
#include "app.h"
#include "draw.h"
#include "query.h"
#include "world.h"
#include "resource.h"
#include <stdio.h>

// Update all [CSprite] [SDL_FRect]'s with [CTransforms] position
void update_sprite_with_transform(World *world) {
    char *types[] = {SPRITE_COMPONENT_TYPE, TRANSFORM_COMPONENT_TYPE};
    Query query = {
        .amountOfComponents = 2,
        .componentTypes = types,
    };

    QueryResult result = world_query(world, query);
    if (query_result_is_empty(result)) {
        query_result_cleanup(&result);
        return;
    }

    for (size_t i = 0; i < result.amountOfResults; i++) {
        QueryResultEntry entry = result.entries[i];
        CSprite *sprite = (CSprite *)entry.components[0]->data;
        CTransform *transform = (CTransform *)entry.components[1]->data;

        sprite->rect->x = transform->position.x;
        sprite->rect->y = transform->position.y;
    }

    query_result_cleanup(&result);
}

// Render all the [CSprite] components to the screen
void render_sprites_sys(World *world) {
    ResourceData *sdlCtxResource =
        world_get_resource(world, SDL_CTX_RESOURCE_TYPE);
    RSDLContext *sdlCtx = (RSDLContext *)sdlCtxResource->data;

    char *types[] = {SPRITE_COMPONENT_TYPE};
    Query query = {
        .amountOfComponents = 1,
        .componentTypes = types,
    };

    QueryResult result = world_query(world, query);
    if (query_result_is_empty(result)) {
        query_result_cleanup(&result);
        return;
    }

    for (size_t i = 0; i < result.amountOfResults; i++) {
        QueryResultEntry entry = result.entries[i];
        CSprite *sprite = (CSprite *)entry.components[0]->data;

        SDL_SetRenderDrawColor(sdlCtx->renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(sdlCtx->renderer, sprite->rect);
    }

    query_result_cleanup(&result);
}
