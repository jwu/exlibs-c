-- ======================================================================================
-- File         : test_require.lua
-- Author       : Wu Jie 
-- Last Change  : 02/08/2011 | 08:38:33 AM | Tuesday,February
-- Description  : 
-- ======================================================================================

print ( package.path )

print ( "hello foobar by lua_print" )
ex.log ( "hello foobar by ex_log" )

require ("foo")
require ("bar")
require ("my.foo.bar")

ex.foo()
ex.bar()
ex.my_foobar()

-- foobar.foo()
-- foobar.bar()
-- foobar.my_foobar()

-- foo.foo()
-- bar.bar()
-- my.foo.bar.my_foobar()

-- package.path = ""
-- package.path = package.path .. "?./"
-- print ( package.path )
