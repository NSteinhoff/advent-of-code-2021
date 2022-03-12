io.input('2.txt')
local depth = 0
local horiz = 0
local aim = 0
local direction, magnitude

for line in io.lines() do
    direction, magnitude = string.match(line, '(%a+) (%d+)')

    if direction == 'up' then aim = aim - magnitude end
    if direction == 'down' then aim = aim + magnitude end
    if direction == 'forward' then
        horiz = horiz + magnitude
        depth = depth + (magnitude * aim)
    end
end


print(string.format("depth = %d, horiz = %d, aim = %d", depth, horiz, aim))
print(string.format("depth * horiz = %d", depth * horiz))
