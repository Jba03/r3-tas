AI = {}
AI.__index = AI

local stream = require("stream")
local console = require("console")

function AI:Read(address)
	local ai = { offset = address }
	local stream = stream.open(address)

    ai.model = stream:readpointer()
    ai.intelligence = {}
    ai.intelligence.normal = stream:readpointer()
    ai.intelligence.reflex = stream:readpointer()
    ai.dsg = stream:readpointer()
    ai.name = memory.readstring(stream:readpointer())

    console.log("yellow", "AI for %s @ %X", ai.name, address)

	setmetatable(ai, AI)
	return ai
end

return AI
