/**
 * @file utils.h
 * @author Thomas Roiseux (thomas.roiseux@mathquantlab.com)
 * @brief Provides utility functions
 * @version 1.0
 * @date 2024-04-21
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "../headers/pch.h"

#include <map>

/**
 * @brief Provides utility functions
 *
 */
namespace Utils
{
    /**
     * @brief Application information
     *
     * @param name Application name
     */
    void info(const char *name);

    /**
     * @brief Parse command line arguments
     *
     * @param argc Number of arguments
     * @param argv Arguments
     * @param gpu GPU flag
     */
    int parse_options(int argc, char *argv[], bool &gpu);

    /**
     * @brief Parse mandatory arguments
     *
     * @param argc Number of arguments
     * @param argv Arguments
     * @param m0 Number of external trajectories
     * @param m1 Number of internal trajectories
     * @param N Number of points
     * @param T Horizon
     */
    void parse_mandatory_arguments(int argc, char *argv[], size_t &m0, size_t &m1, size_t &N, double &T);

    /**
     * @brief Parse XVA type
     *
     * @param str String to parse
     * @param xvas XVA types
     */
    void parse_type(const std::string &str, std::map<XVA, double> &xvas);

    /**
     * @brief Split a string
     *
     * @param str String to split
     * @param delim Delimiter
     * @param tokens Tokens
     */
    void split_string(const std::string &str, const std::string &delim, std::vector<std::string> &tokens);

    /**
     * @brief Pretty print XVA name
     *
     * @param xva XVA type
     * @return const char* XVA name
     */
    const char *pretty_print_xva_name(XVA xva);

    /**
     * @brief Print results
     *
     * @param results Results
     * @param filename Filename
     * @param T Horizon
     */
    void print_results(const std::map<XVA, Vector> &results, const std::string &filename, double T);
}