// ======================================================================================
// File         : queue.inl
// Author       : Wu Jie 
// Last Change  : 07/18/2010 | 16:59:35 PM | Sunday,July
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// queue
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static bool _queue_exec ( bt_node_t *_node )
{
    // TODO:
    ex_assert( false, "pls implement this");
    return false;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bt_node_t *bt_queue()
{
    bt_node_t *node = __bt_node_alloc ();
    node->type = "queue";
    node->exec = _queue_exec;
    return node;
}

