#!/bin/bash
# Calculates the max per band of coverage sentinel2_2018_flevopolder_10m_7x4bands_maxes and ingest them to collection sentinel2_2018_flevopolder_10m_7x4bands_maxes
RASQL="rasql --user $RAS_USER --passwd $RAS_PASSWD"

$RASQL 'insert into sentinel2_2018_flevopolder_10m_7x4bands_maxes values decode($1,"csv", "{ \"formatParameters\":{ \"domain\": \"[0:27]\",\"basetype\": \"ushort\" } }")' -f result.csv
