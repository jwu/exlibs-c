-- ======================================================================================
-- File         : move.lua
-- Author       : Wu Jie 
-- Last Change  : 03/22/2011 | 12:38:11 PM | Tuesday,March
-- Description  : 
-- ======================================================================================

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

local ex = ex
local super = ex.lua_behavior
local getmetatable = getmetatable
module( ..., super.derive )

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function awake ( _self )
    ex.log( "the super type name is: " .. __super.__typename )
    ex.log( "the super type name is: " .. super.__typename )
    -- ex.debug.dump ( getmetatable(_self), "move" )
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function start ( _self )
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function update ( _self )
    -- _self.trans2d:rotate( 5.0 * ex.time.dt )
    -- local t = _self.trans2d.right * 2.0 * ex.time.dt
    -- _self.trans2d:move( t.x, t.y )
    -- _self.trans2d.scale = ex.vec2f( ex.time.time % 4.0, ex.time.time % 4.0 )
    -- _self.trans2d:move( 0.1 * ex.time.dt, 0.0, ex.space.self )
    _self.trans2d:move( 10.0 * ex.time.dt, 0.0, ex.space.world )
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function on_render ( _self )
    local wpos = _self.trans2d.position
    -- ex.draw_text( wpos.x - 30, wpos.y + 15, 0.0, _self.entity.name )
end
