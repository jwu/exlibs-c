-- ======================================================================================
-- File         : shape.lua
-- Author       : Wu Jie 
-- Last Change  : 03/19/2011 | 21:04:02 PM | Saturday,March
-- Description  : 
-- ======================================================================================

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

local ex = ex
local gl = gl
module( "ex.debug.shape", ex.lua_behavior.derive )

--/////////////////////////////////////////////////////////////////////////////
-- properties
--/////////////////////////////////////////////////////////////////////////////

show_coord = true
show_parentlink = true
show_trail = true

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
    if show_coord then
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
    if show_parentlink then
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
end

