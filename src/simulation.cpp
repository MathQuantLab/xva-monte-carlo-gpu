/**
 * @file simulation.cpp
 * @author Thomas Roiseux (thomas.roiseux@mathquantlab.com)
 * @brief Implements {@link simulation.h}
 * @version 1.0
 * @date 2024-04-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "../headers/simulation.h"

void CPUSimulation::run_simulation(std::map<XVA, double>,
                        double m0, double m1,
                        const Utils::DoubleDataFrame &df, size_t nb_points, double T,
                        std::map<XVA, std::vector<std::vector<Vector>>> &paths)
{
    NMC nmc(m0, m1, df, nb_points, T);
}