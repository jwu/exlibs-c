-- ======================================================================================
-- File         : test_coroutine.lua
-- Author       : Wu Jie 
-- Last Change  : 04/23/2011 | 20:36:05 PM | Saturday,April
-- Description  : 
-- ======================================================================================

ex.log ("test invoke...")

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

function create_ent ( _name, _pos )
    local ent = ex.entity(_name)
    ent:add_comp("ex.trans2d", {
        -- local_angle = ex.angf(15),
        local_scale = ex.vec2f(1.0,1.0),
        local_position = _pos
    })
    ent:add_comp("ex.debug2d.rect", {
        show_coord = false,
        show_text = false,
        show_trail = true,
        show_parentlink = true,
        width = 10,
        height = 10,
    })
    ent:add_comp("comps.coroutine", {
    })
    return ent
end

-- ======================================================== 
-- 
-- ======================================================== 

-- create_ent ( ex.vec2f( 0.0, 0.0 ) )
for i=1,10 do
    ent = create_ent ( "ent_"..i, ex.vec2f( 0.0, 0.0 ) )
    ent.trans2d.local_position = ex.vec2f( ex.range_rand( -100.0, 100.0 ), ex.range_rand( -100.0, 100.0 ) )
end
