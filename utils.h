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
/*
 * Utility Functions.
 */

#ifndef UTILS_H_5YFM1GMO
#define UTILS_H_5YFM1GMO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>

/* DEFINES */
#define ERROR (-1)
#define SUCCESS (1)

#define DEBUG (1)
#define debug_print(fmt, ...) \
        do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)

static const char* ignored_words[] = {
    " the "
};

#define NUM_IGNORED_WORDS (1)

/* Function: stricmp
 * Description: Case-insensitive string comparison.
 */
int stricmp( const char* str1, const char* str2 );


/* Function: strstart
 * Description: Check if string starts with other string.
 */
int strstart( const char *str, const char *start );

/* Function: trim
 * Trim whitespace from ends of a string.
 * Returns newly-malloc'd version.
 */
char* trim( char *str );

/* Clean up user input. Remove ignored words and
 * shrink double spaces.
 */
char* clean_input( char* str );

#endif /* end of include guard: UTILS_H_5YFM1GMO */
