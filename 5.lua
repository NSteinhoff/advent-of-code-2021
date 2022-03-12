io.input("5.txt")

function draw_map(map)
    for i, line in ipairs(map) do
        local l = {}
        for j, count in ipairs(line) do
            l[j] = count == 0 and "." or count
        end
        print(table.concat(l))
    end
end

function add_vents(map, vents)
    for _, vent in ipairs(vents) do
        for y = vent.y1, vent.y2, vent.y1 < vent.y2 and 1 or -1 do
            for x = vent.x1, vent.x2, vent.x1 < vent.x2 and 1 or -1 do
                if
                    math.abs(vent.y1 - y) == math.abs(vent.x1 - x)
                    or vent.x1 == vent.x2
                    or vent.y1 == vent.y2
                then
                    map[y][x] = map[y][x] + 1
                end
            end
        end
    end
end

function count_overlaps(map)
    local counts = {}

    for y, line in ipairs(map) do
        for x, count in ipairs(line) do
            for c = count, 1, -1 do
                counts[c] = (counts[c] or 0) + 1
            end
        end
    end

    return counts
end

local max_x = 1
local max_y = 1

local vents = {}
for line in io.lines() do
    local x1, y1, x2, y2

    x1, y1, x2, y2 = string.match(line, "(%d+),(%d+) [-]> (%d+),(%d+)")

    x1 = tonumber(x1) + 1
    y1 = tonumber(y1) + 1
    x2 = tonumber(x2) + 1
    y2 = tonumber(y2) + 1

    vents[#vents + 1] = {
        x1 = x1,
        y1 = y1,
        x2 = x2,
        y2 = y2,
    }

    max_x = math.max(max_x, x1, x2)
    max_y = math.max(max_y, y1, y2)
end

local map = {}
for y = 1, max_y do
    map[y] = {}
    for x = 1, max_x do
        map[y][x] = 0
    end
end

add_vents(map, vents)
-- draw_map(map)
local overlaps = count_overlaps(map)

for n, count in pairs(overlaps) do
    print(n, count)
end
