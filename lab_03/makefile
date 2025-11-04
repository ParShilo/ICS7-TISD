COMPILER_NAME := gcc
CFLAGS := -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Iinc
LDFLAGS := -lm

app.exe: out out/main.o out/process.o out/memory.o out/io.o out/print.o
	$(COMPILER_NAME) $(LDFLAGS) -o $@ out/main.o out/process.o out/memory.o out/io.o out/print.o

out/main.o: src/main.c inc/process.h inc/memory.h inc/io.h inc/print.h inc/struct.h inc/err.h
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out/process.o: src/process.c inc/process.h inc/struct.h inc/err.h inc/memory.h
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out/memory.o: src/memory.c inc/memory.h inc/struct.h inc/err.h
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out/io.o: src/io.c inc/io.h inc/struct.h inc/err.h inc/memory.h
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out/print.o: src/print.c inc/print.h
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out:
	mkdir -p out

.PHONY: clean release debug asan ubsan msan gcov

release: CFLAGS += -g0 -O2
release: app.exe

debug: CFLAGS += -g3 -O0
debug: app.exe

asan: CFLAGS += -fsanitize=address -fno-omit-frame-pointer -g
asan: LDFLAGS += -fsanitize=address
asan: app.exe

ubsan: CFLAGS += -fsanitize=undefined -fno-omit-frame-pointer -g
ubsan: LDFLAGS += -fsanitize=undefined
ubsan: app.exe

msan: CFLAGS += -fsanitize=memory -fPIE -fno-omit-frame-pointer -g
msan: LDFLAGS += -fsanitize=memory -fPIE -pie
msan: app.exe

gcov: CFLAGS += --coverage -g -O0
gcov: LDFLAGS += --coverage
gcov: app.exe

clean:
	rm -rf out
	rm -f *.exe
	rm -f *.gcda *.gcno *.gcov