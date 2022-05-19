StandardGame = {}
StandardGame.__index = StandardGame

local stream = require("stream")

function StandardGame:Read(address)
	local std = {}

	local stream = stream.open(address)

	std.objectTypes = {}
	std.objectTypes[1] = stream:read32()
	std.objectTypes[2] = stream:read32()
	std.objectTypes[3] = stream:read32()

	std.superobject = stream:readpointer()

	setmetatable(std, StandardGame)
	return std
end

return StandardGame
