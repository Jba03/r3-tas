IPO = {}
IPO.__index = IPO

local stream = require("stream")

function IPO:Read(address)
	local ipo = {}

	local stream = stream.open(address)
	ipo.offset = address
	ipo.data = stream:readpointer()
	ipo.radiosity = stream:readpointer()
	stream:advance(4)
	ipo.portalCamera = stream:readpointer()
	stream:advance(4 * 3)
	ipo.name = stream:readstring(0x32)

	--print(ipo.name)

	setmetatable(ipo, IPO)
	return ipo
end

return IPO
