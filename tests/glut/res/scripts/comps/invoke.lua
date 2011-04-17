-- ======================================================================================
-- File         : invoke.lua
-- Author       : Wu Jie 
-- Last Change  : 04/17/2011 | 09:27:52 AM | Sunday,April
-- Description  : 
-- ======================================================================================

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

local ex = ex
local super = ex.lua_behavior
local getmetatable = getmetatable
local ex_ease = ex.ease
module( ..., super.derive )

--/////////////////////////////////////////////////////////////////////////////
-- properties
--/////////////////////////////////////////////////////////////////////////////

-- public
interval = 0.5
scale_for_seconds = 0.3

-- private
_curve = function () end
_state_update = function () end -- update function

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function awake ( _self )
    ex.log( "awaked component invoked in: " .. _self.entity )
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function start ( _self )
    -- _self:invoke ( "scale_me_01", 0.0, _self.interval, _self.scale_me )
    _self:invoke ( "scale_me_01", 0.0, 0.5, _self.scale_me )
    -- _self:invoke ( "scale_me_02", 0.0, _self.interval + 0.1, _self.scale_me )
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function update ( _self )
    _self.trans2d:rotate( 5.0 * ex.time.dt )
    if _self._state_update then _self:_state_update () end
end

-- -- ------------------------------------------------------------------ 
-- -- Desc: 
-- -- ------------------------------------------------------------------ 

-- function on_render ( _self )
--     local wpos = _self.trans2d.position
--     ex.draw_text( wpos.x - 30, wpos.y + 15, 0.0, _self.entity.name )
-- end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function _enter_scale_state ( _self )
    _self._curve = ex_ease.make_curve ( 
        -- ex.range_rand( 2.0, 10.0 ), 
        2.0, 
        1.0, 
        _self.scale_for_seconds, 
        ex_ease.out_expo 
    )
    _self._state_update = _update_scale_state
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function _update_scale_state ( _self )
    local v, finished = _self._curve()
    _self.trans2d.local_scale = ex.vec2f( v, v ) 

    if finished then 
        _self._state_update = nil 
    end
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function scale_me ( _self )
    _self._state_update = _enter_scale_state
    -- _self:cancle_invoke ( "scale_me_02" )
    _self:invoke ( "scale_me_02", 0.1, -1, _self.scale_me )
end
