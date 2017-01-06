#include "utils/utils.h"

/* malloc */
extern inline void
malloc_muffle(void *restrict *const restrict buffer,
	      const size_t size);
extern inline bool
malloc_report(void *restrict *const restrict buffer,
	      const size_t size,
	      const char *restrict *const restrict failure);
