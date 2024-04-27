/**
 * @file simulation.h
 * @author Thomas Roiseux (thomas.roiseux@mathquantlab.com)
 * @brief Provides function to run simulation on CPU
 * @version 1.0
 * @date 2024-04-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "../headers/pch.h"
#include "../headers/nmc.h"

#include <map>

/**
 * @brief Provides simulation functions on CPU
 *
 */
namespace CPUSimulation
{
    /**
     * @brief Run the simulation on CPU
     *
     * @param xva XVA types
     * @param m0 Number of external paths
     * @param m1 Number of internal paths
     * @param nb_points Number of points
     * @param T Time horizon
     * @param external_paths External paths simulated
     * @param paths Paths simulated
     */
    void run_simulation(std::map<XVA, double> xva,
                        double m0, double m1,
                        size_t nb_points, double T,
                        std::map<XVA, std::vector<Vector>> &external_paths,
                        std::map<XVA, std::vector<std::vector<Vector>>> &paths);
}