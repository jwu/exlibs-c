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
static inline ex_list_node_t *__alloc_node ( ex_list_t *_list, const void *_value ) {
    // allocate the node ( node_size + value_size ), and assign the value ptr to address point 
    // to the extract space allocate in the node. 
    ex_list_node_t *node = (ex_list_node_t *)ex_malloc ( _list->element_bytes + sizeof(ex_list_node_t) ); 
    node->value = (char *)node + sizeof(ex_list_node_t); 
    memcpy ( node->value, _value, _list->element_bytes );
    node->next = NULL;
    node->prev = NULL;
    return node;
}

// no managed
static inline ex_list_node_t *__alloc_node_nomng ( ex_list_t *_list, const void *_value ) {
    // allocate the node ( node_size + value_size ), and assign the value ptr to address point 
    // to the extract space allocate in the node. 
    ex_list_node_t *node = (ex_list_node_t *)ex_malloc_nomng ( _list->element_bytes + sizeof(ex_list_node_t) ); 
    node->value = (char *)node + sizeof(ex_list_node_t); 
    memcpy ( node->value, _value, _list->element_bytes );
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
ex_list_t *ex_list_alloc ( size_t _element_bytes )
{
    ex_list_t *list = ex_malloc( sizeof(ex_list_t) ); 

    // init public members
    list->count = 0;
    list->element_bytes = _element_bytes;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

// no managed
ex_list_t *ex_list_alloc_nomng ( size_t _element_bytes )
{
    ex_list_t *list = ex_malloc_nomng( sizeof(ex_list_t) ); 

    // init public members
    list->count = 0;
    list->element_bytes = _element_bytes;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
void ex_list_free ( ex_list_t *_list )
{
    ex_list_node_t *tmp,*node;
    ex_assert_return( _list != NULL, /*void*/, "NULL input" );

    node = _list->head;
    while ( node != NULL ) {
        tmp = node;
        node = node->next;
        ex_free ( tmp );
    }

    _list->count = 0;
    _list->element_bytes = 0;
    _list->head = NULL;
    _list->tail = NULL;
    ex_free(_list);
}

// no managed
void ex_list_free_nomng ( ex_list_t *_list )
{
    ex_list_node_t *tmp,*node; 
    ex_assert_return( _list != NULL, /*void*/, "NULL input" );

    node = _list->head;
    while ( node != NULL ) {
        tmp = node;
        node = node->next;
        ex_free_nomng ( node );
    }

    _list->count = 0;
    _list->element_bytes = 0;
    _list->head = NULL;
    _list->tail = NULL;
    ex_free_nomng(_list);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
void ex_list_append ( ex_list_t *_list, const void *_value )
{
    ex_list_node_t *node = NULL;

    // allocate the node
    node = __alloc_node ( _list, _value );

    //
    if ( _list->head == NULL ) {
        _list->head = node;
        _list->tail = node;
    }
    else {
        _list->tail->next = node;
        node->prev = _list->tail;
        _list->tail = node;
    }

    //
    ++_list->count;
}

// no managed
void ex_list_append_nomng ( ex_list_t *_list, const void *_value )
{
    ex_list_node_t *node = NULL;

    // allocate the node
    node = __alloc_node_nomng ( _list, _value );

    //
    if ( _list->head == NULL ) {
        _list->head = node;
        _list->tail = node;
    }
    else {
        _list->tail->next = node;
        node->prev = _list->tail;
        _list->tail = node;
    }

    //
    ++_list->count;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
void ex_list_prepend ( ex_list_t *_list, const void *_value )
{
    ex_list_node_t *node = NULL;

    // allocate the node
    node = __alloc_node ( _list, _value );

    //
    if ( _list->head == NULL ) {
        _list->head = node;
        _list->tail = node;
    }
    else {
        _list->head->prev = node;
        node->next = _list->head;
        _list->head = node;
    }

    //
    ++_list->count;
}

// no managed
void ex_list_prepend_nomng ( ex_list_t *_list, const void *_value )
{
    ex_list_node_t *node = NULL;

    // allocate the node
    node = __alloc_node_nomng ( _list, _value );

    //
    if ( _list->head == NULL ) {
        _list->head = node;
        _list->tail = node;
    }
    else {
        _list->head->prev = node;
        node->next = _list->head;
        _list->head = node;
    }

    //
    ++_list->count;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
void ex_list_insert_back ( ex_list_t *_list, ex_list_node_t *_at, const void *_value )
{
    ex_list_node_t *node = NULL;

    ex_assert_return ( _at != NULL, /*void*/, "the insert position can't be NULL!" );
    // CHECK: this may not be true, I think { 
    ex_assert_return ( _at >= _list->head && _at <= _list->tail, /*void*/, "the insert position is out of range!" );
    // } CHECK end 

    // allocate the node
    node = __alloc_node ( _list, _value );

    // link the new node's next and prev
    node->next = _at->next;
    node->prev = _at;

    // link the at's next if exists
    // if _at is _tail
    if ( _at == _list->tail )
        _list->tail = node;
    else
        _at->next->prev = node; 

    ++_list->count;
}

// no managed
void ex_list_insert_back_nomng ( ex_list_t *_list, ex_list_node_t *_at, const void *_value )
{
    ex_list_node_t *node = NULL;

    ex_assert_return ( _at != NULL, /*void*/, "the insert position can't be NULL!" );
    // CHECK: this may not be true, I think { 
    ex_assert_return ( _at >= _list->head && _at <= _list->tail, /*void*/, "the insert position is out of range!" );
    // } CHECK end 

    // allocate the node
    node = __alloc_node_nomng ( _list, _value );

    // link the new node's next and prev
    node->next = _at->next;
    node->prev = _at;

    // link the at's next if exists
    // if _at is _tail
    if ( _at == _list->tail )
        _list->tail = node;
    else
        _at->next->prev = node; 

    ++_list->count;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
void ex_list_insert_front ( ex_list_t *_list, ex_list_node_t *_at, const void *_value )
{
    ex_list_node_t *node = NULL;

    ex_assert_return ( _at != NULL, /*void*/, "the insert position can't be NULL!" );
    // CHECK: this may not be true, I think { 
    ex_assert_return ( _at >= _list->head && _at <= _list->tail, /*void*/, "the insert position is out of range!" );
    // } CHECK end 

    // allocate the node
    node = __alloc_node ( _list, _value );

    // link the new node's next and prev
    node->next = _at;
    node->prev = _at->prev;

    // link the at's next if exists
    // if _at is _head
    if ( _at == _list->head )
        _list->head = node;
    else
        _at->prev->next = node; 

    ++_list->count;
}

// no managed
void ex_list_insert_front_nomng ( ex_list_t *_list, ex_list_node_t *_at, const void *_value )
{
    ex_list_node_t *node = NULL;

    ex_assert_return ( _at != NULL, /*void*/, "the insert position can't be NULL!" );
    // CHECK: this may not be true, I think { 
    ex_assert_return ( _at >= _list->head && _at <= _list->tail, /*void*/, "the insert position is out of range!" );
    // } CHECK end 

    // allocate the node
    node = __alloc_node_nomng ( _list, _value );

    // link the new node's next and prev
    node->next = _at;
    node->prev = _at->prev;

    // link the at's next if exists
    // if _at is _head
    if ( _at == _list->head )
        _list->head = node;
    else
        _at->prev->next = node; 

    ++_list->count;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
ex_list_node_t *ex_list_remove_at ( ex_list_t *_list, ex_list_node_t *_at )
{
    ex_list_node_t *next_node;

    ex_assert_return ( _at != NULL, NULL, "the insert position can't be NULL!" );
    // CHECK: this may not be true, I think { 
    ex_assert_return ( _at >= _list->head && _at <= _list->tail, NULL, "the insert position is out of range!" );
    // } CHECK end 

    next_node = _at->next;

    // if only one node in the list
    if ( _list->head == _list->tail ) {
        ex_assert_return ( _at == _list->head, NULL, "invalid input node." );
        _list->head = _list->tail = NULL;
    }
    else if ( _at == _list->head ) {
        _list->head = _list->head->next;
        _list->head->prev = NULL;
    }
    else if ( _at == _list->tail ) {
        _list->tail = _list->tail->prev;
        _list->tail->next = NULL;
    }
    else {
        _at->prev->next = _at->next;
        _at->next->prev = _at->prev;
    }

    ex_free (_at);
    --_list->count;

    return next_node;
}

// no managed
ex_list_node_t *ex_list_remove_at_nomng ( ex_list_t *_list, ex_list_node_t *_at )
{
    ex_list_node_t *next_node;

    ex_assert_return ( _at != NULL, NULL, "the insert position can't be NULL!" );
    // CHECK: this may not be true, I think { 
    ex_assert_return ( _at >= _list->head && _at <= _list->tail, NULL, "the insert position is out of range!" );
    // } CHECK end 

    next_node = _at->next;

    // if only one node in the list
    if ( _list->head == _list->tail ) {
        ex_assert_return ( _at == _list->head, NULL, "invalid input node." );
        _list->head = _list->tail = NULL;
    }
    else if ( _at == _list->head ) {
        _list->head = _list->head->next;
        _list->head->prev = NULL;
    }
    else if ( _at == _list->tail ) {
        _list->tail = _list->tail->prev;
        _list->tail->next = NULL;
    }
    else {
        _at->prev->next = _at->next;
        _at->next->prev = _at->prev;
    }

    ex_free_nomng (_at);
    --_list->count;

    return next_node;
}
