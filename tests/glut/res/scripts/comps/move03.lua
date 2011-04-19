-- ======================================================================================
-- File         : move03.lua
-- Author       : Wu Jie 
-- Last Change  : 03/22/2011 | 12:43:11 PM | Tuesday,March
-- Description  : 
-- ======================================================================================

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

local ex = ex
local super = require ( "comps.move" )
module ( ..., super.derive )

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

ref_trans = ex.trans2d.null

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function awake ( _self )
    -- _self.trans2d.local_angle = ex.angf(15.0)
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function start ( _self )
end


-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function update ( _self )
    -- _self.super.update(_self)
    -- local t = _self.trans2d.right * 0.1 * ex.time.dt
    -- _self.trans2d:move( t.x, t.y )
    -- _self.trans2d:move( 0.1 * ex.time.dt, 0.0, ex.space.self )
    _self.trans2d:move( ex.time.dt, 0.0, _self.ref_trans )
end
