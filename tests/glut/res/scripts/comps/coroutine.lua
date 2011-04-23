-- ======================================================================================
-- File         : coroutine.lua
-- Author       : Wu Jie 
-- Last Change  : 04/23/2011 | 20:37:25 PM | Saturday,April
-- Description  : 
-- ======================================================================================

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

local ex = ex
local super = ex.lua_behavior
local getmetatable = getmetatable
local ex_ease = ex.ease
local math = math
module( ..., super.derive )

--/////////////////////////////////////////////////////////////////////////////
-- properties
--/////////////////////////////////////////////////////////////////////////////

rot_speed = 0.0
move = true

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function awake ( _self )
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function start ( _self )
    -- _self:invoke ( "scale_me_01", 0.0, _self.scale_interval, _self.scale_me )
    -- _self:invoke ( "move_me", 0.0, _self.move_interval ) 
    _self:coroutine ( late_rotate, _self )
    ex.log( _self.entity.name .. " started!" );
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function update ( _self )
    if _self.move then
        _self.trans2d:move( 10.0 * ex.time.dt, 0.0 )
    end
    _self.trans2d:rotate( _self.rot_speed * ex.time.dt )
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function late_rotate ( _self )
    local secs =  ex.range_rand( 1.0, 10.0 )
    ex.log( _self.entity.name .. " step 1 wait for " .. secs );
    ex.yield.wait(secs)
    _self.rot_speed = ex.range_rand( 10.0, 20.0 )
    _self.move = false

    local secs =  ex.range_rand( 1.0, 10.0 )
    ex.log( _self.entity.name .. " step 2 wait for " .. secs );
    ex.yield.wait(secs)
    _self.rot_speed = 0.0
    _self.move = true
end
