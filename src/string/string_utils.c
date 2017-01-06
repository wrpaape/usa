#include "string/string_utils.h"

extern inline size_t
string_length(const char *const restrict string);

extern inline bool
get_double(double *const restrict value,
	   const char *restrict *const restrict buffer_ptr,
	   const char *restrict *const restrict failure);

extern inline bool
get_uint(unsigned int *const restrict value,
	 const char *restrict *const restrict buffer_ptr,
	 const char *restrict *const restrict failure);

extern inline void
put_uint(char *restrict *const restrict buffer_ptr,
	 const unsigned int value);
