Perso = {}
Perso.__index = Perso

local config = require("config")
local console = require("console")
local stream = require("stream")
local StdGame = require("standardgame")
local Dynamics = require("dynamics")
local Graph = require("graph")

function Perso:Read(address)
	local perso = { offset = address }
	local stream = stream.open(address)

	perso.data3d = stream:readpointer()
	perso.stdGame = stream:readpointer()
	perso.dynamics = stream:readpointer()
	stream:advance(4)
	perso.brain = stream:readpointer()
	perso.camera = stream:readpointer()
	perso.collisionSet = stream:readpointer()
    perso.msWay = stream:readpointer()

    --perso.waypoint.graph:Render()

	perso.light = stream:readpointer()
	stream:advance(4)
	perso.sectorInfo = stream:readpointer()


	-- Read standard game info
	perso.stdGame = StdGame:Read(perso.stdGame)
    perso.familyName = perso.stdGame:GetName(0)
    perso.modelName = perso.stdGame:GetName(1)
    perso.name = perso.stdGame:GetName(2)


    -- Read dynamics
    if perso.dynamics ~= 0x00 then
        if config.PRINT_INFO and perso.name then
            --console.log("red", "Perso \"%s\" dynamics @ %X", perso.name, perso.dynamics)
        end
        perso.dynamics = Dynamics:Read(memory.readpointer(perso.dynamics))
    end




    -- TODO: Check if used internally at all
    if perso.msWay ~= 0x00 then
        --Read waypoint microstructure
        local s = stream.open(perso.msWay)
        perso.waypoint = {}
        perso.waypoint.graph = s:readpointer()
        perso.waypoint.graph = Graph:Read(perso.waypoint.graph)
        perso.waypoint.graphIndex = s:read32()
        perso.waypoint.flag = s:read8()
    end

	setmetatable(perso, Perso)
    PersoList[perso.name] = perso

	return perso
end

return Perso
