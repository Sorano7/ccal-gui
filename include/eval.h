#ifndef EVAL_H
#define EVAL_H

#include "ccal/ccal.h"
#include "cut.h"

#define INPUT_MAX 128

typedef struct
{
    char input_text[INPUT_MAX];
    char *result_text;
    CCalVM *vm;
    float eval_timer;
} Evaluator;

void ev_init(Evaluator *ev);
void ev_free(Evaluator *ev);

void ev_update(Evaluator *ev);
void ev_render(Evaluator *ev);

#endif
