-- ======================================================================================
-- File         : draw.lua
-- Author       : Wu Jie 
-- Last Change  : 02/09/2011 | 10:01:46 AM | Wednesday,February
-- Description  : 
-- ======================================================================================

local ex = ex
local gl = gl
module (...)

-- ------------------------------------------------------------------ 
-- Desc: line
-- ------------------------------------------------------------------ 

function line ( _start, _end, _color )
    local color = _color or { 1.0, 1.0, 1.0, 1.0 }

    gl.Begin("LINES")
        gl.Color(color)
        gl.Vertex(_start) -- start
        gl.Vertex(_end) -- end
    gl.End()
end

-- ------------------------------------------------------------------ 
-- Desc: rect 
-- ------------------------------------------------------------------ 

function rect ( _anchor, _pos, _size, _solidColor, _lineColor )
    local anchor = _anchor or "center"
    local pos = _pos or { 0.0, 0.0, 0.0 }
    local size = _size or { 10.0, 10.0 }
    local solidColor = _solidColor or { 0.5, 0.5, 0.5, 1.0 }
    local lineColor = _lineColor or { 1.0, 1.0, 1.0, 1.0 }

    if anchor == "center" then
        verts = {
            { pos[1]-size[1]/2, pos[2]+size[2]/2, (#pos>=3) and pos[3] or 0.0 }, 
            { pos[1]+size[1]/2, pos[2]+size[2]/2, (#pos>=3) and pos[3] or 0.0 }, 
            { pos[1]+size[1]/2, pos[2]-size[2]/2, (#pos>=3) and pos[3] or 0.0 }, 
            { pos[1]-size[1]/2, pos[2]-size[2]/2, (#pos>=3) and pos[3] or 0.0 }, 
        }
    elseif anchor == "bot_center" then
        verts = {
            { pos[1]-size[1]/2, pos[2]+size[2], (#pos>=3) and pos[3] or 0.0 }, 
            { pos[1]+size[1]/2, pos[2]+size[2], (#pos>=3) and pos[3] or 0.0 }, 
            { pos[1]+size[1]/2, pos[2],         (#pos>=3) and pos[3] or 0.0 }, 
            { pos[1]-size[1]/2, pos[2],         (#pos>=3) and pos[3] or 0.0 }, 
        }
    elseif anchor == "bot_left" then
        verts = {
            { pos[1],         pos[2]+size[2], (#pos>=3) and pos[3] or 0.0 }, 
            { pos[1]+size[1], pos[2]+size[2], (#pos>=3) and pos[3] or 0.0 }, 
            { pos[1]+size[1], pos[2],         (#pos>=3) and pos[3] or 0.0 }, 
            { pos[1],         pos[2],         (#pos>=3) and pos[3] or 0.0 }, 
        }
    end

    gl.Begin("QUADS")
        gl.Color( solidColor )
        gl.Vertex( verts[1] ) -- Top Left
        gl.Vertex( verts[2] ) -- Top Right
        gl.Vertex( verts[3] ) -- Bottom Right
        gl.Vertex( verts[4] )  
    gl.End()
    gl.Begin("LINE_LOOP")
        gl.Color(lineColor)
        gl.Vertex( verts[1] ) -- Top Left
        gl.Vertex( verts[2] ) -- Top Right
        gl.Vertex( verts[3] ) -- Bottom Right
        gl.Vertex( verts[4] )  
    gl.End()
end
