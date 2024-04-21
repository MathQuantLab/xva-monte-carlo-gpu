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
    atexit([]() -> void
           { cudaDeviceReset(); });

    if (argc < 2)
    {
        Utils::info(argv[0]);
        return 0;
    }

    try
    {
        bool gpu = CUDA::Utils::is_gpu_available();
        Utils::parse_arguments(argc, argv, gpu);

        std::set<XVA> xvas;
        Utils::parse_type(argv[argc - 1], xvas);

        cout << xvas.size() << " XVA requested" << endl;

        if (!gpu)
        {
            cout << "Running on CPU with maximum " << std::thread::hardware_concurrency() << " threads simultaneously." << endl;
            std::thread* threads = new std::thread[xvas.size()];

            for (size_t i = 0; i < xvas.size(); i++)
            {
                threads[i] = std::thread([]() -> void //TODO change function
                {
                    cout << "Running on CPU with id " << std::this_thread::get_id() << endl;
                });    
            }
            
            for (size_t i = 0; i < xvas.size(); i++)
            {
                threads[i].join();
            }
            delete[] threads;
            threads = nullptr;
        }
        else
        {
            cout << "Running on GPU" << endl;
        }
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