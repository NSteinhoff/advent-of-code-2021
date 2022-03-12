iter = require("iter")
func = require("func")

local num2pat = {
    [0] = "abcefg",
    [1] = "cf",
    [2] = "acdeg",
    [3] = "acdfg",
    [4] = "bcdf",
    [5] = "abdfg",
    [6] = "abdefg",
    [7] = "acf",
    [8] = "abcdefg",
    [9] = "abcdfg",
}

function show(t, name)
    name = name or ""
    print("\n---", name)
    for k, v in pairs(t) do
        print(string.format("%10s: %s", k, v))
    end
end

function invert(t)
    local u = {}

    for k, v in pairs(t) do
        u[v] = k
    end

    return u
end

function sort(s)
    local t = {}

    for c in string.gmatch(s, "%a") do
        t[#t + 1] = c
    end

    table.sort(t)

    return table.concat(t)
end

function trans(s, m)
    local t = {}

    for c in string.gmatch(s, "%a") do
        t[#t + 1] = m[c] or "?"
    end

    return table.concat(t)
end

assert(trans("abc", { a = "x", b = "y", c = "z" }) == "xyz", "trans")

function remove(s, p)
    local t = {}

    for c in string.gmatch(s, "%a") do
        if not string.find(p, c) then
            t[#t + 1] = c
        end
    end

    return table.concat(t)
end

assert(remove("abcde", "ace") == "bd", "remove")

function six(n2p, patterns)
    local one = n2p[1]
    for _, p in ipairs(patterns) do
        if #p == 6 and #remove(p, one) == #p - 1 then
            return p
        end
    end
end

function five(n2p, patterns)
    local six = n2p[6]

    for _, p in ipairs(patterns) do
        if #p == 5 then
            if #remove(p, six) == 0 then
                return p
            end
        end
    end
end

function nine(n2p, patterns)
    local five = n2p[5]
    local one = n2p[1]

    for _, p in ipairs(patterns) do
        if #p == 6 then
            if #remove(p, five..one) == 0 then
                return p
            end
        end
    end
end

function zero(n2p, patterns)
    local five = n2p[5]

    for _, p in ipairs(patterns) do
        if #p == 6 then
            if #remove(p, five) == 2 then
                return p
            end
        end
    end
end

function two(n2p, patterns)
    local nine = n2p[9]

    for _, p in ipairs(patterns) do
        if #p == 5 then
            if #remove(p, nine) == 1 then
                return p
            end
        end
    end
end

function three(n2p, patterns)
    local five = n2p[5]

    for _, p in ipairs(patterns) do
        if #p == 5 then
            if #remove(p, five) == 1 then
                return p
            end
        end
    end
end

local pat2num = invert(num2pat)

local lengths = {}
for n, p in pairs(num2pat) do
    local g = #p
    local members = lengths[g] or {}
    members[#members + 1] = n
    lengths[g] = members
end

show(pat2num, "pat2num")

function create_mapping(patterns)
    local p2n = {}
    for _, p in ipairs(patterns) do
        local nums = lengths[#p]

        if #nums == 1 then
            p2n[p] = nums[1]
        end
    end
    local n2p = invert(p2n)

    local p2p = {}
    p2p[num2pat[1]] = n2p[1]
    p2p[num2pat[7]] = n2p[7]
    p2p[num2pat[4]] = n2p[4]
    p2p[num2pat[8]] = n2p[8]

    p2p[num2pat[6]] = six(n2p, patterns)
    n2p[6] = p2p[num2pat[6]]

    p2p[num2pat[5]] = five(n2p, patterns)
    n2p[5] = p2p[num2pat[5]]

    p2p[num2pat[9]] = nine(n2p, patterns)
    n2p[9] = p2p[num2pat[9]]

    p2p[num2pat[0]] = zero(n2p, patterns)
    n2p[0] = p2p[num2pat[0]]

    p2p[num2pat[2]] = two(n2p, patterns)
    n2p[2] = p2p[num2pat[2]]

    p2p[num2pat[3]] = three(n2p, patterns)
    n2p[3] = p2p[num2pat[3]]

    show(n2p, "broken num2pat")
    show(p2p, "pat2pat")

    return invert(p2p)
end

io.input("8.txt")
local real_input = io.lines()
local test_input = iter.values({
    "be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb | fdgacbe cefdb cefbgd gcbe",
    "edbfga begcd cbg gc gcadebf fbgde acbgfd abcde gfcbed gfec | fcgedb cgb dgebacf gc",
    "fgaebd cg bdaec gdafb agbcfd gdcbef bgcad gfac gcb cdgabef | cg cg fdcagb cbg",
    "fbegcd cbd adcefb dageb afcb bc aefdc ecdab fgdeca fcdbega | efabcd cedba gadfec cb",
    "aecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb dgceab fcbdga | gecf egdcabf bgf bfgea",
    "fgeab ca afcebg bdacfeg cfaedg gcfdb baec bfadeg bafgc acf | gebdcfa ecba ca fadegcb",
    "dbcfg fgd bdegcaf fgec aegbdf ecdfab fbedc dacgb gdcebf gf | cefg dcbef fcge gbcadfe",
    "bdfegc cbegaf gecbf dfcage bdacg ed bedf ced adcbefg gebcd | ed bcgafe cdgba cbgef",
    "egadfb cdbfeg cegd fecab cgb gbdefca cg fgcdab egfdb bfceg | gbdfcae bgc cg cgb",
    "gcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge fdbac fegbdc | fgae cfgab fg bagce",
})
-- local test_input = iter.values({
--     "acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab | cdfeb fcadb cdfeb cdbaf",
-- })

local test = false
local input = test and test_input or real_input

outputs = {}
for line in input do
    local signal_patterns, output_patterns = string.match(line, "(.*) | (.*)")

    signal_patterns = iter.collect(
        iter.map(sort)(string.gmatch(signal_patterns, "(%a+)"))
    )

    local sig2pat = create_mapping(signal_patterns)

    output_patterns = iter.collect(
        iter.map(sort)(string.gmatch(output_patterns, "(%a+)"))
    )

    local nums = {}
    for _, pattern in ipairs(output_patterns) do
        local num = pat2num[sig2pat[pattern]]
        nums[#nums + 1] = num
    end

    outputs[#outputs + 1] = table.concat(nums)
end

local total = 0

for _, o in ipairs(outputs) do
    print(o)
    total = total + o
end

print(string.format('The total is: %d', total))
