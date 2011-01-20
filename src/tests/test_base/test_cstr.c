// ======================================================================================
// File         : test_cstr.c
// Author       : Wu Jie 
// Last Change  : 01/19/2011 | 18:42:38 PM | Wednesday,January
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "../../core/core_inc.h"

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void normal () {
    // strcpy
    {
        char to[128];
        const char *text = "hello world";

        // right
        memset ( to, 1, 128 );
        strcpy( to, text );
        printf("for break");
    }

    // strncpy
    {
        char to[128];
        const char *text = "hello world";
        int len = strlen(text);

        // wrong
        memset ( to, 1, 128 );
        strncpy( to, text, len );

        // right
        memset ( to, 1, 128 );
        strncpy( to, text, len );
        to[len] = 0;
        printf("for break");
    }

    // strlen
    {
        char *to;
        const char *text = "hello world";
        int len = strlen(text);

        // wrong
        to = ex_malloc( len );
        strcpy( to, text );
        ex_free(to);

        // right
        to = ex_malloc( len+1 );
        strcpy( to, text );
        ex_free(to);
        printf("for break");
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void test_cstr () {
    normal();
}

