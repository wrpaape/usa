#include "usa/usa_utils.h"

extern inline void
exit_on_failure(const char *const restrict failure)
__attribute__((noreturn));

extern inline bool
read_file(char *restrict *const restrict buffer_ptr,
	  const char *const restrict path,
	  const char *restrict *const restrict failure);

extern inline bool
write_file(const char *const restrict buffer,
	   const ssize_t size,
	   const char *const restrict path,
	   const char *restrict *const restrict failure);

extern inline void
get_location(const char *restrict *const restrict location,
	     const char *restrict *const restrict buffer_ptr);

extern inline void
put_location(char *restrict *const restrict buffer_ptr,
	     const char *restrict location);
