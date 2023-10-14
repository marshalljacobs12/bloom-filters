// Executables must have the following defined if the library contains
// doctest definitions. For builds with this disabled, e.g. code shipped to
// users, this can be left out.
#ifdef ENABLE_GTEST_IN_LIBRARY
#define GTEST_CONFIG_IMPLEMENT
#include "gtest/gtest.h"
#endif

#include <iostream>
#include <stdlib.h>

#include "exampleConfig.h"
#include "BloomFilter.hpp"

/*
 * Simple main program that demontrates how access
 * CMake definitions (here the version number) from source code.
 */
int main() {
  std::cout << "C++ Boiler Plate v"
            << PROJECT_VERSION_MAJOR
            << "."
            << PROJECT_VERSION_MINOR
            << "."
            << PROJECT_VERSION_PATCH
            << "."
            << PROJECT_VERSION_TWEAK
            << std::endl;
  std::system("cat ../LICENSE");

  std::cout << "Pre constructor" << std::endl;
  BloomFilter<std::string> bf(100, 3);
  std::cout << "Post constructor" << std::endl;
  bf.insert("Hello");
  bf.insert("World");
  if (bf.contains("Hello"))
    std::cout << "Hello is in the bloom filter" << std::endl;
  if (!bf.contains("Goodbye"))
    std::cout << "Goodbye is not in the bloom filter" << std::endl;
  return 0;
}
