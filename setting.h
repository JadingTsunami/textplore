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
#ifndef SETTING_H_ZMTLK4WR
#define SETTING_H_ZMTLK4WR

#include "utils.h"

struct setting {
    char* name;
    bool value;
    struct setting* next;
};

struct setting* create_setting( char* name, bool value );
int link_setting( struct setting* first, struct setting* second );

#endif /* end of include guard: SETTING_H_ZMTLK4WR */

