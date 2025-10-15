#ifndef WCN_X86_SSE4_2_ATOMIC_H
#define WCN_X86_SSE4_2_ATOMIC_H

#include "../../wcn_atomic.h"

#ifdef WCN_X86_SSE4_2

/* SSE4.2 atomic operations inherit from SSE2 atomic implementation
 * SSE4.2 doesn't add atomic-specific instructions, so we use SSE2 base */

#include "wcn_x86_sse2_atomic.h"

/* All SSE2 atomic operations are available for SSE4.2 */

#endif /* WCN_X86_SSE4_2 */

#endif /* WCN_X86_SSE4_2_ATOMIC_H */
