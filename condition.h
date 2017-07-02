/*
 *  Textplore: A text adventure game engine.
 *
 *  Copyright (C) 2014-2017  Jading Tsunami
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef CONDITION_H_ON4UZMFM
#define CONDITION_H_ON4UZMFM

#include "utils.h"
#include "display.h"
#include "textplore.h"

typedef enum {
    EVALUATE_LEVEL_SETTING_SET,
    EVALUATE_LEVEL_SETTING_CLEAR,
    EVALUATE_GLOBAL_SETTING_SET,
    EVALUATE_GLOBAL_SETTING_CLEAR,
    EVALUATE_INVENTORY_HAS,
    EVALUATE_INVENTORY_HAS_NOT,
    EVALUATE_ALWAYS,
} evaluate;

typedef enum {
    DISPLAY_MESSAGE,
    SET_ROOM_MESSAGE,
    APPEND_ROOM_MESSAGE,
    SET_PROMPT,
    SET_GLOBAL_SETTING,
    CLEAR_GLOBAL_SETTING,
    SET_LEVEL_SETTING,
    CLEAR_LEVEL_SETTING,
    EXIT_ROOM,
    DO_NOTHING
} perform;

struct condition {
    evaluate eval;
    perform perf_true; /* perform if true */
    perform perf_false; /* perform if false */
    char* eval_target; /* setting or inventory */
    char* perf_true_target; /* message or setting */
    char* perf_false_target; /* message or setting */
    bool stopiftrue; /* true = and, false = or */
    struct condition* next;
};

bool condition_check_and_act( struct condition* c );

static bool evaluate_condition( evaluate ev, char* ev_t );
static void perform_operation( perform p, char* tgt );

evaluate string_to_eval( char* input );
perform string_to_perform( char* input );

void delete_condition( struct condition* c );

#endif /* end of include guard: CONDITION_H_ON4UZMFM */
