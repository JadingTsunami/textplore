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
#include "action.h"

struct action* create_action( char* command ) {
    struct action* a = malloc( sizeof( struct action ) );
    if( a == NULL ) return NULL;

    a->command          = command;
    a->action_condition = NULL;
    a->next             = NULL;

    return a;
}

bool match_action( struct action* a, char* str ) {
    int i;
    if( str == NULL || a == NULL || a->command == NULL ) return false;

    for( i = 0; i < strlen(str); i++ ) {
        if( a->command[i] != str[i] ) return false;
    }

    return true;
}

int set_action_condition( struct action* a, struct condition* c ) {
    if( a == NULL ) return ERROR;
    a->action_condition = c;
    return SUCCESS;
}

int link_action( struct action* first, struct action* second ) {
    if( first == NULL || second == NULL ) return ERROR;
    first->next = second;
    return SUCCESS;
}

void delete_action( struct action* a ) {
    assert(a);
    struct condition* c = a->action_condition;
    free( a->command );
    while( c ) {
        delete_condition( a->action_condition );
        c = c->next;
    }
    free(a);
}
