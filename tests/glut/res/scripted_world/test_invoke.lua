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
    -- pure-c { 
    -- ent:add_comp("ex.debug2d")
    -- ent:add_comp("ex.simple")
    -- } pure-c end 
    ent:add_comp("ex.trans2d", {
        -- local_angle = ex.angf(15),
        local_scale = ex.vec2f(1.0,1.0),
        local_position = _pos
    })
    ent:add_comp("ex.debug2d.rect", {
        show_coord = false,
        show_text = false,
        show_trail = true,
        width = 10,
        height = 10,
    })
    ent:add_comp("comps.invoke", {
        rot_speed = ex.range_rand( -20, 20 ),
        scale_interval = ex.range_rand( 0.5, 1.5 ),
        move_interval = ex.range_rand( 0.5, 1.5 ),
        -- scale_interval = 0.5,
        -- move_interval = 0.5,
        move_for_seconds = ex.range_rand( 1.0, 5.0 ),
    })
    return ent
end

-- ======================================================== 
-- 
-- ======================================================== 

-- create_ent ( ex.vec2f( 0.0, 0.0 ) )
for i=1,50 do
    ent = create_ent ( ex.vec2f( 0.0, 0.0 ) )
    ent.trans2d.local_position = ex.vec2f( ex.range_rand( -100.0, 100.0 ), ex.range_rand( -100.0, 100.0 ) )
end
