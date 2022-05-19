local fix = {}

local stream = require("stream")
local console = require("console")

function fix.load()
	local stream = stream.open(0x00aee1b8)

	console.log("structure", "Reading fix.lvl @ 0x%X\n", stream.position)

	stream:advance(4 * 4) -- padding
	fix.timestamp = stream:readstring()
	stream:advance(0x18) -- padding

	fix.identityMatrix = stream:readpointer()
	fix.localizationStructure = stream:readpointer()
	fix.numLevelNames = stream:read32()
	fix.numEntries = stream:read32()

	console.log("section", "Reading fix entries")

	-- List entries
	for i = 0, fix.numEntries - 1 do
		local a = console.fmt("address", "\t[fix @ %X]: ", stream.position)
		local s = stream:readstring(0xC);
		local b = console.fmt("value", "%s", s)
		print(a .. b)
	end

	print("")

	for i = 0, fix.numEntries - 1 do
		local a = console.fmt("address", "\t[fix @ %X]: ", stream.position)
		local s = stream:readstring(0xC);
		local b = console.fmt("value", "%s", s)
		print(a .. b)
	end

	console.log("section", "\nReading fix level names")

	-- List level names
	for i = 0, fix.numLevelNames - 1 do
		local s = stream:readstring(0x1E)
		local a = console.fmt("address", "\t[fix @ %X]: ", stream.position)
		local b = console.fmt("value", "%s", s)
		print(a .. b)
	end
end

return fix
