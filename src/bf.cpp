#include <pybind11/pybind11.h>
#include "BloomFilter.hpp"
#include "Bitmap.hpp"
#include "CountingBloomFilter.hpp"

namespace py = pybind11;

PYBIND11_MODULE(bf, m) {
    py::class_<BloomFilter<py::object>>(m, "BloomFilter")
        .def(py::init<size_t, int>())
        .def("insert", &BloomFilter<py::object>::insert)
        .def("contains", &BloomFilter<py::object>::contains);
    py::class_<CountingBloomFilter<py::object>>(m, "CountingBloomFilter")
        .def(py::init<size_t, int>())
        .def("insert", &CountingBloomFilter<py::object>::insert)
        .def("contains", &CountingBloomFilter<py::object>::contains)
        .def("remove", &CountingBloomFilter<py::object>::remove)
        .def("count", &CountingBloomFilter<py::object>::count);
    // Exception translation
    py::register_exception_translator([](std::exception_ptr p) {
        try {
            if (p) std::rethrow_exception(p);
        } catch (const std::runtime_error &e) {
            PyErr_SetString(PyExc_RuntimeError, e.what());
        }
        // You can catch and translate other C++ exceptions here as needed
    });
}
