#include <pybind11/pybind11.h>
#include "BloomFilterPython.hpp"
#include "Bitmap.hpp"

namespace py = pybind11;

PYBIND11_MODULE(my_module, m) {
    py::class_<BloomFilterPython<py::object>>(m, "BloomFilter")
        .def(py::init<size_t, int>())
        .def("insert", &BloomFilterPython<py::object>::insert)
        .def("contains", &BloomFilterPython<py::object>::contains);
}
// .def(py::init<size_t, int>());

    // Templated constructor for arbitrary type
        // .def("insert", &BloomFilterPython<py::object>::insert)
        // .def("contains", &BloomFilterPython<py::object>::contains);