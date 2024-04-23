/**
 * @file nmc.h
 * @author Thomas Roiseux (thomas.roiseux@mathquantlab.com)
 * @brief Provides the nested Monte Carlo system.
 * @version 1.0
 * @date 2024-04-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#include "../headers/pch.h"

/**
 * @brief Provides the nested Monte Carlo system.
 * 
 */
class NMC
{
public:
    /**
     * @brief Construct a new NMC object
     * 
     */
    NMC();
    /**
     * @brief Destroy the NMC object
     * 
     */
    virtual ~NMC() = default;
    /**
     * @brief Run the nested Monte Carlo system.
     * 
     */
    virtual void run() const;
private:
    size_t nb_points;
};