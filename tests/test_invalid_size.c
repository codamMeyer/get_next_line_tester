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

CTEST_DATA(invalid_input) {

	char *filename;
	int fd;
	char *ret_ptr;
};

CTEST_SETUP(invalid_input) {
	data->filename = "./tests/text/malloc_test";
	data->fd = open(data->filename, O_RDONLY);
    data->ret_ptr = NULL;
}

CTEST_TEARDOWN(invalid_input) {
    if (data->ret_ptr) free(data->ret_ptr);
}

CTEST2(invalid_input, invalid_buffer_size)
{
	ASSERT_EQUAL(-1, get_next_line(data->fd, &(data->ret_ptr)));
}

CTEST2(invalid_input, invalid_fd)
{
	ASSERT_EQUAL(-1, get_next_line(-5, &(data->ret_ptr)));
}

int main(int argc, const char *argv[])
{
	return ctest_main(argc, argv);
}
