CC = gcc
AR = ar
PYTHON ?= python3
PKG_CONFIG ?= pkg-config
LCOV ?= lcov
GENHTML ?= genhtml
CLANG_FORMAT ?= clang-format

CPPFLAGS += -I.
CFLAGS = -std=c11 -Wall -Wextra -Werror
CHECK_CFLAGS = $(shell $(PKG_CONFIG) --cflags check)
CHECK_LIBS = $(shell $(PKG_CONFIG) --libs check)
LDLIBS += -lm
COVERAGE_FLAGS = --coverage -O0 -g
SANITIZE_FLAGS = -fsanitize=address,undefined -fno-sanitize-recover=all -g

LIB_SOURCES = $(sort $(wildcard s21_*.c))
TEST_SOURCES = $(sort $(wildcard tests/test_*.c))
LIB_OBJECTS = $(LIB_SOURCES:.c=.o)
TEST_OBJECTS = $(TEST_SOURCES:.c=.o)
MODES = obj coverage sanitize
OBJECTS = $(foreach mode,$(MODES),$(addprefix build/$(mode)/,$(LIB_OBJECTS) $(TEST_OBJECTS)))
RUNNERS = build/tests build/coverage/tests_runner build/sanitize/tests_runner

ifneq ($(findstring clang,$(shell $(CC) --version)),)
GCOV_TOOL ?= $(CURDIR)/tools/llvm-gcov.sh
else
GCOV_TOOL ?= gcov
endif

.PHONY: all clean test gcov_report sanitize format format-check
.DELETE_ON_ERROR:

all: s21_string.a

s21_string.a: $(addprefix build/obj/,$(LIB_OBJECTS)) Makefile
build/coverage/s21_string.a: $(addprefix build/coverage/,$(LIB_OBJECTS))
s21_string.a build/coverage/s21_string.a:
	rm -f $@
	$(AR) rcs $@ $(filter %.o,$^)

$(filter build/obj/%,$(OBJECTS)): build/obj/%.o: %.c Makefile
$(filter build/coverage/%,$(OBJECTS)): build/coverage/%.o: %.c Makefile
$(filter build/sanitize/%,$(OBJECTS)): build/sanitize/%.o: %.c Makefile
$(filter build/coverage/s21_%.o,$(OBJECTS)): BUILD_FLAGS += $(COVERAGE_FLAGS)
$(filter build/sanitize/%,$(OBJECTS)): BUILD_FLAGS += $(SANITIZE_FLAGS)
# Only tests need Check and the macOS exception for deprecated reference functions.
$(foreach mode,$(MODES),$(addprefix build/$(mode)/,$(TEST_OBJECTS))): BUILD_FLAGS += $(CHECK_CFLAGS) -Wno-deprecated-declarations
$(OBJECTS):
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(BUILD_FLAGS) -MMD -MP -c $(filter %.c,$^) -o $@

build/tests: $(addprefix build/obj/,$(TEST_OBJECTS)) s21_string.a
build/coverage/tests_runner: $(addprefix build/coverage/,$(TEST_OBJECTS)) build/coverage/s21_string.a
build/sanitize/tests_runner: $(filter build/sanitize/%,$(OBJECTS))
build/coverage/tests_runner: LINK_FLAGS = $(COVERAGE_FLAGS)
build/sanitize/tests_runner: LINK_FLAGS = $(SANITIZE_FLAGS)
$(RUNNERS):
	$(CC) $(CFLAGS) $(LINK_FLAGS) $(LDFLAGS) $^ $(CHECK_LIBS) $(LDLIBS) -o $@

test: build/tests
	./build/tests || test $$? -eq 1

gcov_report: build/coverage/tests_runner
	find build/coverage -name '*.gcda' -delete
	./build/coverage/tests_runner || test $$? -eq 1
	$(LCOV) --capture --directory build/coverage --gcov-tool "$(GCOV_TOOL)" --include '$(CURDIR)/s21_*.c' --output-file build/coverage/coverage.info
	$(GENHTML) build/coverage/coverage.info --output-directory report --title 's21_string coverage'
	$(PYTHON) tools/check_coverage.py build/coverage/coverage.info --minimum 80 --html report/functions.html
	@printf '\nHTML report: report/index.html\nPer-function coverage: report/functions.html\n'

sanitize: build/sanitize/tests_runner
	./build/sanitize/tests_runner

format:
	$(CLANG_FORMAT) -i $(LIB_SOURCES) s21_string.h tests/*.c tests/*.h

format-check:
	$(CLANG_FORMAT) --dry-run --Werror $(LIB_SOURCES) s21_string.h tests/*.c tests/*.h

clean:
	rm -rf build report s21_string.a
	rm -f *.o *.gcda *.gcno *.gcov *.info

-include $(OBJECTS:.o=.d)
