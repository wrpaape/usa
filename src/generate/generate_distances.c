#include "generate/generate_distances.h"


static const char *locations_map[50];
static struct Coordinates coordinates_map[50];
static const struct Coordinates *const restrict coordinates_map_until
= &coordinates_map[50];


int
main(void)
{
	static char distances_buffer[DISTANCES_BUFFER_SIZE];
	char *restrict buffer;
	unsigned int i_node;

	init_locations_coordinates_maps();

	buffer = &distances_buffer[0];

	i_node = 0u;

	while (1) {
		put_node(i_node,
			 &buffer);

		++i_node;
		if (i_node == 50u)
			break;

		*buffer = '\n';
		++buffer;
	}

	write_distances_file(&distances_buffer[0],
			     buffer - &distances_buffer[0]);

	return 0;
}

static inline bool
get_coordinates(struct Coordinates *const restrict coordinates,
		const char *restrict *const restrict buffer_ptr,
		const char *restrict *const restrict failure)
{
	bool success;

	success = get_double(&coordinates->latitude,
			     buffer_ptr,
			     failure);

	if (LIKELY(success)) {
		coordinates->latitude *= (M_PI / 180.0); /* radians */

		++(*buffer_ptr); /* skip ',' */

		success = get_double(&coordinates->longitude,
				     buffer_ptr,
				     failure);

		if (LIKELY(success))
			coordinates->longitude *= (M_PI / 180.0); /* radians */
	}

	return success;
}


static inline void
init_locations_coordinates_maps(void)
{
	const char *restrict failure;
	char *restrict buffer;
	const char *restrict ptr;
	const char *restrict *restrict location;
	struct Coordinates *restrict coordinates;

	if (UNLIKELY(!read_file(&buffer,
				COORDINATES_PATH,
				&failure)))
		exit_on_failure(failure);

	location    = &locations_map[0];
	coordinates = &coordinates_map[0];

	ptr = buffer;

	while (1) {
		get_location(location,
			     &ptr);

		if (UNLIKELY(!get_coordinates(coordinates,
					      (const char **) &ptr,
					      &failure))) {
			free(buffer);
			exit_on_failure(failure);
		}

		++coordinates;
		if (coordinates == coordinates_map_until) {
			free(buffer);
			return;
		}

		++location;
		++ptr;		/* newline */
	}
}

static inline double
sin_sq(const double angle)
{
	const double sin_angle = sin(angle);

	return sin_angle * sin_angle;
}

static inline unsigned int
haversine_distance(const struct Coordinates *const restrict coordinates1,
		   const struct Coordinates *const restrict coordinates2)
{
	const double h	/* hav(d/r) */
	= sin_sq((coordinates2->latitude - coordinates1->latitude) / 2.0)
	+ (  cos(coordinates1->latitude)
	   * cos(coordinates2->latitude))
	   * sin_sq((coordinates2->longitude - coordinates1->longitude) / 2.0);


	const double asin_sqrt_h = atan2(sqrt(h),
					 sqrt(1.0 - h));

	return (unsigned int) (2.0 * EARTH_RADIUS * asin_sqrt_h);
}

static inline void
put_node(const unsigned int i_node,
	 char *restrict *const restrict buffer_ptr)
{
	char *restrict buffer;
	const struct Coordinates *restrict coordinates;

	buffer = *buffer_ptr;

	put_location(&buffer,
		     locations_map[i_node]);

	const struct Coordinates *const restrict node_coordinates
	= &coordinates_map[i_node];

	coordinates = &coordinates_map[0];

	while (1) {
		*buffer = '\t';
		++buffer;

		if (coordinates == node_coordinates)
			break;

		put_uint(&buffer,
			 haversine_distance(node_coordinates,
					    coordinates));
		++coordinates;
	}

	*buffer = '0';
	++buffer;

	while (1) {
		++coordinates;
		if (coordinates == coordinates_map_until)
			break;

		*buffer = '\t';
		++buffer;

		put_uint(&buffer,
			 haversine_distance(node_coordinates,
					    coordinates));
	}

	*buffer_ptr = buffer;
}

static inline void
write_distances_file(const char *const restrict buffer,
		     const size_t size)
{
	const char *restrict failure;

	if (LIKELY(write_file(buffer,
			      size,
			      DISTANCES_PATH,
			      &failure)))
		return;

	exit_on_failure(failure);
	__builtin_unreachable();
}

