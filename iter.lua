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

function M.values(xs)
    local i = 0
    return function()
        i = i + 1
        return xs[i]
    end
end

function M.map(f)
    return function(iter)
        return function()
            local v = iter()
            return v and f(v)
        end
    end
end

function M.collect(iter)
    local res = {}
    for elem in iter do
        res[#res + 1] = elem
    end
    return res
end

assert(
    equal(
        M.collect(M.map(function(x)
            return x * x
        end)(M.values({ 1, 2, 3 }))),
        { 1, 4, 9 }
    ),
    "map"
)

return M
