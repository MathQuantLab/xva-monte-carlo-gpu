/**
 * @file main.cpp
 * @author Thomas Roiseux (thomas.roiseux@mathquantlab.com)
 * @brief Main file
 * @version 1.0
 * @date 2024-04-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <iostream>
#include <thread>

#include "../headers/cuda_utils.h"
#include "../headers/utils.h"
#include "../headers/simulation.h"
#include "../headers/cuda_simulation.h"

using namespace std;

/**
 * @brief Main function
 *
 * @param argc Number of arguments
 * @param argv Arguments
 * @return int Exit code
 */
int main(int argc, char *argv[])
{
    std::cout << "\t\t-----XVA CUDA-enabled computation system-----" << std::endl;
    if (argc < 2)
    {
        Utils::info(argv[0]);
        return 0;
    }

    try
    {
        bool gpu = CUDA::Utils::is_gpu_available();
        size_t m0(0), m1(0), N(0);
        double T(0);

        int first_mandatory_argument = Utils::parse_options(argc, argv, gpu);

        if (argc < 6)
        {
            cerr << "Missing arguments" << endl;
            Utils::info(argv[0]);
            return 1;
        }

        if (first_mandatory_argument + 5 != argc)
        {
            cerr << "Missing arguments" << endl;
            Utils::info(argv[0]);
            return 1;
        }

        string data_file_name;
        Utils::parse_mandatory_arguments(first_mandatory_argument, argv, m0, m1, N, T);

        cout << "External trajectories number: " << m0 << endl;
        cout << "Internal trajectories number: " << m1 << endl;
        cout << "Points number: " << N << endl;
        cout << "Horizon: " << T << endl;

        std::map<XVA, double> xvas;
        Utils::parse_type(argv[argc - 1], xvas);

#ifdef DEBUG
        cout << "XVA requested:" << endl;
        for (const auto &xva : xvas)
        {
            cout << Utils::pretty_print_xva_name(xva.first) << " with value " << xva.second << endl;
        }
#endif

        cout << xvas.size() << " XVA requested" << endl;

        std::map<ExternalPaths, std::vector<Vector>> external_paths;
        std::map<XVA, Vector> results;

        if (!gpu)
        {
            cout << "Running on CPU with maximum " << std::thread::hardware_concurrency() << " threads simultaneously." << endl;
            CPUSimulation::run_simulation(xvas, m0, m1, N, T, external_paths, results);
        }
        else
        {
            cout << "Running on GPU" << endl;
            atexit([]() -> void
                   { cudaDeviceReset(); });
            CUDA::Simulation::run_simulation(xvas, m0, m1, N, T, external_paths, results);
        }

        cout << "Simulation done" << endl;
        cout << "Writing results to file" << endl;

        Utils::print_results(results, "Data/results.csv", T);

        cout << "Results written to file" << endl;
    }
    catch (const CUDA::CUDAException &e)
    {
        std::cerr << "Error with CUDA" << endl;
        std::cerr << e.what() << " (" << e.get_error() << ")" << endl;
        return e.get_error();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown exception" << endl;
        return -1;
    }

    return 0;
}