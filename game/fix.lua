fix = {}

local stream = require("stream")
local config = require("config")
local console = require("console")

function fix.load(address)
    fix.offset = address
	local stream = stream.open(address)

	console.log("pink", "Reading FIX @ %X", stream.position)

	stream:advance(4 * 4) -- padding
	--fix.timestamp = stream:readstring(0x18)
	stream:advance(4 * 4) -- padding

	fix.identityMatrix = stream:readpointer()
	fix.localizationStructure = stream:readpointer()
	fix.numLevelNames = stream:read32()
	fix.numEntries = stream:read32()


    if config.PRINT_INFO then
        console.log("section", "Reading fix entries [%d]", fix.numEntries * 2)
    end

	-- List entries
	for i = 0, fix.numEntries - 1 do
		local s = stream:readstring(0xC);
        if config.PRINT_INFO then
            local a = console.fmt("address", "\t[fix @ %X]: ", stream.position - 0xC)
		    local b = console.fmt("value", "%s", s)
		    print(a .. b)
        end
	end

    if config.PRINT_INFO then print("") end

	for i = 0, fix.numEntries - 1 do
		local s = stream:readstring(0xC);
        if config.PRINT_INFO then
            local a = console.fmt("address", "\t[fix @ %X]: ", stream.position - 0xC)
		    local b = console.fmt("value", "%s", s)
		    print(a .. b)
        end
	end

    if config.PRINT_INFO then
        console.log("section", "\nReading fix level names [%d]", fix.numLevelNames)
    end

	-- List level names
	for i = 0, fix.numLevelNames - 1 do
		local s = stream:readstring(0x1E)
        if config.PRINT_INFO then
            local a = console.fmt("address", "\t[fix @ %X]: ", stream.position)
		    local b = console.fmt("value", "%s", s)
		    print(a .. b)
        end
	end

    fix.firstMapName = stream:readstring(0x1E)
    stream:advance(2) -- align

    fix.numLanguages = stream:read32()
    fix.languageOffset = stream:readpointer()
    fix.numTextures = stream:read32()

    if config.PRINT_INFO then
        console.log("grey", "Number of languages: %d", fix.numLanguages)
        console.log("grey", "Number of textures: %d", fix.numTextures)
    end

    console.log("yellow", "Finished reading FIX\n")
end
