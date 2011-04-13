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

local do_test = true
if not do_test then return end

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

-- ======================================================== 
-- 
-- ======================================================== 

-- class_a = class {
--     a = 2.0,
--     _b = 20.0, -- _b = 20.0, -- private equals to ex.hide|ex.no_serialize
--     c = "hello", -- ex.prop( "hello", ex.readonly ),
--     d = {1,2,3,4}, -- ex.prop( {1,2,3,4}, ex.shared ), -- or s_d ??? __d ???
--     e = 10, -- ex.prop( 10, ex.hide ),
--     f = 20, -- ex.prop( 20, ex.hide|ex.no_serialize ), -- equals to _b
-- }

-- class_a -> class_b -> class_c

class_a = ex.class {
    __typename = "class_a",

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

class_b = ex.class ({
    __typename = "class_b",

    -- override class_a
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
}, class_a)

-- class_c = ex.class ({
--     -- override class_a
--     m_normal = 100.0,
--     m_array = { "five" },

--     -- override class_b
--     m_string2 = "hello world",
--     m_table2 = {
--         a2 = "i'm a2 in class_c",
--         b2 = "i'm b2 in class_c",
--         c2 = "i'm c2 in class_c",
--     },
--     m_test_func2 = function( self ) 
--         print ( "i'm test function 2 in class_c" )
--     end,

--     -- 
--     m_test_func3 = function( self ) 
--         print ( "i'm test function 3" )
--     end
-- }, class_b)

class_c = class_b.derive ({
    __typename = "class_c",

    -- override class_a
    m_normal = 100.0,
    m_array = { "five" },

    -- override class_b
    m_string2 = "hello world",
    m_table2 = {
        a2 = "i'm a2 in class_c",
        b2 = "i'm b2 in class_c",
        c2 = "i'm c2 in class_c",
    },
    m_test_func2 = function( self ) 
        print ( "i'm test function 2 in class_c" )
    end,

    -- 
    m_test_func3 = function( self ) 
        print ( "i'm test function 3" )
    end
})

-- ======================================================== 
-- 
-- ======================================================== 

object_a = class_a {
    m_normal = 1.0,
}
object_a.m_array = { "class_a" }

object_b = class_b {
    m_normal = 10.0,
}
print( class_b.__super.m_array[2] )
print( object_b.m_array[2] )
object_b.m_array = {"yes", "you", "are", "right"}

object_c = class_c {
    m_normal = 100.0,
}
print( object_c.m_array[1] )


-- ex.debug.dump(class_a,"class_a")
object_a:m_test_func()
ex.debug.dump(object_a,"object_a") 

-- ex.debug.dump(class_b,"class_b") 
-- ex.debug.dump(object_b,"object_b") 

object_c.m_test_func2( object_c )
class_c.__super.m_test_func2( object_c )
-- ex.debug.dump(class_c,"class_c") 
-- -- class_c.__super.m_normal = "hahahahahahaha"
-- -- class_b.m_normal = "hohohohoho"
-- -- class_a.m_normal = "hehehehehe"
-- ex.debug.dump(class_c,"class_c") 
-- -- ex.debug.dump(class_b.__super,"bar_super") 
-- ex.debug.dump(object_c,"object_c") 

print( "object_c isa class_c: " .. tostring(object_c:isa(class_c)) )
print( "object_c isa class_b: " .. tostring(object_c:isa(class_b)) )
print( "object_c isa class_a: " .. tostring(object_c:isa(class_a)) )
print( "object_c is instanceof class_c: " .. tostring(object_c:instanceof(class_c)) )
print( "object_c is instanceof class_b: " .. tostring(object_c:instanceof(class_b)) )
print( "object_c is instanceof class_a: " .. tostring(object_c:instanceof(class_a)) )
print( "object_c is superof class_a: " .. tostring(object_c:superof(class_a)) )
print( "object_a    is superof class_c: " .. tostring(object_a:superof(class_c)) )
print( "object_c is childof class_a: " .. tostring(object_c:childof(class_a)) )
print( "object_c is childof class_b: " .. tostring(object_c:childof(class_b)) )

