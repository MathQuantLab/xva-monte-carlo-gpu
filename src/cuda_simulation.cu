/**
 * @file cuda_simulation.cu
 * @author Thomas Roiseux (thomas.roiseux@mathquantlab.com)
 * @brief Implements {@link cuda_simulation.h}
 * @version 1.0
 * @date 2024-04-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "../headers/cuda_simulation.h"

#include <curand_kernel.h>

void CUDA::Simulation::run_simulation(std::map<XVA, double> xva,
                    double m0, double m1,
                    size_t nb_points, double T,
                    std::map<ExternalPaths, std::vector<Vector>> &external_paths,
                    std::map<XVA, Vector> &paths)
{

}