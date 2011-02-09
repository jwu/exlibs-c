-- ======================================================================================
-- File         : render_3D.lua
-- Author       : Wu Jie 
-- Last Change  : 02/09/2011 | 09:46:47 AM | Wednesday,February
-- Description  : 
-- ======================================================================================

-- draw coord
draw.line ( 
    {-10.0,  0.0, 10.0}, -- start
    { 200.0, 0.0, 10.0}, -- end
    { 1.0, 0.0, 0.0, 1.0 } -- color
)
draw.line ( 
    { 0.0, -10.0,  10.0}, -- start
    { 0.0,  200.0, 10.0}, -- end
    { 0.0, 1.0, 0.0, 1.0 } -- color
)

for i=1,20 do
    draw.line ( 
        { i * 10.0,  0.0, -1.0}, -- start
        { i * 10.0, -2.0, -1.0}, -- end
        { 1.0, 1.0, 1.0, 1.0 } -- color
    )

    draw.line ( 
        {  0.0, i * 10.0, -1.0}, -- start
        { -2.0, i * 10.0, -1.0}, -- end
        { 1.0, 1.0, 1.0, 1.0 } -- color
    )
end

-- draw one rect
draw.rect ( 
    "bot_left",
    {0.0, 0.0, 1.0}, -- pos
    {10.0, 20.0}, -- size
    {1.0, 0.0, 1.0, 1.0}, --solid color
    {1.0, 1.0, 0.0, 1.0} -- line color
)

-- draw one rect
draw.rect ( 
    "bot_left",
    {10.0, 0.0, 1.0}, -- pos
    {10.0, 40.0}, -- size
    {0.5, 0.5, 0.5, 1.0}, --solid color
    {1.0, 1.0, 0.0, 1.0} -- line color
)

-- draw one rect
draw.rect ( 
    "bot_left",
    {20.0, 0.0, 1.0}, -- pos
    {10.0, 80.0}, -- size
    {0.5, 0.5, 1.0, 1.0}, --solid color
    {1.0, 1.0, 0.0, 1.0} -- line color
)
