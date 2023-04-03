import netCDF4 as nc

# Replace 'filename.nc' with the actual filename and path
data = nc.Dataset('random_data.nc')

# Print the netCDF file metadata
print(data)

# Print the variable names in the netCDF file
print(data.variables.keys())

# Print the values of a specific variable in the netCDF file
print(data.variables['var'][:])