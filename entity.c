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
#include "entity.h"

int add_entity_action( struct entity* e, struct action* a ) {
    if( e == NULL ) return ERROR;
    if( e->interaction == NULL ) {
        e->interaction = a;
    } else {
        struct action* iter = e->interaction;
        while( iter->next ) {
            iter = iter->next;
        }
        iter->next = a;
    }
    return SUCCESS;
}

int link_entity( struct entity* first, struct entity* second ) {
    if( first == NULL || second == NULL ) return ERROR;
    first->next = second;
    return SUCCESS;
}

void delete_entity( struct entity* e ) {
    assert( e );

    struct action* a = e->interaction;
    while( a ) {
        delete_action(a);
        a = a->next;
    }

    free( e->name );
    free( e );
}
