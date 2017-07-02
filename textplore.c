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
#include "textplore.h"

/*
 * TODO:
 * - Add an "exit" or "quit" command
 * - Create a "set prompt + variable" option
 * - Create a "short name" for entities
 * - Need to add "EXIT_LEVEL" action operation
 * - Add item drop ability
 * - Add inventory size limit
 */

int main(int argc, char const *argv[]) {
    char user_input_string[MAX_USER_INPUT] = { 0 };

    initDisplay();
    if( argc > 1 ) {
        initializeFileProcessor( argv[1] );
    } else {
        if( access( DEFAULT_LEVEL_FILE, F_OK ) != -1 ) {
            initializeFileProcessor( DEFAULT_LEVEL_FILE );
        } else {
            closeDisplay();
            printf("Error! No levels found.\n");
            printf("Either supply a level file as an argument, or provide a default level named " DEFAULT_LEVEL_FILE " in the run directory.\n");
            exit(1);
        }
    }

    processFile();

    memset( &player_inventory, 0, sizeof( struct inventory ) );

    assert( current_level );

    current_room = current_level->room_head;
    assert( current_room );
    showRoomMessage( false );

    while( true ) {
        getUserInput( user_input_string );
        processUserInput( user_input_string );
    }

    shutdownFileProcessor();
    closeDisplay();
    return 0;
}

void showRoomMessage( bool visited ) {
    int len = 0;
    struct entity * e;
    char* msg = malloc( sizeof(char) * MAX_LINES * MAX_COLS );

    assert( current_room );
    assert( msg );

    if( visited ) {
        strcpy( msg, current_room->short_message );
    } else {
        strcpy( msg, current_room->long_message );
    }

    len = strlen( msg );
    /* search for any entities with a room message */
    e = current_room->entities;

    while(e && (len < (MAX_LINES * MAX_COLS)) ) {
        if( e->room_message ) {
            strcat( msg, "\n\n" );
            strcat( msg, e->room_message );
            len = strlen( msg );
        }
        e = e->next;
    }

    setMessage( msg );
}

void processUserInput( char* str ) {
    char* verb;
    char* object = NULL;

    /* The idea is to split up the user input between
     * verb and object.
     * We know the sentence structure will typically
     * be something like "verb object", such as
     * "go through door", but we don't know how
     * long either section will be.
     */

    /* clean up input */
    str = clean_input( str );

    /* FIXME: Should probably make this a struct
     * with function pointers, to support future
     * fixed command strings, but will do that
     * later
     */
    if( stricmp( str, "look" ) == 0 || stricmp( str, "l" ) == 0 ) {
        showRoomMessage( false );
        setPrompt( "" );
    } else if( stricmp( str, "?" ) == 0 || stricmp( str, "help" ) == 0 ) {
        show_help();
    } else if( stricmp( str, "show inventory" ) == 0 || stricmp( str, "i" ) == 0 ) {
        show_inventory();
    } else {
        /* we are either going to find a verb-object
         * pair of item, entity or exit.
         *
         * Exits have default verbs, so we just
         * need to make an object pairing there.
         */

        /* loop through the string, chunking it by
         * spaces. Start with assuming the verb is
         * the first word, and loop through all
         * possible verbs in the room and among the
         * defaults. If we find a match, great.
         */
        int i;
        struct entity* e = current_room->entities;
        struct exit* ex  = current_room->exits;
        char* end = strchr( str, '\0' );
        verb = str;
        object = strchr( str, ' ' );

        /* FIXME: Print error to user? */
        if( !object ) return;
        *object = '\0';
        object++;

        while( object < end ) {

            if( match_entity_with_input( verb, object, false ) ) return;

            object--;
            *object = ' '; /* put back the space */
            object = strchr( object+1, ' ' );
            if( !object ) break;
            *object = '\0';
            object++;
        }

        /* ok, so we didn't find what we wanted. */
        /* let's search default verbs */
        for( i = 0; i < NUM_DEFAULT_ITEM_VERBS; i++ ) {
            verb = strstr( str, default_item_verbs[i] );
            if( verb ) {
                object = verb + strlen( default_item_verbs[i] );
                *object = '\0';
                object++;
                if( object < end && match_entity_with_input( verb, object, true ) ) return;
            }
        }

        for( i = 0; i < NUM_DEFAULT_VIEWABLE_VERBS; i++ ) {
            verb = strstr( str, default_viewable_verbs[i] );
            if( verb ) {
                object = verb + strlen( default_viewable_verbs[i] );
                *object = '\0';
                object++;
                if( object < end && match_entity_with_input( "look", object, false ) ) return;
            }
        }

        /* let's check exits now. */
        for( i = 0; i < NUM_DEFAULT_EXIT_VERBS; i++ ) {
            verb = strstr( str, default_exit_verbs[i] );
            if( verb ) {
                object = verb + strlen( default_exit_verbs[i] );
                *object = '\0';
                object++;
                if( object < end && try_exit( object ) ) return;
            }
        }
        setPrompt( "You can't do that." );
    }

}

bool match_entity_with_input( char* verb, char* object, bool itemonly ) {
    struct entity* e = current_room->entities;
    bool ret = false;
    /* search for matching command verb
     * among things in the room */
    /* FIXME: Should we use the 'match_action'
     * function here? That would allow substring
     * matches to work.
     */
    while( e && !ret ) {
        if( itemonly && !e->takeable ) {
            e = e->next;
            continue;
        }

        struct action* a = e->interaction;
        while( a && !ret ) {
            if( (itemonly || stricmp( verb, a->command ) == 0) && ((stricmp( object, e->name ) == 0 )||(stricmp( object, e->alias ) == 0)) ) {
                /* found the pair! */
                struct condition* c = a->action_condition;
                ret = true;
                if( !c ) {
                    if( e->takeable ) {
                        char str[MAX_COLS];
                        strcpy( str, "Picked up " );
                        strcat( str, e->name );
                        strcat( str, "." );
                        add_to_inventory( e );
                        setPrompt( str );
                        /* always show long message after item get */
                        showRoomMessage( false );
                    }
                    return ret;
                }

                while( c ) {
                    bool met = condition_check_and_act(c);
                    if( (c->stopiftrue == met) || (met && c->next == NULL) ) {
                        if( e->takeable ) {
                            add_to_inventory( e );
                            showRoomMessage( false );
                        }
                        return ret;
                    }
                    c = c->next;
                }
            }
            a = a->next;
        }
        e = e->next;
    }
    return ret;
}

bool try_exit( char* exit_name ) {
    struct exit* ex = current_room->exits;
    bool ret = false;

    while( ex ) {
        if( stricmp( exit_name, ex->name ) == 0 ) {
            ret = true;
            if( ex->exit_condition ) {
                struct condition* c = ex->exit_condition;
                while( c ) {
                    bool met = condition_check_and_act(c);
                    if( (c->stopiftrue == met) || (met && c->next == NULL) ) {
                        break;
                    }
                    c = c->next;
                }
            } else {
                change_room( ex->destination );
            }
            break;
        }
        ex = ex->next;
    }

    return ret;
}

/* Global settings functions */
int add_global_setting( struct setting* s ) {
    if( !global_settings ) {
        global_settings = s;
    } else {
        struct setting* iter = global_settings;
        while( iter->next ) {
            iter = iter->next;
        }
        iter->next = s;
    }
    return SUCCESS;
}

struct setting* get_global_setting( char* name ) {
    struct setting* iter;
    if( global_settings == NULL ) return NULL;
    iter = global_settings->next;
    while( (iter != NULL) && (strcmp( iter->name, name ) != 0) ) {
        iter = iter->next;
    }
    return iter;
}

int set_global_setting( char* name, bool value ) {
    if( name == NULL || global_settings == NULL ) return ERROR;
    struct setting* iter = global_settings->next;
    while( (iter != NULL) && (strcmp( iter->name, name ) != 0) ) {
        iter = iter->next;
    }
    if( iter == NULL ) return ERROR;
    iter->value = value;
    return SUCCESS;
}

struct inventory* find_inventory_item( char* item ) {
    struct inventory* iter = player_inventory.next;
    while( (iter != NULL) && (strcmp( iter->item, item ) != 0) ) {
        iter = iter->next;
    }
    return iter;
}

void add_level( struct level* l ) {
    if( current_level == NULL ) {
        current_level = l;
    } else {
        struct level* iter = current_level;
        while( iter->next != NULL ) {
            iter = iter->next;
        }
        iter->next = l;
    }
}
/* change current room */
void change_room( char* destination ) {
    assert( destination );
    struct room* r = find_room( current_level, destination );

    if( r ) {
        current_room = r;
    } else {
        setPrompt( "The door went nowhere!" );
        return;
    }

    showRoomMessage( r->visited );
    r->visited = true;
    clearPrompt();
    return;
}

void add_to_inventory( struct entity* e ) {
    struct inventory* inv_iter = &player_inventory;
    struct entity* iter = current_room->entities;
    struct entity* prev = current_room->entities;
    /* remove entity from room, add it to inventory */
    assert( e );


    while( iter ) {
        if( iter == e ) {
            prev->next = iter->next;
            break;
        }

        prev = iter;
        iter = iter->next;
    }

    /* we never moved, so we're removing the head */
    if( iter == current_room->entities ) {
        current_room->entities = current_room->entities->next;
    }

    assert( iter );

    if( inv_iter->item == NULL ) {
        player_inventory.item = strdup( e->name );
    } else {
        struct inventory* i = malloc( sizeof( struct inventory ) );
        assert( i );
        memset( i, 0, sizeof( struct inventory ) );

        while ( inv_iter->next ) {
            inv_iter = inv_iter->next;
        }

        i->item = strdup( e->name );
        inv_iter->next = i;
    }

    delete_entity( e );
}


void show_inventory() {
    char* msg = malloc( sizeof(char) * MAX_LINES * MAX_COLS );
    struct inventory* iter = &player_inventory;
    int everythree = 1;
    assert( msg );

    strcpy( msg, "\nInventory:\n\n" );
    if( iter->item ) {
        while( iter ) {
            strcat( msg, iter->item );
            strcat( msg, "\t\t\t" );
            if( everythree++ == 3 ) {
                strcat( msg, "\n" );
                everythree = 1;
            }
            iter = iter->next;
        }
    }

    setMessage( msg );

    free( msg );
}

void show_help() {
    char* msg = malloc( sizeof(char) * MAX_LINES * MAX_COLS );
    assert( msg );

    strcpy( msg, "\nHelp:\n\n" );
    strcat( msg, "look <object> - Look at something. Can be shortened to 'l'.\n" );
    strcat( msg, "                If you don't provide an object, you will look around the room.\n");
    strcat( msg, "take <object> - Take object, if takeable.\n" );
    strcat( msg, "go <direction> - Move in a direction.\n" );
    strcat( msg, "? - Show help message.\n" );
    strcat( msg, "\n" );
    strcat( msg, "There are many other actions that will work on some objects but you have to figure them out.\n" );

    setMessage( msg );

    free( msg );
}
