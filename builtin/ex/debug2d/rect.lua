-- ======================================================================================
-- File         : rect.lua
-- Author       : Wu Jie 
-- Last Change  : 03/20/2011 | 00:37:23 AM | Sunday,March
-- Description  : 
-- ======================================================================================

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

local ex = ex
local gl = gl
local tostring = tostring

local super = require("ex.debug2d.shape")
module( ..., super.derive )

--/////////////////////////////////////////////////////////////////////////////
-- properties
--/////////////////////////////////////////////////////////////////////////////

center = ex.vec2f.zero
width = 1.0
height = 1.0

--/////////////////////////////////////////////////////////////////////////////
-- functions
--/////////////////////////////////////////////////////////////////////////////

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function awake ( _self )
    super.awake(_self)
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function start ( _self )
    super.start(_self)
end


-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function update ( _self )
    super.update(_self)
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function on_render ( _self )
    super.on_render(_self)

    gl.MatrixMode( "MODELVIEW" )
    gl.LoadIdentity()

    -- DISABLE: un-efficient way { 
    -- local world_pos = _self.trans2d.position
    -- local world_ang = _self.trans2d.angle
    -- local world_scale = _self.trans2d.scale
    -- gl.Translate( world_pos.x, world_pos.y, 0.0 )
    -- gl.Rotate( world_ang.degrees, 0.0, 0.0, 1.0 )
    -- gl.Scale( world_scale.x, world_scale.y, 1.0 )
    -- } DISABLE end 
    local m = _self.trans2d.local_to_world 
    gl.LoadMatrix( {
        m.m00, m.m01, m.m02, 0.0,
        m.m10, m.m11, m.m12, 0.0,
        0.0,   0.0,   1.0,   0.0,
        m.m20, m.m21, m.m22, 1.0
    } )

    local cx = _self.center.x
    local cy = _self.center.y
    local half_width = _self.width/2
    local half_height = _self.height/2
    gl.VertexPointer( {
        { cx - half_width, cy + half_height },
        { cx + half_width, cy + half_height },
        { cx + half_width, cy - half_height },  
        { cx - half_width, cy - half_height }  
    } )
    gl.Color( 1.0, 1.0, 1.0, 1.0 )
    gl.DrawArrays( "LINE_LOOP", 0, 4 )
end
