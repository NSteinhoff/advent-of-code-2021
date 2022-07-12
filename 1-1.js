const { readFile, readFileSync } = require("node:fs");

// Callback API
readFile("1.txt", (_, data) => {
    const [count] = data
        .toString()
        .split("\n")
        .map((s) => Number.parseInt(s, 10))
        .reduce(
            ([count, previous], n) => [previous < n ? count + 1 : count, n],
            [0, Number.MAX_VALUE]
        );

    console.log({ count });
});

// Synchronous API
let count = 0;
let previous = Number.MAX_VALUE;
for (const line of readFileSync("1.txt").toString().split("\n")) {
    const current = Number.parseInt(line, 10);
    if (current > previous) count++;
    previous = current;
}

console.log({ count });
