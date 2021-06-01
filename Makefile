.PHONY: all run clean

CC := gcc
AR := ar
DEBUG ?= 1

override CFLAGS += -Iinclude -Wall -std=c99

ifeq ($(DEBUG), 1)
	override CFLAGS += -O0 -g -DDEBUG=1 -fsanitize=address #-S -save-temps
else
	override CFLAGS += -O3
endif

SRCS := $(wildcard src/*.c)
OBJS := $(patsubst src/%.c, obj/%.o, $(SRCS))
DEPS := $(patsubst src/%.c, obj/%.d, $(SRCS))

all: libcl.a test

libcl.a: $(filter-out test.o, $(OBJS))
	$(AR) rcs $@ $^

test: obj/test.o libcl.a
	$(CC) $(CFLAGS) -o $@ $^
	
run: all
	./test

clean:
	rm -rf obj
	rm -f test

obj:
	mkdir -p obj

obj/%.d: src/%.c | obj
	$(CC) $(CFLAGS) -M $< -MT $(@:.d=.o) -MF $@

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

ifneq (clean,$(filter clean, $(MAKECMDGOALS)))
-include $(DEPS)
endif
