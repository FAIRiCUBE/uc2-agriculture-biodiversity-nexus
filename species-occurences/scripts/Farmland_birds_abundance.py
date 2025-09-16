# Import packages
import os
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import geopandas as gp
import shapely
from shapely.geometry import Polygon
from shapely.wkt import loads as wkt_loads
from shapely.geometry import Polygon
from osgeo import ogr
# import time
# from datetime import timedelta
# # import dask_geopandas as dg

!pip install loguru codecarbon --quiet --user
# !pip install gdal --quiet --user

# from osgeo import ogr
# from measurer import Measurer
# from types import ModuleType

# import warnings
# from shapely.errors import ShapelyDeprecationWarning

# warnings.filterwarnings("ignore", category=ShapelyDeprecationWarning)

# Set workspace to working directory
os.chdir('/home/vittekm')
os.getcwd()

# Set pandas to display all columns next to each other without wrapping
pd.set_option('display.max_columns', None)  # Show all columns
pd.set_option('display.expand_frame_repr', False)  # Disable wrapping
pd.set_option('display.max_colwidth', None)  # Ensure that wide columns are fully shown

# get logger, and initialise a measurer tracker
import sys
from loguru import logger
from measurer import Measurer
from types import ModuleType

#logger.remove(0)
logger.add(sys.stdout, level='INFO')

# Start Measurer
measurer_abundance = Measurer()
tracker_abundance = measurer_abundance.start(logger=logger)

# Specify area of interest
# Full extent:
# aoi_xmin = 136000
# aoi_xmax = 223870
# aoi_ymin = 428495
# aoi_ymax = 535555

# Noordoost polder:
# aoi_xmin = 168910
# aoi_xmax = 194950
# aoi_ymin = 513945
# aoi_ymax = 535555

# Flevoland:
# aoi_xmin = 147460
# aoi_xmax = 186160
# aoi_ymin = 475295
# aoi_ymax = 513095

# Gelderse Vallei
aoi_xmin = 143140
aoi_xmax = 182350
aoi_ymin = 439525
aoi_ymax = 482745

# Create grid layer - in separate script

# Path to the GeoPackage file
geopackage_path = "farmland_breeding_birds_2018.gpkg"
# Layer in the geopackage
layer_name = "farmland_breeding_birds_2018"

# Load grid layer
aoi_grid = "grid_grassland_r3_100m.gpkg"
grid_layer_name = "grid_grassland_r3_100m"
grid_gdf = gp.read_file(aoi_grid, layer=grid_layer_name)

# Open the GeoPackage file using GDAL
gpkg = ogr.Open(geopackage_path)

# Check if the GeoPackage file was opened successfully
if gpkg is None:
    raise Exception(f"Could not open GeoPackage file: {geopackage_path}")

# Get the specified layer by name
layer = gpkg.GetLayerByName(layer_name)

# Check if the layer was retrieved successfully
if layer is None:
    raise Exception(f"Layer '{layer_name}' not found in GeoPackage.")

# Prepare to load features into a list for GeoPandas
features = []
for feature in layer:
    # Get geometry reference
    geom_ref = feature.GetGeometryRef()

    # Check for None geometries
    if geom_ref is None:
        continue  # Skip if geometry is None

    # Attempt to force the geometry to a simpler type
    try:
        # Convert the geometry to a MultiPolygon or simpler type
        geom_simple = ogr.ForceTo(geom_ref, ogr.wkbMultiPolygon)
    except Exception as e:
        print(f"Error forcing geometry to MultiPolygon: {e}, trying to force to MultiLineString.")
        # Try another geometry type
        try:
            geom_simple = ogr.ForceTo(geom_ref, ogr.wkbMultiLineString)
        except Exception as e2:
            print(f"Error forcing geometry to MultiLineString: {e2}, using original geometry.")
            geom_simple = geom_ref  # Use the original geometry if all else fails

    # Convert the simplified geometry to WKT
    geometry_wkt = geom_simple.ExportToWkt()

    try:
        # Convert geometry WKT to a shapely object
        geometry = wkt_loads(geometry_wkt)
    except Exception as e:
        print(f"Error converting WKT to Shapely: {e}")
        continue  # Skip feature if there's an error parsing its geometry

    # Get attributes of the feature
    attributes = feature.items()

    # Add the geometry to the attributes dictionary
    attributes['geometry'] = geometry

    # Check if the geometry is multi-part
    if hasattr(geometry, 'geoms'):
        # Use geometry.geoms to get individual parts
        for part in geometry.geoms:
            part_attributes = attributes.copy()
            part_attributes['geometry'] = part
            features.append(part_attributes)
    else:
        # Single-part geometry, add directly
        features.append(attributes)

# Close the GeoPackage file
# gpkg = None

# Convert the list of features to a GeoDataFrame
data = gp.GeoDataFrame(features)

# Set the coordinate reference system (CRS) if needed
data.set_crs(layer.GetSpatialRef().ExportToWkt(), inplace=True)

# Print the first few rows of the GeoDataFrame
print(data.head())

# # Make list of unique IDs
# bird_id_list = str(data.bird_id.unique())
# print(bird_id_list)

# remove non-numeric abundance values
data = data[pd.to_numeric(data['orig_abundance'], errors='coerce').notnull()]
# data['orig_abundance'] = data['orig_abundance'].astype(float) # gives warning
# data.loc[:, 'orig_abundance'] = data['orig_abundance'].astype(float) # gives warning
data = data.copy()  # Creates a deep copy of the GeoDataFrame slice
data['orig_abundance'] = data['orig_abundance'].astype(float)

# Get min max values and check attribute type
min_value = data['orig_abundance'].min()
max_value = data['orig_abundance'].max()
print("Minimum value:", min_value)
print("Maximum value:", max_value)

attribute_dtype = data['orig_abundance'].dtype
print("Data type is:", attribute_dtype)

# Create "bird_id" attribute
data['bird_id'] = data.groupby(['wetnaam']).ngroup()

# Make list of unique IDs
bird_id_list = str(data.bird_id.unique())
print(bird_id_list)

#####
# Extract unique bird_id numbers
unique_bird_ids = data.bird_id.unique()

# Create a list of tuples containing bird_id and corresponding "wetnaam" attribute
bird_id_list = [(bird_id, data.loc[data['bird_id'] == bird_id, 'wetnaam'].iloc[0]) for bird_id in unique_bird_ids]

# Convert the list of tuples to a string for printing
bird_id_list_str = str(bird_id_list)

# Print the list
print(bird_id_list_str)
#####

# Make list of unique abundance values
abundance_list = data.orig_abundance.unique()
print(abundance_list)

# Add area attribute
data['area'] = data.area

# start_all = time.time()

# Calculating overlap
joined_aoi_gdf = gp.overlay(grid_gdf, data, how='union')
joined_aoi_gdf['area_joined'] = joined_aoi_gdf.area
joined_aoi_gdf.head(3)

# end_all = time.time()
# elapsed_all = (time.time() - start_all)
# duration_all = str(timedelta(seconds=elapsed_all))
# print(" Total duration: " + duration_all)

# Make list of unique IDs
bird_id_list = str(joined_aoi_gdf.bird_id.unique())
print(bird_id_list)

# Counting NaN values in the 'Salary' column
birdid_nan_count = joined_aoi_gdf['bird_id'].isna().sum()
print(birdid_nan_count)

joined_aoi_gdf['bird_id_int'] = joined_aoi_gdf['bird_id'].fillna(99).astype(int)
# Make list of unique IDs
bird_id_list_int = joined_aoi_gdf.bird_id_int.unique()
print(bird_id_list_int)

# # Create separate DataFrames with dynamic names
# for bird_id_int in bird_id_list_int:
#     df_slice = joined_aoi_gdf.loc[joined_aoi_gdf['bird_id_int'] == bird_id_int].copy()
#     locals()[f"joined_aoi_gdf_{bird_id_int}"] = df_slice

# Create separate DataFrames with dynamic names and perform calculations
for bird_id_int in bird_id_list_int:
    # Filter and copy the DataFrame
    df_slice = joined_aoi_gdf.loc[joined_aoi_gdf['bird_id_int'] == bird_id_int].copy()

    # Perform calculations
    df_slice['obs_share'] = df_slice['area_joined'] / df_slice['area']
    df_slice['abundance_share'] = df_slice['obs_share'] * df_slice['orig_abundance']

    # Dynamically create a new DataFrame variable
    locals()[f"joined_aoi_gdf_{bird_id_int}"] = df_slice

# Concatenating species DFs along rows
joined_aoi_gdf_species = pd.concat([joined_aoi_gdf_0, joined_aoi_gdf_1, joined_aoi_gdf_2, joined_aoi_gdf_3, joined_aoi_gdf_4, joined_aoi_gdf_5, joined_aoi_gdf_6, joined_aoi_gdf_7, joined_aoi_gdf_8, joined_aoi_gdf_9, joined_aoi_gdf_10, joined_aoi_gdf_11, joined_aoi_gdf_12, joined_aoi_gdf_13, joined_aoi_gdf_14, joined_aoi_gdf_15, joined_aoi_gdf_16, joined_aoi_gdf_17, joined_aoi_gdf_18, joined_aoi_gdf_19, joined_aoi_gdf_20, joined_aoi_gdf_21, joined_aoi_gdf_22, joined_aoi_gdf_99], axis=0)

merged_aoi_gdf = gp.sjoin(joined_aoi_gdf_species, grid_gdf, how='left', op='covered_by')

merged_aoi_gdf = gp.sjoin(joined_aoi_gdf_species, grid_gdf, how='left')

dissolve = merged_aoi_gdf.dissolve(by=['bird_id_int', 'index_right'], aggfunc={'abundance_share': 'sum'})

dissolve.reset_index(inplace=True, level=['bird_id_int'])

a = str(dissolve.bird_id_int.unique())
print(a)

# copy GeoDataFrame
birds_points = dissolve.copy()
# change geometry
birds_points['geometry'] = birds_points['geometry'].centroid
birds_points.head(5)

dissolve.to_file("fbi_birds_2018_grassland_r3_abundance_species_polygons_100m_v3.gpkg", driver="GPKG")
birds_points.to_file("fbi_birds_2018_arable_r3_abundance_species_points_100m_v3.gpkg", driver="GPKG")
birds_points.to_file("textfile", driver="CSV", geometry="AS_WKT", layer="fbi_birds_2018_arable_r3_abundance_species_points_100m_v3")
# birds_points.to_file("fbi_birds_2018_arable_r3_abundance_species_points_100m_v2.csv", driver="CSV", geometry="AS_WKT")

# End Measurer
measurer_abundance.end(
    tracker=tracker_abundance,
    shape=[],
    libraries=[k for k,v in globals().items() if type(v) is ModuleType and not k.startswith('__')],
    #program_path=__file__,
    variables=locals(),
    csv_file='Measured_FB_abundance.csv',
    logger=logger
)


