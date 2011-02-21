-- ======================================================================================
-- File         : test_class.lua
-- Author       : Wu Jie 
-- Last Change  : 02/17/2011 | 09:55:55 AM | Thursday,February
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
-- 
-- ======================================================== 

-- foo = class {
--     a = 2.0,
--     _b = 20.0, -- _b = 20.0, -- private equals to ex.hide|ex.no_serialize
--     c = "hello", -- ex.prop( "hello", ex.readonly ),
--     d = {1,2,3,4}, -- ex.prop( {1,2,3,4}, ex.shared ), -- or s_d ??? __d ???
--     e = 10, -- ex.prop( 10, ex.hide ),
--     f = 20, -- ex.prop( 20, ex.hide|ex.no_serialize ), -- equals to _b
-- }

-- foo -> bar -> foobar

foo = ex.class {
    m_normal = 2.0,
    m_string = "hello",
    m_array = { "one", "two", "three", "four" },
    m_table = {
        a = "i'm a",
        b = "i'm b",
    },
    m_test_func = function( self ) 
        print ( "i'm test function" )
    end
}

bar = ex.class ({
    -- override foo
    m_normal = 10.0,
    -- 
    m_normal2 = 2.0,
    m_string2 = "world",
    m_array2 = { 1, 2, 3, 4 },
    m_table2 = {
        a2 = "i'm a2",
        b2 = "i'm b2",
    },
    m_test_func2 = function( self ) 
        print ( "i'm test function 2" )
    end
}, foo)

-- foobar = ex.class ({
--     -- override foo
--     m_normal = 100.0,
--     m_array = { "five" },

--     -- override bar
--     m_string2 = "hello world",
--     m_table2 = {
--         a2 = "i'm a2 in foobar",
--         b2 = "i'm b2 in foobar",
--         c2 = "i'm c2 in foobar",
--     },
--     m_test_func2 = function( self ) 
--         print ( "i'm test function 2 in foobar" )
--     end,

--     -- 
--     m_test_func3 = function( self ) 
--         print ( "i'm test function 3" )
--     end
-- }, bar)

foobar = bar.derive ({
    -- override foo
    m_normal = 100.0,
    m_array = { "five" },

    -- override bar
    m_string2 = "hello world",
    m_table2 = {
        a2 = "i'm a2 in foobar",
        b2 = "i'm b2 in foobar",
        c2 = "i'm c2 in foobar",
    },
    m_test_func2 = function( self ) 
        print ( "i'm test function 2 in foobar" )
    end,

    -- 
    m_test_func3 = function( self ) 
        print ( "i'm test function 3" )
    end
})

-- ======================================================== 
-- 
-- ======================================================== 

foo_obj = foo {
    m_normal = 1.0,
}
foo_obj.m_array = { "foo" }

bar_obj = bar {
    m_normal = 10.0,
}
print( bar_obj.super.m_array[2] )
print( bar_obj.m_array[2] )
bar_obj.m_array = {"yes", "you", "are", "right"}

foobar_obj = foobar {
    m_normal = 100.0,
}
print( foobar_obj.m_array[1] )


-- ex.debug.dump(foo,"foo")
foo_obj:m_test_func()
ex.debug.dump(foo_obj,"foo_obj") 

-- ex.debug.dump(bar,"bar") 
-- ex.debug.dump(bar_obj,"bar_obj") 

foobar_obj.m_test_func2( foobar_obj )
foobar_obj.super.m_test_func2( foobar_obj )
-- ex.debug.dump(foobar,"foobar") 
-- -- foobar_obj.super.m_normal = "hahahahahahaha"
-- -- bar.m_normal = "hohohohoho"
-- -- foo.m_normal = "hehehehehe"
-- ex.debug.dump(foobar,"foobar") 
-- -- ex.debug.dump(bar_obj.super,"bar_super") 
-- ex.debug.dump(foobar_obj,"foobar_obj") 

print( "foobar_obj isa foobar: " .. tostring(foobar_obj:isa(foobar)) )
print( "foobar_obj isa bar: " .. tostring(foobar_obj:isa(bar)) )
print( "foobar_obj isa foo: " .. tostring(foobar_obj:isa(foo)) )
print( "foobar_obj is instanceof foobar: " .. tostring(foobar_obj:instanceof(foobar)) )
print( "foobar_obj is instanceof bar: " .. tostring(foobar_obj:instanceof(bar)) )
print( "foobar_obj is instanceof foo: " .. tostring(foobar_obj:instanceof(foo)) )
print( "foobar_obj is superof foo: " .. tostring(foobar_obj:superof(foo)) )
print( "foo_obj    is superof foobar: " .. tostring(foo_obj:superof(foobar)) )
print( "foobar_obj is childof foo: " .. tostring(foobar_obj:childof(foo)) )
print( "foobar_obj is childof bar: " .. tostring(foobar_obj:childof(bar)) )

