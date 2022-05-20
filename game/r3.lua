local r3 = {}

local config = require("config")
local memory = require("memory")
local stream = require("stream")
local console = require("console")

local SuperObject = require("superobject")

require("fix")
require("lvl")

PersoList = {}
IPOList = {}

local FIX_ADDRESS = 0
local LVL_ADDRESS = 0
local HIERARCHY1_ADDRESS = 0
local HIERARCHY2_ADDRESS = 0

transitioing = false
transitionFrame = 0

loadInterval = 0
frame = 0

-- Parse engine structure
local function r3_load()

    -- Clear persos and IPO:s
    for k, v in pairs(PersoList) do PersoList[k] = nil end
    for k, v in pairs(IPOList) do IPOList[k] = nil end

    -- The game seems to alternate between two hierarchy pointers that are
    -- switched when loading a new level. Choose the currently valid pointer.
    local hierarchy = math.max(HIERARCHY1_ADDRESS, HIERARCHY2_ADDRESS)
    local hierarchyEntryPoint = memory.readpointer(hierarchy + 4 * 5)

    level = 0

    if hierarchyEntryPoint == 0x00 then
        -- Found no valid entry point into lvl or hierarchy
        console.log("red", "Found no hierarchy entry point")
    else
        fix.load(FIX_ADDRESS)
        lvl.load(LVL_ADDRESS)

        -- Read the superobject hierarchy
        if config.PRINT_INFO then
            console.log("red", "Superobject hierarchy: %X", hierarchyEntryPoint)
        end

    	local start = os.clock()
    	SuperObject:Read(hierarchyEntryPoint)
    	local stop = os.clock()

        if config.PRINT_INFO then
            console.log("pink", "Parsed superobject hierarchy in %.5f seconds", stop - start)
        end
    end
end

-- Determines whether the structure should be reparsed
-- in the case of a loadstate or level transition
local function shouldLoad()

    -- Read a sample address to check if the level has finished loading
    local finishedLoading = true --= memory.read32(0x003e7bf9) ~= 0
    --local finishedLoading = memory.read32(0x003e7c6c) ~= 0 -- better?
    local finishedLoading = memory.read32(0x003e7bfd) ~= 0

    if not finishedLoading then
        console.log("pink", "Transition frame: %d", transitionFrame)
        transitionFrame = transitionFrame + 1
    end

    if transitioning and finishedLoading and LVL_ADDRESS ~= 0 and (HIERARCHY1_ADDRESS ~= 0 or HIERARCHY2_ADDRESS ~= 0) then
        transitioning = false
        return true
    end

    if LVL_ADDRESS == 0 and HIERARCHY1_ADDRESS == 0 and HIERARCHY2_ADDRESS == 0 then
        -- Game in transition
        transitioning = true
        return false
    end

	return false
end

-- Load, re-read structures, etc.
function r3.update()

    -- Read global structure pointers
	FIX_ADDRESS = memory.readpointer(config.POINTER.FIX)
	LVL_ADDRESS = memory.readpointer(config.POINTER.LVL)
	HIERARCHY1_ADDRESS = memory.readpointer(config.POINTER.HIERARCHY1)
    HIERARCHY2_ADDRESS = memory.readpointer(config.POINTER.HIERARCHY2)

	if shouldLoad() then
        numPersos = 0
		r3_load()
        print("")
	end

    -- Testing drawing of boxes at perso positions
    --ira:begin()
    -- CameraPositionX = memory.readfloat(0x00c531bc + 4 * 0)
	-- CameraPositionY = memory.readfloat(0x00c531bc + 4 * 2)
	-- CameraPositionZ = memory.readfloat(0x00c531bc + 4 * 1)
    -- CameraLookX = memory.readfloat(0x00c53910 + 0 * 4)
	-- CameraLookY = memory.readfloat(0x00c53910 + 2 * 4)
	-- CameraLookZ = memory.readfloat(0x00c53910 + 1 * 4)
    --
    -- ira:camera(CameraPositionX, CameraPositionY-1, CameraPositionZ, 0, 0, 0)
    -- ira:look_at(CameraLookX, CameraLookY-1, CameraLookZ)
    -- ira:fov(60.0)
    --
    -- for k, v in pairs(PersoList) do
    --     if type(v.dynamics) == "table" then
    --         -- gui:window(k)
    --         -- gui:text("X: " .. v.dynamics.position.x)
    --         -- gui:text("Y: " .. v.dynamics.position.y)
    --         -- gui:text("Z: " .. v.dynamics.position.z)
    --
    --         ira:scale(0.5, 0.5, 0.5)
	--         ira:translate(v.dynamics.position.x, v.dynamics.position.y, v.dynamics.position.z)
	--         ira:box(0, 0, 0, 0xAAFFFFFF)
	--         ira:reset()
    --     end
    -- end

    --Re-read persos each frame
    config.PRINT_INFO = false
    for k, v in pairs(PersoList) do
        PersoList[k] = Perso:Read(v.offset)
    end
    config.PRINT_INFO = true



    Rayman = PersoList["Rayman"]
    if Rayman then
        gui:window("Rayman")
        gui:text("X: " .. Rayman.dynamics.position.x)
        gui:text("Y: " .. Rayman.dynamics.position.y)
        gui:text("Z: " .. Rayman.dynamics.position.z)
        --gui:text("Hspeed: " .. math.sqrt(Rayman.dynamics.speed.x ^ 2 + Rayman.dynamics.speed.y ^ 2))
    end

    frame = frame + 1
end

-- Draw GUI
function r3.on_video()
    gui:window("Level")
    if gui:button("Reload hierarchy") then
        r3_load()
    end
end

r3.update()

return r3
