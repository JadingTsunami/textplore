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
#ifndef ACTION_H_PSOL9UGA
#define ACTION_H_PSOL9UGA

#include "utils.h"
#include "condition.h"

struct action {
    char* command;
    struct condition* action_condition;
    struct action* next;
};

struct action* create_action( char* command );

/* match_action
 * Determine if an action string matches this action.
 * Any subset will return true.
 * Any mismatch will return false.
 */
bool match_action( struct action* a, char* str );

int set_action_condition( struct action* a, struct condition* c );

int link_action( struct action* first, struct action* second );

void delete_action( struct action* a );

#endif /* end of include guard: ACTION_H_PSOL9UGA */

