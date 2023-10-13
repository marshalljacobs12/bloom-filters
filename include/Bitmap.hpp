#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>

class Bitmap {
private:
    std::vector<uint64_t> data;  // Using 64-bit words for the bitmap
    size_t size;
public:
    Bitmap(size_t n);
    void set(size_t idx);
    void clear(size_t idx);
    bool test(size_t idx) const;
    void print() const;
};