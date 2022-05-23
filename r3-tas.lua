-----------------------------------------------------------------
-- Rayman 3 GCN (USA) Utility Script
-- See also https://github.com/byvar/raymap
-----------------------------------------------------------------

local SCRIPT_DIR = paths.get_script_directory()

local function add_directory(dir)
    package.path = string.format("%s;%s/%s/?.lua", package.path, SCRIPT_DIR, dir)
end

add_directory("lib")
add_directory("game")
add_directory("game/ai")
add_directory("game/collision")
add_directory("game/graph")
add_directory("game/object")
add_directory("game/types")

local r3 = require("r3")

_G.on_update = r3.update
_G.on_video = r3.on_video
_G.on_savestate = r3.on_savestate
_G.on_loadstate = r3.on_loadstate
