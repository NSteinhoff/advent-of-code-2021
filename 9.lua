io.input("9-example.txt")

local map = {}
for line in io.lines() do
    local t = {}
    for number in string.gmatch(line, "%d") do
        t[#t + 1] = tonumber(number)
    end
    map[#map + 1] = t
end

for _, line in ipairs(map) do
    print(table.concat(line))
end

local total = 0
local low = {}
for i, x in ipairs(map) do
    for j, y in ipairs(x) do
        local neighbors = {}
        for _,coords in ipairs({{i-1, j}, {i+1, j}, {i, j-1}, {i, j+1}}) do
            local ni, nj = table.unpack(coords)
            if ni > 0 and ni <= #map and nj > 0 and nj <= #x then
                neighbors[#neighbors + 1] = map[ni][nj]
            end
        end
        if y < math.min(table.unpack(neighbors)) then
            total = total + y + 1
        end
    end
end

for _, l in ipairs(low) do
    print(table.unpack(l))
end

print('total', total)
