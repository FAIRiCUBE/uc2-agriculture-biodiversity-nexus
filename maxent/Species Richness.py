import rasterio
import numpy as np
import matplotlib.pyplot as plt
from rasterio.plot import show

def normalize_raster_files(raster_files, output_file):
    # Initialize variables
    num_species = len(raster_files)
    total_raster = None

    # Loop through each raster file
    for i, raster_file in enumerate(raster_files):
        with rasterio.open(raster_file) as src:
            # Read the raster data
            raster_data = src.read(1)

            # Initialize the total_raster with the first raster's data
            if total_raster is None:
                total_raster = np.zeros_like(raster_data, dtype=np.float32)

            # Sum the raster values
            total_raster += raster_data

    # Normalize the total raster by dividing by the number of species
    normalized_raster = total_raster / num_species

    # Save the normalized raster to a new file
    with rasterio.open(raster_files[0]) as src:
        meta = src.meta.copy()
        meta.update(dtype=rasterio.float32, count=1)

        with rasterio.open(output_file, 'w', **meta) as dst:
            dst.write(normalized_raster, 1)

    print(f"Normalized raster saved to {output_file}")

    # Plot the resulting raster
    plot_raster(output_file)

def plot_raster(raster_file):
    # Open the raster file
    with rasterio.open(raster_file) as src:
        # Read the raster data
        raster_data = src.read(1)

        # Plot the raster
        plt.figure(figsize=(10, 10))
        ax = plt.gca()

        # Use a colormap suitable for probability data (e.g., 'viridis' or 'plasma')
        cmap = 'viridis'
        im = show(raster_data, ax=ax, transform=src.transform, cmap=cmap, vmin=0, vmax=1)

        # Add a colorbar
        cbar = plt.colorbar(im.get_images()[0], ax=ax, orientation='vertical', fraction=0.046, pad=0.04)
        cbar.set_label('Normalized Species Richness')

        # Add title and labels
        plt.title('Normalized Predicted Richness of Farmland Birds')
        plt.xlabel('X')
        plt.ylabel('Y')

        # Show the plot
        plt.show()

# Example usage
raster_files = ['C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2022/Torenvalk_arable_2022.tif',
                'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2022/Gele kwikstaart_arable_2022.tif',
                'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2022/Spreeuw_arable_2022.tif']

# raster_files = ['C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2022/Veldleeuwerik_arable_2022.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2022/Scholekster_arable_2022.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2022/Gele kwikstaart_arable_2022.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2022/Spreeuw_arable_2022.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2022/Kievit_arable_2022.tif']

# raster_files = ['C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Grassland/2018/Kievit_grassland_2018.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Grassland/2018/Grutto_grassland_2018.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Grassland/2018/Tureluur_grassland_2018.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Grassland/2018/Watersnip_grassland_2018.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Grassland/2018/Wulp_grassland_2018.tif']

# raster_files = ['C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2018/Veldleeuwerik_arable_2018.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2018/Slobeend_arable_2018.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2018/Graspieper_arable_2018.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2018/Putter_arable_2018.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2018/Roek_arable_2018.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2018/Kwartel_arable_2018.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2018/Geelgors_arable_2018.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2018/Torenvalk_arable_2018.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2018/Watersnip_arable_2018.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2018/Scholekster_arable_2018.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2018/Spotvogel_arable_2018.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2018/Boerenzwaluw_arable_2018.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2018/Grutto_arable_2018.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2018/Gele kwikstaart_arable_2018.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2018/Wulp_arable_2018.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2018/Ringmus_arable_2018.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2018/Spreeuw_arable_2018.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2018/Grasmus_arable_2018.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2018/Tureluur_arable_2018.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2018/Grote lijster_arable_2018.tif',
#                 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2018/Kievit_arable_2018.tif']

output_file = 'C:/Users/hekke006/OneDrive - Wageningen University & Research/Applied Spatial Research/FAIRiCUBE/MaxEnt/New runs 25082025/Arable/2022/Species_richness_3_arable_2022.tif'

normalize_raster_files(raster_files, output_file)

