#include <raylib.h>
#include <raymath.h>
#include <stdint.h>
#include <stdio.h>
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

void handleInpTypeToggle(int* inp_type);
void handleDropping(const int inp_type, Drop* drops, size_t* drop_count_ptr);
void handleTine(const int inp_type, bool* start_has_selected, Vector2* start,
                Vector2* end, Drop* drops, size_t drop_count);

int main(void) {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetTargetFPS(SCRN_FPS);
    InitWindow(SCRN_WIDTH, SCRN_HEIGHT, "murrls");

    Drop drops[MAX_DROPS];
    size_t drop_count = 0;
    int inp_type = INP_TYPE_DROPPING;

    bool is_start_selected = false;
    Vector2 start;
    Vector2 end;

    // for (size_t i = 10; i > 0; i--) {
    //     const float radius = 20.0 * (float)i;
    //     // const float hue = (float)GetRandomValue(0, 360);
    //     const Color color = ColorFromHSV(200, 1.0f, 0.5f);
    //     drops[10 - i] = circularDrop(SCRN_CENTER, radius, DROP_SIDES, color);
    //     drop_count++;
    // }

    while (!WindowShouldClose()) {
        BeginDrawing();

        handleInpTypeToggle(&inp_type);
        handleDropping(inp_type, drops, &drop_count);
        handleTine(inp_type, &is_start_selected, &start, &end, drops,
                   drop_count);

        ClearBackground(RAYWHITE);
        DrawFPS(SCRN_WIDTH - 100, 10);

        // draw all of the drops
        for (size_t i = 0; i < drop_count; i++) {
            drawDrop(drops[i]);
        }

        EndDrawing();
    }

    // free the memory
    for (size_t i = 0; i < drop_count - 1; i++) {
        destroyDrop(drops[i]);
    }

    CloseWindow();
    return 0;
}

void handleInpTypeToggle(int* inp_type) {
    if (IsKeyPressed(KEY_SPACE)) {
        *inp_type =
            *inp_type == INP_TYPE_DROPPING ? INP_TYPE_TINE : INP_TYPE_DROPPING;
    }
}

void handleDropping(const int inp_type, Drop* drops, size_t* drop_count_ptr) {
    if (inp_type != INP_TYPE_DROPPING ||
        !IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ||
        *drop_count_ptr >= MAX_DROPS) {
        return;
    }

    Vector2 mouse = GetMousePosition();

    // modify all of the other drops before adding
    for (size_t i = 0; i < *drop_count_ptr; i++) {
        marbleDrop(drops[i], mouse, DROP_RADIUS);
    }

    const float hue = (float)GetRandomValue(0, 360);
    const Color color = ColorFromHSV(hue, 1.f, 1.f);
    drops[*drop_count_ptr] =
        circularDrop(mouse, DROP_RADIUS, DROP_SIDES, color);
    (*drop_count_ptr)++;
}

void handleTine(const int inp_type, bool* is_start_selected_ptr,
                Vector2* start_ptr, Vector2* end_ptr, Drop* drops,
                size_t drop_count) {
    if (inp_type != INP_TYPE_TINE) {
        return;
    }

    if (IsMouseButtonUp(MOUSE_BUTTON_LEFT)) {
        *is_start_selected_ptr = false;
    } else if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        return;
    }

    if (*is_start_selected_ptr) {
        *end_ptr = GetMousePosition();
        *is_start_selected_ptr = false;

        for (size_t i = 0; i < drop_count; i++) {
            const Vector2 mv = Vector2Subtract(*end_ptr, *start_ptr);
            for (size_t i = 0; i < drop_count; i++) {
                tineDrop(drops[i], *start_ptr, mv, 0.01f, 15.0f);
            }
        }
    } else {
        *start_ptr = GetMousePosition();
        *is_start_selected_ptr = true;
    }
}
