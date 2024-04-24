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

void NMC::run(XVA xva, std::vector<std::vector<Vector>>& paths) const
{
    paths.resize(m0);
    for (size_t i = 0; i < paths.size(); i++)
    {
        paths[i].resize(m1);
        for (size_t j = 0; j < paths[i].size(); j++)
        {
            paths[i][j].resize(nb_points);
        }
    }
}