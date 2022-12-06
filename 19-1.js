import { readFileSync } from "node:fs";

const dist = (a, b) => [
    Math.abs(a[0] - b[0]),
    Math.abs(a[1] - b[1]),
    Math.abs(a[2] - b[2]),
];
const key = ([x, y, z]) => `${x},${y},${z}`;

const reports = readFileSync("19-example.txt")
    .toString()
    .trim()
    .split("\n\n") // reports are separated by an empty line
    .map(r =>
        r
            .split("\n")
            .slice(1)
            .map(l => l.split(",").map(s => parseInt(s))),
    );

const distances = reports.map(r => r.map(a => r.map(b => dist(a, b))));
console.log(distances[0]);
