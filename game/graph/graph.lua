local list = require("list")
local stream = require("stream")
local console = require("console")

WayPoint = {}
WayPoint.__index = WayPoint

function WayPoint:Read(address)
    local wp = {}
    local stream = stream.open(address)

    wp.position = {}
	wp.position.x = stream:readfloat()
	wp.position.z = stream:readfloat() -- z & y switched
	wp.position.y = stream:readfloat()
	wp.radius = stream:readfloat()

	setmetatable(wp, WayPoint)
	return wp
end

function WayPoint:Render(color)
	local s = self.radius * 2
	if self.radius == 0.0 then s = s + 0.25 end

	ira:scale(s, s, s)
	ira:translate(self.position.x, self.position.y, self.position.z)
	ira:box(self.position.x, self.position.y, self.position.z, color)
	ira:reset()
end

--
--
-- Graph node
--
--

GraphNode = {}
GraphNode.__index = GraphNode

function GraphNode:Read(address)
    local node = {}

    local stream = stream.open(address)
	node.next = stream:readpointer()
	node.prev = stream:readpointer()
	node.graph = stream:readpointer()
	node.waypoint = stream:readpointer()
    node.arclist = stream:readpointer()

    node.waypoint = WayPoint:Read(node.waypoint)

	setmetatable(node, GraphNode)
	return node
end

function GraphNode:Render(color)
	self.waypoint:Render(color)
end

--
--
-- Graph
--
--

Graph = {}
Graph.__index = Graph

function Graph:Read(address)
    local graph = { offset = address }
    local stream = stream.open(address)

    if address == 0x00 then
        return nil
    end

    graph.nodelist = list.create()
    graph.nodelist.head = stream:readpointer()
    graph.nodelist.tail = stream:readpointer()
    graph.nodelist.count = stream:read32()
    graph.nodelist.entries = {}
    graph.display = false
    graph.color = math.random(0x60202020, 0xAAFFFFFF)

    if graph.nodelist.count > 0 then
		local offset_next = graph.nodelist.head
		for i = 1, graph.nodelist.count do
			graph.nodelist.entries[i] = GraphNode:Read(offset_next)
			graph.nodelist.entries[i].parent = so
			offset_next = graph.nodelist.entries[i].next
		end
	else
        return nil
    end

	setmetatable(graph, Graph)

    if UpdateGraphList then
        GraphList[address] = graph
    end

	return graph
end

function Graph:Render()
    for i, node in ipairs(self.nodelist.entries) do
        local wp = node.waypoint

        -- dx = (rayman.x - wp.position.x) ^ 2
        -- dy = (rayman.y - wp.position.y) ^ 2
        -- dz = (rayman.z - wp.position.z) ^ 2
        -- distance = math.min(math.sqrt(dx + dy + dz), 51)
        --
        -- alpha = ((math.floor(math.max(100, distance * 5))) << 24) & 0xFF000000
        -- color = c - alpha --Color.Graph[5]--
        color = 0x808000FF
        node:Render(self.color)
        --p1 = node.waypoint.position
        --p2 = nodelist[i-1].waypoint.position
        --ira:triangle(p1.x, p1.y, p1.z, p1.x, p1.y + 0.5, p1.z, p2.x, p2.y, p2.z, Color.Graph[6])
    end
end

return Graph
