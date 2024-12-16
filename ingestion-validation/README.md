# Quality check in FAIRiCube / Rasdaman (WCS)
The Python file `quality_check.py` contains utilities to perform quality checks of raster data ingested into FAIRiCube / Rasdaman (WCS).

# List of quality checks
Quality check is done by comparing metadata and statistics of the original raster file, and the created Rasdaman datacube. Specifically, the following list of checks is currently implemented:
- Metadata
    - 1.1, CRS
    - 1.2, cell_size_x
    - 1.2, cell_size_y
    - 1.3, bounds
    - 1.4, n_bands
    - 1.5, dtype
    - 1.6, nodata
    - 1.7, timestamp
- Statistics
    - 2.1, min
    - 2.2, max
    - 2.3, mean
    - 2.4, std
    - 2.5, count
    
Statistics are computed for an area of interest that must be smaller than (2500, 2500) pixels.

## Quality check report

The quality checker can save the results of the checks in a report in CSV format.

## Example
To see the `validate_source_to_wcs.py` in action, change the following parameters in the main function:
```python
    collection_name = "<name-of-ingested-collection>"               
    collection_id   ='<id-of-ingested-collection>'
    original_raster ="<path/to/original/raster.tif"
```
and run the script.

Note: The script only works within the FAIRiCube / Rasdaman (WCS) platform.