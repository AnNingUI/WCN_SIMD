#ifndef WCN_X86_AVX_ATOMIC_H
#define WCN_X86_AVX_ATOMIC_H

#include "../../wcn_atomic.h"

#ifdef WCN_X86_AVX

/* AVX atomic operations inherit from SSE2 atomic implementation
 * AVX doesn't add new atomic instructions for 128-bit vectors */

#include "wcn_x86_sse2_atomic.h"

/* All SSE2 atomic operations are available for AVX */

/* Note: For 256-bit AVX vectors, true atomic operations are not available
 * Use the existing scalar fallback approach from SSE2 */

#endif /* WCN_X86_AVX */

#endif /* WCN_X86_AVX_ATOMIC_H */
