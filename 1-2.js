const { readFile } = require("node:fs");

function* zip(...iterables) {
    if (!iterables.length) return [];
    [leader, ...rest] = iterables;

    for (const i in leader) {
        const elements = [leader[i], ...rest.map((a) => a[i])];
        if (elements.some((e) => e === undefined)) {
            return;
        }
        yield elements;
    }
}

function* map(iterable, func) {
    for (const elem of iterable) {
        yield func(elem);
    }
}

function* accumulate(iterable, func, acc) {
    for (const elem of iterable) {
        acc = func(acc, elem);
        yield acc;
    }
}

function tail(iterable) {
    let t = undefined;
    for (const elem of iterable) {
        t = elem;
    }
    return t;
}

function* evolutions(iterable, predicate) {
    let left = iterable.next().value;

    for (const right of iterable) {
        yield predicate(left, right);
        left = right;
    }
}

readFile("1.txt", (_, data) => {
    const lines = data
        .toString()
        .split("\n")
        .filter(Boolean)
        .map(Number);

    const windows = zip(lines, lines.slice(1), lines.slice(2));
    const sums = map(windows, (w) => w.reduce((sum, v) => sum + v, 0));
    const increases = evolutions(sums, (a, b) => a < b);
    const counts = accumulate(increases, (acc, b) => (b ? acc + 1 : acc), 0);
    const count = tail(counts);

    console.log({ count });
});
