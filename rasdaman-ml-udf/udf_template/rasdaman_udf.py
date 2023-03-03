import numpy as np
import rasterio
import torch
from osgeo import gdal

import rasterio
from rasterio.plot import show
import matplotlib.pyplot as plt

# files to processing

# torchscript trained model to use for inference
model_file = "/media/ocampos/recovery/Repositories/l-sis/projects/FAIRiCUBE/Code/rastorch_proof_of_concept/pytorch_trained_models/rvo_crops_segnet_224x224x28_77classes_100epochs_model_full_traced.pt"
# input_file = "/media/ocampos/recovery/Repositories/l-sis/projects/FAIRiCUBE/Code/rastorch_proof_of_concept/sentinel2_2018_flevopolder_10m_7x4bands.tif"
input_file = "/media/ocampos/recovery/Repositories/l-sis/projects/FAIRiCUBE/Code/rastorch_proof_of_concept/s2_9x9_cutout.tif"

# load the torchscript model
model = torch.jit.load(model_file)
print(model)

# Examine the input dataset
# visualise the input file using rasterio
input_ds = rasterio.open(input_file)

print('Input image width   :', input_ds.width)
print('Input image height  :', input_ds.height)
print('Input image bounds  :', input_ds.bounds)
print('Input image indexes :', input_ds.indexes)
print('Input dataset shape :', input_ds.read().shape)

# Access the input dataset with GDAL
# try to open the input file with gdal
dataset = gdal.Open(input_file)
if not dataset:
    print("could not read the file")

# get some information from the file
print("Driver: {}/{}".format(dataset.GetDriver().ShortName, dataset.GetDriver().LongName))
print("Size is {} x {} x {}".format(dataset.RasterXSize, dataset.RasterYSize, dataset.RasterCount))
print("Projection is {}".format(dataset.GetProjection()))
geotransform = dataset.GetGeoTransform()
if geotransform:
    print("Origin = ({}, {})".format(geotransform[0], geotransform[3]))
    print("Pixel Size = ({}, {})".format(geotransform[1], geotransform[5]))

# get some raster band information
band = dataset.GetRasterBand(1)
print("Band Type={}".format(gdal.GetDataTypeName(band.DataType)))

min = band.GetMinimum()
max = band.GetMaximum()
if not min or not max:
    (min,max) = band.ComputeRasterMinMax(True)
print("Min={:.3f}, Max={:.3f}".format(min,max))

if band.GetOverviewCount() > 0:
    print("Band has {} overviews".format(band.GetOverviewCount()))

if band.GetRasterColorTable():
    print("Band has a color table with {} entries".format(band.GetRasterColorTable().GetCount()))

# Read a subset from the input dataset with GDAL and normalise it
# read multiple bands from the dataset into a numpy array
arr = dataset.ReadAsArray(
    xoff=0,
    yoff=0,
    xsize=9,
    ysize=9,
    buf_obj=None,
    buf_xsize=None,
    buf_ysize=None,
    buf_type=None,
    resample_alg=0,
    callback=None,
    callback_data=None,
    interleave='band',
    band_list=None
).astype("float32")

arr.shape
copy_arr = np.copy(arr)


# normalize the band data before the inference

# This duplicates how the training data was normalized, so that the values
# that we will pass to the model later for inference are in the range it
# expects. Usually somewhere between [-1,1].

def normalize(ds, tile_np):
    for index in range(tile_np.shape[0]):
        band = ds.GetRasterBand(index + 1)
        (_, b_max) = band.ComputeRasterMinMax(True)
        tile_np[index] /= b_max

# normalize the data and check a subset of it
normalize(dataset, arr)

arr[0:1,0:5,0:5]

# Turn the data into a torch tensor and run it through the model
# re-order the dimensions (if needed) and add a batch dimension
# the model expects: [batch, bands, x, y]
tile = arr.transpose(0,1,2)
tile_t = torch.from_numpy(tile).unsqueeze(dim=0)
tile_t.shape

# check a subset of the tensor
tile_t[:,0:1,0:5,0:5]

# infer the output, take the max index, and remove the batch dimension
# also, ask torch to not waste time on tracking gradients

# The max index refers to the most likely crop class, according to the model.

model.eval()
with torch.no_grad():
    pred = model(tile_t).argmax(1).squeeze()

# check a subset of the predictions
print(pred.numpy()[0:9,0:9])

# infer the output, take the max index, and remove the batch dimension
# also, ask torch to not waste time on tracking gradients

# The max index refers to the most likely crop class, according to the model.

model.eval()
with torch.no_grad():
    pred = model(tile_t).argmax(1).squeeze()

print()


sample_tile_file = "/media/ocampos/recovery/Repositories/l-sis/projects/FAIRiCUBE/Code/rastorch_proof_of_concept/sentinel2_2018_flevopolder_10m_7x4bands.tif"
sample_tile_ds = rasterio.open(sample_tile_file)
show(sample_tile_ds, cmap="gist_earth")

print()

# get only a very small cutout (8x8 is the minimum due to conv padding)
# - take 9x9 to have a 'centre' cell to examine
# - the inferred classes at the borders are less stable (lack of data)

x_off  = 250
y_off  = 200
x_size = 250
y_size = 250

dataset = gdal.Open(input_file)
arr = dataset.ReadAsArray(xoff=x_off, yoff=y_off, xsize=x_size, ysize=y_size, interleave='band', band_list=None).astype("float32")
sample_x = x_size//2
sample_y = y_size//2
print("coordinates of sample cell             : ", sample_x, sample_y)
print("input centre cell value channel 3      : ", arr[3, sample_x, sample_y])

# normalize it
normalize(dataset, arr)
print("normalized centre cell value channel 3 : ", arr[3, sample_x, sample_y])

# create a tensor from the cutout
tile = arr.transpose(0, 1, 2)
tile_t = torch.from_numpy(tile).unsqueeze(dim=0)
print("input centre tensor value channel 3    : ", tile_t[0, 3, sample_x, sample_y])

# inference
model.eval()
with torch.no_grad():
    pred = model(tile_t)
    pred_class = pred.argmax(1)
    print("inferred centre tensor argmax class    : ", pred_class[0, sample_x, sample_y])
    print("inferred centre tensor argmax proba    : ", pred[0, pred_class[0, sample_x, sample_y], sample_x, sample_y])

## table with all inferred crop classes
pred_class.squeeze().numpy()[0:x_size, 0:y_size]
