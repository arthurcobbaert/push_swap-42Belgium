#ifndef MALLOC_FAIL_H
# define MALLOC_FAIL_H
# include <stddef.h>

void	*fail_malloc(size_t size);
# define malloc(x) fail_malloc(x)
#endif
