#include <raylib.h>
#include "raygui.h"

#include "eval.h"

#define EVAL_TIMER_S 0.15f

// Intialize and set styles.
void ev_init(Evaluator *ev)
{
    ev->eval_timer = EVAL_TIMER_S;
    ev->vm = ccal_create();
    ev->result_text = NULL;
    ev->input_text[0] = '\0';

    GuiSetStyle(DEFAULT, TEXT_SIZE, 24);
}

// Free the result text if exists.
static inline void ev_clear_result(Evaluator *ev)
{
    if (ev->result_text)
    {
        free(ev->result_text);
        ev->result_text = NULL;
    }
}

// Free the evaluator.
void ev_free(Evaluator *ev)
{
    ccal_free(ev->vm);
    ev->input_text[0] = '\0';
    ev_clear_result(ev);
}

static bool ibase_act = false;
static int  ibase_val = 10;
static bool obase_act = false;
static int  obase_val = 10;
static bool trunc_act = false;
static int  trunc_val = 10;
static bool prec_act  = false;
static int  prec_val  = 50;
static bool fmt_act   = false;
static int  fmt_val   = 0;
static bool input_act = false;

// Update VM options from global values.
static void update_options(Evaluator *ev)
{
    ccal_set_ibase(ev->vm, ibase_val);
    ccal_set_obase(ev->vm, obase_val);
    ccal_set_max_digits(ev->vm, trunc_val);
    ccal_set_precision(ev->vm, prec_val);

    CCalRenderFmt fmt = CCAL_FMT_AUTO;
    switch (fmt_val)
    {
        case 0:  fmt = CCAL_FMT_AUTO;        break;
        case 1:  fmt = CCAL_FMT_FIXED_POINT; break;
        case 2:  fmt = CCAL_FMT_SCIENTIFIC;  break;
        case 3:  fmt = CCAL_FMT_RATIONAL;    break;
        default: UNREACHABLE();
    }
    ccal_set_render_fmt(ev->vm, fmt);
}

// Update the evaluator.
void ev_update(Evaluator *ev)
{
    update_options(ev);

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

// Pad each side of the value.
#define padded(x, p) ((x) - ((p) * 2))

// p pixels below the box b.
#define below(b, p) ((b).y + (b).height + (p))

// Shrink a rectangle from each side.
static inline Rectangle shrink(Rectangle r, float padw, float padh)
{
    r.x += padw; r.y += padh;
    r.width = padded(r.width, padw);
    r.height = padded(r.height, padh);
    return r;
}

// Offset a rectangle from another rectangle's opposing side (i.e., no overlap).
// Negative values means no offset.
static inline Rectangle offset(Rectangle r, float x, float y)
{
    if (x >= 0) r.x += r.width + x;
    if (y >= 0) r.y += r.height + y;
    return r;
}

// Render the evaluator controls.
void ev_render(Evaluator *ev)
{
    if (fmt_act) GuiLock();

    const float scalew = GetScreenWidth() / 800;
    const float scaleh = GetScreenHeight() / 600;

    // Scaled window dimensions.
    const float ww = 800 * scalew;
    const float wh = 600 * scaleh;
    const float padw = 20 * scalew;
    const float padh = 20 * scaleh;

    // Group box dimensions.
    const float boxw = padded(ww, padw);
    const float boxh = padded(wh * 0.5, padh);

    ///////////////////
    // Repl group
    ///////////////////
    Rectangle repl_box = {padw, padh, boxw, boxh};
    GuiGroupBox(repl_box, "repl");

    Rectangle input_box = shrink(repl_box, padw, padh);
    input_box.height *= 0.5;
    Rectangle result_box = offset(input_box, -1, padh);

    // Input active when no other controls active.
    input_act = !(ibase_act || obase_act || trunc_act || prec_act || fmt_act);

    GuiTextBox(input_box, ev->input_text, INPUT_MAX, input_act);
    GuiLabel(result_box, (ev->result_text) ? ev->result_text : "");

    ///////////////////
    // Options
    ///////////////////
    const float optw = padded(ww * 0.5, padw);
    const float opth = padded(wh * 0.5 * 0.3, padh);

    Rectangle opt_box = {padw, below(result_box, padh), optw, opth};
    Rectangle con_box = shrink(opt_box, padw * 0.6, padh * 0.6);

    ///////////////////
    // Right col
    ///////////////////
    GuiGroupBox(opt_box, "ibase");
    if (GuiSpinner(con_box, NULL, &ibase_val, 2, 65536, ibase_act))
        ibase_act = !ibase_act;

    Rectangle next_opt = offset(opt_box, -1, padh);
    con_box = shrink(next_opt, padw * 0.6, padh * 0.6);
    GuiGroupBox(next_opt, "obase");
    if (GuiSpinner(con_box, NULL, &obase_val, 2, 62, obase_act))
        obase_act = !obase_act;

    next_opt = offset(next_opt, -1, padh);
    con_box = shrink(next_opt, padw * 0.6, padh * 0.6);
    GuiGroupBox(next_opt, "truncate");
    if (GuiSpinner(con_box, NULL, &trunc_val, 0, 100, trunc_act))
        trunc_act = !trunc_act;

    next_opt = offset(next_opt, -1, padh);
    con_box = shrink(next_opt, padw * 0.6, padh * 0.6);
    GuiGroupBox(next_opt, "precision");
    if (GuiSpinner(con_box, NULL, &prec_val, 50, 500, prec_act))
        prec_act = !prec_act;

    ///////////////////
    // Left col
    ///////////////////
    next_opt = offset(opt_box, padw * 2, -1);
    con_box = shrink(next_opt, padw * 0.6, padh * 0.6);
    GuiGroupBox(next_opt, "format");
    if (GuiDropdownBox(con_box, "auto;fixed;scientific;rational", &fmt_val, fmt_act))
        fmt_act = !fmt_act;

    GuiUnlock();
}
