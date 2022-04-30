CC      = clang

CFLAGS := $(CFLAGS)
CFLAGS += -O3
CFLAGS += -flto
CFLAGS += --target=wasm32
CFLAGS += --no-standard-libraries
CFLAGS += -Wall -Wextra
CFLAGS += -Werror

# === Linker flags
CFLAGS += -Wl,--lto-O3
CFLAGS += -Wl,--no-entry
CFLAGS += -Wl,--export-all
CFLAGS += -Wl,--import-memory
CFLAGS += -Wl,--import-undefined
CFLAGS += -Wl,--unresolved-symbols=ignore-all

all: mem.wasm mem.wat strlen.wasm strlen.wat add.wasm add.wat
.PHONY: wasm

%.wasm: %.c
	$(CC) $(CFLAGS) -o $@ $<

%.wat: %.wasm
	wasm2wat -o $@ $<

clean:
	@rm -f *.wasm
	@rm -f *.wat
.PHONY: clean
