SuperObject = {}
SuperObject.__index = SuperObject

local config = require("config")
local stream = require("stream")
local console = require("console")
local list = require("list")
local Perso = require("perso")
local IPO = require("ipo")

length = 0
level = 0
numPersos = 0

local function log_read(type, address, numChildren, padding)
	local a = console.fmt("info", "%sReading ", padding)
	local b = console.fmt("green", "%s ", type)
	local c = console.fmt("blue", "@ ")
	local d = console.fmt("blue", "%X ", address)
	local e = console.fmt("grey", "(%d children)", numChildren)
	print(a .. b .. c .. d .. e)
end

-- Read a superobject
function SuperObject:Read(address)
	local so = { offset = address }
	local stream = stream.open(address)

	so.children = list.create()
	so.type = stream:read32()
	so.typename = SuperObject.GetTypeName(so)

	local padding = ""
	for i = 0, level * 4 do padding = padding .. " " end

	if not so.typename and so.data then
		console.log("yellow", "%sEND OF SUPEROBJECT LIST @ %08X", padding, so.type)
		so.data = nil
		return so
	end

	so.data = stream:readpointer()
	-- TODO: Replace with list:read()
	so.children.head = stream:readpointer()
	so.children.tail = stream:readpointer()
	so.children.count = stream:read32()

	so.next = stream:readpointer()
	so.prev = stream:readpointer()
	so.parent = stream:readpointer()

	so.matrix = {}
	so.matrix.default = stream:readpointer()
	so.matrix.static = stream:readpointer()
	so.matrix.global = stream:readpointer()

	if so.type == 0x2 then numPersos = numPersos + 1 end

    -- Print superobject address
	if config.PRINT_INFO and so.typename and so.offset and so.children  then
        log_read(so.typename, so.offset, so.children.count, padding)
    end

	-- Read data
    if so.data ~= 0x00 then
        if so.typename == "Perso" then so.data = Perso:Read(so.data) end
        if so.typename == "IPO" then so.data = IPO:Read(so.data) end
    end

	-- Read children
    if so.children then
    	if so.children.count > 0 then
    		local offset_next = so.children.head
    		for i = 0, so.children.count do
    			level = level + 1

                if offset_next > 0x17FFFFF then
                    break
                end

    			so.children.entries[i] = SuperObject:Read(offset_next)
    			so.children.entries[i].parent = so
    			offset_next = so.children.entries[i].next
    		end
    	end
    end

	level = level - 1

	setmetatable(so, SuperObject)
	return so
end

-- Return the name of a superobject's type
function SuperObject:GetTypeName()
	if self.type == 0x0 then return "Dummy" end
	if self.type == 0x1 then return "World" end
	if self.type == 0x2 then return "Perso" end
	if self.type == 0x4 then return "Sector" end
	if self.type == 0x8 then return "PhysicalObject" end
	if self.type == 0x20 then return "IPO" end
	if self.type == 0x40 then return "IPO-2" end
	if self.type == 0x400 then return "GeometricObject" end
	if self.type == 0x80000 then return "GeometricShadowObject" end
	return nil
end

return SuperObject
