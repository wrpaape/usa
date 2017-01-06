#include "signal_utils.h"

/* API
 * ────────────────────────────────────────────────────────────────────────── */
extern inline void
signal_muffle(sig_t *const restrict prev_action,
	      const int name,
	      const sig_t action);
extern inline bool
signal_report(sig_t *const restrict prev_action,
	      const int name,
	      const sig_t action,
	      const char *restrict *const restrict failure);
