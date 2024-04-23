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
#include "../headers/utils.h"

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

    /**
     * @brief Get the m0 object
     * 
     * @return double m0
     */
    double get_m0() const { return m0; };

    /**
     * @brief Get the m1 object
     * 
     * @return double m1
     */
    double get_m1() const { return m1; };

    /**
     * @brief Get the df object
     * 
     * @return Utils::DoubleDataFrame& df
     */
    const Utils::DoubleDataFrame& get_df() { return df; };

    /**
     * @brief Get the nb_points object
     * 
     * @return size_t nb_points
     */
    size_t get_nb_points() const { return nb_points; };

    double get_T() const { return T; };
private:
    double m0;
    double m1;
    Utils::DoubleDataFrame& df;
    size_t nb_points;
    double T;
};