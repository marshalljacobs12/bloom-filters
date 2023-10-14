#include <vector>
#include <cmath>
#include <string>
#include <functional>

const size_t MB_SIZE = 1024 * 1024;

// Declare the BloomFilter template class
template<typename T>
class ScalableBloomFilter;

#ifdef INCLUDE_PYBIND
#include <pybind11/pybind11.h>
template <>
class ScalableBloomFilter<pybind11::object> {
public:
    ScalableBloomFilter(size_t initial_capacity, double error_rate, size_t max_size, int scale_factor)
        : m_initial_capacity(initial_capacity), 
          m_error_rate(error_rate),
          m_capacity_scale_factor(scale_factor),
          m_current_filter_index(0) 
    {
        m_max_size = max_size * MB_SIZE;
        m_filters.emplace_back(std::vector<bool>(computeNumBits(m_initial_capacity, m_error_rate), false));
    }

    void insert(const pybind11::object item) {
        ensure_hashable(item);
        size_t filter_index = m_current_filter_index;
        while (true) {
            // can only insert into one of the filters that exists
            if (filter_index >= m_filters.size()) {
                return;
            }
            // already present so can just return
            if (m_filters[filter_index][hash(item, filter_index)]) {
                return;
            }
            // insert key into current filter
            m_filters[filter_index][hash(item, filter_index)] = true;
            auto num_entries = std::count(m_filters[filter_index].begin(), m_filters[filter_index].end(), true);
            double false_positive_rate = calculateFalsePositiveRate(m_filters[filter_index].size(), num_entries);
            // if false positive rate for this filter now exceeds the error rate, then create a new filter
            if (false_positive_rate > m_error_rate) {
                // TODO: fix all this one off sloppiness
                // only create a new filter if the previous filter smaller than than the max size
                if (m_filters[filter_index].size() * sizeof(m_filters[filter_index][0]) > m_max_size) {
                    return;
                }
                size_t num_bits = computeNumBits(m_capacity_scale_factor * m_initial_capacity, m_error_rate);
                m_capacity_scale_factor *= m_capacity_scale_factor;
                m_filters.emplace_back(std::vector<bool>(num_bits, false));
                m_current_filter_index++;
            }
            filter_index++;
        }
    }

    // just needs to be present in one of the filters
    bool contains(const pybind11::object item) const {
        ensure_hashable(item);
        // std::cout << "item: " << item << std::endl;
        for (size_t i = 0; i < m_filters.size(); i++) {
            if (m_filters[i][hash(item, i)]) {
                std::cout << "in i: " << i << std::endl;
                return true;
            }
        }
        return false;
    }

private:
    size_t m_initial_capacity, m_max_size;
    double m_error_rate;
    int m_capacity_scale_factor;
    size_t m_current_filter_index;
    std::vector<std::vector<bool>> m_filters;

    void ensure_hashable(const pybind11::object input) const {
        try {
            pybind11::hash(input);
        } catch(...) {
            throw std::runtime_error("Object is not hashable!");
        }
    }

    size_t computeNumBits(size_t capacity, double falsePositiveRate) const {
        return static_cast<size_t>(-1 * static_cast<double>(capacity) * std::log(falsePositiveRate) / std::pow(std::log(2), 2));
    }

    double calculateFalsePositiveRate(size_t num_bits, size_t num_entries) {
        const double m = static_cast<double>(num_bits);
        const double n = static_cast<double>(num_entries);
        double rate = 1.0 - std::exp(-n/m);
        return rate;
    }

    size_t hash(const pybind11::object &input, size_t filter_index) const {
        pybind11::object hash_obj = pybind11::module::import("builtins").attr("hash")(input);
        long hash_value = hash_obj.cast<long>();
        return std::hash<long>{}(hash_value) % m_filters[filter_index].size();
    }    
};
#endif

#ifndef INCLUDE_PYBIND

template <typename T, typename = void>
struct is_hashable : std::false_type {};

template <typename T>
struct is_hashable<T, decltype(std::hash<T>{}(std::declval<T>()), void())> : std::true_type {};

template <typename T>
class ScalableBloomFilter {
public:
    ScalableBloomFilter(size_t initial_capacity, double error_rate, size_t max_size, int scale_factor)
        : m_initial_capacity(initial_capacity), 
          m_error_rate(error_rate),
          m_capacity_scale_factor(scale_factor),
          m_current_filter_index(0) 
    {
        m_max_size = max_size * MB_SIZE;
        m_filters.emplace_back(std::vector<bool>(computeNumBits(m_initial_capacity, m_error_rate), false));
    }

    void insert(const T& item) {
        size_t filter_index = m_current_filter_index;
        while (true) {
            // can only insert into one of the filters that exists
            if (filter_index >= m_filters.size()) {
                return;
            }
            // already present so can just return
            if (m_filters[filter_index][hash(item, filter_index)]) {
                return;
            }
            // insert key into current filter
            m_filters[filter_index][hash(item, filter_index)] = true;
            auto num_entries = std::count(m_filters[filter_index].begin(), m_filters[filter_index].end(), true);
            double false_positive_rate = calculateFalsePositiveRate(m_filters[filter_index].size(), num_entries);
            // if false positive rate for this filter now exceeds the error rate, then create a new filter
            if (false_positive_rate > m_error_rate) {
                // TODO: fix all this one off sloppiness
                // only create a new filter if the previous filter smaller than than the max size
                if (m_filters[filter_index].size() * sizeof(m_filters[filter_index][0]) > m_max_size) {
                    return;
                }
                size_t num_bits = computeNumBits(m_capacity_scale_factor * m_initial_capacity, m_error_rate);
                m_capacity_scale_factor *= m_capacity_scale_factor;
                m_filters.emplace_back(std::vector<bool>(num_bits, false));
                m_current_filter_index++;
            }
            filter_index++;
        }
    }

    // just needs to be present in one of the filters
    bool contains(const T& item) const {
        // std::cout << "item: " << item << std::endl;
        for (size_t i = 0; i < m_filters.size(); i++) {
            if (m_filters[i][hash(item, i)]) {
                // std::cout << "in i: " << i << std::endl;
                return true;
            }
        }
        return false;
    }

private:
    size_t m_initial_capacity, m_max_size;
    double m_error_rate;
    int m_capacity_scale_factor;
    size_t m_current_filter_index;
    std::vector<std::vector<bool>> m_filters;

    size_t computeNumBits(size_t capacity, double falsePositiveRate) const {
        return static_cast<size_t>(-1 * static_cast<double>(capacity) * std::log(falsePositiveRate) / std::pow(std::log(2), 2));
    }

    double calculateFalsePositiveRate(size_t num_bits, size_t num_entries) {
        const double m = static_cast<double>(num_bits);
        const double n = static_cast<double>(num_entries);
        double rate = 1.0 - std::exp(-n/m);
        return rate;
    }

    size_t hash(const T &input, size_t filter_index) const {
        return std::hash<T>{}(input) % m_filters[filter_index].size();
    }

};
#endif
