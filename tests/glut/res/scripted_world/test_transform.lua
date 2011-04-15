-- ======================================================================================
-- File         : test_transform.lua
-- Author       : Wu Jie 
-- Last Change  : 03/20/2011 | 18:20:28 PM | Sunday,March
-- Description  : 
-- ======================================================================================

ex.log ("test transform...")

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

-- ======================================================== 
-- ent1
-- ======================================================== 

local ent1 = ex.entity("no-scale")
ent1:add_comp("ex.trans2d", {
    -- local_angle = ex.angf(15),
    local_scale = ex.vec2f(1.0,1.0),
    local_position = ex.vec2f(0.0,0.0),
})
ent1:add_comp("ex.debug2d.rect", {
    -- show_text = false,
    width = 10,
    height = 10,
})

-- ======================================================== 
-- ent2
-- ======================================================== 

local ent2 = ex.entity("world space")
ent2:add_comp("ex.trans2d", {
    parent = ent1.trans2d,
    local_scale = ex.vec2f(1.0,1.0),
    position = ex.vec2f(100.0,50.0),
})
ent2:add_comp("ex.debug2d.rect", {
    width = 10,
    height = 10,
})
ent2:add_comp("comps.move")

-- ======================================================== 
-- ent3
-- ======================================================== 

local ent3 = ex.entity("self space")
ent3:add_comp("ex.trans2d", {
    parent = ent1.trans2d,
    local_scale = ex.vec2f(1.0,1.0),
    position = ex.vec2f(100.0,150.0),
})
ent3:add_comp("ex.debug2d.rect", {
    width = 10,
    height = 10,
})
ent3:add_comp("comps.move02")

-- ======================================================== 
-- rot
-- ======================================================== 

local rot = ex.entity("scale")
rot:add_comp("ex.trans2d", {
    -- local_angle = ex.angf(15),
    local_scale = ex.vec2f(2.0,2.0),
    local_position = ex.vec2f(0.0,-200.0),
})
rot:add_comp("ex.debug2d.rect", {
    -- show_text = false,
    width = 10,
    height = 10,
})
-- rot:add_comp("comps.rotate")

-- ======================================================== 
-- ent4
-- ======================================================== 

local ent4 = ex.entity("transform space")
ent4:add_comp("ex.trans2d", {
    parent = ent1.trans2d,
    local_scale = ex.vec2f(1.0,1.0),
    position = ex.vec2f(100.0,250.0),
})
ent4:add_comp("ex.debug2d.rect", {
    width = 10,
    height = 10,
})
ent4:add_comp("comps.move03", {
    ref_trans = rot.trans2d 
})

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

-- -- ======================================================== 
-- -- ent1
-- -- ======================================================== 

-- local ent1 = ex.entity("scale")
-- ent1:add_comp("ex.trans2d", {
--     -- local_angle = ex.angf(15),
--     local_scale = ex.vec2f(2.0,2.0),
--     local_position = ex.vec2f(0.0,-200.0),
-- })
-- ent1:add_comp("ex.debug2d.rect", {
--     -- show_text = false,
--     width = 10,
--     height = 10,
-- })
-- ent1:add_comp("comps.rotate")

-- -- ======================================================== 
-- -- ent2
-- -- ======================================================== 

-- local ent2 = ex.entity("world space")
-- ent2:add_comp("ex.trans2d", {
--     parent = ent1.trans2d,
--     local_scale = ex.vec2f(1.0,1.0),
--     position = ex.vec2f(100.0,50.0) + ent1.trans2d.position,
-- })
-- ent2:add_comp("ex.debug2d.rect", {
--     width = 10,
--     height = 10,
-- })
-- ent2:add_comp("comps.move")

-- -- ======================================================== 
-- -- ent3
-- -- ======================================================== 

-- local ent3 = ex.entity("self space")
-- ent3:add_comp("ex.trans2d", {
--     parent = ent1.trans2d,
--     local_scale = ex.vec2f(1.0,1.0),
--     position = ex.vec2f(100.0,150.0) + ent1.trans2d.position,
-- })
-- ent3:add_comp("ex.debug2d.rect", {
--     width = 10,
--     height = 10,
-- })
-- ent3:add_comp("comps.move02")
