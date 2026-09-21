#include <raylib.h>
#include "raygui.h"

#include "eval.h"

#define EVAL_TIMER_S 0.15f

void ev_init(Evaluator *ev)
{
    ev->eval_timer = EVAL_TIMER_S;
    ev->vm = ccal_create();
    ev->result_text = NULL;
    ev->input_text[0] = '\0';
}

static inline void ev_clear_result(Evaluator *ev)
{
    if (ev->result_text)
    {
        free(ev->result_text);
        ev->result_text = NULL;
    }
}

void ev_free(Evaluator *ev)
{
    ccal_free(ev->vm);
    ev->input_text[0] = '\0';
    ev_clear_result(ev);
}

void ev_update(Evaluator *ev)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        ev->input_text[0] = '\0';
        return;
    }

    if (ev->eval_timer > 0)
    {
        ev->eval_timer -= GetFrameTime();
        return;
    }

    char *tmp = NULL;
    CCalResult res = ccal_eval(ev->vm, ev->input_text);
    if (res.value) tmp = ccal_render(ev->vm, res.value);

    if (tmp)
    {
        ev_clear_result(ev);
        ev->result_text = tmp;
    }

    ev->eval_timer = EVAL_TIMER_S;
}

void ev_render(Evaluator *ev)
{
    int ww = GetScreenWidth();
    int wh = GetScreenHeight();

    int width = ww * 0.8;
    int height = wh * 0.15;
    Rectangle input_box = {
        .x      = (ww - width) / 2,
        .y      = wh * 0.3,
        .width  = width,
        .height = height
    };
    GuiTextBox(input_box, ev->input_text, INPUT_MAX, true);

    if (ev->result_text)
    {
        Rectangle result_box = {
            .x      = (ww - width) / 2,
            .y      = wh * 0.5,
            .width  = width,
            .height = height / 2,
        };
        GuiLabel(result_box, ev->result_text);
    }
}
