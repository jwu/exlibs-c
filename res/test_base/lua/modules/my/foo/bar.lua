-- ======================================================================================
-- File         : bar.lua
-- Author       : Wu Jie 
-- Last Change  : 02/07/2011 | 01:08:45 AM | Monday,February
-- Description  : 
-- ======================================================================================

local ex = ex

-- module("my.foo.bar")
-- module("foobar")
module("ex")

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function my_foobar () 
    ex.log("hello my foobar.")
end

function foo () 
    ex.log("hello foo from my foobar.")
end

function bar () 
    ex.log("hello bar from my foobar.")
end

