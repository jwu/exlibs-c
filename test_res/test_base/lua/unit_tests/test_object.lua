-- ======================================================================================
-- File         : test_object.lua
-- Author       : Wu Jie 
-- Last Change  : 02/21/2011 | 13:50:00 PM | Monday,February
-- Description  : 
-- ======================================================================================

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

require ("ex.debug")

local do_test = true
if not do_test then return end

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

-- ======================================================== 
-- test foo that derived from ex.object
-- ======================================================== 


foo = ex.class( {
    __typename = "foo",
    text = "Hello, I'm foo!",
    array = { "one", "two", "three", "four" },
    table = {
        a = "I'm a",
        b = "I'm b",
    },
    say = function (_self) 
        ex.log("foo said: " .. _self.text)
    end
} )

-- ======================================================== 
-- test bar that derived from foo
-- ======================================================== 

bar = ex.class( {
    __typename = "bar",
    text = "Hello, I'm bar!",
    text2 = "foo is my parent.", 
    say = function (_self) 
        bar.__super.say (_self)
        ex.log( _self.text2 )
    end,
}, foo )

-- ======================================================== 
-- test foo_obj 
-- ======================================================== 

ex.debug.dump(foo,"foo") 
foo_obj = foo()
ex.debug.dump(foo_obj,"foo_obj") 

if foo_obj.array ~= nil then ex.log("foo.array is copied from metatable.") end
ex.debug.dump(foo_obj,"foo_obj") 
foo_obj:say()

-- ======================================================== 
-- test bar_obj
-- ======================================================== 

ex.debug.dump(bar,"bar") 
bar_obj = bar()
ex.debug.dump(bar_obj,"bar_obj") 

if bar_obj.array ~= nil then ex.log("bar.array is copied from metatable.") end
ex.debug.dump(bar_obj,"bar_obj") 
bar_obj:say()
