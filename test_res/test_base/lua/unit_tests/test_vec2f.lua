-- ======================================================================================
-- File         : test_vec2f.lua
-- Author       : Wu Jie 
-- Last Change  : 02/09/2011 | 00:42:52 AM | Wednesday,February
-- Description  : 
-- ======================================================================================

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

local do_test = false
if not do_test then return end

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

ex.log ( "test vec2f" )

v1 = ex.vec2f( 10.0, 20.0 )
v2 = ex.vec2f( -20.0, -10.0 )

print ("v1 is "..v1)
print ("v2 is "..v2)
print ("vec2f.one is "..ex.vec2f.one)
print ("vec2f.zero is "..ex.vec2f.zero)

for i=1,#v1 do
    print( "v1[" .. i .. "]" .. " is " .. v1[i])
end

-- check readonly
-- ex.vec2f.one = v2
-- ex.vec2f.zero = v2
-- ex.vec2f.one.x = 20.0
print (ex.vec2f.one)

-- check single element
v3 = v1
v3.x = v3.x + 5.0
v3.y = v3.y - 5.0
print ( "v1.x + 5, v1.y - 5 = " .. v3)

-- check type
print (v3.type)

-- check add
v3 = v1 + v2
print ( "v1 + v2 = " .. v3)

-- check sub
v3 = v1 - v2
print ( "v1 - v2 = " .. v3)

-- check mul
v3 = v1 * v2
print ( "v1 * v2 = " .. v3)

-- check vec * scalar
v3 = v1 * 10.0
print ( "v1 * 10.0 = " .. v3)

-- check scalar * vec 
v3 = 10.0 * v1
print ( "10.0 * v1 = " .. v3)

-- check div
v3 = v1 / v2
print ( "v1 / v2 = " .. v3)

-- check vec / scalar
v3 = v1 / 10.0
print ( "v1 / 10.0 = " .. v3)

-- check scalar / vec 
v3 = 10.0 / v1
print ( "10.0 / v1 = " .. v3)

-- check neg
v3 = -v1
print ( "-v1 = " .. v3)

-- check dot
f = ex.vec2f.dot(v1,v2)
print ( "v1 dot v2 = " .. f)

-- check cross
f = ex.vec2f.cross(v1, v2)
print ( "v1 cross v2 = " .. f)

-- check normalized
f = ex.vec2f.dot(v1.normalized, v2.normalized)
print ( "normliazed v1 dot v2 = " .. f)

-- check length
print ( "v3 length = " .. v3.length)
print ( "v3 squred length = " .. v3.sqr_length)
print ( "v3 length = " .. v3.normalized.length)
print ( "v3 squred length = " .. v3.normalized.sqr_length)

-- check boolean
print ( "v1 is equal to v2: " .. tostring(v1 == v2) )
print ( "v1 is not equal to v2: " .. tostring(v1 ~= v2) )

-- check is type correct
v = ex.vec2f.zero
print ("xxxxxxxxxxxxxxxxxxxxxxx")
print ("is builtin " .. tostring(ex.isbuiltin( ex.typeof(v) )) )
print ("is class " .. tostring(ex.isclass( ex.typeof(v) )) )
print ("is vec2f " .. tostring(ex.typeof(v) == ex.vec2f) )
print ("xxxxxxxxxxxxxxxxxxxxxxx")
