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
#ifndef LEVEL_H_SIUCDRRN
#define LEVEL_H_SIUCDRRN

#include "utils.h"
#include "room.h"
#include "setting.h"

struct level {
    struct room* room_head;
    struct setting* level_settings_head;
    struct level* next;
};

int add_level_setting( struct level* l, struct setting* s );
struct setting* get_level_setting( struct level* l, char* name );
int set_level_setting( struct level* l, char* name, bool value );

void set_level_entry_room( struct level* l, struct room* head );
void add_level_room( struct level* l, struct room* r );

struct room* find_room( struct level* l, char* name );

#endif /* end of include guard: LEVEL_H_SIUCDRRN */

