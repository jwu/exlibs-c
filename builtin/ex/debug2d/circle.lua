-- ======================================================================================
-- File         : circle.lua
-- Author       : Wu Jie 
-- Last Change  : 05/03/2011 | 18:39:01 PM | Tuesday,May
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
r = 10.0

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

    local trans2d = _self.trans2d
    local world_pos = trans2d.position
    -- local world_ang = trans2d.angle
    -- local world_scale = trans2d.scale
    local center = _self.center + world_pos

    ex.draw_circle ( center, r, r, 32, ex.color4f.white, true );
end
