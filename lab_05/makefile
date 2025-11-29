COMPILER_NAME := gcc
CFLAGS := -std=gnu99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Iinc
LDFLAGS := -lm

app.exe: out out/main.o out/arr.o out/list.o out/print.o out/simulate.o
	$(COMPILER_NAME) $(LDFLAGS) -o $@ out/main.o out/arr.o out/list.o out/print.o out/simulate.o
	
out/main.o: src/main.c inc/errors.h inc/defines.h inc/print.h inc/arr.h inc/list.h
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out/arr.o: src/arr.c inc/arr.h inc/errors.h
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out/list.o: src/list.c inc/list.h inc/errors.h
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out/print.o: src/print.c
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out/simulate.o: src/simulate.c inc/simulate.h inc/arr.h inc/list.h inc/errors.h
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out:
	mkdir -p out

.PHONY: clean release debug

release: CFLAGS += -g0 -O2
release: app.exe

debug: CFLAGS += -g3 -O0
debug: app.exe

clean:
	rm -rf out
	rm -f *.exe
	rm -f *.gcda *.gcno *.gcov