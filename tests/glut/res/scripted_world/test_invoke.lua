-- ======================================================================================
-- File         : test_invoke.lua
-- Author       : Wu Jie 
-- Last Change  : 04/17/2011 | 09:26:19 AM | Sunday,April
-- Description  : 
-- ======================================================================================

ex.log ("test invoke...")

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

function create_ent ( _pos )
    local ent = ex.entity("foobar")
    ent:add_comp("ex.trans2d", {
        -- local_angle = ex.angf(15),
        local_scale = ex.vec2f(1.0,1.0),
        local_position = _pos
    })
    ent:add_comp("ex.debug2d.rect", {
        -- show_text = false,
        width = 10,
        height = 10,
    })
    ent:add_comp("comps.invoke", {
        interval = 1.0,
        scale_for_seconds = 0.5
    })
end

-- ======================================================== 
-- 
-- ======================================================== 

create_ent ( ex.vec2f( 0.0, 0.0 ) )
