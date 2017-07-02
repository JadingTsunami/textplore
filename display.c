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
#include "display.h"

void initDisplay() { 
    initscr();
    refresh();
    if( LINES < MIN_LINES || COLS < MIN_COLS ) {
        closeDisplay();
        fprintf( stderr, "ERROR: Display too small.\n\n" );
        fprintf( stderr, "ROWS: %d, Minimum to play: %d.\n", LINES, MIN_LINES );
        fprintf( stderr, "COLS: %d, Minimum to play: %d.\n", COLS, MIN_COLS );
        exit(1);
        return;
    } else if( LINES > MAX_LINES || COLS > MAX_COLS ) {
        closeDisplay();
        fprintf( stderr, "ERROR: Display too large.\n\n" );
        fprintf( stderr, "ROWS: %d, Maxinum to play: %d.\n", LINES, MAX_LINES );
        fprintf( stderr, "COLS: %d, Maxinum to play: %d.\n", COLS, MAX_COLS );
        exit(1);
        return;
    }

    topwin = newwin( LINES-4, COLS, 0, 0 );
    botwin = newwin( 4, COLS, LINES-4, 0 );
    box( topwin, 0, 0 );
    box( botwin, 0, 0 );
    mvwprintw( botwin, 1,1, ">" );
    wrefresh(topwin);
    wrefresh(botwin);
    move( LINES-3, 2 );
    keypad( botwin, TRUE );
}

void closeDisplay() {
    if( topwin ) delwin( topwin );
    if( botwin ) delwin( botwin );
    endwin();
}

void getUserInput(char* str) {
    if( str == NULL ) return;
    mvwgetnstr( botwin, 1, 2, str, MAX_USER_INPUT );
    wmove( botwin, 1, 2 );
    wclrtoeol( botwin );
    box( botwin, 0, 0 );
    wrefresh( botwin );
    
}

void setMessage( char* msg ) {
    if( msg ) {
        char* tmp = wordwrap( msg, COLS );
        wclear( topwin );
        mvwprintw( topwin, 2, 3, tmp );
        free(tmp);
        box( topwin, 0, 0 );
        wrefresh( topwin );
    }
}

void setPrompt( char* prompt ) {
    if( prompt ) {
        wmove( botwin, 2, 1 );
        wclrtoeol( botwin );
        box( botwin, 0, 0 );
        mvwprintw( botwin, 2, 1, prompt );
        wrefresh( botwin );
    }
}

void clearPrompt() {
    wmove( botwin, 2, 1 );
    wclrtoeol( botwin );
    box( botwin, 0, 0 );
    wrefresh( botwin );
}


void showInventory() {
    // TODO: Implement
}

static char * wordwrap( char* str, int COLS ) {
    int i, sincebreak, ri;
    int lastspace = -1;
    char* ret;

    if( str == NULL || COLS < MIN_COLS ) return NULL;

    ret = malloc( sizeof( char ) * strlen(str) * 2 );
    memset( ret, 0, sizeof( char ) * strlen(str) * 2 );

    for( i = 0, sincebreak = 0; i < strlen( str ); i++, sincebreak++ ) {
        if( isspace( str[i] ) ) lastspace = i;
        if( str[i] == '\n' ) sincebreak = 0;
        if( sincebreak >= (COLS-8) ) {
            if( lastspace > 0 ) {
                str[lastspace] = '\n';
                sincebreak = i - lastspace;
            }
            lastspace = -1;
        }
    }

    for( i = 0, ri = 0; i < strlen( str ); i++ ) {
        if( str[i] == '\n' ) {
            ret[ri++] = '\n';
            ret[ri++] = ' ';
            ret[ri++] = ' ';
            ret[ri++] = ' ';
            ret[ri++] = ' ';
        } else {
            ret[ri++] = str[i];
        }
    }

    ret[ri] = '\0';

    return ret;
}
