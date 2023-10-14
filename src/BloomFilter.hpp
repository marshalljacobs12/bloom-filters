#pragma once

#include <Bitmap.hpp>
#include <memory>
#include <functional>

template <typename T>
class BloomFilter {
public:
    BloomFilter(size_t size, int num_hashes) : m_size(size), m_num_hashes(num_hashes) {
        m_bitmap = std::make_unique<Bitmap>(m_size);
        if (m_num_hashes > 100) {
            m_num_hashes = 100;
        }
    }
    void insert(T item) {
        for (int i = 0; i < m_num_hashes; i++) {
            size_t hash_val = (hash1(item) + i * hash2(item)) % m_size;
            m_bitmap.get()->set(hash_val);
        }
    }
    bool contains(T item) {
        for (int i = 0; i < m_num_hashes; i++) {
            size_t hash_val = (hash1(item) + i * hash2(item)) % m_size;
            if (!m_bitmap.get()->test(hash_val)) {
                return false;
            }
        }
        return true;
    }

private:
    size_t m_size;
    int m_num_hashes;
    std::unique_ptr<Bitmap> m_bitmap;

    size_t hash1(const T &input) const {
        return std::hash<T>{}(input) % m_size;
    }

    size_t hash2(const T &input) const {
        return (std::hash<T>{}(input) * 0xdeadbeef) % m_size;
    }
};