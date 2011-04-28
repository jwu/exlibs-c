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
    ex.log( _self.entity.name .. " started!" )

    -- step 1, rotate, stop_rotate
    ex.yield( _self:coroutine(rotate,_self) )

    -- step 2
    _self:coroutine(wait_for_eof,_self)

    -- step 3
    ex.yield.wait(2.0)
    ex.log( _self.entity.name .. ": coroutine finished")

    -- while true do
    --     _self.trans2d:move( 10.0, 0.0 )
    --     ex.yield.wait(2.0)
    -- end
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function update ( _self )
    local t2d = _self.trans2d
    if _self.move then
        local v = t2d.position.normalized * ex.time.dt * 20.0
        t2d:move( v.x, v.y )
    end
    t2d:rotate( _self.rot_speed * ex.time.dt )
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

function wait_for_eof ( _self )
    local ent_name = _self.entity.name
    ex.log( ent_name .. ": yield end of frame in " .. ex.time.frames )
    ex.yield.end_of_frame()
    ex.log( ent_name .. ": reported at the end of the frame " .. ex.time.frames )
    ex.log( ent_name .. ": yield one frame in " .. ex.time.frames )
    ex.yield.one_frame()
    ex.log( ent_name .. ": reported in the frame " .. ex.time.frames )
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function rotate ( _self )
    _self.secs =  ex.range_rand( 1.0, 2.0 )
    ex.log( _self.entity.name .. ": step 1 wait for " .. _self.secs )
    _self._start_at = ex.time.time
    ex.yield.wait(_self.secs)
    _self.rot_speed = ex.range_rand( -20.0, 20.0 )
    _self.move = false

    ex.yield( _self:coroutine(stop_rotate,_self) )
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function stop_rotate ( _self )
    _self.secs =  ex.range_rand( 2.0, 3.0 )
    ex.log( _self.entity.name .. ": step 2 wait for " .. _self.secs )
    _self._start_at = ex.time.time
    ex.yield.wait(_self.secs)
    _self.rot_speed = 0.0
    _self.move = true
end
