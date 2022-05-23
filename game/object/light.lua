Light = {}
Light.__index = Light

local stream = require("stream")
local console = require("console")

function Light:Read(address)
	local light = { offset = address }
	local stream = stream.open(address)

    stream:readpointer() -- ???
    light.dataOffset = stream:readpointer()

    -- TODO: Check what this is
    light.xyz = {}
    light.xyz.x = stream:readfloat()
    light.xyz.z = stream:readfloat() -- z & y switched
    light.xyz.y = stream:readfloat()

    stream:jump(light.dataOffset)
    stream:read32() -- skip parent perso

	setmetatable(light, Light)
	return light
end

function Light:Render(address)
    ira:scale(0.25,0.25,0.25)
    ira:translate(self.xyz.x, self.xyz.y, self.xyz.z)
    ira:box(0,0,0, 0xFF00FFFF)
    ira:reset()
end

return Light
