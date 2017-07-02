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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>

#ifndef TEXTPLORE_H_DBT2MPKL
#define TEXTPLORE_H_DBT2MPKL

#include "utils.h"

#include "level.h"

#include "action.h"
#include "condition.h"
#include "entity.h"
#include "exit.h"
#include "room.h"
#include "setting.h"
#include "fileProcess.h"

/* !! IMPORTANT !!
 * File initialization must happen *before*
 * display initialization.
 *
 * File errors do NOT call the proper display
 * shutdown routines. You will corrupt the
 * terminal if you initialize out of order.
 */

/* DEFINES */
#define DEFAULT_LEVEL_FILE "levels.txt"

/* STRUCTS */

/* GAME STRUCTS */
struct level * current_level;
struct room * current_room;
struct setting * global_settings;
struct inventory {
    char* item;
    struct inventory* next;
} player_inventory;

/* Exit verbs */

static char* default_exit_verbs[] = {
    "go through",
    "open",
    "walk into",
    "go into",
    "go",
    "enter",
    "walk to",
    "walk in",
    "walk through",
    "run through",
    "run into",
    "run to"
};

#define NUM_DEFAULT_EXIT_VERBS (12)

static char* default_item_verbs[] = {
    "pick up",
    "take",
    "get",
    "grab",
};

#define NUM_DEFAULT_ITEM_VERBS (4)

static char* default_viewable_verbs[] = {
    "look at",
    "look out",
    "look through",
    "look toward",
    "what is",
    "what are",
    "watch",
    "look",
    "view",
    "see",
    "l",
};

#define NUM_DEFAULT_VIEWABLE_VERBS (11)

/* FUNCTIONS */

void change_room( char* destination );

int add_global_setting( struct setting* s );
struct setting* get_global_setting( char* name );
int set_global_setting( char* name, bool value );

struct inventory* find_inventory_item( char* item );

void add_level( struct level* l );

void processUserInput( char* str );

/* Returns TRUE if verb/object pair could be mapped to
 * an entity.
 *
 * Returns FALSE if no pairing could be found.
 */
bool match_entity_with_input( char* verb, char* object, bool itemonly );
bool try_exit( char* exit_name );

void add_to_inventory( struct entity* e );
void show_inventory();
void show_help();

void showRoomMessage( bool visited );

#endif /* end of include guard: TEXTPLORE_H_DBT2MPKL */
