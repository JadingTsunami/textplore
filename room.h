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
#ifndef ROOM_H_Z1KNWIFX
#define ROOM_H_Z1KNWIFX

#include <assert.h>
#include "utils.h"
#include "exit.h"
#include "display.h"
#include "entity.h"

/* A linked list of all rooms in a level.
 * Note the linkage has nothing to do with whether rooms
 * can be exited/entered from one another -- exits do that.
 */
struct room {
    char* name;
    char* long_message;
    char* short_message;
    bool visited;
    struct exit* exits;
    struct entity* entities;
    struct room* next;
};

/* room management functions */
struct room* create_room( char* name, char* long_message, char* short_message );

struct exit* find_room_exit( struct room* r, char* exit_name );
int add_room_exit( struct room* r, struct exit* ex );

struct entity* find_room_entity( struct room* r, char* entity_name );
int add_room_entity( struct room* r, struct entity* en );

int link_room( struct room* first, struct room* second ); 

void set_room_short_message( struct room* r, char* msg );

void set_room_long_message( struct room* r, char* msg );

#endif /* end of include guard: ROOM_H_Z1KNWIFX */
