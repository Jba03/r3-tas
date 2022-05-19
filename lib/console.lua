local console = {}
console.__index = console

function console.fmt(opt, ...)
	local fmt = "\27[%sm%s\27[0m"
	local code = "0"
	
	-- improve or remove
	if opt == "info" then code = "0" end
	if opt == "warning" then code = "31" end
	if opt == "structure" then code = "1" end
	if opt == "section" then code = "32;1" end
	if opt == "address" then code = "35;1" end
	if opt == "value" then code = "34;1" end

	if opt == "red" then code = "31;1" end
	if opt == "green" then code = "32;1" end
	if opt == "yellow" then code = "33;1" end
	if opt == "blue" then code = "34;1" end
	if opt == "pink" then code = "35;1" end
	if opt == "cyan" then code = "36;1" end
	if opt == "white" then code = "37;1" end
	if opt == "grey" then code = "38;2" end

	local r = string.format(...)
	local s = string.format(fmt, code, r)
	return s
end

function console.log(opt, ...)
	return print(console.fmt(opt, ...))
end

return console
