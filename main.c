#include <raylib.h>
#include <raymath.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "drop.h"

const size_t SCRN_WIDTH = 900;
const size_t SCRN_HEIGHT = 900;
const Vector2 SCRN_CENTER = {.x = (float)SCRN_WIDTH / 2.f,
                             .y = (float)SCRN_HEIGHT / 2.f};
const size_t SCRN_FPS = 60;

const size_t MAX_DROPS = 1000;
const size_t DROP_RADIUS = 60;
const size_t DROP_SIDES = 600;

const int INP_TYPE_DROPPING = 0;
const int INP_TYPE_TINE = 1;

typedef struct {
    Drop* drops;
    size_t drop_count;
    bool has_tine_started;
    Vector2 tine_start;
    Vector2 tine_end;
} DropHandler;

void handleInpTypeToggle(int* inp_type);
void handleDropping(const int inp_type, DropHandler* handler);
void handleTine(const int inp_type, DropHandler* handler);

int main(void) {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetTargetFPS(SCRN_FPS);
    InitWindow(SCRN_WIDTH, SCRN_HEIGHT, "murrls");

    Drop drops[MAX_DROPS];
    size_t drop_count = 0;
    int inp_type = INP_TYPE_DROPPING;

    DropHandler handler;
    handler.drops = drops;
    handler.drop_count = drop_count;
    handler.has_tine_started = false;

    while (!WindowShouldClose()) {
        BeginDrawing();

        handleInpTypeToggle(&inp_type);
        handleDropping(inp_type, &handler);
        handleTine(inp_type, &handler);

        ClearBackground(RAYWHITE);
        DrawFPS(SCRN_WIDTH - 100, 10);

        // draw all of the drops
        for (size_t i = 0; i < handler.drop_count; i++) {
            drawDrop(handler.drops[i]);
        }

        EndDrawing();
    }

    // free the memory
    for (size_t i = 0; i < drop_count; i++) {
        destroyDrop(handler.drops[i]);
    }

    CloseWindow();
    return 0;
}

void handleInpTypeToggle(int* inp_type) {
    if (!IsKeyPressed(KEY_SPACE)) {
        return;
    }

    if (*inp_type == INP_TYPE_DROPPING) {
        *inp_type = INP_TYPE_TINE;
    } else {
        *inp_type = INP_TYPE_DROPPING;
    }
}

void handleDropping(const int inp_type, DropHandler* handler) {
    if (inp_type != INP_TYPE_DROPPING ||
        !IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ||
        handler->drop_count >= MAX_DROPS) {
        return;
    }

    Vector2 mouse = GetMousePosition();

    // modify all of the other drops before adding
    for (size_t i = 0; i < handler->drop_count; i++) {
        marbleDrop(handler->drops[i], mouse, DROP_RADIUS);
    }

    const float hue = (float)GetRandomValue(0, 360);
    const Color color = ColorFromHSV(hue, 1.f, 1.f);
    const Drop drop = circularDrop(mouse, DROP_RADIUS, DROP_SIDES, color);
    handler->drops[handler->drop_count] = drop;
    handler->drop_count++;
}

void handleTine(const int inp_type, DropHandler* handler) {
    if (inp_type != INP_TYPE_TINE) {
        return;
    }

    if (IsMouseButtonUp(MOUSE_BUTTON_LEFT)) {
        handler->has_tine_started = false;
    } else if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        return;
    }

    if (handler->has_tine_started) {
        handler->tine_end = GetMousePosition();
        handler->has_tine_started = false;

        for (size_t i = 0; i < handler->drop_count; i++) {
            const Vector2 startv = handler->tine_start;
            const Vector2 endv = handler->tine_end;
            const Vector2 mv = Vector2Subtract(endv, startv);

            for (size_t i = 0; i < handler->drop_count; i++) {
                tineDrop(handler->drops[i], startv, mv, 0.01f, 15.0f);
            }
        }
    } else {
        handler->tine_start = GetMousePosition();
        handler->has_tine_started = true;
    }
}
