local stream = require("stream")
local console = require("console")
local config = require("config")

lvl = {}

function lvl.load(address)
    lvl.offset = address
	local stream = stream.open(address)

    console.log("pink", "Reading LVL @ %X", lvl.offset)

    stream:advance(4 * 4)
    lvl.timestamp = stream:readstring(0x18)
    stream:advance(0xf0)
    lvl.numTextures = stream:read32()

    if config.PRINT_INFO then
        console.log("grey", "Number of textures: %d", lvl.numTextures)
    end

    local totalTextureCount = lvl.numTextures - fix.numTextures
    stream:advance(totalTextureCount * 4) -- skip texture pointers
    stream:advance(totalTextureCount * 4) -- skip texture pointers

    -- Most of these pointers seem to be invalid
    lvl.actualWorld = stream:readpointer()
    lvl.dynamicWorld = stream:readpointer()
    lvl.inactiveDynamicWorld = stream:readpointer()
    lvl.fatherSector = stream:readpointer()
    lvl.submapPos = stream:readpointer()
    lvl.numAlways = stream:read32()
    stream:advance(4 * 3) -- TODO: read spawnable persos
    stream:advance(4 * 3) -- skip for now

    if config.PRINT_INFO then
        console.log("green", "Reading lvl object names @ %X", stream.position)
    end

    lvl.objectTypes = {}
    for i = 0, 2 do
        local nameFirst = stream:readpointer()
        local nameLast = stream:readpointer()
        local numNames = stream:read32()

        --print("Num names: " .. numNames)

        if numNames > 10000 then
            console.log("red", "Unreasonable number of names - invalid LVL!")
            return nil
        end

        lvl.objectTypes[i] = {}
        local current = nameFirst

        if config.PRINT_INFO then
            local group = {"Family", "Model", "Object"}
            console.log("grey", "\t[%s]", group[i+1])
        end

        local s = stream.open(current)
        for n = 0, numNames-1 do

            lvl.objectTypes[i][n] = {}
            lvl.objectTypes[i][n].namesNextOffset = s:readpointer()
            lvl.objectTypes[i][n].namesPrevOffset = s:readpointer()
            lvl.objectTypes[i][n].headerOffset = s:readpointer()
            lvl.objectTypes[i][n].nameOffset = s:readpointer()

            lvl.objectTypes[i][n].name = memory.readstring(lvl.objectTypes[i][n].nameOffset)
            --print("name: " .. lvl.objectTypes[i][n].name)

            if config.PRINT_INFO then
                local a = console.fmt("pink", "\t[lvl @ %X]: ", s.position)
        		local b = console.fmt("blue", "%s", lvl.objectTypes[i][n].name)
        		print(a .. b)
            end

            if lvl.objectTypes[i][n].namesNextOffset ~= 0x00 then
                s:jump(lvl.objectTypes[i][n].namesNextOffset)
            end
        end

        --print ""
    end

    console.log("yellow", "Finished reading LVL\n")
end
