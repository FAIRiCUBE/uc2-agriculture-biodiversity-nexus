import numpy as np
import netCDF4 as nc

# Create random integer data for 28 variables
# data = np.random.randint(0, 100, size=(28,))

data = np.array([18803,17742,16911,16096,19135,17927,17077,
                      16199,17477,16813,16203,15558,18048,16736,
                      16152,15568,17712,16752,16144,15529,17632,
                      16704,16112,15544,17904,16928,16272,15672 ])


# Create a new netCDF file with one dimension and 28 variables
with nc.Dataset('maxes_sentinel2_2018_flevopolder_10m_7x4bands.nc', 'w') as file:
    file.createDimension('dim', 1)
    for i in range(28):
        variable = file.createVariable(f'Band_{i+1}', 'i4', ('dim',))
        variable[:] = data[i]