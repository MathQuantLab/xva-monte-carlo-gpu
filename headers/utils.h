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
     * @param data_file_name Data file name
     */
    void parse_mandatory_arguments(int argc, char *argv[], size_t &m0, size_t &m1, size_t &N, double &T, std::string& data_file_name);

    /**
     * @brief Parse XVA type
     *
     * @param type XVA type
     * @param xvas XVA types
     */
    void parse_type(const std::string& str, std::map<XVA, double> &xvas);

    /**
     * @brief Split a string
     *
     * @param str String to split
     * @param delim Delimiter
     * @param tokens Tokens
     */
    void split_string(const std::string &str, const std::string &delim, std::vector<std::string> &tokens);

    /**
     * @brief A class to represent a CSV series
     * 
     * @tparam T Data type. Must be a numeric type to be compatible with GPU
     */
    template<class T, typename U=size_t>
    class CSVSeries
    {
    public:

        /**
         * @brief Construct a new CSVSeries object
         * 
         */
        CSVSeries() = default;
        /**
         * @brief Construct a new CSVSeries object
         * 
         * @param name Series name
         */
        CSVSeries(const std::vector<T> data);

        /**
         * @brief Construct a new CSVSeries object
         * 
         * @param name Series name
         * @param data Series data
         */
        CSVSeries(const std::string& name, const std::vector<T>& data);
        /**
         * @brief Construct a new CSVSeries object
         * 
         * @param name Series name
         * @param index Series index
         * @param data Series data
         * 
         * @throws Exception if the size of the index and data vectors do not match
         * @throws Exception if there are some duplicates in the index vector
         */
        CSVSeries(const std::string& name, const std::vector<U>& index, const std::vector<T>& data);

        /**
         * @brief Size of the series
         * 
         * @return size_t 
         */
        size_t size() const { return m_data.size(); }

        /**
         * @brief Get the data
         * 
         * @return const std::vector<T>& 
         */
        const std::string& name() const { return m_name; }

        /**
         * @brief Set the name
         * 
         * @param new_name New name
         */
        void name(const std::string& new_name) { m_name = new_name; };

        /**
         * @brief Get the data
         * 
         * @return const std::vector<T>& 
         */
        const std::vector<T>& data() const { return m_data; }

        /**
         * @brief Get the index
         * 
         * @return const std::vector<U>& 
         */
        const std::vector<U>& index() const { return m_index; }

        /**
         * @brief Get the data at a given index
         * 
         * @param i Index
         * @return T Value
         * @throws std::out_of_range if index is out of range
         */
        const T& operator[](U i) const;

        /**
         * @brief Get the data at a given index
         * 
         * @param i Index
         * @return T Value
         * @throws std::out_of_range if index is out of range
         */
        T& operator[](U i);

        /**
         * @brief Get the data at a given index
         * 
         * @param i Index as an integer.
         * @return T Value
         * @throws std::out_of_range if index is out of range
        */
        const T& at(size_t i) const {return m_data[i]; };

        /**
         * @brief Get the data at a given index
         * 
         * @param i Index as an integer.
         * @return T& Value that can be modified
         */
        T& at(size_t i) {return m_data[i]; };

        /**
         * @brief Add a value at the end of the series
         * 
         * @param value Value to add
         * @throws std::invalid_argument if the index already exists
         */
        void push_back(const T& value)
        {
            if (std::find(m_index.begin(), m_index.end(), m_index.size()) != m_index.end())
            {
                throw std::invalid_argument("Index already exists");
            }
            m_data.push_back(value);
            m_index.push_back(m_index.size());
        }

        /**
         * @brief Add a value at the end of the series
         * 
         * @param idx Index
         * @param value Value to add
         * @throws std::invalid_argument if the index already exists
         */
        void push_back(const U& idx, const T& value)
        {
            if (std::find(m_index.begin(), m_index.end(), idx) != m_index.end())
            {
                throw std::invalid_argument("Index already exists");
            }
            m_data.push_back(value);
            m_index.push_back(idx);
        }

        /**
         * @brief Sort the Series by index
         * 
         */
        void sort_index();

        /**
         * @brief Sort the Series by values
         * 
         */
        void sort_values();
    private:
        std::string m_name;
        std::vector<U> m_index;
        std::vector<T> m_data;
    };

    /**
     * @brief A class to represent a CSV data frame
     * @param T Data type. Must be a numeric type to be compatible with GPU
     * @param U Index type
     * @param V Column name type
     */
    template<class T, typename U = size_t, typename V = std::string>
    class CSVDataFrame
    {
    public:
        /**
         * @brief No default constructor. A file name or path must be provided
         * 
         */
        CSVDataFrame() = delete;
        /**
         * @brief Construct a new CSVDataFrame object
         * 
         * @param file_name File name
         */
        CSVDataFrame(const std::string& file_name);
        /**
         * @brief Construct a new CSVDataFrame object
         * 
         * @param file_path File path
         */
        CSVDataFrame(const std::filesystem::path& file_path);

        /**
         * @brief Destroy the CSVDataFrame object
         * 
         */
        ~CSVDataFrame() = default;
    private:
        std::vector<V> m_columns;
        std::vector<U> m_index;
        std::vector<CSVSeries<T, U>> m_data;
    };
}