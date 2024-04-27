/**
 * @file nmc.cpp
 * @author Thomas Roiseux (thomas.roiseux@@mathquantlab.com)
 * @brief Implements {@link nmc.h}
 * @version 0.1
 * @date 2024-04-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "../headers/nmc.h"

#include <iostream>
#include <thread>

void NMC::run(XVA xva, double factor, const std::map<ExternalPaths, std::vector<Vector>> &external_paths, Matrix &paths) const
{
    std::cout << "Running NMC for XVA " << xva << " on thread " << std::this_thread::get_id() << " with factor " << factor << std::endl;
    paths.resize(m1);
    for (size_t i = 0; i < paths.size(); i++)
    {
        paths[i].resize(nb_points);
    }
}