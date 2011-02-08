-- ======================================================================================
-- File         : test_wrap.lua
-- Author       : Wu Jie 
-- Last Change  : 02/08/2011 | 08:48:03 AM | Tuesday,February
-- Description  : 
-- ======================================================================================

ex.log ( "test wraps" )

v1 = ex.vec2f.new ( 1.0, 1.0 )
v2 = ex.vec2f.new ( 1.0, 1.0 )
-- v2 = ex.vec2f.zero
-- v3 = ex.vec2f.one

print (v1)
v1.x = v1.x + 20.0
v1.y = v1.y - 20.0
print (v1)

v3 = v1 + v2
print (v3)
