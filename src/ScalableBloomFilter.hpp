#include <vector>
#include <bitset>
#include <cmath>
#include <bitset>
#include <string>

const size_t MB_SIZE = 1024 * 1024;

class ScalableBloomFilter {
public:
    ScalableBloomFilter(size_t initial_capacity, double error_rate, size_t max_size)
        : m_initial_capacity(initial_capacity), m_error_rate(error_rate), m_num_filters(1)
    {
        m_max_size = max_size * MB_SIZE;
        m_capacity_scale_factor = 1;
        m_filters.emplace_back(std::vector<bool>(computeNumBits(m_initial_capacity, m_error_rate), false));
        m_capacity_scale_factor *= 2;
        m_current_filter_index = 0;
    }

    void insert(const std::string& key) {
        size_t filter_index = m_current_filter_index;
        while (true) {
            // can only insert into one of the filters that exists
            if (filter_index >= m_num_filters) {
                return;
            }
            // already present so can just return
            if (m_filters[filter_index][hash(key, filter_index)]) {
                return;
            }
            // insert key into current filter
            m_filters[filter_index][hash(key, filter_index)] = true;
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
                m_capacity_scale_factor *= 2;
                m_filters.emplace_back(std::vector<bool>(num_bits, false));
                m_num_filters++;
                m_current_filter_index++;
            }
            filter_index++;
        }
    }

    // just needs to be present in one of the filters
    bool contains(const std::string& key) const {
        std::cout << "key: " << key << std::endl;
        for (size_t i = 0; i < m_num_filters; i++) {
            if (m_filters[i][hash(key, i)]) {
                std::cout << "in i: " << i << std::endl;
                return true;
            }
        }
        return false;
    }

private:
    size_t m_initial_capacity;
    double m_error_rate;
    size_t m_num_filters;
    int m_capacity_scale_factor;
    size_t m_current_filter_index;
    std::vector<std::vector<bool>> m_filters;
    size_t m_max_size;

    size_t computeNumBits(size_t capacity, double falsePositiveRate) const {
        return static_cast<size_t>(-1 * static_cast<double>(capacity) * std::log(falsePositiveRate) / std::pow(std::log(2), 2));
    }

    double calculateFalsePositiveRate(size_t num_bits, size_t num_entries) {
        const double m = static_cast<double>(num_bits);
        const double n = static_cast<double>(num_entries);
        double rate = 1.0 - std::exp(-n/m);
        return rate;
    }

    size_t hash(const std::string& key, size_t filter_index) const {
        // Use a simple hash function for demonstration purposes
        size_t hash_val = 0;
        for (char c : key) {
            hash_val = (hash_val * 31 + c) % m_filters[filter_index].size();
        }
        return hash_val;
    }
};
