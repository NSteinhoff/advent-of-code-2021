import memoryMap from "./memory-map.js";
import inputFile from "./input-file.js";

const maxMemoryPages = 100;
let curMemoryPages = 10;

// Create some memory to pass to the WASM module
export const memory = new WebAssembly.Memory({
    initial: curMemoryPages,
    maximum: maxMemoryPages,
});

// Free all of the memory of the heap
const free = () => {
    heapPtr = heapStart;
};

// Allocate memory on the heap and return a pointer to it.
const alloc = (size) => {
    if (heapPtr + size > memory.buffer.byteLength) {
        if (curMemoryPages < maxMemoryPages) {
            curMemoryPages += memory.grow(1);
            updateMap();
            return alloc(size);
        } else {
            console.error(
                `Unable to allocate ${size} bytes. ${
                    memory.buffer.byteLength - heapPtr
                } bytes available.`
            );
            return null;
        }
    } else {
        const p = heapPtr;
        heapPtr += size;
        updateMap();
        return p;
    }
};

// Create the WASM module with the required imports
const { instance } = await WebAssembly.instantiateStreaming(
    fetch("./strlen.wasm"),
    { env: { memory, alloc } }
);

// Callback to be invoked with the bytes of the newly read input file.
//
// It resets the memory allocation of the heap and allocates enough memory to
// store the file's contents. This can be passed as a char *pointer to the WASM
// functions.
const onFileLoad = (bytes) => {
    free();
    const inputPtr = alloc(bytes.byteLength + 1);
    if (inputPtr === null) return;

    const buf = new Uint8Array(memory.buffer, inputPtr, bytes.byteLength + 1);
    buf.fill(0);
    buf.set(new Uint8Array(bytes));
    buf[bytes.byteLength] = 0;

    const result = process(inputPtr);
    console.log(`Result: ${result}`);
    updateMap();
};

// Create an file picker
inputFile(document.getElementById("input"), onFileLoad);

// Make the instance and memory globally available for debugging.
window.wasm = { instance, memory };
console.log(wasm);

// Pointers to the different places in the memory buffer
const { __data_end, __heap_base } = instance.exports;
const stackStart = __data_end.value;
const heapStart = __heap_base.value;
let heapPtr = heapStart;
const getHeapPtr = () => heapPtr;

const updateMap = memoryMap(document.getElementById("memory-map"), memory, {
    stackStart,
    heapStart,
    next: getHeapPtr,
});
updateMap();

// Grab the functions exported from the module.
const { process } = instance.exports;
