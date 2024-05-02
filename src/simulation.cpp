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
#include <thread>
#include <iostream>

void CPUSimulation::run_simulation(const std::map<XVA, double>& xvas,
                                   size_t m0, size_t m1,
                                   size_t nb_points, double T,
                                   std::map<ExternalPaths, std::vector<Vector>> &external_paths,
                                   std::map<XVA, Vector> &paths)
{
    NMC nmc(m0, m1, nb_points, T);

    external_paths[ExternalPaths::Interest] = std::vector<Vector>(m0);
    external_paths[ExternalPaths::FX] = std::vector<Vector>(m0);
    external_paths[ExternalPaths::Equity] = std::vector<Vector>(m0);

    std::thread interest_thread(&NMC::generate_interest_rate_paths, &nmc, std::ref(external_paths[ExternalPaths::Interest]));
    std::thread fx_thread(&NMC::generate_fx_rate_paths, &nmc, std::ref(external_paths[ExternalPaths::FX]));
    std::thread equity_thread(&NMC::generate_equity_paths, &nmc, std::ref(external_paths[ExternalPaths::Equity]));

    interest_thread.join();
    fx_thread.join();
    equity_thread.join();

    #ifdef DEBUG
        for (auto const &external_path : external_paths)
        {
            std::cout << "External path " << external_path.first << std::endl;
            std::cout << "External path size: " << external_path.second.size() << std::endl;
        }
    #endif

    std::cout << "Interest, FX and Equity paths generated" << std::endl;

    std::thread *threads = new std::thread[xvas.size()];

    for (auto const &xva : xvas)
    {
        threads[xva.first] = std::thread(&NMC::run, nmc, xva.first, xva.second, std::ref(external_paths), std::ref(paths[xva.first]));
    }
    for (auto const &xva : xvas)
    {
        threads[xva.first].join();
    }
    delete[] threads;
    threads = nullptr;
}