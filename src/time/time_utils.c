#include "time/time_utils.h"

/* seconds since 00:00:00, January 1st, 1970 */
extern inline void
time_muffle(time_t *const restrict now);
extern inline bool
time_report(time_t *const restrict now,
	    const char *restrict *const restrict failure);
