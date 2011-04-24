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
local string = string
module( ..., super.derive )

--/////////////////////////////////////////////////////////////////////////////
-- properties
--/////////////////////////////////////////////////////////////////////////////

rot_speed = 0.0
move = true
secs = 0.0
_start_at = 0.0

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

function on_render ( _self )
    if _self.move == false then
        local wpos = _self.trans2d.position
        local remain = _self.secs - (ex.time.time - _self._start_at) 
        ex.draw_text( wpos.x, wpos.y+15, 0.0, string.format("%.2f", remain) )
    end
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function late_rotate ( _self )
    _self.secs =  ex.range_rand( 1.0, 10.0 )
    ex.log( _self.entity.name .. " step 1 wait for " .. _self.secs );
    _self._start_at = ex.time.time
    ex.yield.wait(_self.secs)
    _self.rot_speed = ex.range_rand( 10.0, 20.0 )
    _self.move = false

    _self.secs =  ex.range_rand( 1.0, 10.0 )
    ex.log( _self.entity.name .. " step 2 wait for " .. _self.secs );
    _self._start_at = ex.time.time
    ex.yield.wait(_self.secs)
    _self.rot_speed = 0.0
    _self.move = true
end
