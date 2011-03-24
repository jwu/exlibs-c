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
module( ..., ex.lua_behavior.derive )

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

function awake ( self )
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function start ( self )
    self:invoke ( "_add_trail", 0.0, 0.5 )
end


-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function update ( self )
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function on_render ( self )
    local world_pos = self.trans2d.position
    local world_ang = self.trans2d.angle

    gl.MatrixMode("MODELVIEW")
    gl.LoadIdentity()
    gl.Translate( world_pos.x, world_pos.y, 0.0 )
    gl.Rotate( world_ang.degrees, 0.0, 0.0, 1.0 );

    -- show coordinate
    if self.show_coord then
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
    if self.show_parentlink then
        local parent = self.trans2d.parent
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
    if self.show_trail then
        gl.MatrixMode( "MODELVIEW" )
        gl.LoadIdentity()
        gl.Begin("LINE_STRIP")
            gl.Color( 1.0, 0.5, 1.0, 1.0 )

            local cnt = 0
            local i = (self._trail_idx+1)%MAX_TRAIL_VERTS
            while ( cnt < MAX_TRAIL_VERTS ) do
                local v2 = self._trails[i]
                if v2 ~= nil then gl.Vertex( v2.x, v2.y, 0.0 ) end
                i = (i+1)%MAX_TRAIL_VERTS 
                cnt = cnt + 1
            end
        gl.End()
    end

    -- show text
    if self.show_text then
        local wpos = self.trans2d.position + ex.vec2f( -50.0, -15.0 )
        local y = wpos.y 
        ex.draw_text( wpos.x, y, 0.0, "pos: " .. self.trans2d.position )
        y = y - 15
        ex.draw_text( wpos.x, y, 0.0, "ang: " .. self.trans2d.angle )
        y = y - 15
        ex.draw_text( wpos.x, y, 0.0, "scale: " .. self.trans2d.scale )
    end
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function _add_trail ( self )
    self._trail_idx = (self._trail_idx + 1) % MAX_TRAIL_VERTS
    self._trails[self._trail_idx] = self.trans2d.position
end
