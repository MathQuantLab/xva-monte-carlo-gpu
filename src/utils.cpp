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
#include "../Include/csv.hpp"

using namespace std;

void Utils::info(const char *name)
{
    cout << "Usage: " << name << " [options] <m0> <m1> <data-file> <N> <T> <type>" << endl;
    cout << "Options:" << endl;
    cout << "  -h, --help      Display this information" << endl;
    cout << "  -v, --version   Display application version" << endl;
    cout << "  --cpu           Use CPU instead of GPU" << endl;
    cout << "  --gpu <id>      Use GPU with device id" << endl;
    cout << "Arguments:" << endl;
    cout << "  m0              External trajectories number" << endl;
    cout << "  m1              Internal trajectories number" << endl;
    cout << "  data-file       Path to the data file" << endl;
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

void Utils::parse_mandatory_arguments(int argc, char *argv[], size_t &m0, size_t &m1, size_t &N, double &T, std::string &data_file_name)
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
    data_file_name = argv[argc + 2];
}

template <typename T, typename U>
Utils::CSVSeries<T, U>::CSVSeries(const std::vector<T> data)
{
    this->m_data = data;
    this->m_name = "";
    this->m_index = std::vector<U>(data.size());
    std::iota(this->m_index.begin(), this->m_index.end(), 0);
}

template <typename T, typename U>
Utils::CSVSeries<T, U>::CSVSeries(const std::string &name, const std::vector<T> &data)
{
    this->m_name = name;
    this->m_data = data;
    this->m_index = std::vector<U>(data.size());
    std::iota(this->m_index.begin(), this->m_index.end(), 0);
}

template <typename T, typename U>
Utils::CSVSeries<T, U>::CSVSeries(const std::string &name, const std::vector<U> &index, const std::vector<T> &data)
{
    this->m_name = name;
    this->m_data = data;
    this->m_index = index;

    if (index.size() != data.size())
    {
        throw Exception("Index and data sizes do not match");
    }

    if (std::unordered_set<U>(m_index.begin(), m_index.end()).size() != m_index.size())
    {
        throw Exception("Index contains duplicates");
    }
}

template <typename T, typename U>
const T& Utils::CSVSeries<T, U>::operator[](U i) const
{
    size_t index = std::distance(this->m_index.begin(), std::find(this->m_index.begin(), this->m_index.end(), i));
    if (index == this->m_index.size())
    {
        throw std::out_of_range("Index not found");
    }
    return this->m_data[index];
}

template <typename T, typename U>
T& Utils::CSVSeries<T, U>::operator[](U i)
{
    size_t index = std::distance(this->m_index.begin(), std::find(this->m_index.begin(), this->m_index.end(), i));
    if (index == this->m_index.size())
    {
        throw std::out_of_range("Index not found");
    }
    return this->m_data[index];
}

template <typename T, typename U>
void Utils::CSVSeries<T,U>::sort_index()
{
    std::vector<std::pair<U, T>> zipped(this->m_index.size());
    std::transform(this->m_index.begin(), this->m_index.end(), this->m_data.begin(), zipped.begin(), [](U i, T d) { return std::make_pair(i, d); });
    std::sort(zipped.begin(), zipped.end(), [](const std::pair<U, T> &a, const std::pair<U, T> &b) { return a.first < b.first; });
    std::transform(zipped.begin(), zipped.end(), this->m_index.begin(), [](const std::pair<U, T> &p) { return p.first; });
    std::transform(zipped.begin(), zipped.end(), this->m_data.begin(), [](const std::pair<U, T> &p) { return p.second; });
}

template <typename T, typename U>
void Utils::CSVSeries<T,U>::sort_values()
{
    std::vector<std::pair<U, T>> zipped(this->m_index.size());
    std::transform(this->m_index.begin(), this->m_index.end(), this->m_data.begin(), zipped.begin(), [](U i, T d) { return std::make_pair(i, d); });
    std::sort(zipped.begin(), zipped.end(), [](const std::pair<U, T> &a, const std::pair<U, T> &b) { return a.second < b.second; });
    std::transform(zipped.begin(), zipped.end(), this->m_index.begin(), [](const std::pair<U, T> &p) { return p.first; });
    std::transform(zipped.begin(), zipped.end(), this->m_data.begin(), [](const std::pair<U, T> &p) { return p.second; });
}

template class Utils::CSVSeries<double, size_t>;
template class Utils::CSVSeries<double, double>;

template<typename T, typename U, typename V>
Utils::CSVDataFrame<T, U, V>::CSVDataFrame(const std::string& file_name)
{ 
    bool first_row = false;
    csv::CSVReader reader(file_name);

    for (auto& row : reader)
    {
        if (!first_row)
        {
            for (auto& cell : row)
            {
                m_columns.push_back(cell.get<V>());
                m_data.push_back(CSVSeries<T, U>(cell.get<std::string>(), std::vector<T>()));
            }
            first_row = true;
        }
        else
        {
            std::vector<T> data;
            U idx;
            bool idx_found = false;
            for (auto& cell : row)
            {
                if (!idx_found)
                {
                    idx = cell.get<U>();
                    idx_found = true;
                }
                else
                {
                    data.push_back(cell.get<T>());   
                }
            }
            if (data.size() != m_columns.size())
            {
                throw std::invalid_argument("Data size does not match the number of columns");
            }
            this->push_back(idx, data);
        }
    }
}

template<typename T, typename U, typename V>
Utils::CSVDataFrame<T, U, V>::CSVDataFrame(const std::filesystem::path& file_path) :
    CSVDataFrame(file_path.string())
{
}

template<typename T, typename U, typename V>
const Utils::CSVSeries<T, U>& Utils::CSVDataFrame<T, U, V>::operator[](const V& column) const
{
    size_t index = std::distance(m_columns.begin(), std::find(m_columns.begin(), m_columns.end(), column));
    if (index == m_columns.size())
    {
        throw std::out_of_range("Column not found");
    }
    return m_data[index];
}

template<typename T, typename U, typename V>
std::vector<T> Utils::CSVDataFrame<T, U, V>::operator[](const U& index) const
{
    std::vector<T> data;

    for (const auto& series : m_data)
    {
        data.push_back(series[index]);
    }

    return data;
}

template<typename T, typename U, typename V>
std::vector<T> Utils::CSVDataFrame<T, U, V>::at(size_t idx) const
{
    std::vector<T> data;

    for (const auto& series : m_data)
    {
        data.push_back(series[idx]);
    }

    return data;
}

template<typename T, typename U, typename V>
void Utils::CSVDataFrame<T, U, V>::push_back(const U& idx, const std::vector<T>& data)
{
    if (data.size() != m_columns.size())
    {
        throw std::invalid_argument("Data size does not match the number of columns");
    }

    for (size_t i = 0; i < m_columns.size(); i++)
    {
        m_data[i].push_back(idx, data[i]);
        m_index.push_back(idx);
    }
}

template class Utils::CSVDataFrame<double, size_t, std::string>;
template class Utils::CSVDataFrame<double, double, std::string>;