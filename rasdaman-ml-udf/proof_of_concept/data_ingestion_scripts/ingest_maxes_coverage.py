import numpy as np
import netCDF4
from io import StringIO

# Create a CSV string
csv_string = '18803,17742,16911,16096,19135,17927,17077,16199,17477,16813,16203,15558,18048,16736,16152,15568,17712,16752,16144,15529,17632,16704,16112,15544,17904,16928,16272,15672'

# Read the data from the CSV string into a numpy array
data = np.loadtxt(StringIO(csv_string), delimiter=',')

# Open the netCDF4 file for writing
ncfile = netCDF4.Dataset('maxes_sentinel2_2018_flevopolder_10m_7x4bands.nc', 'w', format='NETCDF4')

# Define dimensions and variables
dim = ncfile.createDimension('dim', len(data))
var = ncfile.createVariable('var', 'f8', ('dim',))

# Set variable attributes
var.units = 'scalars'

# Set variable data
var[:] = data

# Set global attributes
ncfile.history = 'Created by Otoniel Campos on 03.04.2023'

# Close the file
ncfile.close()
