CONFIG = {}

-- Three most important addresses are found @ 004338b4
CONFIG.POINTER = {}
CONFIG.POINTER.FIX = 0x004338b4
CONFIG.POINTER.LVL = 0x004338b8
CONFIG.POINTER.HIERARCHY = 0x004338bc
CONFIG.POINTER.MAIN_MENU_HIERARCHY = 0x004338c0

-- For most levels:
-- 71DAA8: Dynamic world
-- 71DB28: Inactive dynamic world
-- FDD9E0: Father sector

-- Which structures to load
CONFIG.READ_FIX	= true
CONFIG.READ_LVL = false
CONFIG.READ_HIERARCHY = true

return CONFIG
