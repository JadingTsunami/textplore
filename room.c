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
/* room.c */
#include "room.h"

struct room* create_room( char* name, char* long_message, char* short_message ) {
    struct room* r = malloc( sizeof(struct room) );
    if( r == NULL ) return NULL; /* out of memory */

    r->name          = name;
    r->long_message  = long_message;
    r->short_message = short_message;
    r->visited       = false;
    r->exits         = NULL;
    r->entities      = NULL;
    r->next          = NULL;

    return r;
}

int add_room_exit( struct room* r, struct exit* ex ) {
    if( r == NULL ) return ERROR;
    if( ex == find_room_exit( r, ex->name ) ) return SUCCESS; /* nothing to do */
    if( r->exits == NULL ) {
        r->exits = ex;
    } else {
        struct exit* iter = r->exits;
        while( iter->next ) {
            iter = iter->next;
        }
        iter->next = ex;
    }
    return SUCCESS;
}

int add_room_entity( struct room* r, struct entity* en ) {
    if( r == NULL ) return ERROR;
    if( en == find_room_entity( r, en->name ) ) return SUCCESS; /* nothing to do */
    if( r->entities == NULL ) {
        r->entities = en;
    } else {
        struct entity* iter = r->entities;
        while( iter->next ) {
            iter = iter->next;
        }
        iter->next = en;
    }
    return SUCCESS;
}

int link_room( struct room* first, struct room* second ) {
    if( first == NULL || second == NULL ) return ERROR;
    first->next = second;
    return SUCCESS;
}

void set_room_long_message( struct room* r, char* msg ) {
    if( r && msg ) {
        if( r->long_message ) {
            free( r->long_message );
        }
        r->long_message = malloc( sizeof( char ) * strlen( msg ) );
        assert( r->long_message );
        strcpy( r->long_message, msg );
        if( current_room == r ) {
            setMessage( r->long_message );
        }
    }
}


void set_room_short_message( struct room* r, char* msg ) {
    if( r && msg ) {
        if( r->short_message ) {
            free( r->short_message );
        }
        r->short_message = malloc( sizeof( char ) * strlen( msg ) );
        assert( r->short_message );
        strcpy( r->short_message, msg );
        if( current_room == r ) {
            setMessage( r->short_message );
        }
    }
}

struct exit* find_room_exit( struct room* r, char* exit_name ) {
    struct exit* e;

    assert( r );
    assert( exit_name );

    e = r->exits;
    
    while( e ) {
        if( stricmp( e->name, exit_name ) == 0 ) {
            return e;
        }
        e = e->next;
    }

    return NULL;
}

struct entity* find_room_entity( struct room* r, char* entity_name ) {
    struct entity* e;

    assert( r );
    assert( entity_name );

    e = r->entities;
    
    while( e ) {
        if( stricmp( e->name, entity_name ) == 0 ) {
            return e;
        }
        e = e->next;
    }

    return NULL;
}
