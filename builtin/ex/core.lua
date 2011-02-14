-- ======================================================================================
-- File         : core.lua
-- Author       : Wu Jie 
-- Last Change  : 02/14/2011 | 12:01:21 PM | Monday,February
-- Description  : 
-- ======================================================================================

--/////////////////////////////////////////////////////////////////////////////
-- require and module
--/////////////////////////////////////////////////////////////////////////////

require("ex.math")
require("ex.ease")

local ex_math,ex_ease = ex.math,ex.ease
local os,math = os,math
local assert = assert
local type = type
local getmetatable = getmetatable
local setmetatable = setmetatable
local pairs = pairs
local rawget = rawget
local rawset = rawset

module("ex")

--/////////////////////////////////////////////////////////////////////////////
-- class defines
--/////////////////////////////////////////////////////////////////////////////

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

typeof = getmetatable

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

local function class_new ( _self, ... )
    local table = ...
    return setmetatable( table or {}, _self )
end

--

class_meta = {
    __call = class_new,
}

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function isclass (_class)
    if type(_class) ~= "table" then 
        return false
    end

    local mt = getmetatable(_class)
    if mt == class_meta then return true end
    return false
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function isbuiltin (_class)
    if type(_class) ~= "table" then 
        return false 
    end 

    local r = rawget(_class, "__builtin")
    if r == nil then return false end
    return r
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function typename(_object)
    local tp = type(_object) 
    if tp == "userdata" or tp == "table" then 
        local name = rawget(typeof(_object), "__typename")
        assert ( name ~= nil, "can't find __typename define in your class." )
        return name
    end
    return tp
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function classof(_object, _class)
    return typeof(_object) == _class
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

local function __childof(_myclass,_superclass)
    local super = rawget(_myclass,"__super")
    while super ~= nil do
        if super == _superclass then 
            return true 
        end
        super = rawget(super,"__super")
    end
    return false
end

--

function childof(_object, _superclass)
    return __childof(typeof(_object),_superclass)
end

--

function superof(_object, _subclass)
	return __childof(_subclass,typeof(_object))
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function isa(_object, _class)
    local cls = typeof(_object)
    return cls == _class or __childof(cls,_class)
end

--/////////////////////////////////////////////////////////////////////////////
-- misc functions
--/////////////////////////////////////////////////////////////////////////////

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function make_curve ( _from, _to, _duration, _curve )
    assert ( type(_from) == "number", "_from is not a number" )
    assert ( type(_to) == "number", "_to is not a number" )

    -- init startTime and curve
    local startTime = os.clock()
    local curve = _curve or ex_ease.linear  

    -- create tick closure
    return function ()
        local curTime = os.clock()
        local t = math.min( 1.0, (curTime - startTime) / _duration )
        local ratio = curve(t)
        return ex_math.lerp( _from, _to, ratio ), (t == 1.0) -- current value, finished
    end
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function deepcopy (_obj)
    local lookup_table = {}
    local function _copy(_obj)
        if isbuiltin( typeof(_obj) ) then
            assert(_obj.copy, "please provide copy function for builtin type: " .. typename(_obj) )
            return _obj:copy()
        elseif type(_obj) ~= "table" then
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
