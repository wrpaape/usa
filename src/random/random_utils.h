#ifndef TSP_USA_RANDOM_RANDOM_UTILS_H_
#define TSP_USA_RANDOM_RANDOM_UTILS_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <time.h>			/* unique seed */
#include <stdbool.h>			/* true, false */
#include "random/pcg_random.h"		/* psuedorandom number generator */
#include "time/time_utils.h"		/* time_report */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
typedef pcg32_random_t rng32_t;
typedef pcg64_random_t rng64_t;


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
extern rng32_t glob_rng32; /* global random number generator state */
extern rng64_t glob_rng64; /* global random number generator state */


/* helper macros
 * ────────────────────────────────────────────────────────────────────────── */
#define RANDOM_THRESHOLD(SPAN) (-(SPAN) % (SPAN))

#define RANDOM_UINT32_THRESHOLD(SPAN)					\
((uint32_t) (-((uint32_t) (SPAN))) % ((uint32_t) (SPAN)))

#define RANDOM_UINT64_THRESHOLD(SPAN)					\
((uint64_t) (-((uint64_t) (SPAN))) % ((uint64_t) (SPAN)))


/* constructors, destructors
 * ────────────────────────────────────────────────────────────────────────── */
inline bool
random_32_constructor(const char *restrict *const restrict failure)
{
	time_t now;
	const bool success = time_report(&now,
					 failure);

	if (success) {
		const uint64_t seed32 = (const uint64_t) now;

		pcg32_srandom_r(&glob_rng32,
				&seed32);
	}

	return success;
}

inline bool
random_64_constructor(const char *restrict *const restrict failure)
{
	time_t now;
	const bool success = time_report(&now,
					 failure);

	if (success) {
		const uint128_t seed64 = UINT128_INITIALIZER(now,
							     now);
		pcg64_srandom_r(&glob_rng64,
				&seed64);
	}

	return success;
}

inline bool
random_32_64_constructor(const char *restrict *const restrict failure)
{
	time_t now;
	const bool success = time_report(&now,
					 failure);

	if (success) {
		const uint64_t seed32 = (const uint64_t) now;

		pcg32_srandom_r(&glob_rng32,
				&seed32);

		const uint128_t seed64 = UINT128_INITIALIZER(seed32,
							     seed32);
		pcg64_srandom_r(&glob_rng64,
				&seed64);
	}

	return success;
}


inline uint32_t
random_uint32(void)
{
	return pcg32_random_r(&glob_rng32);
}

inline uint64_t
random_uint64(void)
{
	return pcg64_random_r(&glob_rng64);
}

inline bool
coin_flip(void)
{
	return (bool) (random_uint32() & 1u);
}

inline uint32_t
random_uint32_threshold(const uint32_t span)
{
	return -span % span;
}

inline uint64_t
random_uint64_threshold(const uint64_t span)
{
	return -span % span;
}

inline uint32_t
random_uint32_bound(const uint32_t threshold,
		    const uint32_t span)
{
	uint32_t random;

	do {
		random = random_uint32();
	} while (random < threshold);

	return random % span;
}


inline uint64_t
random_uint64_bound(const uint64_t threshold,
		    const uint64_t span)
{
	uint64_t random;

	do {
		random = random_uint64();
	} while (random < threshold);

	return random % span;
}

inline uint32_t
random_uint32_bound_32_offset_32(const uint32_t threshold,
				 const uint32_t span,
				 const uint32_t offset)
{
	return random_uint32_bound(threshold,
				   span) + offset;
}

inline uint64_t
random_uint64_bound_32_offset_64(const uint32_t threshold,
				 const uint32_t span,
				 const uint64_t offset)
{
	return ((uint64_t) random_uint32_bound(threshold,
					       span)) + offset;
}

inline uint64_t
random_uint64_bound_64_offset_32(const uint64_t threshold,
				 const uint64_t span,
				 const uint32_t offset)
{
	return random_uint64_bound(threshold,
				   span) + ((uint64_t) offset);
}

inline uint64_t
random_uint64_bound_64_offset_64(const uint64_t threshold,
				 const uint64_t span,
				 const uint64_t offset)
{
	return random_uint64_bound(threshold,
				   span) + offset;
}


inline int32_t
random_int32_bound_32_offset_32(const uint32_t threshold,
				const uint32_t span,
				const int32_t offset)
{
	return random_uint32_bound(threshold,
				   span) + offset;
}

inline int64_t
random_int64_bound_32_offset_64(const uint32_t threshold,
				const uint32_t span,
				const int64_t offset)
{
	return ((uint64_t) random_uint32_bound(threshold,
					       span)) + offset;
}

inline int64_t
random_int64_bound_64_offset_32(const uint64_t threshold,
				const uint64_t span,
				const int32_t offset)
{
	return random_uint64_bound(threshold,
				   span) + ((int64_t) offset);
}

inline int64_t
random_int64_bound_64_offset_64(const uint64_t threshold,
				const uint64_t span,
				const int64_t offset)
{
	return random_uint64_bound(threshold,
				   span) + offset;
}



inline uint32_t
random_uint32_from(const uint32_t from)
{
	const uint32_t span = UINT32_MAX - from + 1u;

	return random_uint32_bound_32_offset_32(RANDOM_THRESHOLD(span),
						span,
						from);
}

inline uint64_t
random_uint64_from(const uint64_t from)
{
	const uint64_t span = UINT64_MAX - from + 1u;

	return random_uint64_bound_64_offset_64(RANDOM_THRESHOLD(span),
						span,
						from);
}

inline int32_t
random_int32_from(const int32_t from)
{
	const uint32_t span = INT32_MAX - from + 1u;

	return random_int32_bound_32_offset_32(RANDOM_THRESHOLD(span),
					       span,
					       from);
}

inline int64_t
random_int64_from(const int64_t from)
{
	const uint64_t span = INT64_MAX - from + 1u;

	return random_int64_bound_64_offset_64(RANDOM_THRESHOLD(span),
					       span,
					       from);
}


inline uint32_t
random_uint32_upto(const uint32_t upto)
{
	const uint32_t span = upto + 1u;

	return random_uint32_bound(RANDOM_THRESHOLD(span),
				   span);
}

inline uint64_t
random_uint64_upto(const uint64_t upto)
{
	const uint64_t span = upto + 1u;

	return random_uint64_bound(RANDOM_THRESHOLD(span),
				   span);
}

inline int32_t
random_int32_upto(const int32_t upto)
{
	const uint32_t span = upto + 1u - INT32_MIN;

	return random_int32_bound_32_offset_32(RANDOM_THRESHOLD(span),
					       span,
					       INT32_MIN);
}

inline int64_t
random_int64_upto(const int64_t upto)
{
	const uint64_t span = upto + 1u - INT64_MIN;

	return random_int64_bound_64_offset_64(RANDOM_THRESHOLD(span),
					       span,
					       INT64_MIN);
}

inline int32_t
random_int32_in_range(const int32_t lbound,
		      const int32_t rbound)
{
	const uint32_t span = rbound - lbound + 1u;

	return random_int32_bound_32_offset_32(RANDOM_THRESHOLD(span),
					       span,
					       lbound);
}

inline double
random_probability(void)
{
	return (((double) random_uint64()) / ((double) UINT64_MAX));
}

inline double
random_dbl_upto(const double rbound)
{
	return random_probability() * rbound;
}

inline double
random_dbl_bound_offset(const double delta,
			const double offset)
{
	return random_dbl_upto(delta) + offset;
}

inline double
random_dbl_in_range(const double lbound,
		    const double rbound)
{
	return random_dbl_bound_offset(rbound - lbound,
				       lbound);
}
#endif /* ifndef TSP_USA_RANDOM_RANDOM_UTILS_H_ */
