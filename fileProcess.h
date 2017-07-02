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
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#ifndef FILEPROCESS_H_DOREDCPQ
#define FILEPROCESS_H_DOREDCPQ

#include "display.h"
#include "action.h"
#include "condition.h"
#include "display.h"
#include "entity.h"
#include "exit.h"
#include "level.h"
#include "room.h"
#include "setting.h"
#include "textplore.h"
#include "utils.h"

#define GLOBAL_MARKER     "[GLOBAL]"
#define GLOBAL_MARKER_END "[ENDGLOBAL]"
#define LEVEL_MARKER      "[LEVEL]"
#define LEVEL_MARKER_END  "[ENDLEVEL]"
#define ROOM_MARKER       "[ROOM]"
#define ROOM_MARKER_END   "[ENDROOM]"

#define LINE_MAX ((MAX_COLS-2)*(MAX_LINES-4))

#define NOT_KEYWORD (-1)

enum file_keywords {
    KEYWORD_SETTING = 0,
    KEYWORD_TRUE,
    KEYWORD_FALSE,
    KEYWORD_NAME,
    KEYWORD_LONGMESSAGE,
    KEYWORD_SHORTMESSAGE,
    KEYWORD_ITEM,
    KEYWORD_ENTITY,
    KEYWORD_EXIT,
    KEYWORD_TO,
    KEYWORD_ACTION,
    KEYWORD_CONDITION,
    KEYWORD_NO_CONDITION,
    KEYWORD_STOPIFTRUE,
    KEYWORD_STOPIFFALSE,
    KEYWORD_NOMESSAGE,
    KEYWORD_NOALIAS,
    KEYWORD_VIEWABLE,
    LAST_KEYWORD_ENTRY
} keywords;

typedef enum entity_types {
    ENTITY_NORMAL = 0,
    ENTITY_VIEWONLY,
    ENTITY_TAKEABLE,
    LAST_ENTITY_TYPE
} entity_type;

static char* keyword_strings[] = {
    "setting",
    "true",
    "false",
    "name",
    "longmessage",
    "shortmessage",
    "item",
    "entity",
    "exit",
    "to",
    "action",
    "condition",
    "nocondition",
    "stopiftrue",
    "stopiffalse",
    "nomessage",
    "noalias",
    "viewable"
};


bool initializeFileProcessor( const char* filename );
void shutdownFileProcessor();

/* File Processing Functions
 *
 * After initialization, the file is processed in two ways:
 * Linewise
 * Chunkwise
 *
 * Chunks are keywords (delimited by spaces) or strings
 * contained in quotes.
 *
 * The line tokenizer and chunk tokenizer run in parallel
 * on the same line. 
 *
 * To advance the line tokenizer, call getNextLine().
 * To advance the chunk tokenizer, call getNextChunk().
 *
 * The first chunk is tokenized upon the advancement of
 * the line tokenizer.
 */
static bool hasMoreLines();
static void getNextLine();
static void getNextChunk();
static char* readFileLine( char* line, int n, FILE* file );
static void unexpectedLine( bool fatal );

static int isKeyword(char* input);
static char* getString(char* input, int* length);

/* Function: processFile
 * Main function, processes entire input file
 * at once
 */
bool processFile();

/* Section Types */
static bool processGlobal();
static bool processLevel();
static bool processRoom();

/* Line Types */
static bool processSetting( bool global );
static struct exit * processExit();
static struct entity * processEntity( entity_type entype );

/* Component Types */
static struct action * processAction();
static struct condition * processCondition();




#endif /* end of include guard: FILEPROCESS_H_DOREDCPQ */

