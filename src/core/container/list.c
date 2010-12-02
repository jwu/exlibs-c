// ======================================================================================
// File         : list.c
// Author       : Wu Jie 
// Last Change  : 06/29/2010 | 18:46:46 PM | Tuesday,June
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "list.h"

///////////////////////////////////////////////////////////////////////////////
// decls
///////////////////////////////////////////////////////////////////////////////

// managed
static inline ex_list_node_t* _alloc_node ( ex_list_t* _list, const void* _value ) {
    // allocate the node ( node_size + value_size ), and assign the value ptr to address point 
    // to the extract space allocate in the node. 
    ex_list_node_t* node = (ex_list_node_t*)ex_malloc ( _list->_element_bytes + sizeof(ex_list_node_t) ); 
    node->value = (char*)node + sizeof(ex_list_node_t); 
    memcpy ( node->value, _value, _list->_element_bytes );
    node->next = NULL;
    node->prev = NULL;
    return node;
}

// no managed
static inline ex_list_node_t* _alloc_node_nomng ( ex_list_t* _list, const void* _value ) {
    // allocate the node ( node_size + value_size ), and assign the value ptr to address point 
    // to the extract space allocate in the node. 
    ex_list_node_t* node = (ex_list_node_t*)ex_malloc_nomng ( _list->_element_bytes + sizeof(ex_list_node_t) ); 
    node->value = (char*)node + sizeof(ex_list_node_t); 
    memcpy ( node->value, _value, _list->_element_bytes );
    node->next = NULL;
    node->prev = NULL;
    return node;
}

///////////////////////////////////////////////////////////////////////////////
// public
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
ex_list_t* ex_list_alloc ( size_t _element_bytes )
{
    ex_list_t* list = ex_malloc( sizeof(ex_list_t) ); 

    // init public members
    list->_length = 0;
    list->_element_bytes = _element_bytes;
    list->_head = NULL;
    list->_tail = NULL;

    return list;
}

// no managed
ex_list_t* ex_list_alloc_nomng ( size_t _element_bytes )
{
    ex_list_t* list = ex_malloc_nomng( sizeof(ex_list_t) ); 

    // init public members
    list->_length = 0;
    list->_element_bytes = _element_bytes;
    list->_head = NULL;
    list->_tail = NULL;

    return list;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
void ex_list_free ( ex_list_t* _list )
{
    ex_list_node_t* node;
    ex_assert_return( _list != NULL, /*void*/, "NULL input" );

    node = _list->_head;
    while ( node != NULL ) {
        ex_free ( node );
        node = node->next;
    }

    _list->_length = 0;
    _list->_element_bytes = 0;
    _list->_head = NULL;
    _list->_tail = NULL;
    ex_free(_list);
}

// no managed
void ex_list_free_nomng ( ex_list_t* _list )
{
    ex_list_node_t* node; 
    ex_assert_return( _list != NULL, /*void*/, "NULL input" );

    node = _list->_head;
    while ( node != NULL ) {
        ex_free_nomng ( node );
        node = node->next;
    }

    _list->_length = 0;
    _list->_element_bytes = 0;
    _list->_head = NULL;
    _list->_tail = NULL;
    ex_free_nomng(_list);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
void ex_list_append ( ex_list_t* _list, const void* _value )
{
    ex_list_node_t* node = NULL;

    // allocate the node
    node = _alloc_node ( _list, _value );

    //
    if ( _list->_head == NULL ) {
        _list->_head = node;
        _list->_tail = node;
    }
    else {
        _list->_tail->next = node;
        node->prev = _list->_tail;
        _list->_tail = node;
    }

    //
    ++_list->_length;
}

// no managed
void ex_list_append_nomng ( ex_list_t* _list, const void* _value )
{
    ex_list_node_t* node = NULL;

    // allocate the node
    node = _alloc_node_nomng ( _list, _value );

    //
    if ( _list->_head == NULL ) {
        _list->_head = node;
        _list->_tail = node;
    }
    else {
        _list->_tail->next = node;
        node->prev = _list->_tail;
        _list->_tail = node;
    }

    //
    ++_list->_length;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
void ex_list_prepend ( ex_list_t* _list, const void* _value )
{
    ex_list_node_t* node = NULL;

    // allocate the node
    node = _alloc_node ( _list, _value );

    //
    if ( _list->_head == NULL ) {
        _list->_head = node;
        _list->_tail = node;
    }
    else {
        _list->_head->prev = node;
        node->next = _list->_head;
        _list->_head = node;
    }

    //
    ++_list->_length;
}

// no managed
void ex_list_prepend_nomng ( ex_list_t* _list, const void* _value )
{
    ex_list_node_t* node = NULL;

    // allocate the node
    node = _alloc_node_nomng ( _list, _value );

    //
    if ( _list->_head == NULL ) {
        _list->_head = node;
        _list->_tail = node;
    }
    else {
        _list->_head->prev = node;
        node->next = _list->_head;
        _list->_head = node;
    }

    //
    ++_list->_length;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
void ex_list_insert_back ( ex_list_t* _list, ex_list_node_t* _at, const void* _value )
{
    ex_list_node_t* node = NULL;

    ex_assert_return ( _at != NULL, /*void*/, "the insert position can't be NULL!" );
    // CHECK: this may not be true, I think { 
    ex_assert_return ( _at >= _list->_head && _at <= _list->_tail, /*void*/, "the insert position is out of range!" );
    // } CHECK end 

    // allocate the node
    node = _alloc_node ( _list, _value );

    // link the new node's next and prev
    node->next = _at->next;
    node->prev = _at;

    // link the at's next if exists
    // if _at is _tail
    if ( _at == _list->_tail )
        _list->_tail = node;
    else
        _at->next->prev = node; 

    ++_list->_length;
}

// no managed
void ex_list_insert_back_nomng ( ex_list_t* _list, ex_list_node_t* _at, const void* _value )
{
    ex_list_node_t* node = NULL;

    ex_assert_return ( _at != NULL, /*void*/, "the insert position can't be NULL!" );
    // CHECK: this may not be true, I think { 
    ex_assert_return ( _at >= _list->_head && _at <= _list->_tail, /*void*/, "the insert position is out of range!" );
    // } CHECK end 

    // allocate the node
    node = _alloc_node_nomng ( _list, _value );

    // link the new node's next and prev
    node->next = _at->next;
    node->prev = _at;

    // link the at's next if exists
    // if _at is _tail
    if ( _at == _list->_tail )
        _list->_tail = node;
    else
        _at->next->prev = node; 

    ++_list->_length;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
void ex_list_insert_front ( ex_list_t* _list, ex_list_node_t* _at, const void* _value )
{
    ex_list_node_t* node = NULL;

    ex_assert_return ( _at != NULL, /*void*/, "the insert position can't be NULL!" );
    // CHECK: this may not be true, I think { 
    ex_assert_return ( _at >= _list->_head && _at <= _list->_tail, /*void*/, "the insert position is out of range!" );
    // } CHECK end 

    // allocate the node
    node = _alloc_node ( _list, _value );

    // link the new node's next and prev
    node->next = _at;
    node->prev = _at->prev;

    // link the at's next if exists
    // if _at is _head
    if ( _at == _list->_head )
        _list->_head = node;
    else
        _at->prev->next = node; 

    ++_list->_length;
}

// no managed
void ex_list_insert_front_nomng ( ex_list_t* _list, ex_list_node_t* _at, const void* _value )
{
    ex_list_node_t* node = NULL;

    ex_assert_return ( _at != NULL, /*void*/, "the insert position can't be NULL!" );
    // CHECK: this may not be true, I think { 
    ex_assert_return ( _at >= _list->_head && _at <= _list->_tail, /*void*/, "the insert position is out of range!" );
    // } CHECK end 

    // allocate the node
    node = _alloc_node_nomng ( _list, _value );

    // link the new node's next and prev
    node->next = _at;
    node->prev = _at->prev;

    // link the at's next if exists
    // if _at is _head
    if ( _at == _list->_head )
        _list->_head = node;
    else
        _at->prev->next = node; 

    ++_list->_length;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
ex_list_node_t* ex_list_remove_at ( ex_list_t* _list, ex_list_node_t* _at )
{
    ex_list_node_t* next_node;

    ex_assert_return ( _at != NULL, NULL, "the insert position can't be NULL!" );
    // CHECK: this may not be true, I think { 
    ex_assert_return ( _at >= _list->_head && _at <= _list->_tail, NULL, "the insert position is out of range!" );
    // } CHECK end 

    next_node = _at->next;

    // if only one node in the list
    if ( _list->_head == _list->_tail ) {
        ex_assert_return ( _at == _list->_head, NULL, "invalid input node." );
        _list->_head = _list->_tail = NULL;
    }
    else if ( _at == _list->_head ) {
        _list->_head = _list->_head->next;
        _list->_head->prev = NULL;
    }
    else if ( _at == _list->_tail ) {
        _list->_tail = _list->_tail->prev;
        _list->_tail->next = NULL;
    }
    else {
        _at->prev->next = _at->next;
        _at->next->prev = _at->prev;
    }

    ex_free (_at);
    --_list->_length;

    return next_node;
}

// no managed
ex_list_node_t* ex_list_remove_at_nomng ( ex_list_t* _list, ex_list_node_t* _at )
{
    ex_list_node_t* next_node;

    ex_assert_return ( _at != NULL, NULL, "the insert position can't be NULL!" );
    // CHECK: this may not be true, I think { 
    ex_assert_return ( _at >= _list->_head && _at <= _list->_tail, NULL, "the insert position is out of range!" );
    // } CHECK end 

    next_node = _at->next;

    // if only one node in the list
    if ( _list->_head == _list->_tail ) {
        ex_assert_return ( _at == _list->_head, NULL, "invalid input node." );
        _list->_head = _list->_tail = NULL;
    }
    else if ( _at == _list->_head ) {
        _list->_head = _list->_head->next;
        _list->_head->prev = NULL;
    }
    else if ( _at == _list->_tail ) {
        _list->_tail = _list->_tail->prev;
        _list->_tail->next = NULL;
    }
    else {
        _at->prev->next = _at->next;
        _at->next->prev = _at->prev;
    }

    ex_free_nomng (_at);
    --_list->_length;

    return next_node;
}
