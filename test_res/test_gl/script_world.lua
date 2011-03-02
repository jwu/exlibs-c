-- ======================================================================================
-- File         : script_world.lua
-- Author       : Wu Jie 
-- Last Change  : 02/07/2011 | 22:15:30 PM | Monday,February
-- Description  : 
-- ======================================================================================

ex.log ("creating script world...")

ent1 = ex.entity("entity foo 01")
trans2d = ent1:add_comp("ex.trans2d")
trans2d.scale = ex.vec2f(100.0,100.0)
trans2d.position = ex.vec2f(0.0,0.0)
ent1:add_comp("ex.debug2d")

ent2 = ex.entity("entity foo 02")
trans2d = ent2:add_comp("ex.trans2d")
trans2d.scale = ex.vec2f(50.0,50.0)
trans2d.position = ex.vec2f(200.0,200.0)
trans2d.parent = ent1.trans2d
ent2:add_comp("ex.debug2d")
ent2:add_comp("ex.simple_update")

ent3 = ex.entity("entity foo 03")
trans2d = ent3:add_comp("ex.trans2d")
trans2d.scale = ex.vec2f(50.0,50.0)
trans2d.position = ex.vec2f(-200.0,200.0)
trans2d.parent = ent2.trans2d.parent
ent3:add_comp("ex.debug2d")

-- ex.debug.dump(getmetatable(ent),"ent")
-- ent:destroy()

-- local v1 = ex.vec2f( 1.0, 1.0 )
-- print (v1)

-- local v2 = ex.vec2f( 1.0, 1.0 )
-- print (v2)

-- local v3 = v2:copy()
-- v3.x = 2.0
-- print(v2)
-- print(v3)

-- -- local d3 = ex.vec2f.dot( v1, v2 ) 
-- local d3 = ex.vec2f.dot( v1, v2 ) 
-- print (d3)

-- ex.debug.dump(v1,"v1")
-- ex.debug.dump(ex.vec2f,"ex.vec2f")
