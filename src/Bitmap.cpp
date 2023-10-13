#include <Bitmap.hpp>

#include <iostream>
#include <stdexcept>

Bitmap::Bitmap(size_t n) : size(n) {
    data.resize((n + 63) / 64);  // Each uint64_t holds 64 bits, so we divide by 64
}

void Bitmap::set(size_t idx) {
    if (idx >= size) {
        throw std::out_of_range("Index out of bounds");
    }
    data[idx / 64] |= (1ULL << (idx % 64));
}

void Bitmap::clear(size_t idx) {
    if (idx >= size) {
        throw std::out_of_range("Index out of bounds");
    }
    data[idx / 64] &= ~(1ULL << (idx % 64));
}

bool Bitmap::test(size_t idx) const {
    if (idx >= size) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[idx / 64] & (1ULL << (idx % 64));
}

void Bitmap::print() const {
    for (size_t i = 0; i < size; ++i) {
        std::cout << (test(i) ? "1" : "0");
    }
    std::cout << "\n";
}
