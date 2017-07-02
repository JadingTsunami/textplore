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
#ifndef DISPLAY_H_VJ5YL0LE
#define DISPLAY_H_VJ5YL0LE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>

#define MAX_USER_INPUT (64)

#define MIN_LINES (24)
#define MIN_COLS  (80)

#define MAX_LINES (80)
#define MAX_COLS  (200)

WINDOW *topwin;
WINDOW *botwin;

void initDisplay();
void closeDisplay();
void getUserInput(char* str);

void setMessage( char* msg );
void showInventory();
void setPrompt( char* prompt );
void clearPrompt();

static char * wordwrap( char* str, int COLS );

#endif /* end of include guard: DISPLAY_H_VJ5YL0LE */

