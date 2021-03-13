# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: mmeyer <mmeyer@student.codam.nl>             +#+                      #
#                                                    +#+                       #
#    Created: 2020/11/10 14:32:36 by mmeyer        #+#    #+#                  #
#    Updated: 2021/02/02 17:59:57 by mmeyer        ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

TEST_NAME = test
CC=gcc
CFLAGS= -Wall -Wextra -Werror -ggdb3 -fsanitize=address -fsanitize=leak -fsanitize=signed-integer-overflow
LDFLAGS= -shared -fPIC
CREATE_LIB=ar rcs
FILES_DIR=./yourFilesHere

HEADER_FILE = $(FILES_DIR)/get_next_line.h
HEADER_FILE_BONUS = $(FILES_DIR)/get_next_line_bonus.h

TEST_FILE=tests/test.c
TEST_FILE_BONUS=tests/test.c tests/bonus.c
TEST_FILE_INVALID_SIZE=tests/test_invalid_size.c

SRC_FILES= \
	$(FILES_DIR)/get_next_line.c \
	$(FILES_DIR)/get_next_line_utils.c \

BONUS_FILES = \
	$(FILES_DIR)/get_next_line_bonus.c \
	$(FILES_DIR)/get_next_line_utils_bonus.c \

INC_PATH=-I./$(FILES_DIR)

all: test

clean:
	rm -f $(TEST_NAME)
	rm -f ./tests/results/actual_stdin

test: $(SRC_FILES) $(HEADER_FILE)
	$(CC) $(CFLAGS) $(INC_PATH) -D BUFFER_SIZE=$(SIZE) $(SRC_FILES) $(TEST_FILE) -o $(TEST_NAME)

test_invalid_size:$(SRC_FILES) $(HEADER_FILE)
	$(CC) $(CFLAGS) $(INC_PATH) -D BUFFER_SIZE=0 $(SRC_FILES) $(TEST_FILE_INVALID_SIZE) -o $(TEST_NAME)

stdin: $(SRC_FILES) $(HEADER_FILE)
	$(CC) $(CFLAGS) $(INC_PATH) -D BUFFER_SIZE=$(SIZE) $(SRC_FILES) tests/test_stdin.c -o $(TEST_NAME)

bonus:$(BONUS_FILES) $(HEADER_FILE_BONUS)
	$(CC) $(CFLAGS) $(INC_PATH) -D WITH_BONUS=1 -D BUFFER_SIZE=$(SIZE) $(BONUS_FILES) $(TEST_FILE_BONUS) -o $(TEST_NAME)

.PHONY: all clean fclean re bonus test bonus stdin test_invalid_size
