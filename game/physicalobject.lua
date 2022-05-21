PhysicalObject = {}
PhysicalObject.__index = PhysicalObject

local stream = require("stream")
local CollisionGeometry = require("collision")

function PhysicalObject:Read(address)
	local obj = { offset = address }
	local stream = stream.open(address)

    obj.visual = stream:readpointer()
    obj.collide = stream:readpointer()
    obj.visualBoundingVolume = stream:readpointer()
    obj.collideBoundingVolume = stream:readpointer()

    -- Parse collide set
    stream = stream.open(obj.collide)
    stream:advance(4 * 3) -- skip
    local geometryOffset = stream:readpointer()
    if geometryOffset ~= 0x00 then
        obj.collide = CollisionGeometry:Read(geometryOffset)
    end

	setmetatable(obj, PhysicalObject)
	return obj
end

return PhysicalObject
