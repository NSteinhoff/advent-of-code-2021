const { readFile } = require("node:fs");

readFile("1.txt", (_, data) => {
    const [count] = data
        .toString()
        .split("\n")
        .map(Number)
        .reduce(
            ([count, previous], n) => [previous < n ? count + 1 : count, n],
            [0, Infinity]
        );

    console.log({ count });
});
