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
#include "fileProcess.h"

bool initialized = false;
FILE* file;
int line_number;
char* thisline;
char* nextline;
char* thischunk;
int chunkend;
struct level* thislevel;
struct room*  thisroom;

static char* readFileLine( char* line, int n, FILE* file ) {
    char* ret = NULL;
    while( !feof( file ) && !ferror( file ) && ret == NULL ) {
        ret = trim( fgets( line, n, file ) );
        if( ret != NULL && *ret == '\0' ) ret = NULL;
        if( ret && ret[0] == '#' ) {
            ret = NULL;
            continue;
        }
    }
    return ret;
}

bool initializeFileProcessor( const char* filename ) {
       line_number = 0;
       initialized = false;
       chunkend = 0;
       thisline = malloc(sizeof(char)*LINE_MAX);
       nextline = malloc(sizeof(char)*LINE_MAX);
       thischunk = malloc(sizeof(char)*LINE_MAX);

       file = fopen(filename , "r");
       if (file == NULL || ferror( file ) ) {
           perror("Error opening file");
           return false;
       } 

       /* store first two lines */
       thisline = readFileLine( thisline, LINE_MAX, file );
       nextline = readFileLine( nextline, LINE_MAX, file );

       if( thisline == NULL || nextline == NULL || thischunk == NULL ) {
           perror( "Out of memory or file is no good!" );
           return false;
       }
       initialized = true;

       getNextChunk(); /* get first chunk of line */

       return initialized;
}

void shutdownFileProcessor() {
    fclose( file );
    if( thisline ) free( thisline );
    if( nextline ) free( nextline );
    thisline = NULL;
    nextline = NULL;
    file = NULL;
    initialized = false;
    line_number = 0;
}

static bool hasMoreLines() {
    if( initialized && nextline ) return true;
    return false;
}

static void getNextLine() {
    if( initialized ) {
        if( nextline ) {
            strcpy( thisline, nextline );
            /* FIXME: Need to account for blank skips */
            line_number++;
        }

        nextline = readFileLine( nextline, LINE_MAX, file );
        chunkend = 0;
        getNextChunk(); /* get first chunk of line */
    }
}

static void getNextChunk() {
    if( initialized && thischunk != NULL && thisline != NULL ) {
        int i;
        int chunkstart = chunkend;
        bool found = false;
        int len = strlen( thisline );
        
        if( chunkend >= len ) *thischunk = '\0';

        for( i = chunkend; i < len; i++ ) {
            /* chunks are space delimited except inside quotes */
            if( isspace( thisline[i] ) && i > 0 ) {
                /* we have our chunk */
                strncpy( thischunk, (thisline+chunkstart), (i-chunkend) );
                thischunk[i-chunkend] = '\0';
                chunkend = chunkstart + (i-chunkend) + 1;
                found = true;
                break;
            } else if( thisline[i] == '\"' ) {
                int length = 0;
                /* work on chunk copy, not line */
                strcpy( thischunk, (thisline+i) );
                thischunk = getString( thischunk, &length );
                chunkend = chunkstart + length + 1;
                if( thischunk == NULL ) {
                    unexpectedLine( true );
                }
                found = true;
                break;
            }
        }

        /* find next non-space */
        if( found ) {
            thischunk = trim( thischunk );
            while( isspace( thisline[chunkend] ) && chunkend < len ) chunkend++;
        } else if( *thischunk != '\0' ) {

            /* if we made it here, the last chunk is until the end
             * of the line. I don't expect this to happen much.
             * In fact, I don't expect it to happen at all,
             * but it might in the future.
             */
            strncpy( thischunk, (thisline+chunkend), (len-chunkend) );
            thischunk[(len-chunkend)] = '\0';
            chunkend = len;
        }
    }
}

int isKeyword(char* input) {
    int i;

    assert( input );
    input = trim(input);

    for( i = 0; i < LAST_KEYWORD_ENTRY; i++ ) {
        if( stricmp( input, keyword_strings[i] ) == 0 ) return i;
    }

    return NOT_KEYWORD;
}

char* getString(char* input, int* length) {
    int i, j, len;
    char* start;
    char* end;
    char* ret;

    start = NULL;
    end = NULL;

    assert( length );

    *length = 0;

    if( input == NULL ) return NULL;
    len = strlen(input);

    for( i = 0; i < len; i++ ) {
        if( input[i] == '\"' ) {
            /* skip escaped quotes */
            if( i > 0 && input[i-1] == '\\' ) {
                /* rewind, erase, update len */
                i--;
                for( j = i; j < len; j++ ) {
                    input[j] = input[j+1];
                }
                (*length)++;
                len = strlen(input);
                continue;
            }
            if( start == NULL ) {
                if( i + 1 < len ) {
                    start = (input + i + 1);
                } else {
                    /* error condition */
                    return NULL;
                }
            } else {
                end = (input + i - 1);
                break;
            }
        }
        if( input[i] == 'n' && i > 0 && input[i-1] == '\\' ) {
                /* rewind, erase, update len */
                i--;
                input[i] = '\n';
                for( j = i+1; j < len; j++ ) {
                    input[j] = input[j+1];
                }
                (*length)++;
                len = strlen(input);
                continue;
        }
        (*length)++;
    }

    if( start == NULL || end == NULL ) return NULL;

    ret = malloc( sizeof(char)*(end-start+2) );
    if( ret == NULL ) {
        perror( "Out of memory!" );
        return NULL;
    }

    strncpy( ret, start, end-start+1 );
    ret[(end-start+1)] = '\0';
    strcpy( input, ret );
    free( ret );
    return input;
}

bool processFile() {
    if( !initialized ) return false;
    while( hasMoreLines() ) {
        if( stricmp( thisline, GLOBAL_MARKER ) == 0 ) {
            /* TODO: Add error checking/handling */
            processGlobal();
        } else if( stricmp( thisline, LEVEL_MARKER ) == 0 ) {
            /* TODO: Add error checking/handling */
            processLevel();
        } else {
            unexpectedLine( false );
        }
        getNextLine();
    }
    return true;
}

static bool processGlobal() {
    /* go to next line, we are on marker now */
    while( hasMoreLines() ) {
        getNextLine();
        if( stricmp( thisline, GLOBAL_MARKER_END ) == 0 ) {
            return true;
        } else if( isKeyword( thischunk ) == KEYWORD_SETTING ) {
            /* TODO: Add error checking/handling */
            processSetting( true );
        } else {
            unexpectedLine( false );
        }
    }
    return false;
}

static void unexpectedLine( bool fatal) {
    fprintf( stderr, "Unexpected line %d: %s\n", line_number, thisline );
    fprintf( stderr, "Last chunk: %s\n", thischunk );
    if( fatal ) {
        fprintf( stderr, "Bailing out!\n" );
        fprintf( stderr, "Press any key to exit.\n" );
        getchar();
        closeDisplay();
        exit(1);
    } else {
        fprintf( stderr, "Skipping.\n" );
    }
}

static bool processLevel() {
    /* advance beyond marker */
    getNextLine();

    thislevel = malloc( sizeof(struct level) );
    assert( thislevel );
    memset( thislevel, 0, sizeof(struct level) );
    if( thislevel == NULL ) return false;

    while( hasMoreLines() ) {
        /* expected either setting, room or end level marker */
        if( isKeyword( thischunk ) == KEYWORD_SETTING ) {
            processSetting(false);
        } else if( stricmp( thisline, ROOM_MARKER ) == 0 ) { 
            processRoom();
        } else if( stricmp( thisline, LEVEL_MARKER_END ) == 0 ) {
            if( thislevel != current_level ) {
                add_level( thislevel );
            }
            return true;
        } else {
            unexpectedLine( false );
        }
        getNextLine();
    }

    /* FIXME: Duplicated code */
    if( stricmp( thisline, LEVEL_MARKER_END ) == 0 ) {
        if( thislevel != current_level ) {
            add_level( thislevel );
        }
        return true;
    }

    return false;
}

static bool processSetting( bool global ) {
    struct setting* newsetting = NULL;
    int ret = SUCCESS;

    newsetting = malloc( sizeof( struct setting ) );
    assert(newsetting);
    memset( newsetting, 0, sizeof( struct setting ) );

    /* process setting line */

    /* chunk is on setting keyword now
     * move forward to the value (string)
     */
    getNextChunk();

    newsetting->name = malloc( sizeof(char)*(strlen(thischunk)+1) );
    assert(newsetting->name);

    strcpy( newsetting->name, thischunk );

    getNextChunk();

    if( isKeyword( thischunk ) == KEYWORD_TRUE ) {
        newsetting->value = true;
    } else if( isKeyword( thischunk ) == KEYWORD_FALSE ) {
        newsetting->value = false;
    } else {
        unexpectedLine( false );
    }

    /* insert into appropriate structure */

    if( global ) {
        ret = add_global_setting( newsetting );
    } else {
        ret = add_level_setting( thislevel, newsetting );
    }

    return (ret == SUCCESS);
}


static bool processRoom() {
    /* advance beyond marker */
    getNextLine();

    thisroom = malloc( sizeof( struct room ) );
    assert( thisroom );
    memset( thisroom, 0, sizeof( struct room ) );

    /* valid entries:
     * name
     * longmessage
     * shortmessage
     * item
     * entity
     * exit
     */

    /* we are on room marker line. advance */
    while( hasMoreLines() ) {
        if( stricmp( thisline, ROOM_MARKER_END ) == 0 ) {
            /* at end room marker, add to current level */
            add_level_room( thislevel, thisroom );
            return true;
        } else if( isKeyword( thischunk ) == KEYWORD_NAME ) {
            getNextChunk();
            if( thischunk ) {
                if( thisroom->name ) free( thisroom->name );
                thisroom->name = malloc( sizeof(char*)*(strlen(thischunk)+1) );
                assert( thisroom->name );
                strcpy( thisroom->name, thischunk);
            }
        } else if( isKeyword( thischunk ) == KEYWORD_LONGMESSAGE ) {
            getNextChunk();
            if( thischunk ) {
                if( thisroom->long_message ) free( thisroom->long_message );
                thisroom->long_message = malloc( sizeof(char*)*(strlen(thischunk)+1) );
                assert( thisroom->name );
                strcpy( thisroom->long_message, thischunk);
            }
        } else if( isKeyword( thischunk ) == KEYWORD_SHORTMESSAGE ) {
            getNextChunk();
            if( thischunk ) {
                if( thisroom->short_message ) free( thisroom->short_message );
                thisroom->short_message = malloc( sizeof(char*)*(strlen(thischunk)+1) );
                assert( thisroom->name );
                strcpy( thisroom->short_message, thischunk);
            }
            /* item and entity processing */
        } else if( isKeyword( thischunk ) == KEYWORD_ITEM ) {
            add_room_entity( thisroom, processEntity( ENTITY_TAKEABLE ) );
        } else if( isKeyword( thischunk ) == KEYWORD_ENTITY ) {
            add_room_entity( thisroom, processEntity( ENTITY_NORMAL ) );
        } else if( isKeyword( thischunk ) == KEYWORD_VIEWABLE ) {
            add_room_entity( thisroom, processEntity( ENTITY_VIEWONLY ) );
        } else if( isKeyword( thischunk ) == KEYWORD_EXIT ) {
            add_room_exit( thisroom, processExit() );
        } else {
            unexpectedLine( false );
        }

        getNextLine();
    }

    return false;
}

static struct exit * processExit() {
    /* exit "z" TO "room" condition COND "V" true DO "X" false DO "Y" */
    struct exit * e;

    /* exit name */
    getNextChunk();
    /* are we adding to an existing entry? */
    /* FIXME: This doesn't make much sense. 
     *        I should probably make multiple conditions
     *        possible by adding the condition keyword
     *        multiple times on the same line.
     */
    e = find_room_exit( thisroom, thischunk );

    if( !e ) {
        /* create the exit entry */
        e = malloc( sizeof(struct exit) );
        assert( e );
        memset( e, 0, sizeof(struct exit) );
        e->name = malloc( sizeof( char ) * (strlen( thischunk ) + 1) );
        assert( e->name );
        strcpy( e->name, thischunk );
    }

    /* to */
    getNextChunk();
    if( isKeyword( thischunk ) != KEYWORD_TO ) {
        unexpectedLine( false );
        return NULL;
    }

    /* room name */
    /* NOTE: If this is an existing exit, the new desintation will
     * be ignored */
    getNextChunk();
    if( !e->destination ) {
        e->destination = malloc( sizeof( char ) * (strlen( thischunk ) + 1) );
        assert( e->destination );
        strcpy( e->destination, thischunk );
    }

    /* condition */
    getNextChunk();
    if( isKeyword( thischunk) == KEYWORD_NO_CONDITION || *thischunk == '\0' ) {
        /* condition is optional */
        e->exit_condition = NULL;
        e->next = NULL;
    } else {
        struct condition * c = processCondition();
        assert( c );
        add_exit_condition( e, c );
    }

    return e;
}

static struct entity * processEntity( entity_type entype ) {
    /* entity "y" "room_message" action "command" condition COND "V" true DO "X" false DO "Y" */
    /* entity "y" nomessage action "command" condition COND "V" true DO "X" false DO "Y" */
    struct entity* e;
    struct action* a;
    
    /* entity name */
    getNextChunk();
    /* are we adding to an existing entry? */
    e = find_room_entity( thisroom, thischunk );

    if( !e ) {
        /* create the entity entry */
        e = malloc( sizeof(struct entity) );
        assert( e );
        memset( e, 0, sizeof(struct entity) );
        e->name = malloc( sizeof( char ) * (strlen( thischunk ) + 1) );
        assert( e->name );
        strcpy( e->name, thischunk );
    }

    e->takeable = ( entype == ENTITY_TAKEABLE );

    getNextChunk();
    if( isKeyword( thischunk ) == KEYWORD_NOALIAS ) {
        e->alias = NULL;
    } else {
        e->alias = malloc( sizeof(char) * (strlen( thischunk ) + 1) );
        assert( e->alias );
        strcpy( e->alias, thischunk );
    }

    if( entype != ENTITY_VIEWONLY ) {
        getNextChunk();
        if( isKeyword( thischunk ) == KEYWORD_NOMESSAGE ) {
            e->room_message = NULL;
        } else {
            e->room_message = malloc( sizeof(char) * (strlen( thischunk ) + 1) );
            assert( e->room_message );
            strcpy( e->room_message, thischunk );
        }

        getNextChunk();
        if( isKeyword( thischunk ) != KEYWORD_ACTION ) {
            unexpectedLine( true );
        }

        /* process action */
        a = processAction();
        assert( a );
        add_entity_action( e, a );
    } else {
        /* create a pre-fab action for a viewable entity
         * - action is "look" or "look at" or "look out"
         *    NOTE: This is handled special in the user input function
         * - condition is always true
         * - action is "set_prompt <message>"
         * - false is do nothing
         */
        struct action* a;
        struct condition* c;
        e->room_message = NULL;

        /* initialize action */
        a = malloc( sizeof( struct action ) );
        assert( a );
        memset( a, 0, sizeof( struct action ) );
        a->next = NULL;

        /* initialize condition */
        c = malloc( sizeof( struct condition ) );
        assert( c );
        memset( c, 0, sizeof( struct condition ) );

        /* XXX: look is special and handled differently in 
         * the viewing/processing function */
        a->command = malloc(sizeof(char)*strlen("look")+1);
        strcpy( a->command, "look" );

        c->eval = EVALUATE_ALWAYS;
        c->eval_target = NULL;
        c->perf_true = SET_PROMPT;
        c->perf_false = DO_NOTHING;

        getNextChunk();
        assert( thischunk );
        c->perf_true_target = malloc( sizeof(char)*(strlen(thischunk)+1) );
        assert( c->perf_true_target );
        strcpy( c->perf_true_target, thischunk );

        c->perf_false_target = NULL;
        c->stopiftrue = true;

        a->action_condition = c;
        add_entity_action( e, a );
    }

    return e;
}

static struct action * processAction() {
    struct action* a;

    /* TODO: Search for existing action? */
    a = malloc( sizeof( struct action ) );
    assert( a );
    memset( a, 0, sizeof( struct action ) );

    a->next = NULL;

    getNextChunk();
    assert( thischunk );

    a->command = malloc( sizeof( char ) * (strlen( thischunk ) + 1) );
    assert( a->command );

    strcpy( a->command, thischunk );

    getNextChunk();

    if( isKeyword( thischunk) == KEYWORD_NO_CONDITION || *thischunk == '\0' ) {
        a->action_condition = NULL;
    } else if ( isKeyword( thischunk ) == KEYWORD_CONDITION ) {
        a->action_condition = processCondition();
    } else {
        unexpectedLine( false );
        free( a->command );
        free( a );
        a = NULL;
    }

    return a;
}

static struct condition * processCondition() {
    struct condition* head;
    struct condition* c;
    bool moreconditions = false;

    head = NULL;

    do {
        moreconditions = false;
        c = malloc( sizeof( struct condition ) );
        assert( c );
        memset( c, 0, sizeof( struct condition ) );

        /* condition COND "V" true DO "X" false DO "Y" */
        /* eval target
         * true action target
         * false action target
         */

        getNextChunk();
        assert( thischunk );
        c->eval = string_to_eval( thischunk );

        if( c->eval != EVALUATE_ALWAYS ) {
            getNextChunk();
            assert( thischunk );
            c->eval_target = malloc( sizeof(char)*(strlen(thischunk)+1) );
            assert( c->eval_target );
            strcpy( c->eval_target, thischunk );
        } else {
            c->eval_target = NULL;
        }

        getNextChunk();
        if( isKeyword( thischunk ) != KEYWORD_TRUE ) {
            unexpectedLine( false );
            free( c->eval_target );
            free( c );
            return NULL;
        }

        getNextChunk();
        assert( thischunk );
        c->perf_true = string_to_perform( thischunk );

        if( c->perf_true != DO_NOTHING ) {
            getNextChunk();
            assert( thischunk );
            c->perf_true_target = malloc( sizeof(char)*(strlen(thischunk)+1) );
            assert( c->perf_true_target );
            strcpy( c->perf_true_target, thischunk );
        } else {
            c->perf_true_target = NULL;
        }

        getNextChunk();
        if( isKeyword( thischunk ) != KEYWORD_FALSE ) {
            unexpectedLine( false );
            free( c->eval_target );
            free( c->perf_true_target );
            free( c );
            return NULL;
        }

        getNextChunk();
        assert( thischunk );
        c->perf_false = string_to_perform( thischunk );

        if( c->perf_false != DO_NOTHING ) {
            getNextChunk();
            assert( thischunk );
            c->perf_false_target = malloc( sizeof(char)*(strlen(thischunk)+1) );
            assert( c->perf_false_target );
            strcpy( c->perf_false_target, thischunk );
        } else {
            c->perf_false_target = NULL;
        }

        /* next chunk is either NULL (EOL) or and/or */
        getNextChunk();
        if( thischunk != NULL && *thischunk != '\0' ) {

            if( isKeyword( thischunk ) == KEYWORD_STOPIFTRUE ) {
                c->stopiftrue = true;
            } else if ( isKeyword( thischunk ) == KEYWORD_STOPIFFALSE ) {
                c->stopiftrue = false;
            } else {
                unexpectedLine( true );
            }

            getNextChunk(); /* advance chunk parser to condition keyword */
            if( isKeyword( thischunk ) != KEYWORD_CONDITION ) {
                unexpectedLine( true );
            }

            moreconditions = true;
        }

        if( head == NULL ) {
            head = c;
        } else {
            struct condition* iter;
            /* add newly-created condition to list */
            iter = head;
            assert( iter );
            while( iter->next ) {
                iter = iter->next;
            }
            iter->next = c;
        }
    } while( moreconditions );

    return head;
}
