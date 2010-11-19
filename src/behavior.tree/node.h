// ======================================================================================
// File         : node.h
// Author       : Wu Jie 
// Last Change  : 07/13/2010 | 22:18:28 PM | Tuesday,July
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef NODE_H_1279030729
#define NODE_H_1279030729
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" {
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

typedef struct bt_node_t {
    // data
    const char* type;
    struct ex_array_t* children;
    void* user_data;

    // callback functions
    bool (*exec) ( struct bt_node_t* _self );
    void (*free) ( struct bt_node_t* _self );
} bt_node_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void bt_node_add ( bt_node_t* _self, bt_node_t* _node );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void bt_node_free ( bt_node_t* _node );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bt_node_t* bt_sequence();
bt_node_t* bt_selector();
bt_node_t* bt_random_selector();
bt_node_t* bt_parallel();
bt_node_t* bt_queue();

// ######################### 
#ifdef __cplusplus
} // end extern "C"
#endif
// ######################### 

// #################################################################################
#endif // END NODE_H_1279030729
// #################################################################################


