/**
 * @file cuda_utils.cu
 * @author Thomas Roiseux (thomas.roiseux@mathquantlab.com)
 * @brief Implments {@link cuda_utils.h}
 * @version 1.0
 * @date 2024-04-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "../headers/cuda_utils.h"

bool CUDA::Utils::is_gpu_available() noexcept
{
    int device_count;
    cudaGetDeviceCount(&device_count);
    return device_count > 0;
}