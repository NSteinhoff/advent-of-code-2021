io.input('2.txt')
local depth = 0
local horiz = 0
local direction, magnitude

for line in io.lines() do
    direction, magnitude = string.match(line, '(%a+) (%d+)')

    if direction == 'up' then depth = depth - magnitude end
    if direction == 'down' then depth = depth + magnitude end
    if direction == 'forward' then horiz = horiz + magnitude end
end


print(string.format("depth = %d, horiz = %d", depth, horiz))
print(string.format("depth * horiz = %d", depth * horiz))
