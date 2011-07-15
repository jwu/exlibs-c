-- ======================================================================================
-- File         : shape.lua
-- Author       : Wu Jie 
-- Last Change  : 03/20/2011 | 00:37:27 AM | Sunday,March
-- Description  : 
-- ======================================================================================

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

local ex = ex
local gl = gl

local super = ex.lua_behavior
module( ..., super.derive )

-- static
local MAX_TRAIL_VERTS = 16

-- private
_trail_idx = 0
_trails = {}

--/////////////////////////////////////////////////////////////////////////////
-- properties
--/////////////////////////////////////////////////////////////////////////////

show_coord = true
show_parentlink = true
show_trail = true
show_text = true

texture = ex.texture2d.null

--/////////////////////////////////////////////////////////////////////////////
-- functions
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
    _self:invoke ( "_add_trail", 0.0, 0.02 )
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function update ( _self )
    -- _self:_add_trail ()
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function on_render ( _self )
    local trans2d = _self.trans2d
    local world_pos = trans2d.position
    -- local world_ang = trans2d.angle

    -- show coordinate
    if _self.show_coord then
        ex.draw_line ( world_pos, world_pos + trans2d.right * 20.0, ex.color4f.red );
        ex.draw_line ( world_pos, world_pos + trans2d.up * 20.0, ex.color4f.green );
    end

    -- -- show parent link
    if _self.show_parentlink then
        local parent = _self.trans2d.parent
        if parent then
            local parent_world_pos = parent.position
            ex.draw_line ( world_pos, parent_world_pos, ex.color4f.yellow );
        end
    end

    -- show trail
    if _self.show_trail then

        local verts = {}
        local cnt = 0
        local i = (_self._trail_idx+1)%MAX_TRAIL_VERTS
        while ( cnt < MAX_TRAIL_VERTS ) do
            local v2 = _self._trails[i]
            if v2 ~= nil then verts[#verts+1] = v2 end
            i = (i+1)%MAX_TRAIL_VERTS 
            cnt = cnt + 1
        end
        ex.draw_poly ( verts, ex.color4f( 1.0, 0.5, 1.0, 1.0 ), false )
    end

    -- -- show text
    -- if _self.show_text then
    --     local wpos = _self.trans2d.position + ex.vec2f( -50.0, -15.0 )
    --     local y = wpos.y 
    --     ex.draw_text( wpos.x, y, 0.0, "pos: " .. _self.trans2d.position )
    --     y = y - 15
    --     ex.draw_text( wpos.x, y, 0.0, "ang: " .. _self.trans2d.angle )
    --     y = y - 15
    --     ex.draw_text( wpos.x, y, 0.0, "scale: " .. _self.trans2d.scale )
    -- end
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function _add_trail ( _self )
    _self._trail_idx = (_self._trail_idx + 1) % MAX_TRAIL_VERTS
    _self._trails[_self._trail_idx] = _self.trans2d.position
end
