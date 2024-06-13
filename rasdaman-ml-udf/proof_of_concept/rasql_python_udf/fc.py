import torch
import numpy as np

def normalize_bands(s2_cutout, maxes_per_band):
    band_count, x, y = s2_cutout.shape[0], s2_cutout[0].shape[0], s2_cutout[0].shape[1]
    ret = np.zeros((band_count, x, y), dtype=np.float32)
    for band in range(band_count):
        band_slice = s2_cutout[band].astype(np.float32)
        ret[band] = band_slice / maxes_per_band[band]
    return ret

def predict_crop_class(s2_cutout, maxes_per_band):
    # Divide band pixels by the corresponding max, and cast the array to float64
    s2_cutout_normalized = normalize_bands(s2_cutout, maxes_per_band)

    # Load the model
    model_path = "/home/rknapen/rastorch_proof_of_concept/pytorch_trained_models/rvo_crops_segnet_224x224x28_77classes_100epochs_model_full_traced.pt"
    module = torch.jit.load(model_path)
    
    # Convert numpy array to torch tensor
    input_tensor = torch.from_numpy(s2_cutout_normalized)
    band_count, x, y = input_tensor.shape
    
    # Set the model to evaluation mode
    module.eval()
    
    # Unsqueeze the input tensor to add a batch dimension
    inputs = input_tensor.unsqueeze(0)
    
    # Run inference
    with torch.inference_mode():
        pred = module(inputs)
    
    # Get the inferred classes
    ret = pred.squeeze().argmax(0).numpy()

    # Pytorch returns int64 which rasdaman doesn't support, so we cast it down to int32
    ret = ret.astype(np.int32)

    return ret

def test():
    # For testing when executed outside rasdaman: generate some random data for s2_cutout and maxes_per_band
    band_count, x, y = 28, 8, 8
    s2_cutout = np.random.rand(band_count, x, y).astype(np.float32)
    maxes_per_band = np.random.rand(band_count).astype(np.float32)
    predicted_classes = predict_crop_class(s2_cutout, maxes_per_band)
    print(predicted_classes)

if __name__ == "__main__":
    test()
