#ifndef TSP_USA_USA_USA_UTILS_H_
#define TSP_USA_USA_USA_UTILS_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "string/string_utils.h"	/* string_length, stdlib */
#include "file/file_utils.h"		/* file utils */
#include <math.h>			/* sin, cos, atan2, M_PI, exp */


/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
#define LOCATION_LENGTH			26	/* Charleston, West Virginia */
#define DISTANCE_PATH_DIGITS_MAX	4	/* thousands of km */
#define DISTANCE_TOTAL_DIGITS_MAX	6	/* hundred-thousands of km */

inline void
exit_on_failure(const char *const restrict failure)
{
	file_write_muffle(STDERR_FILENO,
			  failure,
			  string_length(failure));
	exit(EXIT_FAILURE);
}


inline bool
read_file(char *restrict *const restrict buffer_ptr,
	  const char *const restrict path,
	  const char *restrict *const restrict failure)
{
	bool success;
	int file;
	void *restrict buffer;
	struct StatBuffer stat_buffer;

	success = file_open_report(&file,
				   path,
				   O_RDONLY,
				   failure);

	if (LIKELY(success)) {
		success = fstat_report(file,
				       &stat_buffer,
				       failure);

		if (LIKELY(success)) {
			success = malloc_report(&buffer,
						stat_buffer.st_size,
						failure);

			if (LIKELY(success)) {
				success = file_read_report(file,
							   buffer,
							   stat_buffer.st_size,
							   failure);

				if (LIKELY(success)) {
					success = file_close_report(file,
								    failure);

					if (LIKELY(success))
						*buffer_ptr = (char *) buffer;
					else
						free(buffer);

				} else {
					free(buffer);
					file_close_muffle(file);
				}
			} else {
				file_close_muffle(file);
			}
		} else {
			file_close_muffle(file);
		}
	}

	return success;
}


inline bool
write_file(const char *const restrict buffer,
	   const ssize_t size,
	   const char *const restrict path,
	   const char *restrict *const restrict failure)
{
	bool success;
	int file;

	success = file_open_mode_report(&file,
					path,
					O_CREAT | O_TRUNC | O_WRONLY,
					S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH,
					failure);

	if (LIKELY(success)) {
		success = file_write_report(file,
					    buffer,
					    size,
					    failure);

		if (LIKELY(success))
			success = file_close_report(file,
						    failure);
		else
			file_close_muffle(file);
	}

	return success;
}

inline void
get_location(const char *restrict *const restrict location,
	     const char *restrict *const restrict buffer_ptr)
{
	const char *restrict buffer;

	buffer    = *buffer_ptr;
	*location = buffer;

	do {
		++buffer;
	} while (*buffer != '\t');

	*buffer_ptr = buffer;
}

inline void
put_location(char *restrict *const restrict buffer_ptr,
	     const char *restrict location)
{
	char *restrict buffer;

	buffer = *buffer_ptr;

	do {
		*buffer = *location;
		++buffer;
		++location;
	} while (*location != '\t');

	*buffer_ptr = buffer;
}



#endif /* ifndef TSP_USA_USA_USA_UTILS_H_ */
