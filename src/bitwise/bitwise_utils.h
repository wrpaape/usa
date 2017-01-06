#ifndef TSP_USA_BITWISE_BITWISE_UTILS_H_
#define TSP_USA_BITWISE_BITWISE_UTILS_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stdint.h>	/* uint<8|16|32|64>_t */


/* bitwise rotation */
inline uint8_t
uint8_rotate_left(const uint8_t value,
		  const unsigned int rotate)
{
	return (value << rotate) | (value >> ((-rotate) & 7));
}

inline uint8_t
uint8_rotate_right(const uint8_t value,
		   const unsigned int rotate)
{
	return (value >> rotate) | (value << ((-rotate) & 7));
}

inline uint16_t
uint16_rotate_left(const uint16_t value,
		   const unsigned int rotate)
{
	return (value << rotate) | (value >> ((-rotate) & 15));
}

inline uint16_t
uint16_rotate_right(const uint16_t value,
		    const unsigned int rotate)
{
	return (value >> rotate) | (value << ((-rotate) & 15));
}

inline uint32_t
uint32_rotate_left(const uint32_t value,
		   const unsigned int rotate)
{
	return (value << rotate) | (value >> ((-rotate) & 31));
}

inline uint32_t
uint32_rotate_right(const uint32_t value,
		    const unsigned int rotate)
{
	return (value >> rotate) | (value << ((-rotate) & 31));
}


inline uint64_t
uint64_rotate_left(const uint64_t value,
		   const unsigned int rotate)
{
	return (value << rotate) | (value >> ((-rotate) & 63));
}

inline uint64_t
uint64_rotate_right(const uint64_t value,
		    const unsigned int rotate)
{
	return (value >> rotate) | (value << ((-rotate) & 63));
}

#endif /* ifndef TSP_USA_BITWISE_BITWISE_UTILS_H_ */
