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

#include <map>

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
     * @param nb_points Number of points
     * @param T Time horizon
     */
    NMC(double m0, double m1, size_t nb_points, double T) : m0(m0), m1(m1), nb_points(nb_points), T(T) {}

    /**
     * @brief Destroy the NMC object
     * 
     */
    virtual ~NMC() = default;
    /**
     * @brief Run the nested Monte Carlo system.
     * 
     * @param xva XVA types
     * @param factor Factor
     * @param external_paths External paths simulated
     * @param paths Paths simulated
     */
    virtual void run(XVA xva, double factor, const std::map<ExternalPaths, std::vector<Vector>> &external_paths, Matrix& paths) const;

    /**
     * @brief Generate interrest rate paths
     * 
     * @param paths Paths generated
     */
    virtual void generate_interest_rate_paths(std::vector<Vector>& paths) const;

    /**
     * @brief Generate FX rate paths
     * 
     * @param paths Paths generated
     */
    virtual void generate_fx_rate_paths(std::vector<Vector>& paths) const;

    /**
     * @brief Generate equity paths
     * 
     * @param paths Paths generated
     */
    virtual void generate_equity_paths(std::vector<Vector>& paths) const;

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
     * @brief Get the nb_points object
     * 
     * @return size_t nb_points
     */
    size_t get_nb_points() const { return nb_points; };

    /**
     * @brief Get the T object
     * 
     * @return double T
     */
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
     * @brief Number of points
     * 
     */
    size_t nb_points;
    /**
     * @brief Time horizon
     * 
     */
    double T;
private:
    /**
     * @brief Generate internal paths
     * 
     * @param external_paths External paths
     * @param paths Internal paths
     */
    void generate_internal_paths(const Matrix& external_paths, Matrix& paths) const;
};