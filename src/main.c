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

    GuiSetStyle(DEFAULT, BACKGROUND_COLOR,     0xF5F5F5FF);
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL,    0xDDDDDDFF);
    GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED,   0xDDDDDDFF);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL,    0x1A1A1AFF);
    GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED,   0x1A1A1AFF);
    GuiSetStyle(DEFAULT, TEXT_SIZE,            32);
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT,       TEXT_ALIGN_LEFT);
    GuiSetStyle(TEXTBOX, BORDER_COLOR_PRESSED, 0xDDDDDDFF);

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
