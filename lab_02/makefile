COMPILER_NAME := gcc
CFLAGS := -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Iinc
LDFLAGS := -lm

app.exe: out out/main.o out/proc_country.o out/print.o out/sort_country.o out/key.o
	$(COMPILER_NAME) $(LDFLAGS) -o $@ out/main.o out/proc_country.o out/print.o out/sort_country.o out/key.o

out/main.o: src/main.c inc/proc_country.h inc/print.h inc/errors.h inc/country.h
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out/proc_country.o: src/proc_country.c inc/proc_country.h
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out/print.o: src/print.c inc/print.h
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out/sort_country.o: src/sort_country.c inc/sort_country.h
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out/key.o: src/key.c inc/key.h
	$(COMPILER_NAME) $(CFLAGS) -c $< -o $@

out:
	mkdir -p out

.PHONY: clean release debug asan ubsan msan test test_builds asan_test ubsan_test msan_test

release: CFLAGS += -g0
release: app.exe

debug: CFLAGS += -g
debug: app.exe

asan: CFLAGS += -fsanitize=address -fno-omit-frame-pointer
asan: LDFLAGS += -fsanitize=address
asan: debug

ubsan: CFLAGS += -fsanitize=undefined -fno-omit-frame-pointer
ubsan: LDFLAGS += -fsanitize=undefined
ubsan: debug

msan: CFLAGS += -fsanitize=memory -fPIE -fno-omit-frame-pointer
msan: LDFLAGS += -fsanitize=memory -fPIE -pie
msan: debug

test: release
	-./func_tests/scripts/func_tests.sh -v -val

test_builds: asan_test ubsan_test msan_test
test_builds: clean

asan_test: asan
	@echo "1) BUILD_DEBUG_ASAN:"
	@-./func_tests/scripts/func_tests.sh -v

ubsan_test: ubsan
	@echo "2) BUILD_DEBUG_UBSAN:"
	@-./func_tests/scripts/func_tests.sh -v

msan_test: msan
	@echo "3) BUILD_DEBUG_MSAN:"
	@-./func_tests/scripts/func_tests.sh -v

gcov: CFLAGS += -coverage
gcov: LDFLAGS += -coverage
gcov: app.exe

clean:
	rm -rf out
	rm -f *.exe