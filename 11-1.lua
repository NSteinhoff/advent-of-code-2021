io.input("11-example.txt")

local M = 10
local N = 10

local lines = {}
for line in io.lines() do
    table.insert(lines, line)
end

local generations = { [0] = table.concat(lines) }

function itermap(map)
    return coroutine.wrap(function()
        for i = 1, M do
            for j = 1, N do
                coroutine.yield(i, j, get(i, j, map))
            end
        end
    end)
end

function get(m, n, map)
    if m < 1 or m > M or n < 1 or n > N then
        return
    end

    local map = map or generations[#generations]
    local index = M * (m - 1) + n
    return tonumber(string.sub(map, index, index))
end

function draw()
    print(string.format("--- %02d ---", #generations))
    for i, j, n in itermap() do
        io.write(n == 0 and ' ' or n)
        if j == 10 then
            io.write("\n")
        end
    end
end

function propagate()
    :again:
end

function next()
    local nextgen = {}
    for i, j, n in itermap(generations[#generations]) do
        local nn = n + 1
        if nn > 9 then
            -- flash
            -- TODO: propagate
            table.insert(nextgen, 0)
        else
            -- gather energy
            table.insert(nextgen, nn)
        end
    end
    table.insert(generations, table.concat(nextgen))
end

for step=0,3 do
    draw()
    next()
end
