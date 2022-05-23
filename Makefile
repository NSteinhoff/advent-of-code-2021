CC = clang
CFLAGS = -g

help:
	@echo
	@echo "usage: make [target]"
	@echo
	@echo "Targets"
	@echo "    <lang>_<day>          Run the solution for <lang> language and <day> day."
	@echo "    list                  List available solutions."
	@echo "    clean                 Remove build artifacts."
	@echo
	@echo "Solutions"
	@make list | column | while read l; do echo "    $$l"; done
.PHONY: help

list:
	@find -E . \
		-regex '.*/[0-9]+(-[0-9])?\.[a-z]+' \
		-not -name '*.txt' \
		-not -path '*/target/*' \
		| sed -E 's/.*\/([0-9]+(-[12])?).([a-z]+)/\3_\1/g' \
		| sort -t _ -k 1,1 -k 2,2n
.PHONY: list

clean:
	@rm -rf rs/target
	@rm -rf *.dSYM
	@find -E . -regex './[0-9]+(-[0-9])?' -delete
.PHONY: clean

py_%: %.py
	@python $<

lua_%: %.lua
	@lua $<

c_%: %.c
	@$(CC) $(CFLAGS) $< -o $*
	@./$*

rs_%: ./rs/src/bin/%.rs
	@cd ./rs && cargo run --bin $*
