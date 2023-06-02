#!/bin/sh
#
# This file is part of rasdaman community.
#
# Rasdaman community is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Rasdaman community is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
#
# Copyright 2003 - 2023 Peter Baumann / rasdaman GmbH.
#
# For more information please see <http://www.rasdaman.org>
# or contact Peter Baumann via <baumann@rasdaman.com>.
#
# ------------------------------------------------------------------------------
#
# SYNOPSIS
# ./1_create_udf.sh
#
# Description
#  Command-line utility for creating the prediction UDF "predictCropClass" inside 
#  the "fairicube" namespace. The prediction UDF takes a cutout of the 
#  sentinel2_2018_flevopolder_10m_7x4bands coverage and outputs the crop labels 
#  per pixel of that cutout.
#
# PRECONDITIONS
#  1) rasdaman must be running
#  2) fairicube.predictionTest UDF must not be present in rasdaman

RASQL="/opt/rasdaman/bin/rasql --user $RAS_USER --passwd $RAS_PASSWD"

# Create fairicube.predictionTest UDF
$RASQL -q 'create function fairicube.predictCropClass (array s2_cutout, array maxes_per_band)
returns array
language cpp
extern "fairicube.so"'
