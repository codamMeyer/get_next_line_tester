#define CTEST_MAIN
#define CTEST_COLOR_OK
#define CTEST_SEGFAULT
#ifdef WITH_BONUS
#include <get_next_line_bonus.h>
#else
#include <get_next_line.h>
#endif
#include "./lib/ctest.h"
#include <stdio.h>
#include <stdlib.h>
#include <bsd/string.h>
#include <string.h>
#include <fcntl.h>

static void remove_nl(char *expected_ptr)
{
	int len = strlen(expected_ptr);
	if (len)
	{
		if ((expected_ptr + len - 1)[0] == '\n')
			(expected_ptr + len - 1)[0] = '\0';
	}
}

static void reset_ptrs(char **ptr1, char **ptr2)
{
	free(*ptr1);
	*ptr1 = NULL;
	free(*ptr2);
	*ptr2 = NULL;
}

void reset_ptr(char **ptr)
{
	free(*ptr);
	*ptr = NULL;
}

void run_test(const char *filename, char **ret_ptr, char **expected_ptr)
{
	const int fd = open(filename, O_RDONLY);
	FILE *fd_sys = fopen(filename, "r");
	size_t size = 0;
	ASSERT_TRUE(fd > 0);
	int line = 1;
	while (getline(expected_ptr, &size, fd_sys) > 0)
	{
		ASSERT_EQUAL(1, get_next_line(fd, ret_ptr));
		remove_nl(*expected_ptr);
		ASSERT_STR(*expected_ptr, *ret_ptr);
		reset_ptrs(ret_ptr, expected_ptr);
		line++;
	}
	ASSERT_EQUAL(0, get_next_line(fd, ret_ptr));
}

CTEST_DATA(test_without_nl)
{
	char *ret_ptr;
	char *expected_ptr;
};

CTEST_SETUP(test_without_nl)
{
	data->ret_ptr = NULL;
	data->expected_ptr = NULL;
}

CTEST_TEARDOWN(test_without_nl)
{
	if (data->ret_ptr)
		free(data->ret_ptr);
	if (data->expected_ptr)
		free(data->expected_ptr);
}

CTEST2(test_without_nl, empty_file_without_nl)
{
	const char *filename = "./tests/text/empty_file_without_nl";
	const int fd = open(filename, O_RDONLY);

	ASSERT_EQUAL(0, get_next_line(fd, &(data->ret_ptr)));
	ASSERT_STR("", data->ret_ptr);
}

CTEST2(test_without_nl, file_without_nl)
{
	const char *filename = "./tests/text/file_without_nl";
	const int fd = open(filename, O_RDONLY);
	ASSERT_EQUAL(0, get_next_line(fd, &(data->ret_ptr)));
	ASSERT_STR("hello world!", data->ret_ptr);
}

CTEST2(test_without_nl, empty_file)
{
	const char *filename = "./tests/text/empty_file";
	const int fd = open(filename, O_RDONLY);
	ASSERT_EQUAL(0, get_next_line(fd, &(data->ret_ptr)));
	ASSERT_STR("", data->ret_ptr);
}

CTEST_DATA(basic_test)
{
	char *ret_ptr;
	char *expected_ptr;
};

CTEST_SETUP(basic_test)
{
	data->ret_ptr = NULL;
	data->expected_ptr = NULL;
}

CTEST_TEARDOWN(basic_test)
{
	if (data->ret_ptr)
		free(data->ret_ptr);
	if (data->expected_ptr)
		free(data->expected_ptr);
}

CTEST2(basic_test, small_file)
{
	const char *filename = "./tests/text/small_file";
	run_test(filename, &(data->ret_ptr), &(data->expected_ptr));
}

CTEST2(basic_test, same_file_two_times)
{
	const char *filename = "./tests/text/small_file";
	run_test(filename, &(data->ret_ptr), &(data->expected_ptr));
	reset_ptrs(&(data->ret_ptr), &(data->expected_ptr));
	run_test(filename, &(data->ret_ptr), &(data->expected_ptr));
}

CTEST2(basic_test, test_huge_file)
{
	const char *filename = "./tests/text/huge_alphabet";
	run_test(filename, &(data->ret_ptr), &(data->expected_ptr));
}

CTEST2(basic_test, getline_after_eof)
{
	const char *filename = "./tests/text/test_after_eof";
	const int fd = open(filename, O_RDONLY);

	ASSERT_EQUAL(0, get_next_line(fd, &(data->ret_ptr)));
	ASSERT_STR("hello", data->ret_ptr);
	reset_ptr(&(data->ret_ptr));
	ASSERT_EQUAL(0, get_next_line(fd, &(data->ret_ptr)));
}

CTEST2(basic_test, multiple_empty_lines_in_the_end)
{
	const char *filename = "./tests/text/empty_lines";
	run_test(filename, &(data->ret_ptr), &(data->expected_ptr));
}

CTEST2(basic_test, multiple_empty_lines)
{
	const char *filename = "./tests/text/only_nl";
	run_test(filename, &(data->ret_ptr), &(data->expected_ptr));
}

CTEST2(basic_test, test_with_huge_lines)
{
	const char *filename = "./tests/text/huge_lines";
	run_test(filename, &(data->ret_ptr), &(data->expected_ptr));
}

int main(int argc, const char *argv[])
{
	return ctest_main(argc, argv);
}
