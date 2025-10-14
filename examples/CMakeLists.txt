# Examples for WCN_SIMD

add_executable(wcn_simd_example main.c)
target_link_libraries(wcn_simd_example PRIVATE WCN_SIMD)

add_executable(wcn_atomic_test atomic_test.c)
target_link_libraries(wcn_atomic_test PRIVATE WCN_SIMD)

# Copy examples to build output
set_target_properties(wcn_simd_example PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

set_target_properties(wcn_atomic_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Installation
install(TARGETS wcn_simd_example wcn_atomic_test
    RUNTIME DESTINATION bin
)
