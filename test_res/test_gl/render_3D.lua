-- ======================================================================================
-- File         : render_3D.lua
-- Author       : Wu Jie 
-- Last Change  : 02/09/2011 | 09:46:47 AM | Wednesday,February
-- Description  : 
-- ======================================================================================

--/////////////////////////////////////////////////////////////////////////////
-- function defines
--/////////////////////////////////////////////////////////////////////////////

-- function step()
--     local v = ex.range_rand(0.0,1.0)
--     local i = math.floor( v * #freq ) + 1
--     -- the i may be nil
--     if freq[i] then freq[i] = freq[i] + 1 end
-- end

function step()
    local i = cdf()
    assert( i ~= nil and i ~= -1, "invalid i" )

    freq[i] = freq[i] + 1
end

function calc_pdf()
    for i = 1,#freq do
        pdf[i] = freq[i]/ticks 
    end
end

function plot ( _x, _y, _data, _height )
    local line_length = 200.0
    local count = 20
    local step = line_length / count

    -- draw coord
    draw.line ( 
        { _x - 10.0,        _y, 10.0 }, -- start
        { _x + line_length, _y, 10.0 }, -- end
        { 1.0, 0.0, 0.0, 1.0 } -- color
    )
    draw.line ( 
        { _x, _y - 10.0,        10.0 }, -- start
        { _x, _y + line_length, 10.0 }, -- end
        { 0.0, 1.0, 0.0, 1.0 } -- color
    )
    draw.line ( 
        { _x,               _y + 100.0, 10.0 }, -- start
        { _x + line_length, _y + 100.0, 10.0 }, -- end
        { 1.0, 1.0, 0.0, 1.0 } -- color
    )

    for i=1,count do
        draw.line ( 
            { _x + i * step, _y,     10.0 }, -- start
            { _x + i * step, _y-2.0, 10.0 }, -- end
            { 1.0, 1.0, 1.0, 1.0 } -- color
        )
        draw.line ( 
            { _x,       _y + i * step, 10.0}, -- start
            { _x - 2.0, _y + i * step, 10.0}, -- end
            { 1.0, 1.0, 1.0, 1.0 } -- color
        )
    end

    local height = _height
    local width = 200.0 / #_data 
    for i=1,#_data do
        -- draw one rect
        draw.rect ( 
            "bot_left",
            { _x + (i-1)*width, _y, 1.0 }, -- pos
            { width, height * _data[i] }, -- size
            { 0.5, 0.5, 0.5, 1.0 }, --solid color
            { 1.0, 1.0, 1.0, 1.0 } -- line color
        )
    end
end

--/////////////////////////////////////////////////////////////////////////////
-- process
--/////////////////////////////////////////////////////////////////////////////

-- ======================================================== 
-- 
-- ======================================================== 

ticks = ticks or 0

pdf = pdf or { 0.1, 0.2, 0.3, 0.4, 0.5 }
cdf = cdf or ex.probability.gen_cdf(pdf) 
if freq == nil then 
    freq = {}
    for i=1,#pdf do 
        freq[i]=0.0 
    end
end

-- ======================================================== 
-- 
-- ======================================================== 

step()
calc_pdf()

ticks = ticks + 1
plot( 0.0, 0.0, pdf, 100.0 )
-- plot( -250.0, 0.0, cdf, 100.0 )

-- ======================================================== 
-- draws
-- ======================================================== 
