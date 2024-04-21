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
namespace CUDA {
    /**
     * @brief CUDA utils functions
     * 
     */
    namespace Utils {
        /**
         * @brief Check if a GPU is available
         * 
         * @return true GPU is available
         * @return false GPU is not available
         */
        bool is_gpu_available() noexcept;
    }
}