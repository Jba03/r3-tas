AI = {}
AI.__index = AI

local stream = require("stream")
local console = require("console")

require("dsg")

function AI:Read(address)
	local ai = { offset = address }
	local stream = stream.open(address)

    ai.model = stream:readpointer()
    ai.intelligence = {}
    ai.intelligence.normal = stream:readpointer()
    ai.intelligence.reflex = stream:readpointer()
    ai.dsg = DSGMemory.Read(stream:readpointer())
    ai.name = memory.readstring(stream:readpointer())

	setmetatable(ai, AI)
	return ai
end

return AI
