LinkedList = {}
LinkedList.__index = LinkedList

level = 0

local stream = require("stream")
local console = require("console")

function LinkedList.create()
	local list = {entries = {}}
	setmetatable(list, LinkedList)
	return list
end

function LinkedList:readheader(address)
	local list = {}

	stream = stream.open(address)
	stream:advance(4 * 2)
	list.head = stream:readpointer()
	list.tail = stream:readpointer()
	list.count = stream:read32()
	list.entries = {}

	setmetatable(list, LinkedList)
	return list
end

function LinkedList:readEntries(type)
	if not self.head then
		console.log("warning", "Cannot read list: invalid list pointer\n")
		return 0
	end

	local read = 0
	local offset_next = self.head
	for i = 0, self.count do
		local element = offset_next
		console.log("address", "List element @ 0x%X", element)

		self.entries[i] = type:Read(element)
		offset_next = self.entries[i].next
	end

	return read
end

return LinkedList
