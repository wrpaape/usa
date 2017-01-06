#include "usa.h"

static unsigned int distance_map[50][50];
static const char *location_map[50];
char *restrict distances_buffer;
static bool stop_evaluation;
static unsigned int total_distance;

static unsigned int solution[100];

static sig_t initial_handler;

void
catch_interrupt(int signal)
{
	const char *restrict failure;

	if (UNLIKELY(!signal_report(&initial_handler,
				    SIGINT,
				    initial_handler,
				    &failure))) {
		free(distances_buffer);
		exit_on_failure(failure);
	}

	stop_evaluation = true;
}

static inline void
get_distance_row(unsigned int *restrict distance,
		 unsigned int *const restrict distance_until,
		 const char *restrict *const restrict buffer_ptr)
{
	const char *restrict failure;

	do {
		if (UNLIKELY(!get_uint(distance,
				       buffer_ptr,
				       &failure))) {
			free(distances_buffer);
			exit_on_failure(failure);
		}

		++(*buffer_ptr); /* skip '\t' or final '\n' */
		++distance;
	} while (distance < distance_until);
}


static inline void
init_tsp_state(void)
{
	unsigned int node;
	unsigned int next_node;
	unsigned int *restrict row;
	unsigned int *restrict next_row;
	unsigned int *restrict node_ptr;
	unsigned int *restrict distance_ptr;
	const char *restrict ptr;
	const char *restrict failure;
	const char *restrict *restrict location;

	if (UNLIKELY(!read_file(&distances_buffer,
				DISTANCES_PATH,
				&failure)))
	    exit_on_failure(failure);

	ptr	     = distances_buffer;
	location     = &location_map[0];
	row	     = &distance_map[0][0];
	next_row     = row + 50;
	node         = 0u;
	next_node    = 1u;
	node_ptr     = &solution[0];
	distance_ptr = node_ptr + 1;

	do {
		get_location(location,
			     &ptr);

		get_distance_row(row,
				 next_row,
				 (const char **) &ptr);

		*node_ptr	= node;
		*distance_ptr	= row[next_node];
		total_distance += *distance_ptr;

		++location;
		node_ptr     += 2;
		distance_ptr += 2;
		row	      = next_row;
		next_row     += 50;
		node	      = next_node;
		++next_node;
	} while (next_node < 50);

	get_location(location,
		     &ptr);

	get_distance_row(row,
			 next_row,
			 (const char **) &ptr);

	*node_ptr	= node;
	*distance_ptr	= row[0];
	total_distance += *distance_ptr;

	if (UNLIKELY(!(   signal_report(&initial_handler,
					SIGINT,
					&catch_interrupt,
					&failure)
		       && random_32_64_constructor(&failure)))) {
		free(distances_buffer);
		exit_on_failure(failure);
	}
}

static inline void
put_path(const unsigned int node1,
	 const unsigned int node2,
	 const unsigned int distance,
	 char *restrict *const restrict buffer_ptr)
{
	char *restrict buffer;

	buffer = *buffer_ptr;

	put_location(&buffer,
		     location_map[node1]);

	*buffer = '\t'; ++buffer;
	*buffer = 't';	++buffer;
	*buffer = 'o';	++buffer;
	*buffer = '\t'; ++buffer;

	put_location(&buffer,
		     location_map[node2]);

	*buffer = '\t'; ++buffer;

	put_uint(&buffer,
		 distance);

	*buffer = ' ';	++buffer;
	*buffer = 'k';	++buffer;
	*buffer = 'm';	++buffer;
	*buffer = '\n'; ++buffer;

	*buffer_ptr = buffer;
}


static inline void
put_total(char *restrict *const restrict buffer_ptr)
{
	char *restrict buffer;

	buffer = *buffer_ptr;

	*buffer = '\n';	++buffer;
	*buffer = 't';	++buffer;
	*buffer = 'o';	++buffer;
	*buffer = 't';	++buffer;
	*buffer = 'a';	++buffer;
	*buffer = 'l';	++buffer;
	*buffer = ':';	++buffer;
	*buffer = ' ';	++buffer;

	put_uint(&buffer,
		 total_distance);

	*buffer = ' ';	++buffer;
	*buffer = 'k';	++buffer;
	*buffer = 'm';	++buffer;
	*buffer = '\n';	++buffer;

	*buffer_ptr = buffer;
}



static inline void
write_solution(void)
{
	const unsigned int *restrict node1_ptr;
	const unsigned int *restrict node2_ptr;
	const unsigned int *restrict distance_ptr;
	char *restrict ptr;
	const char *restrict failure;
	static char buffer[SOLUTION_BUFFER_SIZE];

	node1_ptr    = &solution[0];
	distance_ptr = node1_ptr + 1;
	node2_ptr    = node1_ptr + 2;

	const unsigned int *const restrict node_until
	= node1_ptr + 100;

	ptr = &buffer[0];

	do {
		put_path(*node1_ptr,
			 *node2_ptr,
			 *distance_ptr,
			 &ptr);

		distance_ptr  += 2;
		node1_ptr  = node2_ptr;
		node2_ptr += 2;
	} while (node2_ptr < node_until);

	put_path(*node1_ptr,
		 solution[0],
		 *distance_ptr,
		 &ptr);

	free(distances_buffer);

	put_total(&ptr);

	if (UNLIKELY(!write_file(&buffer[0],
				 ptr - &buffer[0],
				 SOLUTION_PATH,
				 &failure)))
		exit_on_failure(failure);
}

static inline void
sample_node_indices(unsigned int *const restrict i_node1_ptr,
		    unsigned int *const restrict i_node2_ptr)
{
	uint32_t i_node2_div2;
	const uint32_t i_node1_div2 = random_uint32_upto(49u);

	do {
		i_node2_div2 = random_uint32_upto(49u);
	} while (i_node2_div2 == i_node1_div2);

	*i_node1_ptr = (unsigned int) (i_node1_div2 * 2u);
	*i_node2_ptr = (unsigned int) (i_node2_div2 * 2u);
}


static inline int
do_get_swap(unsigned int *restrict *const restrict swap_distance_ptr,
	    unsigned int *const restrict new_distance_ptr,
	    unsigned int *const restrict old_distance_ptr,
	    const unsigned int neighbor_node,
	    const unsigned int *const restrict distance_swap)
{
	*swap_distance_ptr = old_distance_ptr;

	*new_distance_ptr  = distance_swap[neighbor_node];

	return *new_distance_ptr - *old_distance_ptr;
}

static inline int
get_swap_prev(unsigned int *restrict *const restrict swap_distance_ptr,
	      unsigned int *const restrict new_distance_ptr,
	      unsigned int *const restrict node_ptr,
	      const unsigned int *const restrict distance_swap)
{
	return do_get_swap(swap_distance_ptr,
			   new_distance_ptr,
			   node_ptr - 1,
			   node_ptr[-2],
			   distance_swap);
}

static inline int
get_swap_next(unsigned int *restrict *const restrict swap_distance_ptr,
	      unsigned int *const restrict new_distance_ptr,
	      unsigned int *const restrict node_ptr,
	      const unsigned int *const restrict distance_swap)
{
	return do_get_swap(swap_distance_ptr,
			   new_distance_ptr,
			   node_ptr + 1,
			   node_ptr[ 2],
			   distance_swap);
}

static inline int
get_swap_prev_wrap(unsigned int *restrict *const restrict swap_distance_ptr,
		   unsigned int *const restrict new_distance_ptr,
		   unsigned int *const restrict node_ptr,
		   const unsigned int *const restrict distance_swap)
{
	return do_get_swap(swap_distance_ptr,
			   new_distance_ptr,
			   node_ptr + 99,
			   node_ptr[98],
			   distance_swap);
}

static inline int
get_swap_next_wrap(unsigned int *restrict *const restrict swap_distance_ptr,
		   unsigned int *const restrict new_distance_ptr,
		   unsigned int *const restrict node_ptr,
		   const unsigned int *const restrict distance_swap)
{
	return do_get_swap(swap_distance_ptr,
			   new_distance_ptr,
			   node_ptr + 1,
			   node_ptr[-98],
			   distance_swap);
}


static inline int
swap_set_get(struct SwapSet *const restrict swap_set,
	     unsigned int i_node1,
	     unsigned int i_node2)
{
	unsigned int *restrict *restrict swap_distance_ptr;
	unsigned int *restrict new_distance_ptr;
	int delta_distance;

	if (i_node1 > i_node2) {
		const unsigned int i_tmp = i_node1;
		i_node1 = i_node2;
		i_node2 = i_tmp;
	}

	/* node1 comes before node2 */
	unsigned int *const restrict node1_ptr = &solution[i_node1];
	unsigned int *const restrict node2_ptr = &solution[i_node2];

	const unsigned int *const restrict distance_swap1
	= &distance_map[*node1_ptr][0];
	const unsigned int *const restrict distance_swap2
	= &distance_map[*node2_ptr][0];

	swap_set->node1_ptr = node1_ptr;
	swap_set->node2_ptr = node2_ptr;

	swap_distance_ptr = &swap_set->swap_distances[0];
	new_distance_ptr  = &swap_set->new_distances[0];

	if (i_node1 == 0) {
		if (i_node2 == 98) {
			delta_distance	= get_swap_next(swap_distance_ptr,
							new_distance_ptr,
							node1_ptr,
							distance_swap2);
			++swap_distance_ptr;
			++new_distance_ptr;

			delta_distance += get_swap_prev(swap_distance_ptr,
							new_distance_ptr,
							node2_ptr,
							distance_swap1);
			++swap_distance_ptr;
			*swap_distance_ptr = NULL;

		} else {
			delta_distance	= get_swap_prev_wrap(swap_distance_ptr,
							     new_distance_ptr,
							     node1_ptr,
							     distance_swap2);
			++swap_distance_ptr;
			++new_distance_ptr;

			delta_distance += get_swap_next(swap_distance_ptr,
							new_distance_ptr,
							node2_ptr,
							distance_swap1);
			++swap_distance_ptr;

			if (i_node2 == 2) {
				*swap_distance_ptr = NULL;

			} else {
				++new_distance_ptr;

				delta_distance += get_swap_next(swap_distance_ptr,
								new_distance_ptr,
								node1_ptr,
								distance_swap2);
				++swap_distance_ptr;
				++new_distance_ptr;

				delta_distance += get_swap_prev(swap_distance_ptr,
								new_distance_ptr,
								node2_ptr,
								distance_swap1);
			}
		}
	} else {
		delta_distance	= get_swap_prev(swap_distance_ptr,
						new_distance_ptr,
						node1_ptr,
						distance_swap2);
		++swap_distance_ptr;
		++new_distance_ptr;

		if (i_node2 == 98) {
			delta_distance += get_swap_next_wrap(swap_distance_ptr,
							     new_distance_ptr,
							     node2_ptr,
							     distance_swap1);

			++swap_distance_ptr;

			if (i_node1 == 96) {
				*swap_distance_ptr = NULL;

			} else {
				++new_distance_ptr;

				delta_distance += get_swap_next(swap_distance_ptr,
								new_distance_ptr,
								node1_ptr,
								distance_swap2);
				++swap_distance_ptr;
				++new_distance_ptr;

				delta_distance += get_swap_prev(swap_distance_ptr,
								new_distance_ptr,
								node2_ptr,
								distance_swap1);
			}
		} else {
			delta_distance += get_swap_next(swap_distance_ptr,
							new_distance_ptr,
							node2_ptr,
							distance_swap1);

			++swap_distance_ptr;

			if (i_node2 == (i_node1 + 2)) {
				*swap_distance_ptr = NULL;

			} else {
				++new_distance_ptr;

				delta_distance += get_swap_next(swap_distance_ptr,
								new_distance_ptr,
								node1_ptr,
								distance_swap2);
				++swap_distance_ptr;
				++new_distance_ptr;

				delta_distance += get_swap_prev(swap_distance_ptr,
								new_distance_ptr,
								node2_ptr,
								distance_swap1);
			}
		}
	}

	return delta_distance;
}

static inline void
swap_set_swap(struct SwapSet *const restrict swap_set)
{
	unsigned int *restrict *restrict swap_distance_ptr;
	unsigned int *restrict new_distance_ptr;

	const unsigned int node1 = *(swap_set->node1_ptr);
	*(swap_set->node1_ptr)	 = *(swap_set->node2_ptr);
	*(swap_set->node2_ptr)	 = node1;

	swap_distance_ptr = &swap_set->swap_distances[0];
	new_distance_ptr  = &swap_set->new_distances[0];

	**swap_distance_ptr = *new_distance_ptr;
	++swap_distance_ptr;
	++new_distance_ptr;

	**swap_distance_ptr = *new_distance_ptr;
	++swap_distance_ptr;

	if (*swap_distance_ptr != NULL) {
		++new_distance_ptr;

		**swap_distance_ptr = *new_distance_ptr;

		++swap_distance_ptr;
		++new_distance_ptr;
		**swap_distance_ptr = *new_distance_ptr;
	}
}



static inline bool
make_inferior_transition(const int delta_distance,
			 const double temperature)
{
	const double transition_probability
	= exp(((double) (delta_distance * EXPONENT_CONSTANT)) / temperature);

	/* printf("temperature: %f, transition_probability: %f\n", */
	/*        temperature, */
	/*        transition_probability); */

	return transition_probability > random_probability();
}


static inline void
evaluate(void)
{
	unsigned int i_node1;
	unsigned int i_node2;
	double temperature;
	int delta_distance;
	struct SwapSet swap_set;
	const char *restrict failure;


	/* static const struct timespec sleep_time = { */
	/* 	.tv_nsec = 10000000 */
	/* }; */
	/* struct timespec buffer_time; */

	temperature = INITIAL_TEMPERATURE;

	do {
		sample_node_indices(&i_node1,
				    &i_node2);

		delta_distance = swap_set_get(&swap_set,
					      i_node1,
					      i_node2);

		if (   (delta_distance < 0)
		    || make_inferior_transition(delta_distance,
						temperature)) {
			total_distance += delta_distance;

			swap_set_swap(&swap_set);
		}

		/* (void) nanosleep(&sleep_time, */
		/* 		 &buffer_time); */

		temperature -= DELTA_TEMPERATURE;

	} while (!stop_evaluation && (temperature >= 0.0));


	if (UNLIKELY(!file_write_report(STDOUT_FILENO,
					"\ndone!\n",
					sizeof("\ndone!\n") - 1,
					&failure))) {
		free(distances_buffer);
		exit_on_failure(failure);
	}
}

int
main(void)
{
	init_tsp_state();

	evaluate();

	write_solution();

	return 0;
}

