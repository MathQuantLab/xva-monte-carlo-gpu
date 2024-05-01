/**
 * @file cuda_simulation.h
 * @author Thomas Roiseux (thomas.roiseux@mathquantlab.com)
 * @brief Holds functions to run simulation on GPU
 * @version 1.0
 * @date 2024-04-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "../headers/pch.h"

namespace CUDA
{
    /**
     * @brief Provides simulation functions on CUDA
     * 
     */
    namespace Simulation
    {
        /**
         * @brief Run the simulation on GPU
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
                            std::map<ExternalPaths, std::vector<Vector>> &external_paths,
                            std::map<XVA, Vector> &paths);
    }
}