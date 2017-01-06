#ifndef TSP_USA_DISTANCES_GENERATE_DISTANCES_H_
#define TSP_USA_DISTANCES_GENERATE_DISTANCES_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "usa/usa_utils.h"	/* misc utils */

/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
#ifndef M_PI
#	define M_PI 3.14159265358979323846
#endif /* ifndef M_PI */

#define EARTH_RADIUS 6371.0	/* km */

#define DISTANCES_BUFFER_SIZE						\
(50 * (LOCATION_LENGTH + (50 * (DISTANCE_PATH_DIGITS_MAX + 1)) + 1))



/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct Coordinates {
	double latitude;
	double longitude;
};

static inline bool
get_coordinates(struct Coordinates *const restrict coordinates,
		const char *restrict *const restrict buffer_ptr,
		const char *restrict *const restrict failure);

static inline void
init_locations_coordinates_maps(void);

static inline double
sin_sq(const double angle);

static inline unsigned int
haversine_distance(const struct Coordinates *const restrict coordinates1,
		   const struct Coordinates *const restrict coordinates2);

static inline void
put_node(const unsigned int i_node,
	 char *restrict *const restrict buffer_ptr);

static inline void
write_distances_file(const char *const restrict buffer,
		     const size_t size);


#endif /* ifndef TSP_USA_DISTANCES_GENERATE_DISTANCES_H_ */
