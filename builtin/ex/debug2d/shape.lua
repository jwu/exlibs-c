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
    -- _self:invoke ( "_add_trail", 0.0, 0.05 )
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function update ( _self )
    _self:_add_trail ()
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function on_render ( _self )
    local world_pos = _self.trans2d.position
    local world_ang = _self.trans2d.angle

    gl.MatrixMode("MODELVIEW")
    gl.LoadIdentity()
    gl.Translate( world_pos.x, world_pos.y, 0.0 )
    gl.Rotate( world_ang.degrees, 0.0, 0.0, 1.0 );

    -- show coordinate
    if _self.show_coord then
        gl.Begin("LINES")
            gl.Scale( 1.0, 1.0, 1.0 )

            gl.Color( 1.0, 0.0, 0.0, 1.0 )
            gl.Vertex( 0.0,  0.0, 0.0 )
            gl.Vertex( 20.0, 0.0, 0.0 )

            gl.Color( 0.0, 1.0, 0.0, 1.0 )
            gl.Vertex( 0.0, 0.0,  0.0 )
            gl.Vertex( 0.0, 20.0, 0.0 )
        gl.End()
    end

    -- show parent link
    if _self.show_parentlink then
        local parent = _self.trans2d.parent
        if parent then
            local parent_world_pos = parent.position
            gl.PushMatrix()
            gl.MatrixMode("MODELVIEW")
            gl.LoadIdentity()
            gl.Begin("LINES")
                gl.Color( 1.0, 1.0, 0.0, 1.0 )
                gl.Vertex( world_pos.x, world_pos.y, 0.0 )
                gl.Vertex( parent_world_pos.x, parent_world_pos.y, 0.0 )
            gl.End()
            gl.PopMatrix()
        end
    end

    -- show trail
    if _self.show_trail then
        gl.MatrixMode( "MODELVIEW" )
        gl.LoadIdentity()
        gl.Begin("LINE_STRIP")
            gl.Color( 1.0, 0.5, 1.0, 1.0 )

            local cnt = 0
            local i = (_self._trail_idx+1)%MAX_TRAIL_VERTS
            while ( cnt < MAX_TRAIL_VERTS ) do
                local v2 = _self._trails[i]
                if v2 ~= nil then gl.Vertex( v2.x, v2.y, 0.0 ) end
                i = (i+1)%MAX_TRAIL_VERTS 
                cnt = cnt + 1
            end
        gl.End()
    end

    -- show text
    if _self.show_text then
        local wpos = _self.trans2d.position + ex.vec2f( -50.0, -15.0 )
        local y = wpos.y 
        ex.draw_text( wpos.x, y, 0.0, "pos: " .. _self.trans2d.position )
        y = y - 15
        ex.draw_text( wpos.x, y, 0.0, "ang: " .. _self.trans2d.angle )
        y = y - 15
        ex.draw_text( wpos.x, y, 0.0, "scale: " .. _self.trans2d.scale )
    end
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function _add_trail ( _self )
    _self._trail_idx = (_self._trail_idx + 1) % MAX_TRAIL_VERTS
    _self._trails[_self._trail_idx] = _self.trans2d.position
end
