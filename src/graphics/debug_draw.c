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

#if ( EX_PLATFORM != EX_IOS )
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
#endif

}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_draw_text ( float _x, float _y, float _z, const char *_fmt, ... ) {

#if ( EX_PLATFORM != EX_IOS )
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
#endif

}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_draw_point ( float _x, float _y ) {
    float p[2] = { _x, _y };

    //
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
	glVertexPointer(2, GL_FLOAT, 0, p);	
	glDrawArrays(GL_POINTS, 0, 1);

	// restore default state
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);	
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_draw_line ( const ex_vec2f_t *_start, 
                    const ex_vec2f_t *_end, 
                    const ex_color4f_t *_color ) 
{
	float p[4] = { _start->x, _start->y, _end->x, _end->y };
	float c[8] = {
		_color->r, _color->g, _color->b, _color->a,
		_color->r, _color->g, _color->b, _color->a
	};

    //
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, p);	
    glColorPointer(4, GL_FLOAT, 0, c); // NOTE: in OpenGLES, glColorPointer can't accept 3 as first parameter
    glDrawArrays(GL_LINES, 0, 2);

    // restore default state
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);		
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_draw_poly ( const ex_vec2f_t *_verts, int _num, const ex_color4f_t *_color, bool _close ) {
    int i = 0;
    ex_color4f_t *c = (ex_color4f_t *)ex_malloc( sizeof(ex_color4f_t) * _num );
    while ( i < _num ) {
        c[i] = *_color;
        ++i;
    }

    //
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, _verts);	
    glColorPointer(4, GL_FLOAT, 0, c); // NOTE: in OpenGLES, glColorPointer can't accept 3 as first parameter

    if ( _close )
        glDrawArrays(GL_LINE_LOOP, 0, _num);
    else
        glDrawArrays(GL_LINE_STRIP, 0, _num);

    // restore default state
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);		

    ex_free(c);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_draw_circle ( const ex_vec2f_t *_center, 
                      float _r, float _a, int _segs, 
                      const ex_color4f_t *_color, 
                      bool _lineToCenter )
{
	int additionalSegment = 1;
	int i,num;
    ex_vec2f_t *verts;
    ex_color4f_t *c;
    const float coef = 2.0f * (float)EX_PI/_segs;

	if ( _lineToCenter )
		++additionalSegment;

    //
    i = 0;
    num = (_segs + additionalSegment);
    verts = (ex_vec2f_t *)ex_malloc ( sizeof(ex_vec2f_t) * num );
    c = (ex_color4f_t *)ex_malloc ( sizeof(ex_color4f_t) * num );
    while ( i < num ) {
        c[i] = *_color;
        ++i;
    }

    //
	for( i = 0; i <= _segs; ++i ) {
		float rads = i*coef;
		float j = _r * cosf(rads + _a) + _center->x;
		float k = _r * sinf(rads + _a) + _center->y;
		
		verts[i].x = j;
		verts[i].y = k;
	}
	verts[(_segs+1)].x = _center->x;
	verts[(_segs+1)].y = _center->y;

    //
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, verts);	
    glColorPointer(4, GL_FLOAT, 0, c); // NOTE: in OpenGLES, glColorPointer can't accept 3 as first parameter
    glDrawArrays(GL_LINE_STRIP, 0, num);

    // restore default state
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);		

    //
    ex_free( verts );
    ex_free( c );
}
