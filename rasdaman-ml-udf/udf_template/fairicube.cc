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

#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <algorithm>

#include <boost/format.hpp>
#include <boost/tokenizer.hpp>

// C includes
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cstring>
#include <sstream>
#include <stdlib.h>

// Libtorch includes
#include <torch/torch.h>
#include "torch/script.h"

// ------------------------------------------------
// ------------------------------------------------
// ---------------- FAIRICUBE UDFs ----------------
// ------------------------------------------------
// ------------------------------------------------

using namespace std::literals::string_literals;

// model types (for descriptive purposes), examples only, DL = deep learning
// src: Created by Rob Knapen, Wageningen Environmental Research on 1/11/23.
enum MLModelTypes
{
    dl_binary_classification,
    dl_multiclass_classification,
    dl_semantic_segmentation,
    dl_object_detection
};

// what information to store for the trained models?
// src: Created by Rob Knapen, Wageningen Environmental Research on 1/11/23.
struct TrainedModelInfo
{
    std::string id;
    MLModelTypes model_type;
    std::string purpose;            // one-line indication of the goal of the model
    std::string applicability;      // short clarification of the application domain of the model
    std::string ethical_notes;      // notes on ethical aspects of the model
    std::string file_path;          // internal storage location
    int features_count = 0;         // number of predictors (inputs) required
    int classes_count = 0;          // number of inferred classes
    int epochs_count = 0;           // how much training did this model get?
    double accuracy_perc = 0.0;     // what (validation) accuracy did it reach?
    // enum output type? (text, scalar, mdd)
};

// the model registry
using ModelRegistry = std::map<std::string, TrainedModelInfo>;
ModelRegistry model_registry;

const auto torchscript_model{"rvo_crops_segnet_224x224x28_77classes_100epochs_model_full_traced.pt"s};
// some defaults to use for the model registry
const auto torchscript_models_folder{"../data/models/"s};
const auto default_torchscript_model_id{"crops_segmentation_example_model_v20230112"s};

void init_model_registry() {
    // the example trained model
    model_registry[default_torchscript_model_id] =
            {
                    default_torchscript_model_id,
                    dl_semantic_segmentation,
                    "Semantic Segmentation of Sentinel 2 data to dutch crop types."s,
                    "Trained on dutch crop types in the Flevoland region (NL), "
                    "using R,G,B and NIR bands of 7 seasonal Sentinel 2 images from 2018."s,
                    "This model is fully based on publicly available data and "
                    "privacy-preserving towards farmers."s,
                    "rvo_crops_segnet_224x224x28_77classes_100epochs_model_full_traced.pt"s,
                    28, 76, 10, 73.0
            };
}

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
std::vector<float> getNormalizedVector(MyBaseType *arrayData, size_t arraySize){

    std::vector<float> result;
    std::vector<float> tempVec;
    int max;
    unsigned int vecSize;

    // Go through each band and normalize each value using the max per band
    // Band 0
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        tempVec.push_back(temp->band0);
    }

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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

    max = *max_element(std::begin(tempVec), std::end(tempVec));

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
 * @brief Returns the normalized arranged vector using the maximum value per band
 * @param arrayData base type array data
 * @param arraySize size of the arrayData
 * @param maxPerBand size of the arrayData
 * @return float vector containing the arrange normalized data
 */
std::vector<float> getNormalizedVector(MyBaseType *arrayData, size_t arraySize, std::vector<ushort> maxPerBand){

    std::vector<float> result;
    std::vector<float> tempVec;
    int maxPerBandCounter = 0;

    // Go through each band and normalize each value using the values from maxPerBand
    // Band 0
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band0);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 1
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band1);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 2
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band2);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 3
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band3);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 4
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band4);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 5
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band5);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 6
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band6);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 7
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band7);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 8
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band8);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 9
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band9);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 10
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band10);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 11
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band11);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 12
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band12);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 13
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band13);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 14
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band14);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 15
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band15);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 16
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band16);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 17
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band17);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 18
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band18);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 19
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band19);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 20
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band20);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 21
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band21);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 22
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band22);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 23
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band23);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 24
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band24);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 25
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band25);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 26
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band26);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    // Band 27
    for (long unsigned int i = 0; i < arraySize; i++)
    {
        auto temp = (arrayData + i);
        ushort k = maxPerBand.at(maxPerBandCounter);
        tempVec.push_back(temp->band1);
        std::transform(tempVec.begin(), tempVec.end(), tempVec.begin(), [k](float &c){ return c/k; });
    }

    // Add the tempVec to the result vector and clear the tempVec
    result.insert(std::end(result), std::begin(tempVec), std::end(tempVec));
    tempVec.clear();
    maxPerBandCounter++;

    return result;

}

/**
 * @brief Predicts the crop classification for a predefined sentinel2 image
 * @param inputImg Sentinel-2 image
 * @return r_GMarray containing the predicted output of applying the trained model to the sentinel2 image
 */
extern "C" r_GMarray* predictionTest(r_GMarray *s2_cutout) {

    // ######################
    // Pytorch Code Block
    // ######################
    init_model_registry();

    // set nograd guard since we will be doing inference only
    c10::InferenceMode guard;

    // Load the TorchScript model
    torch::jit::script::Module module = torch::jit::load(torchscript_model);

    // ######################
    // End Pytorch Code Block
    // ######################

    // ###################################
    // rasdaman Data formatting code block
    // ###################################
    const auto& domArray = s2_cutout->spatial_domain(); // Get the GMarray spatial domain. e.g., [0:250, 0:250]
    size_t arraySize = domArray.cell_count(); // Get the size of the array

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
    MyBaseType *arrayData = reinterpret_cast<MyBaseType*>(presult);

    // Create torch::Tensor from inputVector
    std::vector<float> inputVector =  getNormalizedVector(arrayData, arraySize);

    // #######################################
    // End rasdaman Data formatting code block
    // #######################################

    // ######################
    // Pytorch Code Block
    // ######################

    torch::Tensor inputTensor = torch::from_blob(/*VOID **/ inputVector.data(),/*AT::INTARRAYREF*/ {bandCount, x, y});
    // auto tensor_options = torch::TensorOptions()
    //         .dtype(torch::kFloat32)
    //         .layout(torch::kStrided)
    //         .device(torch::kCPU)
    //         .requires_grad(false);

    // Call the prediction module
    module.eval();
    std::vector<torch::jit::IValue> inputs{inputTensor.unsqueeze(0)};

    // get the inferred classes
    auto pred = module.forward(inputs).toTensor().squeeze().argmax(0);

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

    // Create result r_GMarray
    auto *result = new (mddTypeName) r_GMarray(mddDomain, baseTypeSize);
    const auto mddSize = mddDomain.cell_count() * baseTypeSize;
    result->set_array_size(mddSize);
    const auto *mddType = r_Type::get_any_type(mddTypeStructure);
    result->set_type_schema(mddType);

    // Define result array (The data will be stored here)
    auto *resArrayData = new BaseType[mddSize];

    // Pointer to the pred data pointer
    // src: https://discuss.pytorch.org/t/iterating-over-tensor-in-c/60333/2
    long *pred_data_ptr = (long*)pred.data_ptr();

    // Copy the predicted elements to the GMarray
    for (int i = 0; i < pred.numel(); ++i)
    {
        auto temp = *(pred_data_ptr + i);
        std::cout << "*(pred_data_ptr" << " + " << i << ") = " << temp << std::endl; 
        *(resArrayData + i) = (r_Long)temp;
    }

    result->set_array(reinterpret_cast<char*>(resArrayData));

    // ###############################
    // End Result r_GMarray code block
    // ###############################

    // #########
    // Debugging
    // #########

    // const auto& r_domArray = result->spatial_domain(); // Get the GMarray spatial domain. e.g., [0:250, 0:250]
    // size_t r_arraySize = r_domArray.cell_count(); // Get the size of the array

    // long* r_array = (long*)result->get_array();    

    // std::cout << "Print the values of r_array..." << std::endl;
    // for (int i = 0; i < r_arraySize; ++i)
    // {
    //     auto temp = *(r_array + i);
    //     std::cout << "*(r_array" << " + " << i << ") = " << temp << std::endl; 
    // }

    // #############
    // End Debugging
    // #############

    return result;

}//End extern "C" r_GMarray* predictionTest(r_GMarray *s2_cutout)

/**
 * @brief Predicts the crop classification for a predefined sentinel2 image
 * @param inputImg Sentinel-2 image
 * @param maxesPerBand Maxes per band from inputImg 
 * @return r_GMarray containing the predicted output of applying the trained model to the sentinel2 image
 */
extern "C" r_GMarray* predictionTestV2(r_GMarray *s2_cutout, r_GMarray *maxesPerBand) {

    // ######################
    // Pytorch Code Block
    // ######################
    init_model_registry();

    // set nograd guard since we will be doing inference only
    c10::InferenceMode guard;

    // Load the TorchScript model
    torch::jit::script::Module module = torch::jit::load(torchscript_model);

    // ######################
    // End Pytorch Code Block
    // ######################

    // ###################################
    // rasdaman Data formatting code block
    // ###################################
    const auto& domArray = s2_cutout->spatial_domain(); // Get the GMarray spatial domain. e.g., [0:250, 0:250]
    size_t arraySize = domArray.cell_count(); // Get the size of the array

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
    MyBaseType *arrayData = reinterpret_cast<MyBaseType*>(presult);

    // Create torch::Tensor from inputVector
    std::vector<float> inputVector =  getNormalizedVector(arrayData, arraySize);

    // #######################################
    // End rasdaman Data formatting code block
    // #######################################

    // ######################
    // Pytorch Code Block
    // ######################

    torch::Tensor inputTensor = torch::from_blob(/*VOID **/ inputVector.data(),/*AT::INTARRAYREF*/ {bandCount, x, y});
    // auto tensor_options = torch::TensorOptions()
    //         .dtype(torch::kFloat32)
    //         .layout(torch::kStrided)
    //         .device(torch::kCPU)
    //         .requires_grad(false);

    // Call the prediction module
    module.eval();
    std::vector<torch::jit::IValue> inputs{inputTensor.unsqueeze(0)};

    // get the inferred classes
    auto pred = module.forward(inputs).toTensor().squeeze().argmax(0);

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

    // Create result r_GMarray
    auto *result = new (mddTypeName) r_GMarray(mddDomain, baseTypeSize);
    const auto mddSize = mddDomain.cell_count() * baseTypeSize;
    result->set_array_size(mddSize);
    const auto *mddType = r_Type::get_any_type(mddTypeStructure);
    result->set_type_schema(mddType);

    // Define result array (The data will be stored here)
    auto *resArrayData = new BaseType[mddSize];

    // Pointer to the pred data pointer
    // src: https://discuss.pytorch.org/t/iterating-over-tensor-in-c/60333/2
    long *pred_data_ptr = (long*)pred.data_ptr();

    // Copy the predicted elements to the GMarray
    for (int i = 0; i < pred.numel(); ++i)
    {
        auto temp = *(pred_data_ptr + i);
        std::cout << "*(pred_data_ptr" << " + " << i << ") = " << temp << std::endl; 
        *(resArrayData + i) = (r_Long)temp;
    }

    result->set_array(reinterpret_cast<char*>(resArrayData));

    // ###############################
    // End Result r_GMarray code block
    // ###############################

    // #########
    // Debugging
    // #########

    // const auto& r_domArray = result->spatial_domain(); // Get the GMarray spatial domain. e.g., [0:250, 0:250]
    // size_t r_arraySize = r_domArray.cell_count(); // Get the size of the array

    // long* r_array = (long*)result->get_array();    

    // std::cout << "Print the values of r_array..." << std::endl;
    // for (int i = 0; i < r_arraySize; ++i)
    // {
    //     auto temp = *(r_array + i);
    //     std::cout << "*(r_array" << " + " << i << ") = " << temp << std::endl; 
    // }

    // #############
    // End Debugging
    // #############

    return result;

}//End extern "C" r_GMarray* predictionTest(r_GMarray *s2_cutout)

// ------------------------------------------------
// ------------------------------------------------
// ---------------- FAIRICUBE UDFs ----------------
// ------------------------------------------------
// ------------------------------------------------
