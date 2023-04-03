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

const auto torchscript_model{"/data/rvo_crops_segnet_224x224x28_77classes_100epochs_model_full_traced.pt"s};
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
 * @param arrayData input array data
 * @param arraySize size of the arrayData
 * @param maxPerBand multidimensional array MDD with the max values for all 28 bands
 * @return float vector containing the arrange normalized data
 */
std::vector<float> getNormalizedVector(MyBaseType *arrayData, size_t arraySize, r_GMarray* maxPerBand){

    std::vector<float> result;

    // Retrieve the max values
    MyBaseType *maxData = reinterpret_cast<MyBaseType*>(maxPerBand->get_array());

    // Iterate over arrayData
    for (size_t i = 0; i < arraySize; ++i) {

        auto temp = (arrayData + i);

        // Iterate over bands
        for (int j = 0; j < 28; ++j) {
            // Normalize
            float tempNormalized = 0;
            switch (j) {
                case 0:
                    tempNormalized = temp->band0/(ushort)maxData->band0;
                    break;

                case 1:
                    tempNormalized = temp->band1/(ushort)maxData->band1;
                    break;

                case 2:
                    tempNormalized = temp->band2/(ushort)maxData->band2;
                    break;

                case 3:
                    tempNormalized = temp->band3/(ushort)maxData->band3;
                    break;

                case 4:
                    tempNormalized = temp->band4/(ushort)maxData->band4;
                    break;

                case 5:
                    tempNormalized = temp->band5/(ushort)maxData->band5;
                    break;

                case 6:
                    tempNormalized = temp->band6/(ushort)maxData->band6;
                    break;

                case 7:
                    tempNormalized = temp->band7/(ushort)maxData->band7;
                    break;

                case 8:
                    tempNormalized = temp->band8/(ushort)maxData->band8;
                    break;

                case 9:
                    tempNormalized = temp->band9/(ushort)maxData->band9;
                    break;

                case 10:
                    tempNormalized = temp->band10/(ushort)maxData->band10;
                    break;

                case 11:
                    tempNormalized = temp->band11/(ushort)maxData->band11;
                    break;

                case 12:
                    tempNormalized = temp->band12/(ushort)maxData->band12;
                    break;

                case 13:
                    tempNormalized = temp->band13/(ushort)maxData->band13;
                    break;

                case 14:
                    tempNormalized = temp->band14/(ushort)maxData->band14;
                    break;

                case 15:
                    tempNormalized = temp->band15/(ushort)maxData->band15;
                    break;

                case 16:
                    tempNormalized = temp->band16/(ushort)maxData->band16;
                    break;

                case 17:
                    tempNormalized = temp->band17/(ushort)maxData->band17;
                    break;

                case 18:
                    tempNormalized = temp->band18/(ushort)maxData->band18;
                    break;

                case 19:
                    tempNormalized = temp->band19/(ushort)maxData->band19;
                    break;

                case 20:
                    tempNormalized = temp->band20/(ushort)maxData->band20;
                    break;

                case 21:
                    tempNormalized = temp->band21/(ushort)maxData->band21;
                    break;

                case 22:
                    tempNormalized = temp->band22/(ushort)maxData->band22;
                    break;

                case 23:
                    tempNormalized = temp->band23/(ushort)maxData->band23;
                    break;

                case 24:
                    tempNormalized = temp->band24/(ushort)maxData->band24;
                    break;

                case 25:
                    tempNormalized = temp->band25/(ushort)maxData->band25;
                    break;

                case 26:
                    tempNormalized = temp->band26/(ushort)maxData->band26;
                    break;

                case 27:
                    tempNormalized = temp->band27/(ushort)maxData->band27;
                    break;

                default:
                    break;
            }
            // Push to result vector
            result.push_back(tempNormalized);
        }
    }

    return result;

}

/**
 * @brief Predicts the crop classification for a preprocessed sentinel2 image
 * @param inputImg Preprocessed 28 band Sentinel-2 image
 * @param maxesPerBand Maxes per band from inputImg 
 * @return r_GMarray containing the predicted output of applying the trained model to the sentinel2 image
 */
extern "C" r_GMarray* predictCropClass(r_GMarray *inputImg, r_GMarray *maxesPerBand) {

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
    const auto& domArray = inputImg->spatial_domain(); // Get the GMarray spatial domain. e.g., [0:250, 0:250]
    size_t arraySize = domArray.cell_count(); // Get the size of the array

    // Input dimensions
    int x = domArray[0].get_extent();
    int y = domArray[1].get_extent();

     // Format interleaved values
     using BandType = unsigned short;
     const size_t bandCount = 28;
     const size_t cellSize = bandCount * sizeof(BandType);
     char *presult = new char[arraySize*bandCount*sizeof(BandType)];
     char *src = inputImg->get_array();

     for (size_t band = 0, bandOffset = 0; band < bandCount; ++band, bandOffset += sizeof(BandType)) {
        auto *dst = presult + bandOffset;
        for (size_t cell = 0; cell < arraySize; ++cell, dst += cellSize, src += sizeof(BandType)) {
             *reinterpret_cast<BandType*>(dst) = *reinterpret_cast<BandType*>(src);
        }
    }

    // Pointer to the arranged data
    MyBaseType *arrayData = reinterpret_cast<MyBaseType*>(presult);

    // Get a normalized vector from arrayData
    std::vector<float> inputVector =  getNormalizedVector(arrayData, arraySize, maxesPerBand);

    // #######################################
    // End rasdaman Data formatting code block
    // #######################################

    // ######################
    // Pytorch Code Block
    // ######################

    // Create torch::Tensor from inputVector
    torch::Tensor inputTensor = torch::from_blob(/*VOID **/ inputVector.data(),/*AT::INTARRAYREF*/ {bandCount, x, y});

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
    using BaseType = r_Short;
    const char *mddTypeName = "ShortImage";
    const char *mddTypeStructure = "marray<short, 2>";
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
    short *pred_data_ptr = (short*)pred.data_ptr();

    // Copy the predicted elements to the GMarray
    for (int i = 0; i < pred.numel(); ++i)
    {
        auto temp = *(pred_data_ptr + i);
        *(resArrayData + i) = (r_Long)temp;

        // Debug only
        // std::cout << "*(pred_data_ptr" << " + " << i << ") = " << temp << std::endl;
    }

    result->set_array(reinterpret_cast<char*>(resArrayData));

    // ###############################
    // End Result r_GMarray code block
    // ###############################

    return result;

}

// ------------------------------------------------
// ------------------------------------------------
// ---------------- FAIRICUBE UDFs ----------------
// ------------------------------------------------
// ------------------------------------------------
