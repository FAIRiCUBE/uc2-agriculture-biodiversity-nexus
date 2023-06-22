//
// Created by Otoniel Campos, Constructor University Bremen on 09/02/23.
//

// rasdaman includes
#include "raslib/minterval.hh"
#include "raslib/odmgtypes.hh"
#include "raslib/error.hh"
#include "raslib/type.hh"
#include "rasodmg/database.hh"
#include "rasodmg/transaction.hh"
#include "rasodmg/set.hh"
#include "rasodmg/ref.hh"
#include "rasodmg/gmarray.hh"
#include "rasodmg/iterator.hh"
#include "rasodmg/oqlquery.hh"

#include <Python.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <functional> 

#include <boost/format.hpp>
#include <boost/tokenizer.hpp>

// C includes
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cstring>
#include <sstream>
#include <stdlib.h>

double maxes[28];

// r_GMArray base type
struct MyBaseType { ushort band0; ushort band1; ushort band2; 
    ushort band3; ushort band4; ushort band5; 
    ushort band6; ushort band7; ushort band8; 
    ushort band9; ushort band10; ushort band11; 
    ushort band12; ushort band13; ushort band14; 
    ushort band15; ushort band16; ushort band17; 
    ushort band18; ushort band19; ushort band20; 
    ushort band21; ushort band22; ushort band23; 
    ushort band24; ushort band25; ushort band26; ushort band27; };

/**
 * @brief Returns the normalized arranged vector using the maximum value per band
 * @param arrayData base type array data
 * @param arraySize size of the arrayData
 * @return float vector containing the arrange normalized data
 */
std::vector<float> getTensorVector(MyBaseType *arrayData, size_t arraySize){

    std::vector<float> result;
    std::vector<float> tempVec;
    int max = 0;
    int counter = 0;
    unsigned int vecSize;

    // Go through each band and normalize each value using the max per band
    // Band 0
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band0);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 1
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band1);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 2
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band2);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 3
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band3);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 4
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band4);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 5
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band5);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 6
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band6);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 7
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band7);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 8
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band8);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 9
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band9);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 10
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band10);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 11
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band11);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 12
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band12);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 13
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band13);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 14
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band14);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 15
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band15);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 16
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band16);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 17
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band17);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 18
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band18);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 19
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band19);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 20
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band20);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 21
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band21);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 22
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band22);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 23
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band23);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 24
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band24);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 25
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band25);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 26
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band26);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    // Band 27
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band27);
    }

    // max = *max_element(std::begin(tempVec), std::end(tempVec));
    max = maxes[counter];
    counter++;

    // Normalization
    vecSize = tempVec.size();

    for(unsigned int i = 0; i < vecSize; i++)
    {
        tempVec[i] = tempVec[i] / max;
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();

    return result;

}

/**
 * @brief Predicts the crop classification for a predefined sentinel2 image
 * @param inputImg Sentinel-2 image
 * @return r_GMarray containing the predicted output of applying the trained model to the sentinel2 image
 */
extern "C" r_GMarray* predictCropClass(r_GMarray *s2_cutout, r_GMarray *maxesPerBand) {

    // Set the PYTHONPATH environmental variable
    const char* variableName = "PYTHONPATH";
    const char* variableValue = "/media/ocampos/recovery1/Repositories/l-sis/projects/FAIRiCUBE/Code/uc2-agriculture-biodiversity-nexus/rasdaman-ml-udf/proof_of_concept/rasql_python_udf/:.";
    
    setenv(variableName, variableValue, 1);

    // ###################################
    // rasdaman Data formatting code block
    // ###################################
    const auto& domArray = s2_cutout->spatial_domain(); // Get the GMarray spatial domain. e.g., [0:250, 0:250]
    size_t arraySize = domArray.cell_count(); // Get the size of the array


    const auto& r_domArray = maxesPerBand->spatial_domain(); // Get the GMarray spatial domain. e.g., [0:250, 0:250]
    size_t r_arraySize = r_domArray.cell_count(); // Get the size of the array

    double* r_array = (double*)maxesPerBand->get_array();

    std::cout << "Print the values of r_array..." << std::endl;
    for (size_t i = 0; i < r_arraySize; ++i)
    {
        maxes[i] = (double)*(r_array + i);
    }

    // Input dimensions
    int x = domArray[0].get_extent();
    int y = domArray[1].get_extent();

    // Format interleave values
    using BandType = unsigned short;
    const size_t bandCount = 28;
    const size_t cellSize = bandCount * sizeof(BandType);
    char *presult = new char[arraySize*bandCount*sizeof(BandType)];
    char *src = s2_cutout->get_array();

    for (size_t band = 0, bandOffset = 0; band < bandCount; ++band, bandOffset += sizeof(BandType)) {
        auto *dst = presult + bandOffset;
        for (size_t cell = 0; cell < arraySize; ++cell, dst += cellSize, src += sizeof(BandType)) {
            *reinterpret_cast<BandType*>(dst) = *reinterpret_cast<BandType*>(src);
        }
    }

    // Interleave arranged data
    // MyBaseType *arrayData = reinterpret_cast<MyBaseType*>(presult);

    // Create torch::Tensor from inputVector
    // std::vector<float> inputVector = getTensorVector(arrayData, arraySize);
    
    // #######################################
    // End rasdaman Data formatting code block
    // #######################################

    // ######################
    // Pytorch Code Block
    // ######################

    // Initialize the Python interpreter
    Py_Initialize();

    // Import the Python module containing the function
    PyObject* module = PyImport_ImportModule("predict");
    if (!module) {
        std::cerr << "Failed to import the Python module." << std::endl;
    }

    // Get the Python function from the module
    PyObject* function = PyObject_GetAttrString(module, "predictCropClass");
    if (!function || !PyCallable_Check(function)) {
        std::cerr << "Failed to retrieve the Python function." << std::endl;
        Py_XDECREF(function);
        Py_DECREF(module);
    }

    // Create a vector of floats
    std::vector<float> inputVector = {3.14, 2.71, 1.618, 0.0};

    // Convert the C++ vector to a Python list
    PyObject* inputList = PyList_New(inputVector.size());
    for (size_t i = 0; i < inputVector.size(); ++i) {
        PyObject* floatObject = PyFloat_FromDouble(static_cast<double>(inputVector[i]));
        PyList_SetItem(inputList, i, floatObject);
    }

    // Call the Python function with the input list
    PyObject* args = PyTuple_Pack(1, inputList);
    PyObject* result = PyObject_CallObject(function, args);

    // Check if the function call succeeded
    if (!result) {
        std::cerr << "Failed to call the Python function." << std::endl;
        Py_DECREF(args);
        Py_XDECREF(result);
        Py_XDECREF(function);
        Py_DECREF(module);
    }

    // Extract the sorted list from the Python result
    PyObject* sortedList = PyList_AsTuple(result);

    // Iterate over the sorted list and print the values
    std::cout << "Sorted array: ";
    for (Py_ssize_t i = 0; i < PyList_Size(sortedList); ++i) {
        PyObject* item = PyList_GetItem(sortedList, i);
        double value = PyFloat_AsDouble(item);
        std::cout << value << " ";
    }
    std::cout << std::endl;

    // Clean up Python objects and close the interpreter
    Py_DECREF(sortedList);
    Py_DECREF(result);
    Py_DECREF(args);
    Py_DECREF(function);
    Py_DECREF(module);
    Py_Finalize();

    // torch::Tensor inputTensor = torch::from_blob(/*VOID **/ inputVector.data(),/*AT::INTARRAYREF*/ {bandCount, x, y});
    // auto tensor_options = torch::TensorOptions()
    //         .dtype(torch::kFloat32)
    //         .layout(torch::kStrided)
    //         .device(torch::kCPU)
    //         .requires_grad(false);

    // Call the prediction module
    // module.eval();
    // std::vector<torch::jit::IValue> inputs{inputTensor.unsqueeze(0)};

    // get the inferred classes
    // auto pred = module.forward(inputs).toTensor().squeeze().argmax(0);

    // ######################
    // End Pytorch Code Block
    // ######################

    // ###########################
    // Result r_GMarray code block
    // ###########################

    // Result r_GMarray settings
    using BaseType = r_Long;
    const char *mddTypeName = "LongImage";
    const char *mddTypeStructure = "marray<long, 2>";
    // Define the interval
    std::string domain = "[0:" + std::to_string(x-1) + ",0:" + std::to_string(y-1) + "]";
    // Convert string to char
    char cdm[domain.size() + 1];
    std::strcpy(cdm, domain.c_str());
    r_Minterval mddDomain = r_Minterval(cdm);
    const auto baseTypeSize = sizeof(BaseType);

    // Create res r_GMarray
    auto *res = new (mddTypeName) r_GMarray(mddDomain, baseTypeSize);
    const auto mddSize = mddDomain.cell_count() * baseTypeSize;
    res->set_array_size(mddSize);
    const auto *mddType = r_Type::get_any_type(mddTypeStructure);
    res->set_type_schema(mddType);

    // Define res array (The data will be stored here)
    auto *resArrayData = new BaseType[mddSize];

    // Pointer to the pred data pointer of the output predicted tensor pred
    // src: https://discuss.pytorch.org/t/iterating-over-tensor-in-c/60333/2
    // long *pred_data_ptr = (long*)pred.data_ptr();
    long *pred_data_ptr =  new long[4]{10, 20, 30, 40};

    // Copy the predicted elements to the GMarray
    for (int i = 0; i < 4; ++i)
    {
        auto temp = *(pred_data_ptr + i);
        // Debug
        // std::cout << "*(pred_data_ptr" << " + " << i << ") = " << temp << std::endl; 
        *(resArrayData + i) = (r_Long)temp;
    }

    res->set_array(reinterpret_cast<char*>(resArrayData));

    // ###############################
    // End res r_GMarray code block
    // ###############################

    // #########
    // Debugging
    // #########

    // const auto& r_domArray = res->spatial_domain(); // Get the GMarray spatial domain. e.g., [0:250, 0:250]
    // size_t r_arraySize = r_domArray.cell_count(); // Get the size of the array

    // long* r_array = (long*)res->get_array();    

    // std::cout << "Print the values of r_array..." << std::endl;
    // for (int i = 0; i < r_arraySize; ++i)
    // {
    //     auto temp = *(r_array + i);
    //     std::cout << "*(r_array" << " + " << i << ") = " << temp << std::endl; 
    // }

    // #############
    // End Debugging
    // #############

    return res;

}//End extern "C" r_GMarray* predictCropClass(r_GMarray *s2_cutout)

// ------------------------------------------------
// ------------------------------------------------
// ---------------- FAIRICUBE UDFs ----------------
// ------------------------------------------------
// ------------------------------------------------

