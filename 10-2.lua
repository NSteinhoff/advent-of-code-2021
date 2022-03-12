io.input("10.txt")

local delims = {
    ["("] = ")",
    ["["] = "]",
    ["{"] = "}",
    ["<"] = ">",
}
local points = {
    [")"] = 1,
    ["]"] = 2,
    ["}"] = 3,
    [">"] = 4,
}

local function isOpening(delim)
    for open, close in pairs(delims) do
        if delim == open then
            return true
        end
    end
    return false
end

local scores = {}
local l = 0
for line in io.lines() do
    l = l + 1
    local opening = {}
    local c = 0
    for delim in string.gmatch(line, ".") do
        c = c + 1
        if isOpening(delim) then
            table.insert(opening, delim)
        else
            local expected = delims[opening[#opening]]
            if delim == expected then
                table.remove(opening)
            else
                -- print(string.format('%2d:%2d Expecting %s but found %s', l, c, expected, delim))
                goto next
            end
        end
    end

    local closing = {}
    for i=#opening, 1 , -1 do
        closing[#closing + 1] = delims[opening[i]]
    end

    print(string.format("Complete %5d: %s", l, table.concat(closing)))

    local total = 0
    for _,c in ipairs(closing) do
        total = total * 5 + points[c]
    end
    table.insert(scores, total)

    ::next::
end

print(table.unpack(scores))
table.sort(scores)
print(table.unpack(scores))

local total = scores[(#scores+1) / 2]
print(string.format("Total: %d", total))
