/**
 * @file utils.h
 * @author Thomas Roiseux (thomas.roiseux@mathquantlab.com)
 * @brief Provides utility functions
 * @version 0.1
 * @date 2024-04-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "../headers/pch.h"

/**
 * @brief Provides utility functions
 * 
 */
namespace Utils
{
    /**
     * @brief Application information
     *
     * @param name Application name
     */
    void info(const char *name);

    /**
     * @brief Parse command line arguments
     *
     * @param argc Number of arguments
     * @param argv Arguments
     * @param gpu GPU flag
     */
    void parse_arguments(int argc, char *argv[], bool &gpu);
}