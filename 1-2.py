previous = None
count = 0
with open('1.txt') as file:
    lines = list(map(int, file))
    # 1 2 3
    # 2 3 4
    # 3 4 5
    # . . .
    # 7 8 9
    # 8 9
    # 9
    windows = zip(lines, lines[1:], lines[2:])
    sums = map(sum, windows)

    for current in sums:
        if previous is not None:
            if previous < current:
                count = count + 1

        previous = current

print(f'number of increases: {count}')
