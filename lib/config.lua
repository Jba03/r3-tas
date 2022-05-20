CONFIG = {}

-- Print info to console?
CONFIG.PRINT_INFO = true

CONFIG.POINTER = {}
CONFIG.POINTER.FIX = 0x004334cc
CONFIG.POINTER.LVL = 0x004334d0
CONFIG.POINTER.HIERARCHY1 = 0x004334d4
CONFIG.POINTER.HIERARCHY2 = 0x004334d8


-- For most levels:
-- 71DAA8: Dynamic world
-- 71DB28: Inactive dynamic world
-- FDD9E0: Father sector

-- Which structures to load
CONFIG.READ_FIX	= true
CONFIG.READ_LVL = true
CONFIG.READ_HIERARCHY = true

return CONFIG
