Dynamics = {}
Dynamics.__index = Dynamics

local stream = require("stream")
local console = require("console")

function Dynamics:Read(address)
	local dynamics = { offset = address }
	local stream = stream.open(address)

	dynamics.f0 = stream:read32()
	dynamics.f1 = stream:read32()
	dynamics.f2 = stream:read32()
    dynamics.f3 = stream:read32()

    dynamics.type = 0
    if dynamics.f3 & 0x4 then dynamics.type = 2 end
    if (dynamics.f3 & 0x2) ~= 0 then dynamics.type = 1 end

    stream:jump(dynamics.offset + 0x54)
    --stream:jump(dynamics.offset + 0x3c)
    --console.log("cyan", "[Dynamics Speed Vector @ %X]", stream.position)


    -- dynamics.speed = {}
    -- -- Not real speed address? scale is correct
    -- memory.writefloat(stream.position, 2)
    -- dynamics.speed.x = stream:readfloat()
    -- dynamics.speed.z = stream:readfloat() -- x & y switched
    -- dynamics.speed.y = stream:readfloat()

    -- dynamics.scale = {}
    -- dynamics.scale.x = stream:readfloat()
    -- dynamics.scale.z = stream:readfloat() -- x & y switched
    -- dynamics.scale.y = stream:readfloat()

    stream:jump(dynamics.offset + 0x78)
    --console.log("cyan", "[Dynamics Matrix A @ %X]", stream.position)
    --dynamics.matrix0 = stream:readpointer()
    stream:read32()

    stream:advance(4 * 12)
    dynamics.position = {}
    dynamics.position.x = stream:readfloat()
    dynamics.position.z = stream:readfloat()
    dynamics.position.y = stream:readfloat()


    stream:jump(dynamics.offset + 0xD0)
    --console.log("cyan", "[Dynamics Matrix B @ %X]", stream.position)
    --dynamics.matrix1 = stream:readpointer()


	setmetatable(dynamics, Dynamics)
	return dynamics
end

return Dynamics
