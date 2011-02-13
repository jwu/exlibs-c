-- ======================================================================================
-- File         : class.lua
-- Author       : Wu Jie 
-- Last Change  : 02/10/2011 | 22:39:08 PM | Thursday,February
-- Description  : 
-- ======================================================================================

--/////////////////////////////////////////////////////////////////////////////
-- require and module
--/////////////////////////////////////////////////////////////////////////////

local getmetatable = getmetatable
local setmetatable = setmetatable
local require = require
local print = print
local assert = assert
local rawget = rawget
local rawset = rawset
local type = type
local pairs = pairs

module ("ex.class_v2")

--/////////////////////////////////////////////////////////////////////////////
-- functions defines
--/////////////////////////////////////////////////////////////////////////////

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function deepcopy(_obj)
    local lookup_table = {}
    local function _copy(_obj)
        if type(_obj) ~= "table" then
            return _obj
        elseif lookup_table[_obj] then
            return lookup_table[_obj]
        end
        local new_table = {}
        lookup_table[_obj] = new_table
        for index, value in pairs(_obj) do
            new_table[_copy(index)] = _copy(value)
        end
        return setmetatable(new_table, getmetatable(_obj))
    end
    return _copy(_obj)
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function isclass (_class)
    -- TODO { 
    -- local mt = getmetatable(_class)
    -- if mt == class_meta then return true end
    -- } TODO end 
    return false
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function member_readonly ( _t, _k, _v )
    assert( false, "keys are readonly" )
end

function member_readonly_get ( _t, _k )
    local mt = getmetatable(_t) 
    assert( mt, "can't find the metatable of _t" )
    local v = rawget( mt, _k )
    if v ~= nil then return v end

    local super = rawget( mt, "super" )
    local super_mt = nil
    while super ~= nil do
        super_mt = getmetatable(super)
        assert( super_mt, "can't find the metatable of _t" )
        local v = rawget( super_mt, _k )
        if v ~= nil then return v end

        super = rawget( super_mt, "super" )
    end
    return nil
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

-- function class_newindex ( _t, _k )
-- end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function class_index ( _t, _k )
    -- NOTE: the _t can only be object instance, 
    --       we can garantee this, case if it is a class, 
    --       it never use class_index as __index method. 
    --       it use class_meta.__index

    -- speical case
    if _k == "super" then
        local mt = getmetatable(getmetatable(_t)) 
        assert( mt, "can't find valid metatable in _t" )
        return rawget(mt,"super")
    end

    -- check if the table have the key
    local v = rawget(_t,_k)
    if v ~= nil then return v end

    -- check if the metatable have the key
    local mt = getmetatable(getmetatable(_t)) 
    assert( mt, "can't find valid metatable in _t" )
    v = rawget(mt,_k)
    if v ~= nil then 
        local vv = v
        if type(vv) == "table" and isclass(vv) == false then
            vv = deepcopy(v)
        end
        rawset(_t,_k,vv)
        return vv
    end

    -- check if the super have the key
    local super = rawget(mt,"super")
    local super_mt = nil
    while super ~= nil do
        -- get key from super's metatable
        super_mt = getmetatable(super)
        assert( super_mt, "can't find the metatable of _t" )
        local v = rawget( super_mt, _k )

        --
        if v ~= nil then 
            local vv = v
            if type(vv) == "table" and isclass(vv) == false then
                vv = deepcopy(v)
            end
            rawset(_t,_k,vv)
            return vv
        end

        -- get super's super from super's metatable
        super = rawget( super_mt, "super" )
    end

    -- return
    return nil
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function class_new ( _self, ... )
    local table = ...
    return setmetatable( table or {}, _self )
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

-- class_meta = {
--     __call = class_new,
--     __index = member_readonly_get,
--     __newindex = member_readonly,
-- }

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function class(...)
    local base,super = ...
    if super == nil then
        base.super = nil
    else
        -- assert( isclass(super), "super is not a class" )
        -- base.super = setmetatable({},super) -- this allow foo.super.d save values in the super's table
        base.super = super
    end

    local t = {
        __index = class_index,
        -- __newindex = class_newindex,
    }
    base.__call = class_new
    base.__index = member_readonly_get
    base.__newindex = member_readonly
    return setmetatable(t,base)

    -- DISABLE { 
    -- base.__index = class_index
    -- -- base.__newindex = class_newindex,
    -- return setmetatable(base,class_meta)
    -- } DISABLE end 
end

--/////////////////////////////////////////////////////////////////////////////
-- unit tests
--/////////////////////////////////////////////////////////////////////////////

local do_unit_test = false
if do_unit_test == false then
    return
end

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

foo = class {
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
bar = class ({
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
foobar = class ({
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
}, bar)

-- ======================================================== 
-- 
-- ======================================================== 

-- foo_obj = foo {
--     m_normal = 1.0,
-- }
-- foo_obj.m_array = { "foo" }

-- bar_obj = bar {
--     m_normal = 10.0,
-- }
-- print( bar_obj.super.m_array[2] )
-- print( bar_obj.m_array[2] )

-- foobar_obj = foobar {
--     m_normal = 100.0,
-- }
-- print( foobar_obj.m_array[2] )


dbg = require("ex.debug")
print ( dbg.print_table(foo,"foo") )
print ( dbg.print_table(foo_obj,"foo_obj") )

-- print ( dbg.print_table(bar,"bar") )
-- print ( dbg.print_table(bar_obj,"bar_obj") )

-- foobar_obj.test_func2( foobar_obj )
-- foobar_obj.super.test_func2( foobar_obj )
-- print ( dbg.print_table(foobar,"foobar") )
-- foobar_obj.super.m_normal = "hahahahahahaha"
-- print(bar.m_normal)
-- print(bar.m_string)
-- bar.m_normal = "hohohohoho"
-- foo.m_normal = "hehehehehe"
-- print ( dbg.print_table(foobar,"foobar") )
-- print ( dbg.print_table(bar_obj.super,"bar_super") )
-- print ( dbg.print_table(foobar_obj,"foobar_obj") )
