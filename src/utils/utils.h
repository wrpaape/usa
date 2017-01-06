#ifndef TSP_USA_UTILS_UTILS_H_
#define TSP_USA_UTILS_UTILS_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stdlib.h>		/* malloc, free */
#include <stdbool.h>		/* bool */
#include <errno.h>		/* errno */

/* helper macros
 * ────────────────────────────────────────────────────────────────────────── */
#define LIKELY(BOOL)   __builtin_expect(BOOL, true)
#define UNLIKELY(BOOL) __builtin_expect(BOOL, false)


inline void
malloc_muffle(void *restrict *const restrict buffer,
	      const size_t size)
{
	*buffer = malloc(size);
}

inline bool
malloc_report(void *restrict *const restrict buffer,
	      const size_t size,
	      const char *restrict *const restrict failure)
{
	*buffer = malloc(size);

	const bool success = (*buffer != NULL);

	if (UNLIKELY(!success))
		*failure = "malloc failure: out of memory\n";

	return success;
}

#endif /* ifndef TSP_USA_UTILS_UTILS_H_ */
