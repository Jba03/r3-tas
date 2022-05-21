CollisionGeometry = {}
CollisionGeometry.__index = CollisionGeometry

local stream = require("stream")
local console = require("console")

-- Read triangle collision mesh
local function readTriangles(address)
    local stream = stream.open(address)

    local materialOffset = stream:readpointer()
    local trianglesOffset = stream:readpointer()
    local normalsOffset = stream:readpointer()
    local triangleCount = stream:read16()
    local parBoxIndices = stream:read16()
    stream:advance(4)

    local mappingOffset = stream:readpointer()
    stream:advance(4 * 2) -- Skip unknown
    local mappingCount = stream:read16()
    stream:advance(2) -- Align

    -- Read triangles
    local triangles = {}
    stream:jump(trianglesOffset)
    --console.log("red", "Reading triangle indices @ %X", trianglesOffset)
    for i = 0, triangleCount do
        triangles[i * 3 + 0] = stream:read16()
        triangles[i * 3 + 1] = stream:read16()
        triangles[i * 3 + 2] = stream:read16()
    end

    return triangleCount, triangles
end

function CollisionGeometry:Read(address)
	local geometry = { offset = address }
	local stream = stream.open(address)

    geometry.vertices = {}
    geometry.vertices.data = {}
    geometry.vertices.count = stream:read16()

    geometry.elements = {}
    geometry.elements.count = stream:read16()
    stream:advance(4)

    geometry.vertices.offset = stream:readpointer()
    geometry.elements.types = stream:readpointer()
    geometry.elements.data = stream:readpointer()
    stream:advance(4)

    geometry.parallelBoxes = stream:readpointer()

    -- Read bounding sphere
    geometry.boundingSphere = { offset = stream.position }
    geometry.boundingSphere.radius = stream:readfloat()
    geometry.boundingSphere.x = stream:readfloat()
    geometry.boundingSphere.y = stream:readfloat()
    geometry.boundingSphere.z = stream:readfloat()

    -- Read vertices
    stream:jump(geometry.vertices.offset)
    for i = 0, geometry.vertices.count - 1 do
        local x = stream:readfloat()
        local y = stream:readfloat()
        local z = stream:readfloat()
        geometry.vertices.data[i] = { x, y, z }
    end

    -- Read element types
    stream:jump(geometry.elements.types)
    geometry.elements.types = {}
    for i = 0, geometry.elements.count - 1 do
        geometry.elements.types[i] = stream:read16()
    end

    -- Read elements
    for i = 0, geometry.elements.count - 1 do
        stream:jump(geometry.elements.data + 4 * i)
        local block = stream:readpointer()
        -- Element types
        --  1: indexedtriangles
        --  2: facemap
        --  3: sprite
        --  4: TMesh
        --  5: points
        --  6: lines
        --  7: spheres
        --  8: alignedboxes
        --  9: cones
        --  13: deformationsetinfo
        --console.log("cyan", "element type (%d): %d", i, geometry.elements.types[i])
        local elementType = geometry.elements.types[i]
        if elementType == 1 then numTriangles, triangleIndices = readTriangles(block) end

        local aaa=0
        for j = 0, numTriangles * 3 - 1 do
            geometry.vertices.data[j] = geometry.vertices.data[triangleIndices[j]]
            --console.log("cyan", "%d: %d", aaa, triangleIndices[j])
            aaa = aaa + 1
        end

        --console.log("cyan", "Read %d triangle indices", numTriangles)
    end



    --console.log("cyan", "Vertices @ %X: %d", address, geometry.vertices.count)
    --console.log("cyan", "Elements @ %X: %d", address, geometry.elements.count)



	setmetatable(geometry, CollisionGeometry)
	return geometry
end

function CollisionGeometry:Draw()

    for i = 0, self.vertices.count, 3 do
        local a = self.vertices.data[i+0]
        local b = self.vertices.data[i+1]
        local c = self.vertices.data[i+2]

        --print("len: " .. #self.vertices.data)
        if a and b and c then
            ira:triangle(a[1], a[2], a[3], b[1], b[2], b[3], c[1], c[2], c[3], 0x80FFFFFF)
            ira:reset()
        end
    end

    -- radius * 2
    -- ira:scale(self.boundingSphere.radius*0.5,self.boundingSphere.radius*0.5,self.boundingSphere.radius*0.5)
    -- ira:translate(self.boundingSphere.x, self.boundingSphere.y, self.boundingSphere.z)
    -- ira:box(0,0,0,0x80FF00FF)
    -- ira:reset()
end

return CollisionGeometry
