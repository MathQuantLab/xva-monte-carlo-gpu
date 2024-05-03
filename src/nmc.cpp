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

/**
 * @brief Compute the maximum of two vectors element-wise
 *
 * @tparam T Element type
 * @param a First vector
 * @param b Second vector
 * @param c Output vector
 */
template <typename T>
void maximum(const std::vector<T> &a, const std::vector<T> &b, std::vector<T> &c)
{
    if (a.size() != b.size())
    {
        throw std::invalid_argument("Vectors a and b must have the same size");
    }
    c.resize(a.size());
    for (size_t i = 0; i < a.size(); i++)
    {
        c[i] = std::max(a[i], b[i]);
    }
}

/**
 * @brief Compute the maximum of two vectors element-wise
 *
 * @tparam T Element type
 * @param a First vector
 * @param b Second vector
 * @param c Output vector
 */
template <typename T>
void maximum(const std::vector<T> &a, const T &b, std::vector<T> &c)
{
    c.resize(a.size());
    for (size_t i = 0; i < a.size(); i++)
    {
        c[i] = std::max(a[i], b);
    }
}

/**
 * @brief Compute the minimum of two vectors element-wise
 *
 * @tparam T Element type
 * @param a First vector
 * @param b Second vector
 * @param c Output vector
 */
template <typename T>
void minimum(const std::vector<T> &a, const std::vector<T> &b, std::vector<T> &c)
{
    if (a.size() != b.size())
    {
        throw std::invalid_argument("Vectors a and b must have the same size");
    }
    c.resize(a.size());
    for (size_t i = 0; i < a.size(); i++)
    {
        c[i] = std::min(a[i], b[i]);
    }
}

/**
 * @brief Compute the minimum of two vectors element-wise
 *
 * @tparam T Element type
 * @param a First vector
 * @param b Second vector
 * @param c Output vector
 */
template <typename T>
void minimum(const std::vector<T> &a, const T &b, std::vector<T> &c)
{
    c.resize(a.size());
    for (size_t i = 0; i < a.size(); i++)
    {
        c[i] = std::min(a[i], b);
    }
}

void NMC::run(XVA xva, double factor, const std::map<ExternalPaths, std::vector<Vector>> &external_paths, Vector &final_path) const
{
    std::cout << "Running NMC for XVA " << Utils::pretty_print_xva_name(xva) << " on thread " << std::this_thread::get_id() << " with factor " << factor << std::endl;

    std::map<ExternalPaths, Matrix> internal_paths;
    std::map<ExternalPaths, Vector> mean_internal_paths;
    Vector path(nb_points);

#ifdef DEBUG
    std::cout << "Internal paths and mean internal paths initialized" << std::endl;
#endif

    for (auto &external_path : external_paths)
    {
        generate_internal_paths(external_paths.at(external_path.first),
                                internal_paths[external_path.first]);
    }

#ifdef DEBUG
    std::cout << "Internal paths generated" << std::endl;
#endif

    for (auto const &internal_path : internal_paths)
    {
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

    Vector EPE, DPE;
    maximum<double>(path, factor, EPE);
    DPE = EPE;

    for (size_t i = 0; i < nb_points; i++)
    {
        EPE[i] -= factor;
        DPE[i] = factor - DPE[i];
    }

    double loss_given_default = 0.4;
    double funding_cost = 0.05;
    double capital_cost = 0.1;

    switch (xva)
    {
    case CVA:
        for (size_t i = 0; i < nb_points; i++)
        {
            final_path[i] = EPE[i] * (1 - loss_given_default) * 0.01;
        }
        break;

    case DVA:
        for (size_t i = 0; i < nb_points; i++)
        {
            final_path[i] = DPE[i] * (1 - loss_given_default) * 0.01;
        }
        break;

    case FVA:
        for (size_t i = 0; i < nb_points; i++)
        {
            final_path[i] = std::max(EPE[i] - DPE[i], 0.0) * funding_cost * exp(-0.03 * i * T / nb_points);
        }

        break;

    case MVA:
        for (size_t i = 0; i < nb_points; i++)
        {
            final_path[i] = EPE[i] * funding_cost * exp(-0.03 * i * T / nb_points);
        }
        break;

    case KVA:
        for (size_t i = 0; i < nb_points; i++)
        {
            final_path[i] = EPE[i] * capital_cost * exp(-0.03 * i * T / nb_points);
        }
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
        paths[i].push_back(r0);
        for (size_t j = 1; j < nb_points; j++)
        {
            double dW = std::normal_distribution<double>(0.0, std::sqrt(dt))(gen);
            paths[i].push_back(paths[i][j - 1] + k * (theta - paths[i][j - 1]) * dt + sigma * dW * std::sqrt(paths[i][j - 1]));

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
        paths[i].push_back(S0);
        for (size_t j = 1; j < nb_points; j++)
        {
            double dW = std::normal_distribution<double>(0.0, std::sqrt(dt))(gen);
            paths[i].push_back(paths[i][j - 1] * std::exp((mu - 0.5 * sigma * sigma) * dt + sigma * dW));
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
        paths[i].push_back(S0);
        for (size_t j = 1; j < nb_points; j++)
        {
            double dW = std::normal_distribution<double>(0.0, std::sqrt(dt))(gen);
            paths[i].push_back(paths[i][j - 1] * std::exp((mu - 0.5 * sigma * sigma) * dt + sigma * dW));
        }
    }
}

void NMC::generate_internal_paths(const Matrix &external_paths, Matrix &paths) const
{
    std::cout << "Generating internal paths on thread " << std::this_thread::get_id() << std::endl;

#ifdef DEBUG
    std::cout << "External paths size: " << external_paths.size() << std::endl;
    std::cout << "First path size: " << external_paths[0].size() << std::endl;
#endif

    paths.resize(m1);

    for (size_t i = 0; i < nb_points; i++)
    {
        paths[0].push_back(external_paths[0][i]);
    }

    #ifdef DEBUG
        std::cout << "First path generated" << std::endl;
    #endif

    std::random_device rd;
    std::mt19937 gen(rd());

    double sigma = 0.2;
    double mu = 0.05;

    double dt = T / double(nb_points);

    for (size_t i = 0; i < m1; i++)
    {
        for (size_t j = 1; j < nb_points; j++)
        {
            double dW = std::normal_distribution<double>(0.0, std::sqrt(dt))(gen);
            paths[i].push_back(paths[i][j - 1] * exp((mu - 0.5 * sigma * sigma) * dt + sigma * dW));
        }
    }
}