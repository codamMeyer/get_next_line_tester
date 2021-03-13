#define CTEST_COLOR_OK
#define CTEST_SEGFAULT
#include <get_next_line_bonus.h>
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
CTEST_DATA(bonus)
{

    char *filename2;
    char *filename3;
    int fd2;
    int fd3;
    int fds[2];

    FILE *fd_sys2;
    FILE *fd_sys3;
    FILE *fd_syss[2];

    size_t size;

    char *ret_ptr;
    char *expected_ptr;
};

CTEST_SETUP(bonus)
{
    data->filename2 = "./tests/text/lorem_text";
    data->filename3 = "./tests/text/simple_text";
    data->fd2 = open(data->filename2, O_RDONLY);
    data->fd3 = open(data->filename3, O_RDONLY);
    data->fds[0] = data->fd2;
    data->fds[1] = data->fd3;
    data->fd_sys2 = fopen(data->filename2, "r");
    data->fd_sys3 = fopen(data->filename3, "r");
    data->fd_syss[0] = data->fd_sys2;
    data->fd_syss[1] = data->fd_sys3;

    data->size = 0;
    data->ret_ptr = NULL;
    data->expected_ptr = NULL;
}

CTEST_TEARDOWN(bonus)
{
    if (data->ret_ptr)
        free(data->ret_ptr);
    if (data->expected_ptr)
        free(data->expected_ptr);
}

CTEST2(bonus, multiple_files)
{
    for (int i = 1; i <= 28; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            getline(&(data->expected_ptr), &(data->size), data->fd_syss[j]);
            ASSERT_EQUAL(1, get_next_line(data->fds[j], &(data->ret_ptr)));
            remove_nl(data->expected_ptr);
            ASSERT_STR(data->expected_ptr, data->ret_ptr);
            reset_ptrs(&(data->ret_ptr), &(data->expected_ptr));
        }
    }
}
