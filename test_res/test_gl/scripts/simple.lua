-- ======================================================================================
-- File         : simple.lua
-- Author       : Wu Jie 
-- Last Change  : 02/06/2011 | 23:51:16 PM | Sunday,February
-- Description  : 
-- ======================================================================================

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

local setmetatable = setmetatable
local print = print

module (...)

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

-- a = { 
--     foo_a = "hello foo_a",
--     bar_a = "hello bar_a",
-- }
-- a.__index = a

-- b = { 
--     foo_b = "hello foo_b",
--     bar_b = "hello bar_b",
-- }
-- b.__index = b
-- setmetatable( b, a )

-- c = { 
--     foo_c = "hello foo_c",
--     bar_c = "hello bar_c",
-- }
-- c.__index = c
-- setmetatable( c, b )

-- inst = setmetatable( { foobar = "hello foobar" }, c )
-- print( inst.foo_a )
-- print( inst.foo_b )
-- print( inst.foo_c )
-- print( inst.foobar )

