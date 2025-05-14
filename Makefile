# Compiler and flags
CC = gcc
CFLAGS = -g -Isrc -Wall -Wno-incompatible-pointer-types
LDFLAGS = -lcheck
LEX = flex
BISON = bison

# Directories
BUILD_DIR = build
SRC_DIR = src
TEST_DIR = test

# Other constants
TEST_PREFIX=test_

# Source files
LEX_FILE = $(SRC_DIR)/taco.lex
BISON_FILE = $(SRC_DIR)/taco.y
SRC_C_FILES = $(shell find $(SRC_DIR) -name '*.c')
TEST_FILES = $(shell find $(TEST_DIR) -name '$(TEST_PREFIX)*.c')
# SRC_FILES = $(subst $(TEST_DIR)/,$(SRC_DIR)/,$(subst $(TEST_PREFIX/),,$(TEST_FILES)))

# Generated files
LEX_C = $(BUILD_DIR)/lex.yy.c
LEX_OUTPUT = $(BUILD_DIR)/lex.output
BISON_C = $(BUILD_DIR)/taco.tab.c
BISON_H = $(BUILD_DIR)/taco.tab.h
BISON_OUTPUT = $(BUILD_DIR)/taco.output

# Object files
BUILD_O_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.c.o,$(SRC_C_FILES))
LEX_O = $(LEX_C).o
BISON_O = $(BISON_C).o
TEST_O_FILES = $(TEST_FILES:.c=.o)

# Final executables
EXECUTABLE = $(BUILD_DIR)/taco
TEST_BINARY = $(TEST_DIR)/all_tests

# Targets
all: $(EXECUTABLE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BISON_C): $(BISON_FILE) | $(BUILD_DIR)
	$(BISON) -vd -o $@ $< > $(BISON_OUTPUT)
	@echo "Generated $@ from $<"

$(LEX_C): $(LEX_FILE) $(BISON_C) | $(BUILD_DIR)
	$(LEX) -v -o $@ $< 2> $(LEX_OUTPUT)
	@echo "Generated $@ from $<"

$(BISON_O): $(BISON_C) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $< to $@"

$(LEX_O): $(LEX_C) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $< to $@"

$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $< to $@"

$(EXECUTABLE): $(BUILD_O_FILES) $(LEX_O) $(BISON_O)
	$(CC) -o $@ $^
	@echo "Linked all object files to create $@"

$(TEST_BINARY): $(TEST_O_FILES) $(BUILD_O_FILES)
	$(CC) -o $@ $^ $(LDFLAGS)
	@echo "Linked all test object files to create $@"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TEST_O_FILES) $(TEST_BINARY)
	@echo "Cleaned build and test directories"

run-tests: $(TEST_BINARY)
	./$(TEST_BINARY)

.PHONY: all clean run-tests
