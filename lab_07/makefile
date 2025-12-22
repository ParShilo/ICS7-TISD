COMPILER_NAME := gcc
CFLAGS := -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Iinc
LDFLAGS := -lm

app.exe: out out/main.o out/bst_node.o out/avl_node.o out/hashtable_chain.o out/hashtable_open.o out/print.o out/compare.o
	$(COMPILER_NAME) -o $@ out/main.o out/bst_node.o out/avl_node.o out/hashtable_chain.o out/hashtable_open.o out/print.o out/compare.o $(LDFLAGS)
	
out/main.o: src/main.c inc/errors.h inc/define.h inc/print.h inc/bst_node.h inc/hashtable_chain.h inc/hashtable_open.h inc/compare.h
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out/bst_node.o: src/bst_node.c inc/bst_node.h inc/define.h inc/errors.h
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out/avl_node.o: src/avl_node.c inc/avl_node.h inc/define.h inc/errors.h
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out/hashtable_chain.o: src/hashtable_chain.c inc/hashtable_chain.h inc/define.h inc/errors.h
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out/hashtable_open.o: src/hashtable_open.c inc/hashtable_open.h inc/define.h inc/errors.h
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out/print.o: src/print.c inc/print.h inc/define.h inc/errors.h
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out/compare.o: src/compare.c inc/compare.h inc/avl_node.h inc/bst_node.h inc/hashtable_chain.h inc/hashtable_open.h inc/define.h inc/errors.h
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