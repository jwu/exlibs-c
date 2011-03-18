-- ======================================================================================
-- File         : simple02.lua
-- Author       : Wu Jie 
-- Last Change  : 03/18/2011 | 11:13:26 AM | Friday,March
-- Description  : 
-- ======================================================================================

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

local ex = ex
require ( "ex.simple" )
module ( "ex.simple02", ex.simple.derive )

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

my_name = "unknown"

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function awake ( self )
    ex.log("awake: " .. self.my_name)
    self.trans2d.local_angle = ex.angf(15.0)
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function start ( self )
    ex.log("start: " .. self.my_name )
end


-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function update ( self )
    -- local pos = self.trans2d.local_position
    -- self.trans2d.local_position = ex.vec2f ( 
    --     pos.x + 0.1 * ex.time.dt,
    --     pos.y
    -- )
    local t = self.trans2d.right * 0.1 * ex.time.dt
    self.trans2d:move( t.x, t.y )
    -- self.trans2d:move( 0.1 * ex.time.dt, 0.0, ex.space.self )
    -- self.trans2d:move( 100.0 * ex.time.dt, 0.0, ex.space.world )
end
