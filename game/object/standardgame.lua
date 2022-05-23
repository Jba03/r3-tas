StandardGame = {}
StandardGame.__index = StandardGame

local stream = require("stream")

function StandardGame:Read(address)
	local std = { offset = address }
	local stream = stream.open(address)

	std.objectTypes = {}
	std.objectTypes[0] = stream:read32()
	std.objectTypes[1] = stream:read32()
	std.objectTypes[2] = stream:read32()
	std.superobject = stream:readpointer()

	setmetatable(std, StandardGame)
	return std
end

function StandardGame:GetName(idx)
    if not lvl.invalid then
        if self.objectTypes[idx] >= 0 and self.objectTypes[idx] < #lvl.objectTypes[idx] then
            return lvl.objectTypes[idx][self.objectTypes[idx]].name
        end
    end

    return "Invalid name - LVL not loaded?"
end

return StandardGame
