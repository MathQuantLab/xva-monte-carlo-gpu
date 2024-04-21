/**
 * @file utils.cpp
 * @author Thomas Roiseux (thomas.roiseux@mathquantlab.com)
 * @brief Implements {@link utils.h}
 * @version 0.1
 * @date 2024-04-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include <cstring>

#include "../headers/utils.h"
#include "../headers/cuda_utils.h"

using namespace std;

void Utils::info(const char *name)
{
    cout << "Usage: " << name << " [options] <m0> <m1> <type>" << endl;
    cout << "Options:" << endl;
    cout << "  -h, --help      Display this information" << endl;
    cout << "  -v, --version   Display application version" << endl;
}

void Utils::parse_arguments(int argc, char *argv[], bool &gpu)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-')
        {
            continue;
        }
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
        {
            info(argv[0]);
            exit(0);
        }
        else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version"))
        {
            cout << "Version: 1.0.0" << endl;
            exit(0);
        }
        else if (!strcmp(argv[i], "--cpu"))
        {
            gpu = false;
        }
        else if (std::string(argv[i]).find("--gpu") != std::string::npos)
        {
            int device_id;
            if (i + 1 < argc)
            {
                if (sscanf(argv[i + 1], "%d", &device_id) == 0)
                {
                    throw Exception("Invalid device id");
                }
                i++;
            }
            else
            {
                cerr << "Missing device id" << endl;
                exit(1);
            }
            CUDA::Utils::select_gpu(device_id);
        }
        else
        {
            cerr << "Unknown option: " << argv[i] << endl;
            exit(1);
        }
    }
}