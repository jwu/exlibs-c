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
typedef struct list_node_t {
    void* value;
    struct list_node_t* prev;
    struct list_node_t* next;
} list_node_t;

//
typedef struct list_t {
    // private
    size_t _length;
    size_t _element_bytes;
    list_node_t* _head;
    list_node_t* _tail;
} list_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

list_t* list_alloc ( size_t _element_bytes );
list_t* list_alloc_nomng ( size_t _element_bytes );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void list_free ( list_t* _list );
void list_free_nomng ( list_t* _list );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void list_push_back ( list_t* _list, void* _value );
void list_push_back_nomng ( list_t* _list, void* _value );

void list_push_front ( list_t* _list, void* _value );
void list_push_front_nomng ( list_t* _list, void* _value );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void list_insert_back ( list_t* _list, list_node_t* _at, void* _value );
void list_insert_back_nomng ( list_t* _list, list_node_t* _at, void* _value );

void list_insert_front ( list_t* _list, list_node_t* _at, void* _value );
void list_insert_front_nomng ( list_t* _list, list_node_t* _at, void* _value );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

list_node_t* list_erase ( list_t* _list, list_node_t* _at );
list_node_t* list_erase_nomng ( list_t* _list, list_node_t* _at );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline size_t list_len ( list_t* _list ) { return _list->_length; }
inline list_node_t* list_head ( list_t* _list ) { return _list->_head; }
inline list_node_t* list_tail ( list_t* _list ) { return _list->_tail; }

// ######################### 
#ifdef __cplusplus
} // end extern "C"
#endif
// ######################### 

// #################################################################################
#endif // END LIST_H_1277805267
// #################################################################################


