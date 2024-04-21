/**
 * @file pch.h
 * @author Thomas Roiseux (thomas.roiseux@mathquantlab.com)
 * @brief Precompiled header
 * @version 0.1
 * @date 2024-04-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <cuda.h>
#include <cuda_runtime.h>

#include <string>
#include <stdexcept>
#include <exception>
#include <thread>
#include <coroutine>
#include <future>
#include <random>
#include <filesystem>

/**
 * @brief Exception class
 * 
 */
class Exception : public std::exception
{
public:
    /**
     * @brief Construct a new Exception object
     * 
     * @param message Exception message
     */
    Exception(const std::string& message) : m_message(message) {}

    /**
     * @brief Destroy the Exception object
     * 
     */
    virtual ~Exception()  = default;

    /**
     * @brief Get the exception message
     * 
     * @return const char* Exception message
     */
    virtual const char* what() const noexcept override { return m_message.c_str(); }
private:
    std::string m_message;
};