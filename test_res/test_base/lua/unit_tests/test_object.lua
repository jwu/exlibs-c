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

local do_test = false
if not do_test then return end

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

-- ======================================================== 
-- test ex.object
-- ======================================================== 

-- ex.debug.dump(ex.object,"ex.object") 

-- local obj = ex.object()
-- ex.debug.dump(getmetatable(obj),"obj") 
-- ex.debug.dump(obj,"obj") 
-- obj:destroy()
-- obj = nil

-- ======================================================== 
-- test foo that derived from ex.object
-- ======================================================== 


foo = ex.class( {
    __typename = "foo",
    text = "hello foo",
    say = function (_self) 
        ex.log("foo said: " .. _self.text)
    end
}, ex.object )

-- foo_obj = foo()
-- foo_obj.name = "foobar object"
-- foo_obj.text = "hello foo, mu ha ha ha!"
-- ex.log ( "the name is: " .. foo_obj.name)
-- ex.log ( "the text is: " .. foo_obj.text)

-- ex.debug.dump(foo,"foo") 
-- ex.debug.dump(foo_obj,"foo_obj") 
-- ex.debug.dump(getmetatable(foo_obj),"foo_obj_meta") 

-- ======================================================== 
-- test bar that derived from foo
-- ======================================================== 

bar = ex.class( {
    __typename = "bar",
    text2 = "hello bar",
    say = function (_self) 
        ex.log("bar said: " .. _self.text2)
    end,
    destroy = function (_self)
        ex.log("haha, my destroy")
        _self.super.destroy(_self)
    end
}, foo )

bar_obj = bar()
assert ( bar_obj.destroy ~= nil, "destory can't be nil" )
bar_obj.text = "hello foo from bar, mu ha ha ha!"
bar_obj.text2 = "hello bar, mu ha ha ha!"
ex.log ( "the text is: " .. bar_obj.text)
ex.log ( "the text2 is: " .. bar_obj.text2)
bar_obj:say()
bar_obj.super.say(bar_obj)
-- bar_obj.super.text = "yes"

ex.debug.dump(bar,"bar") 
ex.debug.dump(bar_obj,"bar_obj") 
ex.debug.dump(getmetatable(bar_obj),"bar_obj_meta") 
-- bar_obj:destroy()
