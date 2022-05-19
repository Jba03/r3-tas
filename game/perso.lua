Perso = {}
Perso.__index = Perso

local stream = require("stream")
local StdGame = require("standardgame")

function Perso:Read(address)
	local perso = {}

	local stream = stream.open(address)
	perso.offset = address
	perso.data3d = stream:readpointer()
	perso.stdGame = stream:readpointer()
	perso.transform = stream:readpointer()
	stream:advance(4)
	perso.brain = stream:readpointer()
	perso.camera = stream:readpointer()
	perso.collisionSet = stream:readpointer()
	perso.waypoint = stream:readpointer()
	perso.light = stream:readpointer()
	stream:advance(4)
	perso.sectorInfo = stream:readpointer()
	--
	-- -- Read standard game info
	--perso.stdGame = StdGame:Read()


	setmetatable(perso, Perso)
	return perso
end

return Perso
