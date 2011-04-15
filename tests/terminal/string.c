// ======================================================================================
// File         : test_string.c
// Author       : Wu Jie 
// Last Change  : 01/22/2011 | 09:47:54 AM | Saturday,January
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "core/core_inc.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void normal () {
    ex_string_t *s1, *s2, *s3, *s4;

    s1 = ex_string( "hello world" ); 
    s2 = ex_string_fmt( "vector %f, %f, %f", 1.0f, 2.0f, 3.0f ); 
    s3 = ex_string_copy(s1); 
    s4 = ex_string_copy(s1);
    ex_string_cat( s4, " -- " );
    ex_string_ncat( s4, s2->text, s2->len );

    printf ( "s1 is %s\n", s1->text );
    printf ( "s2 is %s\n", s2->text );
    printf ( "s3 is %s\n", s3->text );
    printf ( "s4 is %s\n", s4->text );
    ex_string_cpy ( s4, "foobar!" );
    printf ( "s4 cpy is %s\n", s4->text );

    ex_string_free ( s1 );
    ex_string_free ( s2 );
    ex_string_free ( s3 );
    ex_string_free ( s4 );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void special () {
    ex_string_t *s1, *s2, *s3, *s4;

    s1 = ex_string( "hello world" ); 
    s2 = ex_string( "" ); // test empty
    s3 = ex_string_copy(s2); // test copy empty
    s4 = ex_string_copy(s1);
    ex_string_cat( s4, "" ); // test cat empty

    printf ( "s1 is %s\n", s1->text );
    printf ( "s2 is %s\n", s2->text );
    ex_string_cat( s2, "foobar" ); // test empty cat 
    printf ( "s2 cat is %s\n", s2->text );
    printf ( "s3 is %s\n", s3->text );
    printf ( "s4 is %s\n", s4->text );
    ex_string_cpy ( s4, "" ); // test cpy empty
    printf ( "s4 cpy is %s\n", s4->text );

    ex_string_free ( s1 );
    ex_string_free ( s2 );
    ex_string_free ( s3 );
    ex_string_free ( s4 );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void run_test () {
    normal();
    special();
}
