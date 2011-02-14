local function class_new ( _self, ... )
    local table = ...
    return setmetatable( table or {}, _self )
end
