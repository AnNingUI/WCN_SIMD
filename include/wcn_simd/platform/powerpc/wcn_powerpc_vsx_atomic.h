#ifndef WCN_POWERPC_VSX_ATOMIC_H
#define WCN_POWERPC_VSX_ATOMIC_H

#include "../../wcn_atomic.h"

#ifdef WCN_POWERPC_VSX

/* PowerPC VSX atomic operations inherit from AltiVec atomic implementation
 * VSX extends AltiVec but doesn't add new atomic-specific instructions */

#include "wcn_powerpc_altivec_atomic.h"

/* All AltiVec atomic operations are available for VSX */

/* Additional VSX-specific atomic operations for double precision */

WCN_INLINE vector double wcn_vsx_atomic_load_f64(const vector double* ptr, 
                                                  wcn_memory_order_t order) {
    wcn_altivec_memory_fence(order);
    
    volatile double* src = (volatile double*)ptr;
    vector double result;
    double* dst = (double*)&result;
    
    for (int i = 0; i < 2; i++) {
        dst[i] = src[i];
    }
    
    return result;
}

WCN_INLINE void wcn_vsx_atomic_store_f64(vector double* ptr, vector double value,
                                         wcn_memory_order_t order) {
    volatile double* dst = (volatile double*)ptr;
    double* src = (double*)&value;
    
    for (int i = 0; i < 2; i++) {
        dst[i] = src[i];
    }
    
    wcn_altivec_memory_fence(order);
}

#endif /* WCN_POWERPC_VSX */

#endif /* WCN_POWERPC_VSX_ATOMIC_H */
