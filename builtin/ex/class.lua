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

module ("ex.class")

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
    local mt = getmetatable(_class)
    while mt ~= nil do
        if mt == class_meta then return true end
        mt = getmetatable(mt)
    end
    return false
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function class_index ( _t, _k )
    -- check if the table have the key
    local v = rawget(_t,_k)
    if v ~= nil then return v end

    -- get checking if the metatable have the ke
    local mt = getmetatable(_t) 
    while mt ~= nil do
        v = rawget(mt,_k)
        if v ~= nil then 
            local vv = v
            if type(vv) == "table" and isclass(vv) == false then
                vv = deepcopy(v)
            end
            rawset(_t,_k,vv)
            return vv
        end
        mt = getmetatable(mt) 
    end
    -- return
    return nil
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function class_new ( _self, ... )
    local table = ...
    _self.__index = class_index
    return setmetatable( table or {}, _self )
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

class_meta = {
    __index = class_index,
    -- __newindex = class_set,
    __call = class_new,
}

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function class(...)
    local base,super = ...

    if super then
        assert( isclass(super), "super is not a class" )
        super.__index = class_index
        super.__call = class_new
        return setmetatable(base,super)
    else
        return setmetatable(base,class_meta)
    end
end

--/////////////////////////////////////////////////////////////////////////////
-- tests
--/////////////////////////////////////////////////////////////////////////////

-- ======================================================== 
-- 
-- ======================================================== 

foo = class {
    a = 10.0,
    _b = 20.0, -- private equals to ex.hide|ex.no_serialize
    c = "hello", -- ex.prop( "hello", ex.readonly ),
    d = {1,2,3,4}, -- ex.prop( {1,2,3,4}, ex.shared ), -- or s_d ??? __d ???
    e = 10, -- ex.prop( 10, ex.hide ),
    f = 20, -- ex.prop( 20, ex.hide|ex.no_serialize ), -- equals to _b
}
bar = class ({
    gg = 10.0,
    a = 1.0,
}, foo)

-- ======================================================== 
-- 
-- ======================================================== 

foo_obj = foo {
    a = 1.0,
}
foo_obj.d = { 3.0 }

bar_obj = bar{
    a = 100.0,
}
print( bar_obj.d[2] )


dbg = require("ex.debug")
print ( dbg.print_table(foo_obj,"foo_obj") )
print ( dbg.print_table(bar_obj,"bar_obj") )

-- TODO: think about super, so would we need __index for those super functions ????
