-- ======================================================================================
-- File         : test_vec2f.lua
-- Author       : Wu Jie 
-- Last Change  : 02/09/2011 | 00:42:52 AM | Wednesday,February
-- Description  : 
-- ======================================================================================

--/////////////////////////////////////////////////////////////////////////////
-- 
--/////////////////////////////////////////////////////////////////////////////

require ("ex.debug")
local do_test = false
if not do_test then return end

--/////////////////////////////////////////////////////////////////////////////
-- unit tests
--/////////////////////////////////////////////////////////////////////////////

t = {
    x = 1, 
    y = 1, 
    z = 1, 
    foobar = {
        txt = "hello world",
        tt = {
            d = "yes"
        }
    },
    v = ex.vec2f.zero,
}
-- ex.debug.dump(t.v,"t.v");
-- ex.debug.dump(getmetatable(t.v),"t.v.meta");

tt = ex.deepcopy(t)

tt.x = 10.0
tt.y = 20.0
tt.z = 30.0
tt.foobar.txt = "hello foobr!!!"
tt.foobar.tt.d = "no"
tt.v.x = 10.0
ex.debug.dump(t,"t");
ex.debug.dump(tt,"tt");
