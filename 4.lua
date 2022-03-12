io.input("4.txt")

function show_board(n, board, selection)
    print()
    print(string.format("%d)", n), 1, 2, 3, 4, 5)
    print(" ", "--", "--", "--", "--", "--")
    for r, row in ipairs(board) do
        local numbers = {}
        for c, number in ipairs(row) do
            numbers[c] = selection[r][c] and "*" .. number or number
        end
        print(r .. "|", table.unpack(numbers))
    end
end

function copy_grid(grid)
    local copy = {}

    for i = 1, 5 do
        copy[i] = {}
        for j = 1, 5 do
            copy[i][j] = grid[i][j]
        end
    end

    return copy
end

function pos_of(number, board)
    for i, r in pairs(board) do
        for j, n in ipairs(r) do
            if n == number then
                return i, j
            end
        end
    end
end

function check_winner(selections, winners)
    local new_winners = {}

    for b, _ in ipairs(selections) do
        local has_won
        for _, winner in ipairs(winners) do
            if winner["n"] == b then
                has_won = true
                break
            end
        end

        if not has_won then
            local rows = { 0, 0, 0, 0, 0 }
            local cols = { 0, 0, 0, 0, 0 }
            for r = 1, 5 do
                for c = 1, 5 do
                    if selections[b][r][c] then
                        rows[r] = rows[r] + 1
                        cols[c] = cols[c] + 1
                        if cols[c] == 5 or rows[r] == 5 then
                            new_winners[#new_winners + 1] = b
                        end
                    end
                end
            end
        end
    end

    return new_winners
end

function calculate_score(board, selection, pick)
    local score = 0
    for r, row in ipairs(board) do
        for c, number in ipairs(row) do
            if not selection[r][c] then
                score = score + number
            end
        end
    end

    return score * pick
end

local picks = {}
for pick in string.gmatch(io.read("l"), "(%d+),?") do
    picks[#picks + 1] = tonumber(pick)
end

local boards = {}
for line in io.lines() do
    if not line then
        break
    end

    if line == "" then
        boards[#boards + 1] = {}
    else
        local row = {}
        for n in string.gmatch(line, "(%d+)") do
            row[#row + 1] = tonumber(n)
        end
        boards[#boards][#boards[#boards] + 1] = row
    end
end

local selections = {}
for b, _ in ipairs(boards) do
    selections[b] = {}
    for r = 1, 5 do
        selections[b][r] = {}
    end
end

local winners = {}
for i, pick in ipairs(picks) do
    for b, board in ipairs(boards) do
        local r, c
        r, c = pos_of(pick, board)
        if r and c then
            selections[b][r][c] = true
        end
    end

    local new_winners = check_winner(selections, winners)

    for _, winner in ipairs(new_winners) do
        winners[#winners + 1] = {
            n = winner,
            score = calculate_score(boards[winner], selections[winner], pick),
            pick = pick,
            selection = copy_grid(selections[winner]),
        }
    end
end

print(string.format("We have %d winners out of %d boards", #winners, #boards))

print(
    string.format(
        "\n\nThe first winner is board %d with a score of %d and the winning pick of %d.",
        winners[1]["n"],
        winners[1]["score"],
        winners[1]["pick"]
    )
)
show_board(winners[1]['n'], boards[winners[1]['n']], winners[1]['selection'])

print(
    string.format(
        "\n\nThe last winner is board %d with a score of %d and the winning pick of %d.",
        winners[#winners]["n"],
        winners[#winners]["score"],
        winners[#winners]["pick"]
    )
)
show_board(winners[#winners]['n'], boards[winners[#winners]['n']], winners[#winners]['selection'])
