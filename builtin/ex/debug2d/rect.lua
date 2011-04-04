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

function awake ( self )
    super.awake(self)
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function start ( self )
    super.start(self)
end


-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function update ( self )
    super.update(self)
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function on_render ( self )
    super.on_render(self)

    gl.MatrixMode( "MODELVIEW" )
    gl.LoadIdentity()

    -- DISABLE: un-efficient way { 
    -- local world_pos = self.trans2d.position
    -- local world_ang = self.trans2d.angle
    -- local world_scale = self.trans2d.scale
    -- gl.Translate( world_pos.x, world_pos.y, 0.0 )
    -- gl.Rotate( world_ang.degrees, 0.0, 0.0, 1.0 )
    -- gl.Scale( world_scale.x, world_scale.y, 1.0 )
    -- } DISABLE end 
    local m = self.trans2d.local_to_world 
    gl.LoadMatrix( {
        m.m00, m.m01, m.m02, 0.0,
        m.m10, m.m11, m.m12, 0.0,
        0.0,   0.0,   1.0,   0.0,
        m.m20, m.m21, m.m22, 1.0
    } )

    local cx = self.center.x
    local cy = self.center.y
    local half_width = self.width/2
    local half_height = self.height/2
    gl.VertexPointer( {
        { cx - half_width, cy + half_height },
        { cx + half_width, cy + half_height },
        { cx + half_width, cy - half_height },  
        { cx - half_width, cy - half_height }  
    } )
    gl.Color( 1.0, 1.0, 1.0, 1.0 )
    gl.DrawArrays( "LINE_LOOP", 0, 4 )
end
