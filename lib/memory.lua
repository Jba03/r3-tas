-- Read effective address space pointer
function memory.readpointer(address)
	return memory.read32(address) & 0xFFFFFFF
end

return memory
