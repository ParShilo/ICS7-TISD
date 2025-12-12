COMPILER_NAME := gcc
CFLAGS := -std=gnu99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Iinc
LDFLAGS := -lm

app.exe: out out/main.o out/node.o out/print.o out/output.o out/compare.o
	$(COMPILER_NAME) $(LDFLAGS) -o $@ out/main.o out/node.o out/print.o out/output.o out/compare.o
	
out/main.o: src/main.c inc/errors.h inc/define.h inc/print.h inc/node.h inc/output.h inc/compare.h
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out/node.o: src/node.c inc/node.h inc/define.h inc/errors.h
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out/print.o: src/print.c inc/print.h inc/define.h inc/errors.h
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out/output.o: src/output.c inc/output.h inc/node.h inc/define.h inc/errors.h
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out/compare.o: src/compare.c inc/compare.h inc/node.h inc/define.h
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
	rm -f *.png
	rm -f *.dot