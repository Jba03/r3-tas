-----------------------------------------------------------------
-- Rayman 3 GCN (USA) Utility Script
-- See also https://github.com/byvar/raymap
-----------------------------------------------------------------

local SCRIPT_DIR = paths.get_script_directory()
package.path = package.path .. ";" .. SCRIPT_DIR .. "/lib/?.lua"
package.path = package.path .. ";" .. SCRIPT_DIR .. "/game/?.lua"

local r3 = require("r3")

function on_update()
	r3.update()
end

function on_video()
	r3.on_video()
end

function on_savestate(slot)
    r3.on_savestate(slot)
end

function on_loadstate(slot)
    r3.on_loadstate(slot)
end
