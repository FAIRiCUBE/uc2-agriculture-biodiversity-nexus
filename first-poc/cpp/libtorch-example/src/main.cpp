//
// Created by Rob Knapen, Wageningen Environmental Research on 1/11/23.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include <boost/format.hpp>
#include <boost/tokenizer.hpp>

#include <torch/torch.h>
#include <torch/script.h>

using namespace std::literals::string_literals;

// model types (for descriptive purposes), examples only, DL = deep learning
enum MLModelTypes {
    dl_binary_classification,
    dl_multiclass_classification,
    dl_semantic_segmentation,
    dl_object_detection
};

// what information to store for the trained models?
struct TrainedModelInfo {
    std::string id;
    MLModelTypes model_type;
    std::string purpose;            // one-line indication of the goal of the model
    std::string applicability;      // short clarification of the application domain of the model
    std::string ethical_notes;      // notes about ethical aspects of the model
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

// some defaults to use for the model registry (make sure path prefixes are correct!)
const auto torchscript_models_folder{"../../../models/"s};
const auto default_torchscript_model_id{"crops_segmentation_example_model_v20230112"s};
const auto input_files_folder{"../../../data/processed/"s};

void init_model_registry() {
    // the example trained model
    model_registry[default_torchscript_model_id] = {
            default_torchscript_model_id,
            dl_semantic_segmentation,
            "Semantic Segmentation of Sentinel 2 data to dutch crop types."s,
            "Trained on dutch crop types in the Flevoland region (NL), "
            "using R,G,B and NIR bands of 7 seasonal Sentinel 2 images from 2018."s,
            "This model is fully based on openly available data and "
            "privacy-preserving towards farmers."s,
            "rvo_crops_segnet_224x224x28_77classes_100epochs_model_full_traced.pt"s,
            28, 76, 100, 73.0
    };
}

// TODO: Proposed functionality via UDFs (if possible):
// - search/list all available models (return a 1D MDD with the text data)
// - describe a selected model (return a 1D MDD with the text data)
// - use a model for an inference task (-> write progress to a 1D MDD)
// - view progress of a model inference task (-> read the 1D MDD)
// - interrupt a model inference task
// - get result of a model inference task (-> N dimensional MDD, depending on model output type)

// TODO for full model inference:
//  - For the input the model needs to be applied to (the geotiff image or rasdaman mdd?)
//      - for each part (tile):
//          - create a vector of inputs (read the part of the input geotiff or rasdaman mdd?)
//              std::vector<torch::jit::IValue> inputs;
//              inputs.push_back(torch::ones({1, 3, 224, 224}));
//          - execute the model and turn its output into a tensor
//              at::Tensor output = module.forward(inputs).toTensor();
//              output some progress:
//                  std::clog << output.slice(/*dim=*/1, /*start=*/0, /*end=*/5) << '\n';
//          - merge the output tensor for the part into the total result
//      - write the total output (as geotiff image or mdd?)

// Note: Due to the need to process the data in tiles there can be
// inference artifacts at the tile's edges. This can be addressed by
// using overlapping tiles, and/or smarter merging of the tiles into
// the final output.

torch::Tensor readTensorFromCsvFile(const std::string& filename, int channels, int rows, int cols) {
    auto tensor_options = torch::TensorOptions()
            .dtype(torch::kFloat32)
            .layout(torch::kStrided)
            .device(torch::kCPU)
            .requires_grad(false);

    torch::Tensor result = torch::zeros({channels, rows, cols}, tensor_options);

    std::ifstream in(filename.c_str());
    if (!in.is_open()) return result;

    typedef boost::tokenizer< boost::escaped_list_separator<char> > Tokenizer;

    std::vector<std::string> vec;
    std::string line;

    int row_index = 0;
    int col_index = 0;
    while (getline(in, line)) {
        Tokenizer tok(line);
        vec.assign(tok.begin(), tok.end());

        // write channel values into the tensor
        int channel_index = 0;
        for (const auto& channel_value : vec) {
            result[channel_index][row_index][col_index] = std::stof(channel_value);
            ++channel_index;
        }

        // increment column and row indexes
        ++col_index;
        if (col_index >= cols) {
            ++row_index;
            col_index = 0;
        }
    }

    return result;
}

int main() {
    std::clog << ":: TorchScript Deep Learning Inference ::" << std::endl;

    init_model_registry();

    // set nograd guard since we will be doing inference only
    c10::InferenceMode guard;

    // try loading a TorchScript model
    std::clog << "loading the torchscript model ..." << std::endl;
    torch::jit::script::Module module;
    try {
        const auto model_file = torchscript_models_folder + model_registry[default_torchscript_model_id].file_path;
        std::clog << "file: " << model_file << std::endl;
        module = torch::jit::load(model_file);
    }
    catch (const c10::Error &e) {
        std::cerr << "error loading the model: " << (e.what()) << std::endl;
        return EXIT_FAILURE;
    }
    std::clog << "model loaded successfully" << std::endl;

    // create a tensor from input data read from a csv file (lines must contain channel data, sorted by rows by columns)
    std::clog << "Reading input data from csv file ..." << std::endl;
    const auto csv_input_filename{"sentinel2_normalised_100rows_100cols_28channels.csv"s};
    const auto csv_data_file = input_files_folder + csv_input_filename;
    std::clog << "file: " << csv_data_file << std::endl;
    torch::Tensor tensor = readTensorFromCsvFile(csv_data_file, 28, 100, 100);

    // use the model to infer the result
    std::clog << "Running model inference ..." << std::endl;
    module.eval();
    std::vector<torch::jit::IValue> inputs{tensor.unsqueeze(0)};
    auto outputs = module.forward(inputs).toTensor().squeeze();
    std::clog << "Inference completed" << std::endl;

    // get the inferred classes
    auto out_classes = outputs.argmax(0);

    // output the inferred classes using a tensor accessor
    std::cout << "Inferred classes for the input data:" << std::endl;
    auto out_classes_a = out_classes.accessor<std::int64_t, 2>();
    for(auto i = 0; i < out_classes_a.size(0); i++) {
        for (auto j = 0; j < out_classes_a.size(1); j++) {
            std::cout << boost::format("%2d ") % out_classes_a[i][j];
        }
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}
