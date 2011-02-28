-- ======================================================================================
-- File         : test_angf.lua
-- Author       : Wu Jie 
-- Last Change  : 02/28/2011 | 13:38:06 PM | Monday,February
-- Description  : 
-- ======================================================================================

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

local do_test = true
if not do_test then return end

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

ex.log ( "test angf" )

a1 = ex.angf(30.0)
a2 = ex.angf(45.0)
a3 = ex.angf.zero

print ("a1 is "..a1)
print ("a2 is "..a2)
print ("angf.zero is "..ex.angf.zero)

-- check readonly
-- ex.angf.zero = a2

-- check type
print ( "typename(a3) is " .. ex.typename(a3) )
print ( "typeof(a3) is ex.angf: " .. tostring(ex.typeof(a3) == ex.angf) )

-- check single element
a3 = a1:copy()
a3.radians = a3.radians + 5.0
print ( "a3.radians = " .. a3.radians)

-- check add
a3 = a1 + a2
print ( "a1 + a2 = " .. a3)

-- check sub
a3 = a1 - a2
print ( "a1 - a2 = " .. a3)

-- check mul
a3 = a1 * a2
print ( "a1 * a2 = " .. a3)

-- check ang * scalar
a3 = a1 * 10.0
print ( "a1 * 10.0 = " .. a3)

-- check scalar * ang 
a3 = 10.0 * a1
print ( "10.0 * a1 = " .. a3)

-- check div
a3 = a1 / a2
print ( "a1 / a2 = " .. a3)

-- check ang / scalar
a3 = a1 / 10.0
print ( "a1 / 10.0 = " .. a3)

-- check scalar / ang 
a3 = 10.0 / a1
print ( "10.0 / a1 = " .. a3)

-- check neg
a3 = -a1
print ( "-a1 = " .. a3)

-- check boolean
print ( "a1 is equal to a2: " .. tostring(a1 == a2) )
print ( "a1 is not equal to a2: " .. tostring(a1 ~= a2) )

-- check is type correct
local v = ex.angf.zero
print ("xxxxxxxxxxxxxxxxxxxxxxx")
print ("is builtin " .. tostring(ex.isbuiltin(v)) )
print ("is class " .. tostring(ex.isclass(v)) )
print ("xxxxxxxxxxxxxxxxxxxxxxx")

