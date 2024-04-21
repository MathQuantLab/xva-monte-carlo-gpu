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

void Utils::split_string(const std::string &str, const std::string &delim, std::vector<std::string> &tokens)
{
    size_t start = 0;
    size_t end = str.find(delim);
    while (end != std::string::npos)
    {
        tokens.push_back(str.substr(start, end - start));
        start = end + delim.length();
        end = str.find(delim, start);
    }
    tokens.push_back(str.substr(start, end));
}

void Utils::parse_type(const std::string &str, std::set<XVA> &xvas)
{
    std::vector<std::string> tokens;
    split_string(str, ",", tokens);
    for (const auto &token : tokens)
    {
        if (token == "CVA")
        {
            xvas.insert(XVA::CVA);
        }
        else if (token == "DVA")
        {
            xvas.insert(XVA::DVA);
        }
        else if (token == "FVA")
        {
            xvas.insert(XVA::FVA);
        }
        else if (token == "MVA")
        {
            xvas.insert(XVA::MVA);
        }
        else if (token == "KVA")
        {
            xvas.insert(XVA::KVA);
        }
        else
        {
            throw Exception("Unknown XVA type: " + token);
        }
    }
}