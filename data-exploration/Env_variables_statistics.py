import geopandas as gpd
from rasterstats import zonal_stats
import numpy as np
import rasterio
import os
from glob import glob

# Define a custom Gini index function
def gini(array):
    """Calculate the Gini index of an array."""
    array = np.sort(array)
    n = len(array)
    cumulative_sum = np.cumsum(array, dtype=float)
    relative_mean = cumulative_sum[-1] / n
    gini_index = (n + 1 - 2 * (cumulative_sum / relative_mean).sum() / n) / n
    return gini_index

# Paths
gpkg_path = "grassland_regions.gpkg"
layer_name = "grassland_regions"
raster_folders = ["Stats"]

# Load the polygons
polygons = gpd.read_file(gpkg_path, layer=layer_name)

# Initialize lists to store all mean, variance, and Gini values across rasters for each polygon
all_means, all_variances, all_ginis = [], [], []

# Process each folder and each raster within the folder
for folder in raster_folders:
    raster_files = glob(os.path.join(folder, "*.tif"))

    for raster_path in raster_files:
        # Extract raster name to use as prefix for the columns
        raster_name = os.path.splitext(os.path.basename(raster_path))[0]

        # Perform zonal statistics and get individual pixel values
        stats = zonal_stats(polygons, raster_path, stats=["mean", "count"], raster_out=True)

        # Initialize lists to store results for this raster
        means, variances, ginis = [], [], []

        # Open the raster to get its size (for transformation purposes)
        with rasterio.open(raster_path) as src:
            raster_transform = src.transform

        # Calculate variance and Gini index per polygon for the current raster
        for i, stat in enumerate(stats):
            pixel_values = stat['mini_raster_array'].compressed()  # Extract non-NaN values
            if len(pixel_values) > 0:
                # Calculate mean and variance
                mean = np.mean(pixel_values)
                variance = np.var(pixel_values)

                # Calculate Gini index using custom function
                gini_index = gini(pixel_values)

                # Store the statistics in lists
                means.append(mean)
                variances.append(variance)
                ginis.append(gini_index)
            else:
                # Handle empty zones or no data
                means.append(None)
                variances.append(None)
                ginis.append(None)

        # Add the statistics as new columns in the GeoDataFrame
        polygons[f"{raster_name}_mean"] = means
        polygons[f"{raster_name}_variance"] = variances
        polygons[f"{raster_name}_gini"] = ginis

        # Append the results to overall lists for calculating averages later
        all_means.append(means)
        all_variances.append(variances)
        all_ginis.append(ginis)

# Calculate the mean value per type of each statistic (ignoring None values)
polygons["mean_of_means"] = np.nanmean(np.array(all_means), axis=0)
polygons["mean_of_variances"] = np.nanmean(np.array(all_variances), axis=0)
polygons["mean_of_ginis"] = np.nanmean(np.array(all_ginis), axis=0)

# Save the updated polygons with all statistics to a new GeoPackage
output_gpkg_path = "grass_region_stats_multiple_rasters_with_averages_v2.gpkg"
output_layer_name = "grass_stats_multiple_rasters_with_averages"
polygons.to_file(output_gpkg_path, layer=output_layer_name, driver="GPKG")

print("Zonal statistics with variance and Gini index for multiple rasters successfully calculated and saved to GeoPackage.")
