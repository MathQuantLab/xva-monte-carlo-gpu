/**
 * @file cuda_utils.h
 * @author Thomas Roiseux (thomas.roiseux@mathquantlab.com)
 * @brief Holds CUDA utilities
 * @version 1.0
 * @date 2024-04-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "../headers/pch.h"

/**
 * @brief All CUDA functions
 *
 */
namespace CUDA
{
    /**
     * @brief CUDA utils functions
     *
     */
    namespace Utils
    {
        /**
         * @brief Check if a GPU is available
         *
         * @return true GPU is available
         * @return false GPU is not available
         */
        bool is_gpu_available() noexcept;

        /**
         * @brief Select a GPU
         *
         * @param device_id GPU device id
         * @throws CUDAException If the id is invalid or if the device cannot be selected
         */
        void select_gpu(int device_id);
    };

    /**
     * @brief CUDA exception
     * 
     */
    class CUDAException : public Exception
    {
    public:
        /**
         * @brief Construct a new CUDAException object
         *
         * @param message Exception message
         * @param error CUDA error
         */
        CUDAException(const std::string &message, cudaError_t error) : Exception(message), m_error(error) {}

        /**
         * @brief Destroy the CUDAException object
         *
         */
        virtual ~CUDAException() = default;

        /**
         * @brief Get the CUDA error
         *
         * @return cudaError_t CUDA error
         */
        cudaError_t get_error() const noexcept { return m_error; }

        /**
         * @brief Get the exception message
         * 
         * @return const char* Exception message
         */
        virtual const char *what() const noexcept override;
    private:
        cudaError_t m_error;
    };
}