-- ======================================================================================
-- File         : script_world.lua
-- Author       : Wu Jie 
-- Last Change  : 02/07/2011 | 22:15:30 PM | Monday,February
-- Description  : 
-- ======================================================================================

ex.log ("creating script world...")

local v1 = ex.vec2f( 1.0, 1.0 )
print (v1)

local v2 = ex.vec2f( 1.0, 1.0 )
print (v2)

local v3 = v2:copy()
v3.x = 2.0
print(v2)
print(v3)

-- local d3 = ex.vec2f.dot( v1, v2 ) 
local d3 = ex.vec2f.dot( v1, v2 ) 
print (d3)

ex.debug.dump(v1,"v1")
ex.debug.dump(ex.vec2f,"ex.vec2f")

t = {
    v = ex.vec2f.one,
    tt = {
        foobar = "hello"
    }
}
ex.debug.dump(t,"t") 
