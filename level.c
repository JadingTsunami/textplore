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
#include "level.h"

int add_level_setting( struct level* l, struct setting* s ) {
    if( l == NULL ) return ERROR;
    if( l->level_settings_head == NULL ) {
        l->level_settings_head = s;
    } else {
        struct setting* iter = l->level_settings_head;
        while( iter->next ) {
            iter = iter->next;
        }
        iter->next = s;
    }
    return SUCCESS;
}

struct setting* get_level_setting( struct level* l, char* name ) {
    if( l == NULL ) return NULL;
    struct setting* iter = l->level_settings_head;
    while( (iter != NULL) && (strcmp( iter->name, name ) != 0) ) {
        iter = iter->next;
    }
    return iter;
}

int set_level_setting( struct level* l, char* name, bool value ) {
    if( l == NULL || name == NULL ) return ERROR;
    struct setting* iter = l->level_settings_head;
    while( (iter != NULL) && (strcmp( iter->name, name ) != 0) ) {
        iter = iter->next;
    }
    if( iter == NULL ) return ERROR;
    iter->value = value;
    return SUCCESS;
}

void set_level_entry_room( struct level* l, struct room* head ) {
    if( l == NULL ) return;
    l->room_head = head;
}

struct room* find_room( struct level* l, char* name ) {
    if( l == NULL ) return NULL;
    struct room* r = l->room_head;
    while( (r != NULL) && (strcmp( r->name, name ) != 0) ) {
        r = r->next;
    }
    return r;
}


void add_level_room( struct level* l, struct room* r ) {
    if( l == NULL || r == NULL ) return;

    struct room * iter;

    if( l->room_head == NULL ) {
        set_level_entry_room( l, r );
    } else {
        iter = l->room_head;
        while( iter->next ) iter = iter->next;
        iter->next = r;
    }
}
