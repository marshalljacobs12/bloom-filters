#include <pybind11/pybind11.h>
#include "BloomFilter.hpp"
#include "Bitmap.hpp"

namespace py = pybind11;

PYBIND11_MODULE(bf, m) {
    py::class_<BloomFilter<py::object>>(m, "BloomFilter")
        .def(py::init<size_t, int>())
        .def("insert", &BloomFilter<py::object>::insert)
        .def("contains", &BloomFilter<py::object>::contains);
}
