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
#include <unistd.h>

CTEST(stdin_test, basic)
{
	FILE *fd = fopen("./tests/results/actual_stdin", "a");
	int		j;
	char	*line = 0;
	int ret;
	j = 1;
	ret = get_next_line(0, &line);
	while (ret > 0)
	{
		fwrite("|", 1, 1, fd);
		fwrite(line, 1, strlen(line), fd);
		fwrite("|", 1, 1, fd);
		fwrite("\n", 1, 1, fd);
		free(line);
		j++;
		ret = get_next_line(0, &line);
	}
	if (ret == 0)
	{
		fwrite("|", 1, 1, fd);
		fwrite(line, 1, strlen(line), fd);
		fwrite("|", 1, 1, fd);
		fwrite("\n", 1, 1, fd);
	}
	free(line);
	fwrite("\n\n", 1, 2, fd);
}

int main(int argc, const char *argv[])
{
	return ctest_main(argc, argv);
}
