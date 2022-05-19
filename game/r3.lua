local r3 = {}

local fix = require("fix")
local config = require("config")
local console = require("console")
local memory = require("memory")
local SuperObject = require("superobject")

-- Parse engine structure
local function r3_load()

	-- Read global structure pointers
	local FIX_ADDRESS = memory.readpointer(config.POINTER.FIX)
	local LVL_ADDRESS = memory.readpointer(config.POINTER.LVL)
	local HIERARCHY_ADDRESS = memory.readpointer(config.POINTER.HIERARCHY)

	-- -- Load structures from pointers
	-- if config.READ_FIX then fix.load(FIX_ADDRESS) end
	-- --if CONFIG.READ_LVL then lvl.load(LVL_ADDRESS) end
	-- if config.READ_HIERARCHY then
	-- 	-- Global superobject is at a 3x dword offset
	-- 	r3.hierarchy = SuperObject:Read(HIERARCHY_ADDRESS + 4 * 3)
	-- end

	local start = os.clock()
	SuperObject:Read(0x0071da28)
	local stop = os.clock()

	console.log("pink", "Parsed superobject hierarchy in %.5f seconds", stop - start)

	print("numPersos: " .. numPersos)


end

r3_load()

-- Determines whether the structure should be reparsed
-- in the case of a loadstate or level transition
local function shouldLoad()
	return false
end

-- Load, re-read structures, etc.
function r3.update()
	if shouldLoad() then
		r3_load()
	end
end

-- Draw GUI
function r3.on_video()

end

return r3
