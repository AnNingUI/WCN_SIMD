#ifndef WCN_LOONGARCH_LASX_ATOMIC_H
#define WCN_LOONGARCH_LASX_ATOMIC_H

#include "../../wcn_atomic.h"

#ifdef WCN_LOONGARCH_LASX

/* LoongArch LASX (256-bit) atomic operations using scalar fallback
 * LASX inherits LSX atomic base and extends to 256-bit */

#include "wcn_loongarch_lsx_atomic.h"

/* Note: True 256-bit atomic operations are not available
 * Use scalar fallback for LASX vectors */

#endif /* WCN_LOONGARCH_LASX */

#endif /* WCN_LOONGARCH_LASX_ATOMIC_H */
