#ifndef TSP_USA_TIME_TIME_UTILS_H_
#define TSP_USA_TIME_TIME_UTILS_H_


/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <time.h>		/* time, time_t */
#include "utils/utils.h"	/* bool, LIKELY, UNLIKELY */


/* seconds since 00:00:00, January 1st, 1970 */
inline void
time_muffle(time_t *const restrict now)
{
	(void) time(now);
}


inline bool
time_report(time_t *const restrict now,
	    const char *restrict *const restrict failure)
{
	if (LIKELY(time(now) >= 0))
		return true;

	switch (errno) {
	case ENOSYS:
		*failure = "time failure: function not implemented\n";
		return false;
	case EFAULT:
		*failure = "time failure: An argument address referenced "
			   "invalid memory.\n";
		return false;
	default:
		*failure = "time failure: unknown\n";
		return false;
	}
}
#endif /* ifndef TSP_USA_TIME_TIME_UTILS_H_ */
