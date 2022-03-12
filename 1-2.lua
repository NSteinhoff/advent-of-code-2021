io.input('1.txt')
local numbers = {}
for line in io.lines() do
    numbers[#numbers + 1] = tonumber(line)
end

local count = 0
for i = 4, #numbers do
    local previous = numbers[i - 3] + numbers[i - 2] + numbers[i - 1]
    local current = numbers[i - 2] + numbers[i - 1] + numbers[i]
    if previous < current then count = count + 1 end
end

print(string.format('1.2: Count = %d', count))
