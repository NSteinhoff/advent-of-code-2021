from itertools import groupby


words = ['foo', 'bar', 'baz', 'hello', 'world', 'a', 'b']
bylength = groupby(words, lambda word: len(word))

for (length, words) in bylength:
    print(length, [*words])
