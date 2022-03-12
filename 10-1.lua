io.input("10.txt")

local delims = {
    ["("] = ")",
    ["["] = "]",
    ["{"] = "}",
    ["<"] = ">",
}
local points = {
    [")"] = 3,
    ["]"] = 57,
    ["}"] = 1197,
    [">"] = 25137,
}

local function isOpening(delim)
    for open, close in pairs(delims) do
        if delim == open then
            return true
        end
    end
    return false
end

local total = 0
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
            local closing = delims[opening[#opening]]
            if delim == closing then
                table.remove(opening)
            else
                print(string.format('%2d:%2d Expecting %s but found %s', l, c, closing, delim))
                total = total + points[delim]
                goto next
            end
        end
    end
    ::next::
end

print(string.format("Total: %d", total))
