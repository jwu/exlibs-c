// ======================================================================================
// File         : list.h
// Author       : Wu Jie 
// Last Change  : 06/29/2010 | 17:54:26 PM | Tuesday,June
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef LIST_H_1277805267
#define LIST_H_1277805267
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" {
#endif
// ######################### 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// when allocate the node, it actually allocate the a memory block which the
// size is equal to the size of the node plus the size of the value store in the node.
// then it assign the address of the value to the node->value in the node. 
typedef struct ex_list_node_t {
    void* value;
    struct ex_list_node_t* prev;
    struct ex_list_node_t* next;
} ex_list_node_t;

//
typedef struct ex_list_t {
    // private
    size_t _length;
    size_t _element_bytes;
    ex_list_node_t* _head;
    ex_list_node_t* _tail;
} ex_list_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern ex_list_t* ex_list_alloc ( size_t _element_bytes );
extern ex_list_t* ex_list_alloc_nomng ( size_t _element_bytes );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_list_free ( ex_list_t* _list );
extern void ex_list_free_nomng ( ex_list_t* _list );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_list_append ( ex_list_t* _list, const void* _value );
extern void ex_list_append_nomng ( ex_list_t* _list, const void* _value );

extern void ex_list_prepend ( ex_list_t* _list, const void* _value );
extern void ex_list_prepend_nomng ( ex_list_t* _list, const void* _value );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_list_insert_back ( ex_list_t* _list, ex_list_node_t* _at, const void* _value );
extern void ex_list_insert_back_nomng ( ex_list_t* _list, ex_list_node_t* _at, const void* _value );

extern void ex_list_insert_front ( ex_list_t* _list, ex_list_node_t* _at, const void* _value );
extern void ex_list_insert_front_nomng ( ex_list_t* _list, ex_list_node_t* _at, const void* _value );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern ex_list_node_t* ex_list_remove_at ( ex_list_t* _list, ex_list_node_t* _at );
extern ex_list_node_t* ex_list_remove_at_nomng ( ex_list_t* _list, ex_list_node_t* _at );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline size_t ex_list_len ( const ex_list_t* _list ) { return _list->_length; }
static inline ex_list_node_t* ex_list_head ( const ex_list_t* _list ) { return _list->_head; }
static inline ex_list_node_t* ex_list_tail ( const ex_list_t* _list ) { return _list->_tail; }

// ######################### 
#ifdef __cplusplus
} // end extern "C"
#endif
// ######################### 

// #################################################################################
#endif // END LIST_H_1277805267
// #################################################################################


