local stream = {}
stream.__index = stream

function stream.open(address)
	local s = { baseAddress = address, position = address }
	setmetatable(s, stream)
	return s
end

-- Advance stream position
function stream:advance(step)
    if self.position then
    	self.position = self.position + step
    	return self.position
    else
        return 0x00000000
    end
end

function stream:jump(address)
    self.position = address
end

function stream:read8()		return memory.read8(self:advance(1) - 1) 	 end
function stream:read16()	return memory.read16(self:advance(2) - 2) 	 end
function stream:read32()	return memory.read32(self:advance(4) - 4) 	 end
function stream:readfloat() return memory.readfloat(self:advance(4) - 4) end

function stream:readstring(length)
	local s = ""

	if length == nil then
		-- Read until terminating '\0'
		s = memory.readstring(self.position)
		self:advance(string.len(s))
	else
		-- Read by length
		for c = 0, length-1 do
			s = s .. string.char(self:read8())
		end
	end

	return s
end

function stream:readpointer()
	return self:read32() & 0xFFFFFFF
end

return stream
