#include <raylib.h>

#define CUT_IMPL
#include "cut.h"

int main(void)
{
    InitWindow(800, 600, "ccal-gui");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

            ClearBackground(RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
