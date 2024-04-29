/**
 * @file utils.cpp
 * @author Thomas Roiseux (thomas.roiseux@mathquantlab.com)
 * @brief Implements {@link utils.h}
 * @version 1.0
 * @date 2024-04-21
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_set>
#include <numeric>
#include <fstream>

#include "../headers/utils.h"
#include "../headers/cuda_utils.h"

using namespace std;

void Utils::info(const char *name)
{
    cout << "Usage: " << name << " [options] <m0> <m1> <N> <T> <type>" << endl;
    cout << "Options:" << endl;
    cout << "  -h, --help      Display this information" << endl;
    cout << "  -v, --version   Display application version" << endl;
    cout << "  --cpu           Use CPU instead of GPU" << endl;
    cout << "  --gpu <id>      Use GPU with device id" << endl;
    cout << "Arguments:" << endl;
    cout << "  m0              External trajectories number" << endl;
    cout << "  m1              Internal trajectories number" << endl;
    cout << "  N               Points number" << endl;
    cout << "  T               Horizon" << endl;
    cout << "  type            XVA type (CVA, DVA, FVA, MVA, KVA), using form XVA=rate,XVA=rate..." << endl;
}

int Utils::parse_options(int argc, char *argv[], bool &gpu)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-')
        {
            return i;
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
        else if (gpu && std::string(argv[i]).find("--gpu") != std::string::npos)
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
    return argc;
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

void Utils::parse_type(const std::string &str, std::map<XVA, double> &xvas)
{
    std::vector<std::string> tokens;
    split_string(str, ",", tokens);
    for (const auto &token : tokens)
    {
        std::vector<std::string> xva_values;
        if (token.find("=") == std::string::npos)
        {
            throw Exception("Invalid XVA type: " + token);
        }
        if (token.find("CVA") != std::string::npos)
        {
            split_string(token, "=", xva_values);
            xvas[XVA::CVA] = 0.0;
            if (sscanf(xva_values[1].c_str(), "%lf", &xvas[XVA::CVA]) == 0)
            {
                throw Exception("Invalid CVA rate");
            }
        }
        else if (token.find("DVA") != std::string::npos)
        {
            split_string(token, "=", xva_values);
            xvas[XVA::DVA] = 0.0;
            if (sscanf(xva_values[1].c_str(), "%lf", &xvas[XVA::DVA]) == 0)
            {
                throw Exception("Invalid DVA rate");
            }
        }
        else if (token.find("FVA") != std::string::npos)
        {
            split_string(token, "=", xva_values);
            xvas[XVA::FVA] = 0.0;
            if (sscanf(xva_values[1].c_str(), "%lf", &xvas[XVA::FVA]) == 0)
            {
                throw Exception("Invalid FVA rate");
            }
        }
        else if (token.find("MVA") != std::string::npos)
        {
            split_string(token, "=", xva_values);
            xvas[XVA::MVA] = 0.0;
            if (sscanf(xva_values[1].c_str(), "%lf", &xvas[XVA::MVA]) == 0)
            {
                throw Exception("Invalid MVA rate");
            }
        }
        else if (token.find("KVA") != std::string::npos)
        {
            split_string(token, "=", xva_values);
            xvas[XVA::KVA] = 0.0;
            if (sscanf(xva_values[1].c_str(), "%lf", &xvas[XVA::KVA]) == 0)
            {
                throw Exception("Invalid KVA rate");
            }
        }
        else
        {
            throw Exception("Unknown XVA type: " + token);
        }
    }
}

void Utils::parse_mandatory_arguments(int argc, char *argv[], size_t &m0, size_t &m1, size_t &N, double &T)
{
    if (sscanf(argv[argc], "%lu", &m0) == 0)
    {
        throw Exception("Invalid number of of external trajectories");
    }
    if (sscanf(argv[argc + 1], "%lu", &m1) == 0)
    {
        throw Exception("Invalid number of of internal trajectories");
    }
    if (sscanf(argv[argc + 3], "%lu", &N) == 0)
    {
        throw Exception("Invalid number of points");
    }
    if (sscanf(argv[argc + 4], "%lf", &T) == 0)
    {
        throw Exception("Invalid horizon");
    }
}
