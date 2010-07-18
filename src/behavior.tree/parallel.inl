// ======================================================================================
// File         : parallel.inl
// Author       : Wu Jie 
// Last Change  : 07/18/2010 | 16:59:33 PM | Sunday,July
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// parallel
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static bool _parallel_exec ( bt_node_t* _node )
{
    // TODO:
    ex_assert( false, "pls implement this");
    return false;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bt_node_t* bt_parallel()
{
    bt_node_t* node = _bt_node_alloc ();
    node->type = "parallel";
    node->exec = _parallel_exec;
    return node;
}

