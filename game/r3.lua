local r3 = {}

local config = require("config")
local memory = require("memory")
local stream = require("stream")
local console = require("console")

local SuperObject = require("superobject")

require("fix")
require("lvl")

PersoList = {}
PersoColorList = {}
IPOList = {}

local menuOpen = false

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

    for i=0,400 do
        PersoColorList[i] = math.random(0x80303030, 0xFFFFFFFF)
    end

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
            console.log("pink", "Reading superobject hierarchy @ %X", hierarchyEntryPoint)
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

addrTable = {
    2159177220,
    2159177272,
    2159177324,
    2159177376,
    2165906048,
    2165906608,
    29,
    2165906904,
    2165908224,
    67,
    2165909476,
    2165913296,
    192,
    2164258192,
    0,
    0,
    0,
    156725103,
    1683960113,
    0,
    0,
    0,
    0,
    0,
    87,
    1869571167,
    825294848,
    0,
    0,
    0,
    0,
    0,
    7169390,
    1970102640,
    0,
    0,
    0,
    0,
    0,
    0,
    4278190080,
    4294967295,
    25883,
    0,
    431394,
    17,
    1,
    0,
    0,
    1,
    2,
    2,
    4,
    3,
    2,
    7,
    2,
    10,
    10,
    12,
    10,
    7,
    17,
    418365,
    1015580809,
    291474861,
    1,
    4058751590,
    0,
    40500,
    16842752,
    2160004108,
    0,
    0,
    0,
    1,
    196613,
    458761,
    448,
    640,
    448,
    640,
    0,
    448,
    0,
    640,
    0,
    447,
    0,
    639,
    100,
    100,
    0,
    448,
    0,
    640,
    0,
    1000,
    0,
    1000,
    0,

}

-- Load, re-read structures, etc.
function r3.update()

    --console.log("red", "\n\n\nAAAAAAA")
    -- for i=0,100 do
    --     print(memory.read32(0x003e7bc8 + i * 4) .. ",")
    -- end

    -- for i=0,100 do
    --     memory.write32(0x003e7bc8 + i * 4, addrTable[i+1])
    -- end

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

    --Re-read persos each frame
    config.PRINT_INFO = false
    for k, v in pairs(PersoList) do
        PersoList[k] = Perso:Read(v.offset)
    end
    config.PRINT_INFO = true


    frame = frame + 1
end

-- Draw GUI
function r3.on_video()


    ira:begin() -- Prepare IRA
    CameraPositionX = memory.readfloat(0x00c531bc + 4 * 0)
	CameraPositionY = memory.readfloat(0x00c531bc + 4 * 2)
	CameraPositionZ = memory.readfloat(0x00c531bc + 4 * 1)
    CameraLookX = memory.readfloat(0x00c53910 + 0 * 4)
	CameraLookY = memory.readfloat(0x00c53910 + 2 * 4)
	CameraLookZ = memory.readfloat(0x00c53910 + 1 * 4)

    ira:camera(CameraPositionX, CameraPositionY-1, CameraPositionZ, 0, 0, 0)
    ira:look_at(CameraLookX, CameraLookY-1, CameraLookZ)
    ira:fov(60.0)

    -- Draw menu
    -- if menuOpen then
    --     gui:window("Menu window", 0, 0, 1 << 9 | 1 << 0 | 1 << 10)
    --     gui:window_end()
    -- else
    --     -- Draw menu button
    --     gui:window("Menu", 0, 0, 0xFF)
    --     if gui:button("Menu") then
    --         menuOpen = not menuOpen
    --     end
    --     gui:window_end()
    -- end

    gui:window("Level")
    if gui:button("Reload hierarchy") then
        r3_load()
    end
    gui:window_end()



    Rayman = PersoList["Rayman"]
    if Rayman then
        if type(Rayman.dynamics) == "table" then
            gui:window("Rayman")

            gui:text("X: " .. Rayman.dynamics.position.x)
            gui:text("Y: " .. Rayman.dynamics.position.y)
            gui:text("Z: " .. Rayman.dynamics.position.z)
            gui:text("")

            gui:text("Hspeed: " .. math.sqrt(Rayman.dynamics.speed.x ^ 2 + Rayman.dynamics.speed.z ^ 2))
            gui:text("Vspeed: " .. Rayman.dynamics.speed.y)
            gui:text(string.format("(%.2f, %.2f, %.2f)", Rayman.dynamics.speed.x, Rayman.dynamics.speed.y, Rayman.dynamics.speed.z))

            gui:window_end()
        end

        World = PersoList["global"]
        gui:window("World DSG variables")
        local dsg = World.ai.dsg
        for i = 0, dsg.memory.length - 1 do
            local value = dsg.memory.current.values[i].data
            local offset = dsg.memory.current.values[i].offset
            if type(value) == "table" then
                gui:text(string.format("%X: (%.2f, %.2f, %.2f)", offset, value.x, value.y, value.z))
            else
                gui:text(string.format("%X: %s", offset, tostring(value)))
            end
        end
        --console.log("pink", "rayman dsg current @ %x", dsg.memory.current.offset)
        gui:window_end()
    end

    if gui:window("Persos") then
        local drawPosition = gui:checkbox("Draw positions?")
        local drawSpeedVector = gui:checkbox("Draw speed vectors?")
        local drawLights = gui:checkbox("Draw light nodes?")

        gui:text("")
        gui:text(string.format("Currently loaded (%d)", numPersos))
        gui:separator()

        numPersos = 0

        local pcc = 0
        for k, v in pairs(PersoList) do
            gui:text(v.name)

            if type(v.dynamics) == "table" then

                if drawPosition then
                    ira:scale(0.5, 0.5, 0.5)
        	        ira:translate(v.dynamics.position.x, v.dynamics.position.y, v.dynamics.position.z)
        	        ira:box(0, 0, 0, PersoColorList[pcc])
        	        ira:reset()
                end

                if drawSpeedVector then
                    -- X
                    ira:scale(v.dynamics.speed.x * 0.25, 0.1, 0.1)
        	        ira:translate(v.dynamics.position.x + (v.dynamics.speed.x * 0.25)/2, v.dynamics.position.y, v.dynamics.position.z)
        	        ira:box(0, 0, 0, 0xFF0000FF)
        	        ira:reset()
                    -- Y
                    ira:scale(0.1, v.dynamics.speed.y * 0.25, 0.1)
        	        ira:translate(v.dynamics.position.x, v.dynamics.position.y + (v.dynamics.speed.y * 0.25)/2, v.dynamics.position.z)
        	        ira:box(0, 0, 0, 0xFF00FF00)
        	        ira:reset()
                    -- Z
                    ira:scale(0.1, 0.1, v.dynamics.speed.z * 0.25)
        	        ira:translate(v.dynamics.position.x, v.dynamics.position.y, v.dynamics.position.z + (v.dynamics.speed.z * 0.25)/2)
        	        ira:box(0, 0, 0, 0xFFFF0000)
        	        ira:reset()
                end
            end

            if drawLights then
                if type(v.light) == "table" then
                    v.light:Render()
                end
            end

            numPersos = numPersos + 1
            pcc = pcc + 1
        end
    end
    gui:window_end()


    local ccc = 0
    for k, v in pairs(IPOList) do
        if type(v.data) == "table" then
            if type(v.data.collide) == "table" and (ccc % 1) == 0 then
                v.data.collide:Draw()
            end
        end
        ccc = ccc + 1
    end
end

-- Called when state saved
function r3.on_savestate(slot)
    console.log("green", "State %d saved", slot)
end

function r3.on_loadstate(slot)
    console.log("green", "State %d loaded", slot)
    r3_load()
    -- TODO: Figure out a way to draw same frame state is loaded
    -- although this is a problem in Dolphin.
    r3.on_video()
end

r3.update()

return r3
