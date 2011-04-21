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
scale_interval = 0.5
scale_for_seconds = 0.3
move_interval = 0.5
move_for_seconds = 5.0
rot_speed = 5.0
scale_to = 3.0

-- private
_scale_curve = function () end
_scale_state_update = function () end -- update function

_move_curve = function () end
_move_state_update = function () end -- update function
_move_to = ex.vec2f.zero

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function awake ( _self )
    -- ex.log( "awaked component invoked in: " .. _self.entity )
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function start ( _self )
    _self:invoke ( "scale_me_01", 0.0, _self.scale_interval, _self.scale_me )
    _self:invoke ( "move_me", 0.0, _self.move_interval ) 
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function update ( _self )
    _self.trans2d:rotate( _self.rot_speed * ex.time.dt )
    if _self._scale_state_update then _self:_scale_state_update () end
    if _self._move_state_update then _self:_move_state_update () end
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
    _self._scale_curve = ex_ease.make_curve ( 
        -- ex.range_rand( 2.0, 10.0 ), 
        _self.scale_to, 
        1.0, 
        _self.scale_for_seconds, 
        ex_ease.out_expo 
    )
    _self._scale_state_update = _update_scale_state
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function _update_scale_state ( _self )
    local v, finished = _self._scale_curve()
    _self.trans2d.local_scale = ex.vec2f( v, v ) 

    if finished then 
        _self._scale_state_update = nil 
    end
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function _enter_move_state ( _self )
    _self._move_curve = ex_ease.make_curve ( 
        _self.trans2d.position, 
        _self._move_to, 
        _self.move_for_seconds, 
        ex_ease.out_expo 
    )
    _self._move_state_update = _update_move_state
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function _update_move_state ( _self )
    local v, finished = _self._move_curve()
    _self.trans2d.position = v

    if finished then 
        _self._move_state_update = nil 
    end
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- NOTE: this test if invoking, do nothing
-- ------------------------------------------------------------------ 

function scale_me ( _self )
    _self._scale_state_update = _enter_scale_state
    if _self:is_invoking("scale_me_02") == false then
        _self:invoke ( "scale_me_02", 0.1, -1, 
            function (_self) 
                _self._scale_state_update = _enter_scale_state
            end 
        )
    end
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- NOTE: this test if invoking, cancle invoke
-- ------------------------------------------------------------------ 

function move_me ( _self )
    _self._move_to = ex.vec2f( ex.range_rand(-100.0,100.0), ex.range_rand(-100.0,100.0) )
    _self._move_state_update = _enter_move_state

    -- move me 02
    if _self:is_invoking("move_me_02") then
        _self:cancle_invoke ( "move_me_02" )
    end
    _self:invoke ( "move_me_02", 0.1, -1, 
        function (_self) 
            _self._move_to = ex.vec2f( ex.range_rand(-100.0,100.0), ex.range_rand(-100.0,100.0) )
            _self._move_state_update = _enter_move_state
        end 
    )
end
