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

void CUDA::Utils::select_gpu(int device_id)
{
    int device_count;
    cudaGetDeviceCount(&device_count);

    if (device_id < 0 || device_id >= device_count)
    {
        throw Exception("Invalid device id");
    }

    cudaError_t error = cudaSetDevice(device_id);

    if (error != cudaSuccess)
    {
        throw CUDA::CUDAException("Failed to select device", error);
    }
}