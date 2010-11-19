// ======================================================================================
// File         : sequence.inl
// Author       : Wu Jie 
// Last Change  : 07/18/2010 | 16:59:37 PM | Sunday,July
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// sequence
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static bool _sequence_exec ( bt_node_t* _node )
{
    ex_array_each ( bt_node_t*, child, _node->children ) {
        if ( (*(child->exec))(child) == false )
            return false;
    } ex_array_each_end
    return true;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bt_node_t* bt_sequence ()
{
    bt_node_t* node = _bt_node_alloc ();
    node->type = "sequence";
    node->exec = _sequence_exec;
    return node;
}
