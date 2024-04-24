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
    NMC() = delete;
    /**
     * @brief Destroy the NMC object
     * 
     */

    /**
     * @brief Construct a new NMC object
     * 
     * @param m0 Number of external paths
     * @param m1 Number of internal paths
     * @param df Data frame
     * @param nb_points Number of points
     * @param T Time horizon
     */
    NMC(double m0, double m1, const Utils::DoubleDataFrame& df, size_t nb_points, double T) : m0(m0), m1(m1), df(df), nb_points(nb_points), T(T) {}
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
protected:
    /**
     * @brief Number of external paths
     * 
     */
    double m0;
    /**
     * @brief Number of internal paths
     * 
     */
    double m1;
    /**
     * @brief Data frame
     * 
     */
    const Utils::DoubleDataFrame& df;
    /**
     * @brief Number of points
     * 
     */
    size_t nb_points;
    /**
     * @brief Time horizon
     * 
     */
    double T;
};