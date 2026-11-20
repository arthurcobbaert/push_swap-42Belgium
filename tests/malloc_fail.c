/*
** Test-only malloc wrapper. push_swap is compiled with
**   -include tests/malloc_fail.h
** so every malloc() in the project goes through fail_malloc().
** Set FAIL_AT=N to make the N-th malloc return NULL.
** Set MALLOC_COUNT=1 to print how many mallocs ran (to stderr) at exit.
*/
#include <stdlib.h>
#include <stdio.h>

static int	g_calls = 0;

static void	report(void)
{
	fprintf(stderr, "MALLOC_CALLS=%d\n", g_calls);
}

void	*fail_malloc(size_t size)
{
	static int	fail_at = -2;

	if (fail_at == -2)
	{
		fail_at = getenv("FAIL_AT") ? atoi(getenv("FAIL_AT")) : -1;
		if (getenv("MALLOC_COUNT"))
			atexit(report);
	}
	g_calls++;
	if (g_calls == fail_at)
		return (NULL);
	return (malloc(size));
}
