#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "eval.h"

#define CUT_IMPL
#include "cut.h"

int main(void)
{
    InitWindow(800, 600, "ccal-gui");
    SetTargetFPS(60);

    Font font = LoadFontEx("res/IosevkaWide-Regular.ttf", 32, NULL, 0);
    GuiSetFont(font);

    Evaluator ev;
    ev_init(&ev);

    while (!WindowShouldClose())
    {
        ev_update(&ev);

        BeginDrawing();

            ClearBackground(RAYWHITE);
            ev_render(&ev);

        EndDrawing();
    }

    ev_free(&ev);

    CloseWindow();
    return 0;
}
