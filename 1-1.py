previous = None
count = 0

with open('1.txt') as file:
    for line in file:
        current = int(line)
        if previous is not None:
            if previous < current:
                count = count + 1

        previous = current

print(f'number of increases: {count}')
