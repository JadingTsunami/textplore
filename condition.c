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
#include "condition.h"

static bool evaluate_condition( evaluate ev, char* ev_t ) {
    switch ( ev ) {
        case EVALUATE_LEVEL_SETTING_SET:
        case EVALUATE_LEVEL_SETTING_CLEAR:
            {
            struct setting* s = get_level_setting( current_level, ev_t );
            if( s == NULL ) return false;
            return ( ev == EVALUATE_LEVEL_SETTING_SET ) ? s->value : !s->value;
            }
            break;
        case EVALUATE_GLOBAL_SETTING_SET:
        case EVALUATE_GLOBAL_SETTING_CLEAR:
            {
            struct setting* s = get_global_setting( ev_t );
            if( s == NULL ) return false;
            return ( ev == EVALUATE_LEVEL_SETTING_SET ) ? s->value : !s->value;
            }
            break;
        case EVALUATE_INVENTORY_HAS:
        case EVALUATE_INVENTORY_HAS_NOT:
            {
            struct inventory* i = find_inventory_item( ev_t );
            return ( ev == EVALUATE_INVENTORY_HAS ) ? (i!=NULL) : (i==NULL);
            }
            break;
        case EVALUATE_ALWAYS:
            return true;
            break;
        default:
            return false;
            break;
    }
    return false;
}

static void perform_operation( perform p, char* tgt ) {
    if( tgt == NULL ) return;
    switch( p ) {
        case DISPLAY_MESSAGE:
            setPrompt( tgt );
            break;
        case SET_ROOM_MESSAGE:
            set_room_long_message( current_room, tgt );
            break;
        case APPEND_ROOM_MESSAGE:
            {
            char* newmsg;
            /* +3 for newlines and null terminator */
            newmsg = malloc( sizeof( char ) * ( strlen( current_room->long_message ) + strlen( tgt ) ) + 3 );
            assert( newmsg );
            strcat( newmsg, current_room->long_message );
            strcat( newmsg, "\n" );
            strcat( newmsg, "\n" );
            strcat( newmsg, tgt );
            set_room_long_message( current_room, newmsg );
            free( newmsg );
            break;
            }
        case SET_PROMPT:
            setPrompt( tgt );
            break;
        case SET_GLOBAL_SETTING:
        case CLEAR_GLOBAL_SETTING:
            if( set_global_setting( tgt, (p == SET_GLOBAL_SETTING) ? true : false ) == ERROR ) {
                debug_print( "Global setting %s not found\n", tgt );
            }
            break;
        case SET_LEVEL_SETTING:
        case CLEAR_LEVEL_SETTING:
            if( set_level_setting( current_level, tgt, (p == SET_LEVEL_SETTING) ? true : false ) == ERROR ) {
                debug_print( "Level setting %s not found\n", tgt );
            }
            break;
        case EXIT_ROOM:
            change_room( tgt );
            break;
        case DO_NOTHING:
        default:
            return;
    }
}

bool condition_check_and_act( struct condition* c ) {
    bool condition_met = false;
    if( c == NULL ) {
        debug_print("Condition check and act called with NULL condition (%d).\n", condition_met );
        assert( false );
    }

    condition_met = evaluate_condition( c->eval, c->eval_target );

    if( condition_met && c->perf_true_target ) {
        perform_operation( c->perf_true, c->perf_true_target );
    } else if( !condition_met && c->perf_false_target ) {
        perform_operation( c->perf_false, c->perf_false_target );
    } 

    return condition_met;
}

evaluate string_to_eval( char* input ) {

    if( stricmp( input, "level_setting_set" ) == 0 ) {
        return EVALUATE_LEVEL_SETTING_SET;
    } else if( stricmp( input, "level_setting_clear" ) == 0 ) {
        return EVALUATE_LEVEL_SETTING_CLEAR;
    } else if( stricmp( input, "global_setting_set" ) == 0 ) {
        return EVALUATE_GLOBAL_SETTING_SET;
    } else if( stricmp( input, "global_setting_clear" ) == 0 ) {
        return EVALUATE_GLOBAL_SETTING_CLEAR;
    } else if( stricmp( input, "inventory_has" ) == 0 ) {
        return EVALUATE_INVENTORY_HAS;
    } else if( stricmp( input, "inventory_has_not" ) == 0 ) {
        return EVALUATE_INVENTORY_HAS_NOT;
    } else if( stricmp( input, "always" ) == 0 ) {
        return EVALUATE_ALWAYS;
    }

    /* TODO: Error condition? */
    return EVALUATE_ALWAYS;
}

perform string_to_perform( char* input ) {

    if( stricmp( input, "display" ) == 0 ) {
        return DISPLAY_MESSAGE;
    } else if( stricmp( input, "set_room_message" ) == 0 ) {
        return SET_ROOM_MESSAGE;
    } else if( stricmp( input, "append_room_message" ) == 0 ) {
        return APPEND_ROOM_MESSAGE;
    } else if( stricmp( input, "set_prompt" ) == 0 ) {
        return SET_PROMPT;
    } else if( stricmp( input, "set_global" ) == 0 ) {
        return SET_GLOBAL_SETTING;
    } else if( stricmp( input, "clear_global" ) == 0 ) {
        return CLEAR_GLOBAL_SETTING;
    } else if( stricmp( input, "set_level_setting" ) == 0 ) {
        return SET_LEVEL_SETTING;
    } else if( stricmp( input, "clear_level_setting" ) == 0 ) {
        return CLEAR_LEVEL_SETTING;
    } else if( stricmp( input, "exit_room" ) == 0 ) {
        return EXIT_ROOM;
    } else if( stricmp( input, "do_nothing" ) == 0 ) {
        return DO_NOTHING;
    }

    return DO_NOTHING;
}

void delete_condition( struct condition* c ) {
    assert( c );
    free( c->eval_target );
    free( c->perf_true_target );
    free( c->perf_false_target );
    free( c );
}
