local stream = require("stream")
local console = require("console")

local DSGVariableType = {
    [0] = "None",
    [1] = "Boolean",
    [2] = "Byte",
    [3] = "UByte",
    [4] = "Short",
    [5] = "UShort",
    [6] = "Int",
    [7] = "UInt",
    [8] = "Float",
    [9] = "Vector",
    [10] = "List",
    [11] = "Comport",
    [12] = "Action",
    [13] = "Capabilities",
    [14] = "Input",
    [15] = "SoundEvent",
    [16] = "Light",
    [17] = "GameMaterial",
    [18] = "VisualMaterial",
    [19] = "Perso",
    [20] = "WayPoint",
    [21] = "Graph",
    [22] = "Text",
    [23] = "SuperObject",
    [24] = "SOLinks",
    [25] = "PersoArray",
    [26] = "VectorArray",
    [27] = "FloatArray",
    [28] = "IntegerArray",
    [29] = "WayPointArray",
    [30] = "TextArray",
    [31] = "TextRefArray",
    [32] = "GraphArray",
    [33] = "Array9",
    [34] = "SoundEventArray",
    [35] = "Array11",
    [36] = "Way",
    [37] = "ActionArray",
    [38] = "SuperObjectArray",
    [39] = "ObjectList",
}

DSGVariableInfo = {}

function DSGVariableInfo.Read(address)
	local info = { offset = address }
	local stream = stream.open(address)

    info.offsetInbuffer = stream:read32()
    info.typeIdentifier = stream:read32()
    info.saveType = stream:read32()
    info.initType = stream:read32()

    if info.typeIdentifier >= 0 and info.typeIdentifier <= 39 then
        console.log("red", "type: %s", DSGVariableType[info.typeIdentifier+1])
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

    console.log("red", "info_length: %d", var.info.length)

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

DSGMemory = {}

-- Read DSG memory
function DSGMemory.Read(address)
	local dsg = { offset = address }
	local stream = stream.open(address)

    --console.log("yellow", "DSG memory @ %X", address)

    dsg.varPointer = stream:readpointer()

    dsg.memory = {}
    dsg.memory.initial = stream:readpointer()
    dsg.memory.current = stream:readpointer()

    if dsg.varPointer ~= 0x00 then
        dsg.variable = DSGVariable.Read(memory.readpointer(dsg.varPointer))
    else
        console.log("red", "Invalid DSG variable pointer in DSG memory section @ %X", address)
    end

    if dsg.memory.initial == 0x00 or dsg.memory.current == 0x00 then
        console.log("red", "Invalid pointer to DSG memory section @ %X", address)
    end

	setmetatable(dsg, DSGMemory)
	return dsg
end
