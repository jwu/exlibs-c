-- ======================================================================================
-- File         : test_wrap.lua
-- Author       : Wu Jie 
-- Last Change  : 02/08/2011 | 08:48:03 AM | Tuesday,February
-- Description  : 
-- ======================================================================================

ex.log ( "test wraps" )

v1 = ex.vec2f.new ( 10.0, 20.0 )
v2 = ex.vec2f.new ( -20.0, -10.0 )

print (v1)
print (v2)
print (ex.vec2f.one)
print (ex.vec2f.zero)

-- check readonly
ex.vec2f.one = v2
ex.vec2f.zero = v2

print (ex.vec2f.one)

-- check single element
v1.x = v1.x + 5.0
v1.y = v1.y - 5.0
print ( "v1.x + 5, v1.y - 5 = " .. tostring(v1))

-- check add
v3 = v1 + v2
print ( "v1 + v2 = " .. tostring(v3))

-- check sub
v3 = v1 - v2
print ( "v1 - v2 = " .. tostring(v3))

-- check type
n = 10
print (v3.type)
print ( type(n) )
