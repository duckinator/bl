CC := clang

CFLAGS := -std=c11 -pedantic-errors -fdiagnostics-show-option \
			-Werror -Weverything \
			-Wno-missing-noreturn -Wno-reserved-id-macro

SRCFILES := $(wildcard src/*.c)
EXEFILES := $(patsubst src/%,bin/%,$(patsubst %.c,%,${SRCFILES}))

all: ${EXEFILES}

bin/%: src/%.c
	@mkdir -p bin/
	${CC} ${CFLAGS} $< -o $@

clean:
	rm -rf bin/

.PHONY: clean all
