// ======================================================================================
// File         : debug_draw.c
// Author       : Wu Jie 
// Last Change  : 12/27/2010 | 00:16:14 AM | Monday,December
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "debug_draw.h"

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

#define BUF_SIZE 1024

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_screen_print ( int _x, int _y, const char *_fmt, ... ) {
    int     result = -1;
    char    buf[BUF_SIZE];
    char   *buffer = NULL;
    int i;

    //
    EX_GET_VA_STRING_WITH_RESULT( buf, BUF_SIZE-1, _fmt, &result ); // NOTE: the buffer_count-1 will leave 1 character for null terminal
    buffer = buf;
    if ( result == -1 ) {
        char *dyn_buf = NULL;
        int buffer_count = BUF_SIZE * 2;

        // keep get va string until success 
        do {
            dyn_buf = (char *)ex_realloc_nomng( dyn_buf, buffer_count * sizeof(char) );
            EX_GET_VA_STRING_WITH_RESULT( dyn_buf, buffer_count-1, _fmt, &result ); // NOTE: the buffer_count-1 will leave 1 character for null terminal
            buffer_count *= 2;
        } while ( result == -1 );
        buffer = dyn_buf;
    }
    buffer[result] = '\0';

    //
	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2i(_x, _y);
	for ( i = 0; i < result; ++i ) {
        if ( buffer[i] == '\n' ) {
            _y += 12;
            glRasterPos2i(_x,_y);
        } else {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, buffer[i]);
        }
	}

    // if we use dynamic buffer, free it
    if ( buffer != buf )
        ex_free_nomng ( buffer );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_draw_text ( float _x, float _y, float _z, const char *_fmt, ... ) {
    int     result = -1;
    char    buf[BUF_SIZE];
    char   *buffer = NULL;
    int i;

    //
    EX_GET_VA_STRING_WITH_RESULT( buf, BUF_SIZE-1, _fmt, &result ); // NOTE: the buffer_count-1 will leave 1 character for null terminal
    buffer = buf;
    if ( result == -1 ) {
        char *dyn_buf = NULL;
        int buffer_count = BUF_SIZE * 2;

        // keep get va string until success 
        do {
            dyn_buf = (char *)ex_realloc_nomng( dyn_buf, buffer_count * sizeof(char) );
            EX_GET_VA_STRING_WITH_RESULT( dyn_buf, buffer_count-1, _fmt, &result ); // NOTE: the buffer_count-1 will leave 1 character for null terminal
            buffer_count *= 2;
        } while ( result == -1 );
        buffer = dyn_buf;
    }
    buffer[result] = '\0';

    //
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	glTranslatef(_x, _y, _z);
    glRotatef( 0.0, 0.0, 0.0, 1.0 );
	glScalef( 0.1f, 0.1f, 1.0f );

    glPushAttrib( GL_ENABLE_BIT|GL_LINE_BIT );
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(1.5);

        glColor3f(1.0f, 1.0f, 1.0f);
        for ( i = 0; i < result; ++i ) {
            if ( buffer[i] == '\n' ) {
                _y += 12;
                glTranslatef(0.0f, _y, 0.0f);
            } else {
                glutStrokeCharacter(GLUT_STROKE_ROMAN, buffer[i]);
            }
        }
    glPopAttrib();

    // if we use dynamic buffer, free it
    if ( buffer != buf )
        ex_free_nomng ( buffer );
}

