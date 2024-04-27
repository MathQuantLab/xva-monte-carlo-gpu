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

void CPUSimulation::run_simulation(std::map<XVA, double> xvas,
                                   double m0, double m1,
                                   size_t nb_points, double T,
                                   std::map<ExternalPaths, std::vector<Vector>> &external_paths,
                                   std::map<XVA, Matrix> &paths)
{
    NMC nmc(m0, m1, nb_points, T);

    std::cout << "Running simulation on CPU using at maximum " << std::thread::hardware_concurrency() << " threads" << std::endl;

    for (auto &external_path : external_paths)
    {
        external_path.second.resize(m0);
        for (size_t i = 0; i < external_path.second.size(); i++)
        {
            external_path.second[i].resize(nb_points);
        }
    }

    std::thread interest_thread(&NMC::generate_interest_rate_paths, &nmc, std::ref(external_paths[ExternalPaths::Interest]));
    std::thread fx_thread(&NMC::generate_fx_rate_paths, &nmc, std::ref(external_paths[ExternalPaths::FX]));
    std::thread equity_thread(&NMC::generate_equity_paths, &nmc, std::ref(external_paths[ExternalPaths::Equity]));

    interest_thread.join();
    fx_thread.join();
    equity_thread.join();

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