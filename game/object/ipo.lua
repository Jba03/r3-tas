IPO = {}
IPO.__index = IPO

local stream = require("stream")
local PhysicalObject = require("physicalobject")

function IPO:Read(address)
	local ipo = { offset = address }
	local stream = stream.open(address)

	ipo.data = stream:readpointer()
	ipo.radiosity = stream:readpointer()
	stream:advance(4)
	ipo.portalCamera = stream:readpointer()
	stream:advance(4 * 3)
	ipo.name = stream:readstring(0x32)

    ipo.data = PhysicalObject:Read(ipo.data)

	--gui:text(ipo.name)

    IPOList[ipo.name] = ipo

	setmetatable(ipo, IPO)
	return ipo
end

return IPO
