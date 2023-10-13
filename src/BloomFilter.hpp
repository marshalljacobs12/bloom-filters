#pragma once

#include <Bitmap.hpp>
#include <memory>

template <typename T>
class BloomFilter {
public:
    BloomFilter(int size, int numHashes) : mSize(size), mNumHashes(numHashes) {
        mBitmap = std::make_unique<Bitmap>(size);
    }
    void insert(T item) {
        return;
    }
    bool contains(T item) {
        return true;
    }
private:
    int mSize;
    int mNumHashes;
    std::unique_ptr<Bitmap> mBitmap;
};