-- ------------------------------------------------------------------------- --
--                                Definitions                                --
-- ------------------------------------------------------------------------- --

-- Infer the number of bits from the first line in the file
local fname = '3.txt'
io.input(fname)
local nbits = #io.read('l')
local mask = 2^nbits - 1

function complement(number)
    return ~number & mask
end

function b2n(bits)
    return tonumber(string.reverse(table.concat(bits)), 2)
end

function check_bit(number, position)
    return number & 2^(position - 1) > 0
end

function get_bit(number, position)
    return check_bit(number, position) and 1 or 0
end

function count_bits(numbers, position)
    local count = 0
    for _, number in ipairs(numbers) do
        if check_bit(number, position) then
            count = count + 1
        end
    end
    return count
end

function most_common_bit(numbers, position)
    local count = count_bits(numbers, position)
    if count == #numbers/2 then
        return 1
    end
    return count > #numbers/2 and 1 or 0
end

function least_common_bit(numbers, position)
    local count = count_bits(numbers, position)
    if count == #numbers/2 then
        return 0
    end
    return count < #numbers/2 and 1 or 0
end

function funnel(numbers, most)
    local most = most == nil or most
    local funneled = { [0] = numbers }
    local common = (most and most_common_bit or least_common_bit)

    for position = nbits, 1, -1 do
        local bit = common(funneled[#funneled], position)

        local numbers = {}
        for _, number in ipairs(funneled[#funneled]) do
            if get_bit(number, position) == bit then
                numbers[#numbers+1] = number
            end
        end

        funneled[#funneled+1] = numbers

        if #numbers == 1 then
            return funneled
        end
    end

    return funneled
end

function select(funneled)
    return funneled[#funneled][1]
end

-- ------------------------------------------------------------------------- --
--                                   Main                                    --
-- ------------------------------------------------------------------------- --

-- Rewind
io.input(fname)

-- Convert the bit pattern string to numbers so that we can use bitwise
-- operations
local numbers = {}
for line in io.lines() do
    numbers[#numbers+1] = tonumber(line, 2)
end

local most_common_bits = {}
for i=1, nbits do
    most_common_bits[i] = most_common_bit(numbers, i)
end

local gamma = b2n(most_common_bits)
local epsilon = complement(gamma)
local power = gamma * epsilon

local oxygen = select(funnel(numbers))
local co2 = select(funnel(numbers, false))
local life = oxygen * co2
print(string.format("g = %d; e = %d; p = %d; o = %d; c = %d; l = %d", gamma, epsilon, power, oxygen, co2, life))
