/**
 * @file nmc.cpp
 * @author Thomas Roiseux (thomas.roiseux@@mathquantlab.com)
 * @brief Implements {@link nmc.h}
 * @version 1.0
 * @date 2024-04-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "../headers/nmc.h"

#include <iostream>
#include <thread>
#include <random>
#include <cmath>

void NMC::run(XVA xva, double factor, const std::map<ExternalPaths, std::vector<Vector>> &external_paths, Vector &path) const
{
    std::cout << "Running NMC for XVA " << Utils::pretty_print_xva_name(xva) << " on thread " << std::this_thread::get_id() << " with factor " << factor << std::endl;
    path.resize(m1);

    std::map<ExternalPaths, Matrix> internal_paths;
    std::map<ExternalPaths, Vector> mean_internal_paths;

    for (auto const &external_path : external_paths)
    {
        internal_paths[external_path.first].resize(m1);
        for (size_t i = 0; i < m1; i++)
        {
            internal_paths[external_path.first][i].resize(nb_points);
        }
    }

    for (auto& internal_path : internal_paths)
    {
        generate_internal_paths(external_paths.at(internal_path.first), internal_path.second);
    }

    for (auto const &internal_path : internal_paths)
    {
        mean_internal_paths[internal_path.first].resize(nb_points);
        for (size_t j = 0; j < nb_points; j++)
        {
            double sum = 0;
            for (size_t i = 0; i < m1; i++)
            {
                sum += internal_path.second[i][j];
            }
            mean_internal_paths[internal_path.first][j] = sum / m1;
        }
    }

    for (size_t i = 0; i < nb_points; i++)
    {
        double sum = 0;
        for (auto const &internal_path : internal_paths)
        {
            sum += mean_internal_paths[internal_path.first][i];
        }
        path[i] = sum / 3;
    }

    switch (xva)
    {
    case CVA:
        /* code */
        break;

    case DVA:
        /* code */
        break;

    case FVA:
        /* code */
        break;

    case MVA:
        /* code */
        break;

    case KVA:
        /* code */
        break;
    
    default:
        break;
    }
}

void NMC::generate_interest_rate_paths(std::vector<Vector> &paths) const
{
    std::cout << "Generating interest rate paths on thread " << std::this_thread::get_id() << std::endl;
    double r0 = 0.03;
    double k(0.5);
    double theta = 0.04;
    double sigma = 0.1;


    std::random_device rd;
    std::mt19937 gen(rd());

    double dt = T / double(nb_points);
    
    for (size_t i = 0; i < paths.size(); i++)
    {
        paths[i].resize(nb_points);
        paths[i][0] = r0;
        for (size_t j = 1; j < nb_points; j++)
        {
            double dW = std::normal_distribution<double>(0.0, std::sqrt(dt))(gen);
            paths[i][j] = paths[i][j - 1] + k * (theta - paths[i][j - 1]) * dt + sigma * dW * std::sqrt(paths[i][j-1]);

            if (paths[i][j] < 0)
            {
                paths[i][j] = 0;
            }
        }
    }
}

void NMC::generate_fx_rate_paths(std::vector<Vector> &paths) const
{
    std::cout << "Generating interest rate paths on thread " << std::this_thread::get_id() << std::endl;
    double S0(1.15);
    double mu(0.02);
    double sigma = 0.1;
    
    std::random_device rd;
    std::mt19937 gen(rd());

    double dt = T / double(nb_points);

    for (size_t i = 0; i < paths.size(); i++)
    {
        paths[i].resize(nb_points);
        paths[i][0] = S0;
        for (size_t j = 1; j < nb_points; j++)
        {
            double dW = std::normal_distribution<double>(0.0, std::sqrt(dt))(gen);
            paths[i][j] = paths[i][j - 1] * std::exp((mu - 0.5 * sigma * sigma) * dt + sigma * dW);
        }
    }
}

void NMC::generate_equity_paths(std::vector<Vector> &paths) const
{
    std::cout << "Generating interest rate paths on thread " << std::this_thread::get_id() << std::endl;
    double S0(100);
    double mu(0.08);
    double sigma = 0.2;

    std::random_device rd;
    std::mt19937 gen(rd());

    double dt = T / double(nb_points);

    for (size_t i = 0; i < paths.size(); i++)
    {
        paths[i].resize(nb_points);
        paths[i][0] = S0;
        for (size_t j = 1; j < nb_points; j++)
        {
            double dW = std::normal_distribution<double>(0.0, std::sqrt(dt))(gen);
            paths[i][j] = paths[i][j - 1] * std::exp((mu - 0.5 * sigma * sigma) * dt + sigma * dW);
        }
    }
}

void NMC::generate_internal_paths(const Matrix& external_paths, Matrix& paths) const
{
    std::cout << "Generating internal paths on thread " << std::this_thread::get_id() << std::endl;

    paths[0] = external_paths[0];

    std::random_device rd;
    std::mt19937 gen(rd());

    double sigma = 0.2;
    double mu = 0.05;

    double dt = T / double(nb_points);

    for (size_t i = 1; i < m1; i++)
    {
        for (size_t j = 1; j < nb_points; j++)
        {
            double dW = std::normal_distribution<double>(0.0, std::sqrt(dt))(gen);
            paths[i][j] = paths[i][j - 1] * exp((mu - 0.5 * sigma * sigma) * dt + sigma * dW);
        }
    }
}