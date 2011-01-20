// ======================================================================================
// File         : test_btree.c
// Author       : Wu Jie 
// Last Change  : 07/14/2010 | 22:12:00 PM | Wednesday,July
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "../../core/core_inc.h"
#include "../../ai/behavior_tree/btree_inc.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static bool _say_exec ( bt_node_t *_node ) {
    printf ("%s\n", (const char *)_node->user_data );
    return true;
}

static bt_node_t *action_say ( const char *_someting ) {
    bt_node_t *node = ex_malloc ( sizeof(bt_node_t) );
    node->children = NULL;
    node->user_data = (void *)_someting;
    node->exec = _say_exec;
    return node;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void normal () {
    bt_node_t *root = bt_random_selector();

    bt_node_add ( root, action_say("foo") );
    bt_node_add ( root, action_say("bar") );
    bt_node_add ( root, action_say("hello") );
    bt_node_add ( root, action_say("world") );

    (*root->exec)(root);
    (*root->exec)(root);
    (*root->exec)(root);
    (*root->exec)(root);
    (*root->exec)(root);
    (*root->exec)(root);
    (*root->exec)(root);

    bt_node_free ( root );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void invalid () {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void test_btree () {
    normal();
    invalid();
}
