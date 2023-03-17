#!/bin/bash
# Calculates the max per band of coverage sentinel2_2018_flevopolder_10m_7x4bands_maxes and ingest them to collection sentinel2_2018_flevopolder_10m_7x4bands_maxes

rasql -q 'insert into sentinel2_2018_flevopolder_10m_7x4bands_maxes values decode($1,"csv", "{ \"formatParameters\":{ \"domain\": \"[0:27]\",\"basetype\": \"ushort\" } }")' -f result.csv --user rasadmin --passwd rasadmin

# COLLECTION_NAME=sentinel2_2018_flevopolder_10m_7x4bands_maxes
# SET_TYPE=UShortSet1
# OUT=maxes_per_band
# OUTEXT=$OUT.csv

# # Credentials
# USER=rasadmin
# PWD=rasadmin
# RASQL="rasql --user $USER --passwd $PWD -q"

# $RASQL "create collection $COLLECTION_NAME $SET_TYPE"

# GET_CWD=$(pwd)
# $RASQL "select encode(max_cells(c), \"csv\") from $COLLECTION_NAME as c" --out file --outfile media/ocampos/recovery/Repositories/l-sis/projects/FAIRiCUBE/Code/uc2-biodiversity-agriculture/rasdaman-ml-udf/udf_template/out

# # Gets n-1
# MAX_BAND_COUNT=$(head -1 $OUT | sed 's/[^, ]//g' | wc -c)

# $RASQL 'insert into $COLLECTION_NAME values decode($1,\"csv\", \"{ \"formatParameters\":{ \"domain\": \"[0:$MAX_BAND_COUNT]\",\"basetype\": \"ushort\" } }\")' -f $out

# ## $RASQL "select encode(c, \"csv\") from $COLLECTION_NAME as c" --out file --outfile maxes

# # rm -rvf 