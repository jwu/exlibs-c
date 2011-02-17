-- ======================================================================================
-- File         : core.lua
-- Author       : Wu Jie 
-- Last Change  : 02/14/2011 | 12:01:21 PM | Monday,February
-- Description  : 
-- ======================================================================================

--/////////////////////////////////////////////////////////////////////////////
-- require and module
--/////////////////////////////////////////////////////////////////////////////

local assert = assert
local type = type
local getmetatable = getmetatable
local setmetatable = setmetatable
local pairs = pairs
local rawget = rawget
local rawset = rawset

module("ex")

--/////////////////////////////////////////////////////////////////////////////
-- base functions
--/////////////////////////////////////////////////////////////////////////////

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

--/////////////////////////////////////////////////////////////////////////////
-- type-op
--/////////////////////////////////////////////////////////////////////////////

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

typeof = getmetatable

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

meta_class = {
    __call = function ( _self, ... )
        local table = ...
        return setmetatable( table or {}, _self )
    end
}

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function isclass (_class)
    if type(_class) ~= "table" then 
        return false
    end

    local mt = getmetatable(_class)
    if mt == meta_class then return true end
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

--/////////////////////////////////////////////////////////////////////////////
-- classes functions
--/////////////////////////////////////////////////////////////////////////////

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

local function classof(_object, _class)
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

local function childof(_object, _superclass)
    return __childof(typeof(_object),_superclass)
end

--

local function superof(_object, _subclass)
	return __childof(_subclass,typeof(_object))
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

local function isa(_object, _class)
    local cls = typeof(_object)
    return cls == _class or __childof(cls,_class)
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

local function member_readonly ( _t, _k, _v )
    assert( false, "keys are readonly" )
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

local function member_readonly_get ( _t, _k )
    print( "get value " .. _k )
    return rawget(_t,_k)
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

local function class_newindex ( _t, _k, _v )
    -- NOTE: the _t can only be object instance, 
    --       we can garantee this, case if it is a class, 
    --       it never use class_index as __index method. 
    --       it use meta_class.__index

    -- make sure only get __readonly in table _t, not invoke __index method.
    local is_readonly = rawget(_t,"__readonly")
    if is_readonly then -- this equals to (is_readonly ~= nil and is_readonly == true)
        assert ( false, "the table is readonly" )
        return
    end

    -- check if the metatable have the key
    local mt = getmetatable(_t) 
    assert( mt, "can't find the metatable of _t" )
    local v = rawget(mt,_k)
    if v ~= nil then 
        -- DISABLE: the new index can only garantee 'new' value, but old one still not protected. { 
        -- if type(v) ~= type(_v) then
        --     assert( false, "can't set the key ".._k..", the type is not the same" )
        --     return
        -- end
        -- } DISABLE end 
        rawset(_t,_k,_v)
        return
    end

    -- check if the super have the key
    local super = rawget(mt,"__super")
    while super ~= nil do
        -- get key from super's metatable
        v = rawget(super,_k)

        --
        if v ~= nil then 
            -- DISABLE: the new index can only garantee 'new' value, but old one still not protected. { 
            -- if type(v) ~= type(_v) then
            --     assert( false, "can't set the key ".._k..", the type is not the same" )
            --     return
            -- end
            -- } DISABLE end 
            rawset(_t,_k,_v)
            return
        end

        -- get super's super from super's metatable
        super = rawget(super,"__super")
    end

    -- 
    assert( false, "can't find the key " .. _k )
    return
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

local function class_index ( _t, _k )
    -- NOTE: the _t can only be object instance, 
    --       we can garantee this, case if it is a class, 
    --       it never use class_index as __index method. 
    --       it use meta_class.__index

    -- speical case
    if _k == "super" then
        local mt = getmetatable(_t) 
        assert( mt, "can't find the metatable of _t" )
        -- NOTE: in class_newindex, it will check if table have __readonly, and prevent setting things.
        return setmetatable( { __readonly = true }, rawget(mt,"__super") )
    end

    -- check if the metatable have the key
    local mt = getmetatable(_t) 
    assert( mt, "can't find the metatable of _t" )
    local v = rawget(mt,_k)
    if v ~= nil then 
        local vv = v
        if type(vv) == "table" and getmetatable(vv) == nil then
            vv = deepcopy(v)
        end
        rawset(_t,_k,vv)
        return vv
    end

    -- check if the super have the key
    local super = rawget(mt,"__super")
    while super ~= nil do
        -- get key from super's metatable
        v = rawget(super,_k)

        --
        if v ~= nil then 
            local vv = v
            if type(vv) == "table" and getmetatable(vv) == nil then
                vv = deepcopy(v)
            end
            rawset(_t,_k,vv)
            return vv
        end

        -- get super's super from super's metatable
        super = rawget(super,"__super")
    end

    -- return
    return nil
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function class(...)
    local base,super = ...
    assert( type(base) == "table", "the first parameter must be a table" )

    if super == nil then
        rawset(base, "__super", nil)
    else
        assert( isclass(super), "super is not a class" )
        rawset(base, "__super", super)
    end

    base.__index = class_index
    base.__newindex = class_newindex
    base.classof = classof
    base.superof = superof
    base.childof = childof
    base.isa = isa
    base.derive = function (_t)
        return class( _t, base )
    end
    return setmetatable(base,meta_class)
end
