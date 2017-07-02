/*
 *  Textplore: A text adventure game engine.
 *
 *  Copyright (C) 2014  Jading Tsunami
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
#ifndef EXIT_H_W7F5YF38
#define EXIT_H_W7F5YF38

#include "utils.h"
#include "condition.h"
#include "room.h"

/* Exit room is stored as a string
 * referring to the room name. This
 * prevents us from having to link
 * rooms after the entire file is
 * processed.
 */
struct exit {
    char* name;
    struct condition* exit_condition;
    char* destination; /* room name */
    struct exit* next;
};

int add_exit_condition( struct exit* e, struct condition* c );
int link_exit( struct exit* first, struct exit* second );

#endif /* end of include guard: EXIT_H_W7F5YF38 */

