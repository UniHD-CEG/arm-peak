# ARM PEAK

This is a small program to measure the computational peak performance of embedded ARM processors with NEON SIMD extension.
It is tested with ARM Cortex A53 and Cortex A72 processors, used in Raspberry Pi 3B and Pi 4B.

To measure the single-precision, floating-point peak performance VMLA instructions with only on-register access are launched.
Memory access is explicitly avoided to have a unbiased computational peak performance.
The number of multiply-accumulates (MACs) is oriented on the number of MACs in a general matrix multiply (GEMM) for various sizes.

For small workloads side effects---like multi-threading overhead---are dominant, but for larger workloads the measured performance approximates the computational peak performance.
