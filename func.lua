local M = {}

local function equal(t1, t2)
    if #t1 ~= #t2 then
        return false
    end

    for k, v in pairs(t1) do
        if t2[k] ~= v then
            return false
        end
    end

    return true
end

assert(equal({ 1, 2, 3 }, { 1, 2, 3 }), "equal")
assert(not equal({ 1, 3, 2 }, { 1, 2, 3 }), "equal")
assert(not equal({ 1, 2 }, { 1, 2, 3 }), "equal")
assert(not equal({ 1, 2, 3 }, { 1, 2 }), "equal")

function M.map(f)
    return function(xs)
        local ys = {}

        for i, x in ipairs(xs) do
            ys[i] = f(x)
        end

        return ys
    end
end

assert(
    equal(
        M.map(function(x)
            return x * x
        end)({ 1, 2, 3 }),
        { 1, 4, 9 }
    ),
    "map"
)

function M.filter(f)
    return function(xs)
        local ys = {}

        for _, x in ipairs(xs) do
            if f(x) then
                ys[#ys + 1] = x
            end
        end

        return ys
    end
end

assert(
    equal(
        M.filter(function(x)
            return x % 2 == 0
        end)({ 1, 2, 3, 4, 5, 6, 7 }),
        { 2, 4, 6 }
    ),
    "filter"
)

function M.reduce(f)
    return function(xs)
        if #xs == 0 then
            return nil
        end

        local acc
        for _, x in ipairs(xs) do
            acc = acc == nil and x or f(acc, x)
        end

        return acc
    end
end

assert(M.reduce(function(acc, x)
    return acc + x
end)({ 1, 2, 3, 4, 5 }) == 15, "reduce")

function M.groupby(f)
    return function(xs)
        local groups = {}

        for _, x in ipairs(xs) do
            local group = f(x)
            local members = groups[group] or {}
            members[#members + 1] = x
            groups[group] = members
        end

        return groups
    end
end

assert(
    equal(
        M.groupby(function(x)
            return x % 2
        end)({ 1, 2, 3, 4, 5, 6, 7 })[0],
        { 2, 4, 6 }
    ),
    "groupby"
)
assert(
    equal(
        M.groupby(function(x)
            return x % 2
        end)({ 1, 2, 3, 4, 5, 6, 7 })[1],
        { 1, 3, 5, 7 }
    ),
    "groupby"
)

return M
