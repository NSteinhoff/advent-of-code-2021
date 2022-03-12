iter = require("iter")
io.input("7.txt")

local test = false

local test_input = iter.values({ 16, 1, 2, 0, 4, 2, 7, 1, 2, 14 })
local real_input = iter.map(tonumber)(string.gmatch(io.read("l"), "(%d+),?"))

local input = test and test_input or real_input

local positions = {}
for number in input do
    positions[#positions + 1] = number
end

function cost(distance)
    if distance == 0 then
        return 0
    end
    return (distance * (distance + 1)) / 2
end

local max = math.max(table.unpack(positions))
print("positions ", table.concat(positions, ","))
print("#positions", #positions)
print("max       ", max)

local costs = {}
for _, number in ipairs(positions) do
    for target = 0, max do
        costs[target] = (costs[target] or 0) + cost(math.abs(number - target))
    end
end

local min_cost
local position
for target = 0, max do
    if not min_cost or min_cost > costs[target] then
        min_cost = costs[target]
        position = target
    end
end

print(
    string.format("cheapest position: %2d at a cost of %d", position, min_cost)
)
