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

int CUDA::Utils::get_gpu_id()
{
    int device;
    cudaGetDevice(&device);
    return device;
}

const char *CUDA::CUDAException::what() const noexcept
{
    std::string error_string = cudaGetErrorString(m_error);
    return (m_message + "\n" + error_string).c_str();
}

CUDA::GridBlockSize CUDA::Utils::get_grid_size()
{
    int id = get_gpu_id();

    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, id);

    return {prop.maxGridSize[0], prop.maxGridSize[1], prop.maxGridSize[2]};
}

CUDA::GridBlockSize CUDA::Utils::get_grid_size(int device_id)
{
    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, device_id);

    return {prop.maxGridSize[0], prop.maxGridSize[1], prop.maxGridSize[2]};
}

CUDA::GridBlockSize CUDA::Utils::get_block_size()
{
    int id = get_gpu_id();

    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, id);

    return {prop.maxThreadsDim[0], prop.maxThreadsDim[1], prop.maxThreadsDim[2]};
}

CUDA::GridBlockSize CUDA::Utils::get_block_size(int device_id)
{
    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, device_id);

    return {prop.maxThreadsDim[0], prop.maxThreadsDim[1], prop.maxThreadsDim[2]};
}