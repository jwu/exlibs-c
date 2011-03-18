-- ======================================================================================
-- File         : simple_update.lua
-- Author       : Wu Jie 
-- Last Change  : 03/01/2011 | 22:45:26 PM | Tuesday,March
-- Description  : 
-- ======================================================================================

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

local ex = ex
module( "ex.simple_update", ex.lua_behavior.derive )

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

my_name = "unknown"

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function awake ( self )
    ex.log("awake: " .. self.my_name)
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
    local ang = self.entity.trans2d.local_angle
    self.entity.trans2d.local_angle = ang + ex.angf(200.0) * ex.time.dt
end
