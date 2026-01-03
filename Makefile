CC=gcc
CFLAGS=-Wall -Wextra -Iinclude

SRC=src/main.c src/task.c src/heuristics.c src/scheduler.c src/io.c src/utils.c
OUT=task_planner

TEST_SRC=tests/test_runner.c src/task.c src/heuristics.c src/scheduler.c src/io.c src/utils.c
TEST_OUT=test_runner

all: build

build:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

test:
	$(CC) $(CFLAGS) $(TEST_SRC) -o $(TEST_OUT)
	./$(TEST_OUT)

clean:
	rm -f $(OUT) $(TEST_OUT)
