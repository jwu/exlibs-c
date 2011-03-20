-- ======================================================================================
-- File         : test_transform.lua
-- Author       : Wu Jie 
-- Last Change  : 03/20/2011 | 18:20:28 PM | Sunday,March
-- Description  : 
-- ======================================================================================

ex.log ("test transform...")

-- ======================================================== 
-- ent1
-- ======================================================== 

local ent1 = ex.entity("entity foo 01")
ent1:add_comp("ex.trans2d", {
    scale = ex.vec2f(10.0,10.0),
    position = ex.vec2f(0.0,0.0),
})
ent1:add_comp("ex.debug2d.rect", {
    show_text = false,
    width = 2,
    height = 2,
})

-- ======================================================== 
-- ent2
-- ======================================================== 

local ent2 = ex.entity("entity foo 02")
ent2:add_comp("ex.trans2d", {
    -- scale = ex.vec2f(50.0,50.0),
    position = ex.vec2f(100.0,100.0),
    parent = ent1.trans2d,
})
ent2:add_comp("ex.debug2d.rect", {
    width = 10,
    height = 10,
})

-- ======================================================== 
-- ent3
-- ======================================================== 

local ent3 = ex.entity("entity foo 03")
ent3:add_comp("ex.trans2d", {
    -- scale = ex.vec2f(50.0,50.0),
    position = ex.vec2f(-100.0,100.0),
    parent = ent2.trans2d.parent,
})
ent3:add_comp("ex.debug2d.rect", {
    width = 10,
    height = 10,
})

