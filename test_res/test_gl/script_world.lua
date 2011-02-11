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

local d3 = ex.vec2f.dot( v1, v2 ) 
print (d3)
