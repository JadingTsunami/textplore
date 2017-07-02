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
#include "utils.h"

int stricmp( const char* str1, const char* str2 ) {
    unsigned int i = 0;
    char* s1;
    char* s2;
    if( !str1 || !str2 ) return -1;
    s1 = strdup( str1 );
    s2 = strdup( str2 );
    if( s1 ) for( i = 0; s1[i]; i++ ) s1[i] = tolower(s1[i]);
    if( s2 ) for( i = 0; s2[i]; i++ ) s2[i] = tolower(s2[i]);
    i = strcmp( s1, s2 );
    free( s1 );
    free( s2 );
    return i;
}


int strstart( const char *str, const char *start ) {
    return strlen(str) < strlen(start) ? ERROR : strncmp(start, str, strlen(start)) == 0;
}

char* trim( char* str ) {
    char* ret;
    char* end;
    if( str == NULL ) return NULL;
    end = str + (strlen(str) - 1);
    while( isspace(*str) ) str++;
    while( isspace(*end) ) end--;
    if( end < str ) {
        *str = '\0';
        return str;
    }
    ret = malloc( sizeof(char) * (end-str+2) );
    assert( ret );
    strncpy( ret, str, (end-str+1) );
    ret[(end-str+1)] = '\0';
    strcpy( str, ret );
    free( ret );
    return str;
}

char* clean_input( char* str ) {
    int len, i;
    char* s = str;
    /* delete duplicate spaces, and any words from
     * the list of "ignored words" */

    /* first, eliminate all ignored words */
    for( i = 0; i < NUM_IGNORED_WORDS; i++ ) {
        while( (s=strstr(s,ignored_words[i])) ) {
            len = strlen( ignored_words[i] );
            memmove(s,s+len,1+strlen(s+len));
        }
        s = str;
    }

    /* now trim multiple spaces */
    s = str;

    while( (s=strstr(str,"  ")) ) {
        len = strlen( "  " );
        memmove(s,s+len,1+strlen(s+len));
    }

    return str;
}
