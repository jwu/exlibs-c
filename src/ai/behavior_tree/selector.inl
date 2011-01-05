// ======================================================================================
// File         : selector.inl
// Author       : Wu Jie 
// Last Change  : 07/18/2010 | 16:59:36 PM | Sunday,July
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// selector
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static bool _selector_exec ( bt_node_t* _node )
{
    ex_array_each ( _node->children, bt_node_t*, child ) {
        if ( (*(child->exec))(child) == true )
            return true;
    } ex_array_each_end
    return false;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bt_node_t* bt_selector()
{
    bt_node_t* node = __bt_node_alloc ();
    node->type = "selector";
    node->exec = _selector_exec;
    return node;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static bool _random_selector_exec ( bt_node_t* _node )
{
    static uint last_idx_len = 0;
    uint count = ex_array_len(_node->children);
    uint* indices = NULL;
    uint i = 0; 

    // if the indices len less than the children count, realloc it.
    if ( last_idx_len < count ) {
        _node->user_data = ex_realloc ( _node->user_data, sizeof(uint) * count ); 
        // init indices
        indices = _node->user_data;
        for ( i = 0; i < count; ++i ) {
            indices[i] = i;
        }
    }
    else {
        indices = _node->user_data;
    }

    // shuffle the indices
    for ( i = 0; i < count-1; ++i ) {
        uint r = i + ( rand() % (count-i) ); // random remaining position.
        uint temp = indices[i]; 
        indices[i] = indices[r]; 
        indices[r] = temp;
    }

    // execute the children
    for ( i = 0; i < count; ++i ) {
        uint idx = indices[i];
        bt_node_t* child = *( (bt_node_t**) (ex_array_get(_node->children,idx)) );
        if ( (*(child->exec))(child) == true )
            return true;
    }
    return false;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void _random_selector_free ( bt_node_t* _node )
{
    ex_free ( _node->user_data );
    _node->free = NULL;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bt_node_t* bt_random_selector()
{
    bt_node_t* node = __bt_node_alloc ();
    node->type = "random.selector";
    node->exec = _random_selector_exec;
    node->free = _random_selector_free;
    return node;
}
