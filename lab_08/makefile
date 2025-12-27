COMPILER_NAME := gcc
CFLAGS := -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Iinc
LDFLAGS := -lm

app.exe: out out/main.o out/graph_node.o out/print.o
	$(COMPILER_NAME) -o $@ out/main.o out/graph_node.o out/print.o $(LDFLAGS)
	
out/main.o: src/main.c inc/errors.h inc/define.h inc/print.h inc/graph_node.h
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out/graph_node.o: src/graph_node.c inc/graph_node.h inc/define.h inc/errors.h
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out/print.o: src/print.c inc/print.h inc/define.h inc/errors.h
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
	rm -f nul