import os
import subprocess
import fiona
from shapely.geometry import shape
from tqdm import tqdm
import geopandas as gpd
import pandas as pd

# -------------------------------
# Set working directory
# -------------------------------
os.chdir('E:/PROJECTS/20221005_FAIRiCUBE/NDFF/All_species2')

# -------------------------------
# File paths and layer names
# -------------------------------
input_gpkg = 'z44349_1923.gpkg'
converted_gpkg = 'z44349_1923_converted.gpkg'
layer_name = 'z44349_1923'
filtered_output_gpkg = 'birds_all.gpkg'
filtered_output_layer = 'birds_all'
final_output_gpkg = 'breeding_birds_all.gpkg'
final_output_layer = 'breeding_birds_all'
csv_file = 'breedingbirds.csv'

# -------------------------------
# Step 1: Convert with ogr2ogr (only if file doesn't exist)
# -------------------------------
if not os.path.exists(converted_gpkg):
    ogr_command = [
        'ogr2ogr',
        '-f', 'GPKG',
        converted_gpkg,
        input_gpkg,
        '-nlt', 'PROMOTE_TO_MULTI',
        '-dim', 'XY',
        '-progress'
    ]

    print("Starting ogr2ogr conversion...")

    process = subprocess.Popen(
        ogr_command,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        universal_newlines=True
    )

    for line in process.stdout:
        print(line, end='')

    process.wait()

    if process.returncode != 0:
        raise RuntimeError("ogr2ogr failed.")
    else:
        print("\nogr2ogr completed successfully.\n")
else:
    print(f"{converted_gpkg} already exists — skipping ogr2ogr conversion.\n")

# -------------------------------
# Step 2: Stream-read and filter species group
# -------------------------------
print("Streaming and filtering for 'Vogels'...")

features = []
with fiona.open(converted_gpkg, layer=layer_name) as src:
    crs = src.crs  # Get CRS to reuse later
    try:
        total = len(src)
    except:
        total = None

    for feat in tqdm(src, total=total, desc="Filtering features"):
        props = feat["properties"]
        if props.get("spec_groep") == "Vogels":
            geom = shape(feat["geometry"])
            features.append({**props, "geometry": geom})

# Create GeoDataFrame with defined CRS
gdf = gpd.GeoDataFrame(features, crs=crs)

# Save filtered 'Vogels' data
print(f"Saving {len(gdf)} 'Vogels' records to {filtered_output_gpkg}...")
gdf.to_file(filtered_output_gpkg, layer=filtered_output_layer, driver='GPKG')

# -------------------------------
# Step 3: Filter by obs_uri using CSV
# -------------------------------
print("Filtering further using CSV list of obs_uri...")

csv_df = pd.read_csv(csv_file)
matching_ids = csv_df['obs_uri'].unique()

filtered_gdf = gdf[gdf['obs_uri'].isin(matching_ids)]

# Save final result
print(f"Saving {len(filtered_gdf)} breeding bird records to {final_output_gpkg}...")
filtered_gdf.to_file(final_output_gpkg, layer=final_output_layer, driver='GPKG')

print("All processing complete.")
