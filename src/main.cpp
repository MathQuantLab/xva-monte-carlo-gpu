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
#include <cstring>

#include "../headers/cuda_utils.h"

using namespace std;

/**
 * @brief Application information
 *
 * @param name Application name
 */
void info(const char *name)
{
    cout << "Usage: " << name << " [options]" << endl;
    cout << "Options:" << endl;
    cout << "  -h, --help      Display this information" << endl;
    cout << "  -v, --version   Display application version" << endl;
}


/**
 * @brief Parse command line arguments
 *
 * @param argc Number of arguments
 * @param argv Arguments
 * @param gpu GPU flag
*/
void parse_arguments(int argc, char *argv[], bool& gpu)
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
        else
        {
            cout << "Unknown option: " << argv[i] << endl;
            exit(1);
        }
    }
}

/**
 * @brief Main function
 *
 * @param argc Number of arguments
 * @param argv Arguments
 * @return int Exit code
 */
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        info(argv[0]);
        return 0;
    }

    bool gpu = CUDA::Utils::is_gpu_available();
    parse_arguments(argc, argv, gpu);

    if (gpu)
    {
        cout << "GPU is enabled" << endl;
    }
    else
    {
        cout << "GPU is disabled" << endl;
    }

    try
    {
        cout << "Hello, World!" << endl;
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