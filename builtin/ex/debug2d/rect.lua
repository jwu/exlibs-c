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
    local trans2d = _self.trans2d
    local world_pos = trans2d.position
    local world_ang = trans2d.angle
    local world_scale = trans2d.scale

    local center = _self.center + world_pos
    local half_width = _self.width/2
    local half_height = _self.height/2
    local verts = {
        center - trans2d.right * half_width + trans2d.up * half_height, 
        center + trans2d.right * half_width + trans2d.up * half_height, 
        center + trans2d.right * half_width - trans2d.up * half_height, 
        center - trans2d.right * half_width - trans2d.up * half_height, 
    }
    ex.draw_poly ( verts, ex.color4f.white, true );

    super.on_render(_self)
end
