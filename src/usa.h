#ifndef TSP_USA_USA_H_
#define TSP_USA_USA_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "usa/usa_utils.h"		/* misc utils */
#include "random/random_utils.h"	/* random number generator */
#include "signal/signal_utils.h"	/* signal_report */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct SwapSet {
	unsigned int *restrict node1_ptr;
	unsigned int *restrict node2_ptr;
	unsigned int *restrict swap_distances[4];
	unsigned int new_distances[4];
};


/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
#define PATH_BUFFER_SIZE						\
(50 * ((2 * LOCATION_LENGTH) + DISTANCE_PATH_DIGITS_MAX + 9))

#define SOLUTION_BUFFER_SIZE						\
(PATH_BUFFER_SIZE + sizeof("\ntotal: km") + DISTANCE_TOTAL_DIGITS_MAX)

#define EXPONENT_CONSTANT	-1000
#define INITIAL_TEMPERATURE	1000000.0
#define DELTA_TEMPERATURE	0.01

void
catch_interrupt(int signal);

static inline void
get_distance_row(unsigned int *restrict distance,
		 unsigned int *const restrict distance_until,
		 const char *restrict *const restrict buffer_ptr);

static inline void
init_tsp_state(void);

static inline void
put_path(const unsigned int node1,
	 const unsigned int node2,
	 const unsigned int distance,
	 char *restrict *const restrict buffer_ptr);

static inline void
put_total(char *restrict *const restrict buffer_ptr);

static inline void
write_solution(void);

static inline void
sample_node_indices(unsigned int *const restrict i_node1_ptr,
		    unsigned int *const restrict i_node2_ptr);

static inline int
do_get_swap(unsigned int *restrict *const restrict swap_distance_ptr,
	    unsigned int *const restrict new_distance_ptr,
	    unsigned int *const restrict old_distance_ptr,
	    const unsigned int neighbor_node,
	    const unsigned int *const restrict distance_swap);
static inline int
get_swap_prev(unsigned int *restrict *const restrict swap_distance_ptr,
	      unsigned int *const restrict new_distance_ptr,
	      unsigned int *const restrict node_ptr,
	      const unsigned int *const restrict distance_swap);
static inline int
get_swap_next(unsigned int *restrict *const restrict swap_distance_ptr,
	      unsigned int *const restrict new_distance_ptr,
	      unsigned int *const restrict node_ptr,
	      const unsigned int *const restrict distance_swap);
static inline int
get_swap_prev_wrap(unsigned int *restrict *const restrict swap_distance_ptr,
		   unsigned int *const restrict new_distance_ptr,
		   unsigned int *const restrict node_ptr,
		   const unsigned int *const restrict distance_swap);
static inline int
get_swap_next_wrap(unsigned int *restrict *const restrict swap_distance_ptr,
		   unsigned int *const restrict new_distance_ptr,
		   unsigned int *const restrict node_ptr,
		   const unsigned int *const restrict distance_swap);
static inline int
swap_set_get(struct SwapSet *const restrict swap_set,
	     unsigned int i_node1,
	     unsigned int i_node2);

static inline void
swap_set_swap(struct SwapSet *const restrict swap_set);

static inline bool
make_inferior_transition(const int delta_distance,
			 const double temperature);

static inline void
evaluate(void);


#endif /* ifndef TSP_USA_USA_H_ */
