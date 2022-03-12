io.input("9.txt")

local map = {}
for line in io.lines() do
    local t = {}
    for number in string.gmatch(line, "%d") do
        t[#t + 1] = tonumber(number)
    end
    map[#map + 1] = t
end

local M = #map
local N = #map[1]

for _, line in ipairs(map) do
    print(table.concat(line))
end

function iter_map()
    return coroutine.wrap(function()
        for i = 1, M do
            for j = 1, N do
                coroutine.yield(i, j)
            end
        end
    end)
end

function iter_neighbors(i, j)
    return coroutine.wrap(function()
        if i > 1 then
            coroutine.yield(i - 1, j)
        end
        if j > 1 then
            coroutine.yield(i, j - 1)
        end
        if j < N then
            coroutine.yield(i, j + 1)
        end
        if i < M then
            coroutine.yield(i + 1, j)
        end
    end)
end

function iter_basin(i, j, visited)
    local visited = visited or {}
    local size = 0
    local k = i..','..j
    if visited[k] or map[i][j] == 9 then return size end
    visited[k] = true
    size = size + 1

    for ii, jj in iter_neighbors(i, j) do
        size = size + iter_basin(ii, jj, visited)
    end

    return size
end

local basins = {}
for i, j in iter_map(map) do
    local neighbors = {}
    for ii, jj in iter_neighbors(i, j) do
        neighbors[#neighbors + 1] = map[ii][jj]
    end
    if map[i][j] < math.min(table.unpack(neighbors)) then
        local basin_total = iter_basin(i, j)
        basins[#basins + 1] = basin_total
    end
end

table.sort(basins)
local total = basins[#basins] * basins[#basins - 1] * basins[#basins - 2]

print("total", total)
