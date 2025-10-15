#ifndef WCN_X86_SSE4_1_ATOMIC_H
#define WCN_X86_SSE4_1_ATOMIC_H

#include "../../wcn_atomic.h"

#ifdef WCN_X86_SSE4_1

/* SSE4.1 atomic operations inherit from SSE2 atomic implementation
 * SSE4.1 doesn't add atomic-specific instructions, so we use SSE2 base */

#include "wcn_x86_sse2_atomic.h"

/* All SSE2 atomic operations are available for SSE4.1 */

#endif /* WCN_X86_SSE4_1 */

#endif /* WCN_X86_SSE4_1_ATOMIC_H */
