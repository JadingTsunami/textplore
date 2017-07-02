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
#include "exit.h"

int add_exit_condition( struct exit* e, struct condition* c ) {
    if( e == NULL ) return ERROR;
    if( e->exit_condition == NULL ) {
        e->exit_condition = c;
    } else {
        struct condition* iter = e->exit_condition;
        while( iter->next ) {
            iter = iter->next;
        }
        iter->next = c;
    }
    return SUCCESS;
}

int link_exit( struct exit* first, struct exit* second ) {
    if( first == NULL || second == NULL ) return ERROR;
    first->next = second;
    return SUCCESS;
}
