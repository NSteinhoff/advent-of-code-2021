io.input('1.txt')
local current
local previous

local count = 0
for line in io.lines() do
    current = tonumber(line)
    if previous then
        if current > previous then
            count = count + 1
        end
    end
    previous = current
end

print(string.format('1.1: Count = %d', count))
