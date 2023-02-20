#define PY_SSIZE_T_CLEAN
#include "Python.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "preprocess.h"
#include "msa.h"
#include "postprocess.h"
#include "align.h"

std::vector<std::string> string_list_to_vector(PyObject *py_list) {
    /*
     * Parse python list of strings to c++ vector of strings
     */
    long long size = PyList_Size(py_list);
    std::vector<std::string> token_vector;
    for (int i = 0; i < size; ++i) {
        PyObject *py_string = PyList_GetItem(py_list, i);
        const char *token = PyUnicode_AsUTF8(py_string);
        token_vector.emplace_back(token);
    }
    return token_vector;
}

PyObject *string_vector_to_list(const std::vector<std::string> &token_vector) {
    /*
     * Parse c++ vector of strings to python list of strings
     */
    PyObject *py_list = PyList_New(token_vector.size());
    if (!py_list) {
        return NULL;
    }
    for (int i = 0; i < token_vector.size(); ++i) {
        PyObject *py_string = PyUnicode_FromString(token_vector[i].c_str());
        if (!py_string) {
            Py_DECREF(py_list);
            return NULL;
        }
        if (PyList_SetItem(py_list, i, py_string) != 0) {
            Py_DECREF(py_string);
            Py_DECREF(py_list);
            return NULL;
        }
    }
    return py_list;
}

PyObject *nested_vector_to_list(const std::vector<std::vector<std::string>> &sequences) {
    PyObject *py_list = PyList_New(sequences.size());
    if (!py_list) {
        return NULL;
    }
    for (int i = 0; i < sequences.size(); ++i) {
        PyObject *py_token_list = string_vector_to_list(sequences[i]);
        if (!py_token_list) {
            return NULL;
        }
        if (PyList_SetItem(py_list, i, py_token_list) != 0) {
            Py_DECREF(py_list);
            return NULL;
        }
    }
    return py_list;
}

static PyObject *align_without_segment(PyObject *self, PyObject *args) {
    PyObject *hypothesis_list;
    PyObject *reference_list;
    PyObject *reference_label_list;

    if (!PyArg_ParseTuple(args, "O!O!O!", &PyList_Type, &hypothesis_list, &PyList_Type, &reference_list, &PyList_Type, &reference_label_list)) {
        return NULL;
    }

    std::vector<std::string> hypothesis = string_list_to_vector(hypothesis_list);
    std::vector<std::string> reference = string_list_to_vector(reference_list);
    std::vector<std::string> reference_label = string_list_to_vector(reference_label_list);

    std::vector<std::vector<std::string>> align_result = align_without_segment(hypothesis, reference, reference_label);
    PyObject *py_align_result = nested_vector_to_list(align_result);
    return Py_BuildValue("O", py_align_result);
}

static PyObject *align_with_auto_segment(PyObject *self, PyObject *args) {
    PyObject *hypothesis_list;
    PyObject *reference_list;
    PyObject *reference_label_list;

    if (!PyArg_ParseTuple(args, "O!O!O!", &PyList_Type, &hypothesis_list, &PyList_Type, &reference_list, &PyList_Type, &reference_label_list)) {
        return NULL;
    }

    std::vector<std::string> hypothesis = string_list_to_vector(hypothesis_list);
    std::vector<std::string> reference = string_list_to_vector(reference_list);
    std::vector<std::string> reference_label = string_list_to_vector(reference_label_list);

    std::vector<std::vector<std::string>> align_result = align_with_auto_segment(hypothesis, reference, reference_label);
    PyObject *py_align_result = nested_vector_to_list(align_result);
    return Py_BuildValue("O", py_align_result);
}

static PyObject *align_with_manual_segment(PyObject *self, PyObject *args) {
    PyObject *hypothesis_list;
    PyObject *reference_list;
    PyObject *reference_label_list;
    int segment_length = 0;
    int barrier_length = 0;

    if (!PyArg_ParseTuple(args, "O!O!O!ii", &PyList_Type, &hypothesis_list, &PyList_Type, &reference_list, &PyList_Type, &reference_label_list, &segment_length, &barrier_length)) {
        return NULL;
    }

    std::vector<std::string> hypothesis = string_list_to_vector(hypothesis_list);
    std::vector<std::string> reference = string_list_to_vector(reference_list);
    std::vector<std::string> reference_label = string_list_to_vector(reference_label_list);

    std::vector<std::vector<std::string>> align_result = align_with_manual_segment(hypothesis, reference, reference_label, segment_length, barrier_length);
    PyObject *py_align_result = nested_vector_to_list(align_result);
    return Py_BuildValue("O", py_align_result);
}

static PyMethodDef align4d_funcs[] = {
        {"align_without_segment", align_without_segment, METH_VARARGS, "multi-sequence alignment without segmentation."},
        {"align_with_auto_segment", align_with_auto_segment, METH_VARARGS, "multi-sequence alignment with automatic segmentation."},
        {"align_with_manual_segment", align_with_manual_segment, METH_VARARGS, "multi-sequence alignment with manual segmentation."},
        {NULL, NULL, 0, NULL}
};

static PyModuleDef align4d = {
        PyModuleDef_HEAD_INIT,
        "align4d",
        NULL,
        -1,
        align4d_funcs
};

PyMODINIT_FUNC
PyInit_align4d(void) {
    return PyModule_Create(&align4d);
}