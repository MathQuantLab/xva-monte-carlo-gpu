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

/**
 * @brief Generate a sample from a Gaussian distribution
 * 
 * @param mean 
 * @param std_dev 
 * @param state 
 * @return double Gaussian sample
 */
__device__ double generate_gaussian_sample(double mean, double std_dev, curandState *state)
{
    return mean + std_dev * curand_normal_double(state);
}

/**
 * @brief Generate the external path for the interest rate on GPU
 * 
 * @param paths External paths
 * @param m0 Number of paths
 * @param N Size of each path
 * @param T Time horizon
 */
__global__ void generate_external_path_interest_rate(double **paths, size_t *m0, size_t *N, double *T)
{
    double r0 = 0.03;
    double k(0.5);
    double theta = 0.04;
    double sigma = 0.1;

    double dt = *T / *N;

    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    curandState state;
    curand_init(1234, idx, 0, &state);

    if (idx < *m0)
    {
        paths[idx][0] = r0;
        for (size_t i = 1; i < *N; i++)
        {
            double dW = generate_gaussian_sample(0, sqrt(dt), &state);
            paths[idx][i] = paths[idx][i - 1] + k * (theta - paths[idx][i - 1]) * dt + sigma * dW * sqrt(paths[idx][i - 1]);
        }
    }
}

/**
 * @brief Generate the external path for the FX rate on GPU
 * 
 * @param paths External paths
 * @param m0 Number of paths
 * @param N Size of each path
 * @param T Time horizon
 */
__global__ void generate_external_path_fx(double **paths, size_t *m0, size_t *N, double *T)
{
    double S0 = 1.15;
    double mu = 0.02;
    double sigma = 0.1;

    double dt = *T / *N;

    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    curandState state;
    curand_init(1234, idx, 0, &state);

    if (idx < *m0)
    {
        paths[idx][0] = S0;
        for (size_t i = 1; i < *N; i++)
        {
            double dW = generate_gaussian_sample(0, sqrt(dt), &state);
            paths[idx][i] = paths[idx][i - 1] * exp((mu - 0.5 * sigma * sigma) * dt + sigma * dW);
        }
    }
}

__global__ void generate_external_path_equity(double **paths, size_t *m0, size_t *N, double *T)
{
    double S0 = 100;
    double mu = 0.05;
    double sigma = 0.2;

    double dt = *T / *N;

    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    curandState state;
    curand_init(1234, idx, 0, &state);

    if (idx < *m0)
    {
        paths[idx][0] = S0;
        for (size_t i = 1; i < *N; i++)
        {
            double dW = generate_gaussian_sample(0, sqrt(dt), &state);
            paths[idx][i] = paths[idx][i - 1] * exp((mu - 0.5 * sigma * sigma) * dt + sigma * dW);
        }
    }
}

void CUDA::Simulation::run_simulation(const std::map<XVA, double>& xva,
                    size_t m0, size_t m1,
                    size_t nb_points, double T,
                    std::map<ExternalPaths, std::vector<Vector>> &external_paths,
                    std::map<XVA, Vector> &paths)
{
    double *d_T;
    size_t *d_N, *d_m0, *d_m1;
    double **d_paths_interest, **d_paths_fx, **d_paths_equity;
    double **generated_paths = new double*[m0];


    cudaMalloc(&d_m0, sizeof(size_t));
    cudaMalloc(&d_m1, sizeof(size_t));
    cudaMalloc(&d_T, sizeof(double));
    cudaMalloc(&d_N, sizeof(size_t));

    cudaMemcpy(d_m0, &m0, sizeof(size_t), cudaMemcpyHostToDevice);
    cudaMemcpy(d_m1, &m1, sizeof(size_t), cudaMemcpyHostToDevice);
    cudaMemcpy(d_T, &T, sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_N, &nb_points, sizeof(size_t), cudaMemcpyHostToDevice);

    cudaMalloc(&d_paths_interest, m0 * sizeof(double *));

    for (size_t i = 0; i < m0; i++)
    {
        cudaMalloc(&d_paths_interest[i], nb_points * sizeof(double));
    }

    generate_external_path_interest_rate<<<m0, 1>>>(d_paths_interest, d_m0, d_N, d_T);
    for (size_t i = 0; i < m0; i++)
    {
        generated_paths[i] = new double[nb_points];
        cudaMemcpy(generated_paths[i], d_paths_interest[i], nb_points * sizeof(double), cudaMemcpyDeviceToHost);
        external_paths[ExternalPaths::Interest].push_back(Vector(nb_points));
        for (size_t j = 0; j < nb_points; j++)
        {
            external_paths[ExternalPaths::Interest][i][j] = generated_paths[i][j];
        }
    }

    cudaMalloc(&d_paths_fx, m0 * sizeof(double *));
    for (size_t i = 0; i < m0; i++)
    {
        cudaMalloc(&d_paths_fx[i], nb_points * sizeof(double));
    }

    generate_external_path_fx<<<m0, 1>>>(d_paths_fx, d_m0, d_N, d_T);
    for (size_t i = 0; i < m0; i++)
    {
        generated_paths[i] = new double[nb_points];
        cudaMemcpy(generated_paths[i], d_paths_fx[i], nb_points * sizeof(double), cudaMemcpyDeviceToHost);
        external_paths[ExternalPaths::FX].push_back(Vector(nb_points));
        for (size_t j = 0; j < nb_points; j++)
        {
            external_paths[ExternalPaths::FX][i][j] = generated_paths[i][j];
        }
    }

    cudaMalloc(&d_paths_equity, m0 * sizeof(double *));
    for (size_t i = 0; i < m0; i++)
    {
        cudaMalloc(&d_paths_equity[i], nb_points * sizeof(double));
    }

    generate_external_path_equity<<<m0, 1>>>(d_paths_equity, d_m0, d_N, d_T);
    for (size_t i = 0; i < m0; i++)
    {
        generated_paths[i] = new double[nb_points];
        cudaMemcpy(generated_paths[i], d_paths_equity[i], nb_points * sizeof(double), cudaMemcpyDeviceToHost);
        external_paths[ExternalPaths::Equity].push_back(Vector(nb_points));
        for (size_t j = 0; j < nb_points; j++)
        {
            external_paths[ExternalPaths::Equity][i][j] = generated_paths[i][j];
        }
    }

    for (size_t i = 0; i < m0; i++)
    {
        delete[] generated_paths[i];
    }
    delete[] generated_paths;

    for (size_t i = 0; i < m0; i++)
    {
        cudaFree(d_paths_interest[i]);
        cudaFree(d_paths_fx[i]);
        cudaFree(d_paths_equity[i]);
    }

    cudaFree(d_paths_interest);
    cudaFree(d_paths_fx);
    cudaFree(d_paths_equity);
    cudaFree(d_m0);
    cudaFree(d_m1);
    cudaFree(d_T);
    cudaFree(d_N);
}
