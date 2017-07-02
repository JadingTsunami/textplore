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
#ifndef ENTITY_H_E0IVHC5D
#define ENTITY_H_E0IVHC5D

#include "action.h"

struct entity {
    char* name;
    char* room_message;
    char* alias;
    bool takeable;
    struct action* interaction;
    struct entity* next;
};

int add_entity_action( struct entity* e, struct action* a );
int link_entity( struct entity* first, struct entity* second );
void delete_entity( struct entity* e );

#endif /* end of include guard: ENTITY_H_E0IVHC5D */

