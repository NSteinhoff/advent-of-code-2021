const CELL_SIZE = 16; // px
const FONT_SIZE = (CELL_SIZE * 2) / 3;
const GRID_COLUM_WIDTH = CELL_SIZE + 1;
const GRID_ROW_HEIGHT = CELL_SIZE + 1;

const TEXT_COLOR = "#000";
const GRID_COLOR = "#AAA";
const CELL_COLOR_HEAP = "rgba(0, 0, 0, 0)";
const CELL_COLOR_FREE = "rgba(100, 200, 100, 0.2)";
const CELL_COLOR_STCK = "rgba(200, 100, 100, 0.2)";
const CELL_COLOR_DATA = "rgba(100, 200, 200, 0.2)";
const CELL_COLOR_NEXT = "rgba(255, 100, 255, 0.5)";

const KB_PER_PAGE = 64;
const BYTES_PER_KB = 1024;
const BYTES_PER_PAGE = BYTES_PER_KB * KB_PER_PAGE;

export default (canvas, memory, opts) => {
    makeGrid(canvas);

    const update = (n) => {
        const numPages = Math.floor(memory.buffer.byteLength / BYTES_PER_PAGE);
        setPage(Math.min(n, numPages - 1));
        const pageNumber = document.getElementById("memory-page");
        pageNumber.innerHTML = `${getPage() + 1} / ${numPages}`;

        draw(canvas, memory, opts);

        makeButtons((direction) => () => {
            update((numPages + getPage() + direction) % numPages);
        });
    };

    update(getPage());

    return () => update(getPage());
};

// Get the page index starting at 0
const getPage = () => {
    const search = new URLSearchParams(window.location.search);

    if (!search.has("page")) return 0;

    const page = Number.parseInt(search.get("page"), 10);

    return page === NaN || page <= 0 ? 0 : page - 1;
};

const setPage = (n) => {
    const url = new URL(window.location.href);
    url.searchParams.set("page", n + 1);
    window.history.replaceState(null, null, url);
};

const makeGrid = (canvas) => {
    // Draw a grid for one page of WASM memory
    const width = KB_PER_PAGE;
    const height = BYTES_PER_KB;

    // + 1 for the closing separator lines
    canvas.width = width * GRID_COLUM_WIDTH + 1;
    canvas.height = height * GRID_ROW_HEIGHT + 1;

    const ctx = canvas.getContext("2d");
    ctx.beginPath();
    ctx.strokeStyle = GRID_COLOR;

    // Vertical lines.
    const yBottom = GRID_ROW_HEIGHT * height + 1;
    for (let column = 0; column <= width; column++) {
        const x = column * GRID_COLUM_WIDTH + 1;
        ctx.moveTo(x, 0);
        ctx.lineTo(x, yBottom);
    }

    // Horizontal lines.
    const xEnd = GRID_COLUM_WIDTH * width + 1;
    for (let row = 0; row <= height; row++) {
        const y = row * (CELL_SIZE + 1) + 1;
        ctx.moveTo(0, y);
        ctx.lineTo(xEnd, y);
    }

    ctx.stroke();
};

const makeButtons = (changePage) => {
    document.getElementById("memory-previous").onclick = changePage(-1);
    document.getElementById("memory-next").onclick = changePage(1);
};

const draw = (canvas, memory, { stackStart, heapStart, next }) => {
    const nextByte = next();
    const ctx = canvas.getContext("2d");

    const arr = new Uint8Array(
        memory.buffer,
        getPage() * BYTES_PER_PAGE,
        BYTES_PER_PAGE
    );

    ctx.font = `${FONT_SIZE}px monospace`;
    ctx.textAlign = "center";
    ctx.textBaseline = "middle";
    for (let i = 0; i < arr.length; i++) {
        const byte = i + getPage() * BYTES_PER_PAGE;
        const bg =
            byte === nextByte
                ? CELL_COLOR_NEXT
                : byte < stackStart
                ? CELL_COLOR_DATA
                : byte < heapStart
                ? CELL_COLOR_STCK
                : byte < nextByte
                ? CELL_COLOR_HEAP
                : CELL_COLOR_FREE;
        drawCell(ctx, Math.floor(i / KB_PER_PAGE), i % KB_PER_PAGE, arr[i], bg);
    }
};

const drawCell = (ctx, row, col, num, bg) => {
    const xStart = GRID_COLUM_WIDTH * col + 1;
    const yStart = GRID_ROW_HEIGHT * row + 1;
    const xCenter = xStart + CELL_SIZE / 2;
    const yCenter = yStart + CELL_SIZE / 2;

    ctx.clearRect(xStart, yStart, CELL_SIZE, CELL_SIZE);

    ctx.fillStyle = bg;
    ctx.fillRect(xStart, yStart, CELL_SIZE, CELL_SIZE);

    if (num) {
        ctx.fillStyle = TEXT_COLOR;
        ctx.fillText(
            (num % 256).toString(16).padStart(2, "0"),
            xCenter,
            yCenter,
            CELL_SIZE
        );
    }
};
