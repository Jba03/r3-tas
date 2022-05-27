local stream = require("stream")
local console = require("console")
local Graph = require("graph")

local DSGVariableType = {
    [0]  = {name = "None",              read = function(s) return nil end },
    [1]  = {name = "Boolean",           read = function(s) return not s:read32() == 0 end },
    [2]  = {name = "Byte",              read = function(s) return s:read8() end },
    [3]  = {name = "UByte",             read = function(s) return s:read8() end },
    [4]  = {name = "Short",             read = function(s) return s:read16() end },
    [5]  = {name = "UShort",            read = function(s) return s:read16() end },
    [6]  = {name = "Int",               read = function(s) return s:read32() end },
    [7]  = {name = "UInt",              read = function(s) return s:read32() end },
    [8]  = {name = "Float",             read = function(s) return s:readfloat() end },
    [9]  = {name = "Vector",            read = function(s) return { x = s:readfloat(), y = s:readfloat(), z = s:readfloat() } end },
    [10] = {name = "List",              read = function(s) return nil end },
    [11] = {name = "Comport",           read = function(s) return nil end },
    [12] = {name = "Action",            read = function(s) return nil end },
    [13] = {name = "Capabilities",      read = function(s) return nil end },
    [14] = {name = "Input",             read = function(s) return nil end },
    [15] = {name = "SoundEvent",        read = function(s) return nil end },
    [16] = {name = "Light",             read = function(s) return nil end },
    [17] = {name = "GameMaterial",      read = function(s) return nil end },
    [18] = {name = "VisualMaterial",    read = function(s) return nil end },
    [19] = {name = "Perso",             read = function(s) return nil end },
    [20] = {name = "WayPoint",          read = function(s) return WayPoint:Read(s:readpointer()) end },
    [21] = {name = "Graph",             read = function(s) return Graph:Read(s:readpointer()) or nil end },
    [22] = {name = "Text",              read = function(s) return nil end },
    [23] = {name = "SuperObject",       read = function(s) return nil end },
    [24] = {name = "SOLinks",           read = function(s) return nil end },
    [25] = {name = "PersoArray",        read = function(s) return nil end },
    [26] = {name = "VectorArray",       read = function(s) return nil end },
    [27] = {name = "FloatArray",        read = function(s) return nil end },
    [28] = {name = "IntegerArray",      read = function(s) return nil end },
    [29] = {name = "WayPointArray",     read = function(s) return nil end },
    [30] = {name = "TextArray",         read = function(s) return nil end },
    [31] = {name = "TextRefArray",      read = function(s) return nil end },
    [32] = {name = "GraphArray",        read = function(s) return nil end },
    [33] = {name = "Array9",            read = function(s) return nil end },
    [34] = {name = "SoundEventArray",   read = function(s) return nil end },
    [35] = {name = "Array11",           read = function(s) return nil end },
    [36] = {name = "Way",               read = function(s) return nil end },
    [37] = {name = "ActionArray",       read = function(s) return nil end },
    [38] = {name = "SuperObjectArray",  read = function(s) return nil end },
    [39] = {name = "ObjectList",        read = function(s) return nil end },
}

DSGVariableInfo = {}

function DSGVariableInfo.Read(address)
	local info = { offset = address }
	local stream = stream.open(address)

    info.offsetInBuffer = stream:read32()
    info.typeIdentifier = stream:read32()
    info.saveType = stream:read32()
    info.initType = stream:read32()
    info.typeName = "NULL"

    if info.typeIdentifier >= 0 and info.typeIdentifier <= 39 then
        info.typeName = DSGVariableType[info.typeIdentifier+1].name
        --console.log("red", "type: %s", info.typeName)
    end

    setmetatable(info, DSGVariableInfo)
    return info
end

DSGVariable = {}

-- Rayman dsg var: 0c26398

-- Read actor DSG variable
function DSGVariable.Read(address)
	local var = { offset = address }
	local stream = stream.open(address)

    var.memory = {}
    var.memory.offset = stream:readpointer()

    var.info = {}
    var.info.offset = stream:readpointer()

    var.memory.length = stream:read32()
    var.info.length = stream:read8()
    stream:advance(3) -- align

    --console.log("red", "info_length: %d", var.info.length)

    if var.info.length > 0 then
        var.info.entries = {}
        stream:jump(var.info.offset)
        for i = 0, var.info.length - 1 do
            var.info.entries[i] = DSGVariableInfo.Read(stream.position)
            stream:advance(4 * 4)
        end
    end


	setmetatable(var, DSGVariable)
	return var
end

-- Read variable value
local function readDSGValue(type, address)
    local val = { offset = address }
    local stream = stream.open(address)

    return val
end

DSGMemory = {}

-- Read DSG memory
function DSGMemory.Read(address)
	local dsg = { offset = address }
	local stream = stream.open(address)

    --console.log("yellow", "DSG memory @ %X", address)

    dsg.varPointer = stream:readpointer()

    dsg.memory = {}
    dsg.memory.length = 0

    dsg.memory.initial = {}
    dsg.memory.initial.offset = stream:readpointer()

    dsg.memory.current = {}
    dsg.memory.current.offset = stream:readpointer()

    if dsg.varPointer ~= 0x00 then
        dsg.variable = DSGVariable.Read(memory.readpointer(dsg.varPointer))
        if dsg.variable.info.length > 0 then

            dsg.memory.length = dsg.variable.info.length

            -- Read initial DSG variables
            if dsg.memory.initial ~= 0x00 then
                dsg.memory.initial.values = {}
                for i = 0, dsg.variable.info.length - 1 do
                    local entry = dsg.variable.info.entries[i]
                    local entryOffset = dsg.memory.initial.offset + entry.offsetInBuffer
                    -- Open stream at entry offset
                    local s = stream.open(entryOffset)

                    local T = DSGVariableType[entry.typeIdentifier+1]
                    dsg.memory.initial.values[i] = { offset = entryOffset, type = T.name, data = T.read(s) }
                    --console.log("green", "(%s) @ (%X + %d): %s", entry.typeName, dsg.memory.initial.offset, entry.offsetInBuffer, tostring(dsg.memory.initial.values[i]))
                end
            else
                dsg.memory.initial = nil
            end

            -- Read current DSG variables
            if dsg.memory.current ~= 0x00 then
                dsg.memory.current.values = {}
                for i = 0, dsg.variable.info.length - 1 do
                    local entry = dsg.variable.info.entries[i]
                    local entryOffset = dsg.memory.current.offset + entry.offsetInBuffer
                    -- Open stream at entry offset
                    local s = stream.open(entryOffset)

                    local T = DSGVariableType[entry.typeIdentifier+1]
                    dsg.memory.current.values[i] = { offset = entryOffset, type = T.name, data = T.read(s) }
                    --console.log("green", "(%s) @ (%X + %d): %s", entry.typeName, dsg.memory.initial.offset, entry.offsetInBuffer, tostring(dsg.memory.initial.values[i]))
                end
            else
                dsg.memory.current = nil
            end
        end
    else
        console.log("red", "Invalid DSG variable pointer in DSG memory section @ %X", address)
    end

	setmetatable(dsg, DSGMemory)
	return dsg
end
