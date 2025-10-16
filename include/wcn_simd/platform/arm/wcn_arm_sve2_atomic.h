#ifndef WCN_ARM_SVE2_ATOMIC_H
#define WCN_ARM_SVE2_ATOMIC_H

#include "../../wcn_atomic.h"

#ifdef WCN_ARM_SVE2

/* ARM SVE2 atomic operations inherit from SVE atomic implementation
 * SVE2 doesn't add new atomic-specific instructions */

#include "wcn_arm_sve_atomic.h"

/* All SVE atomic operations are available for SVE2 */

#endif /* WCN_ARM_SVE2 */

#endif /* WCN_ARM_SVE2_ATOMIC_H */
