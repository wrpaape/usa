#ifndef TSP_USA_STRING_STRING_UTILS
#define TSP_USA_STRING_STRING_UTILS

/* external dependencies
 *─────────────────────────────────────────────────────────────────────────── */
#include "utils/utils.h"	/* bool, errno, stdlib, LIKELY, UNLIKELY */
#include <stdio.h>		/* sprintf */
#include <limits.h>		/* LONG_MAX, UINT_MAX */


inline size_t
string_length(const char *const restrict string)
{
	register const char *restrict ptr = string;

	while (*ptr != '\0')
		++ptr;

	return ptr - string;
}

inline bool
get_double(double *const restrict value,
	   const char *restrict *const restrict buffer_ptr,
	   const char *restrict *const restrict failure)
{
	const char *const restrict buffer = *buffer_ptr;

	*value = strtod(buffer,
			(char **restrict) buffer_ptr);

	if (buffer == *buffer_ptr) {
		*failure = "strtod failure: no conversion possible\n";
		return false;
	}

	switch (errno) {
	case 0:
		return true;

	case ERANGE:
		*failure = "strtod failure: Overflow or underflow occurred.\n";
		return false;

	default:
		*failure = "strtod failure: unknown\n";
		return false;
	}
}

inline bool
get_uint(unsigned int *const restrict value,
	 const char *restrict *const restrict buffer_ptr,
	 const char *restrict *const restrict failure)
{
	const long retval = strtol(*buffer_ptr,
				   (char **restrict) buffer_ptr,
				   10);

	if (retval < 0) {
		*failure = "get_uint failure: strtol returned negative.\n";
		return false;

	} else if (retval == 0) {
		switch (errno) {
		case 0:
			break;

		case EINVAL:
			*failure = "strtol failure: The value of 'base' is not "
				   "supported or no conversion could be "
				   "performed.\n";
			return false;

		case ERANGE:
			*failure = "strtol failure: Overflow occurred.\n";
			return false;

		default:
			*failure = "strtol failure: unknown\n";
			return false;
		}

#if (LONG_MAX > UINT_MAX)
	} else if (retval > UINT_MAX) {
		*failure = "get_uint failure: Overflow occurred.\n";
		return false;
#endif /* if (LONG_MAX > UINT_MAX) */
	}

	*value = (unsigned int) retval;
	return true;
}

inline void
put_uint(char *restrict *const restrict buffer_ptr,
	 const unsigned int value)
{
	char *const restrict buffer = *buffer_ptr;

	*buffer_ptr = buffer + sprintf(buffer,
				       "%u",
				       value);
}

#endif	/* TSP_USA_STRING_STRING_UTILS */
