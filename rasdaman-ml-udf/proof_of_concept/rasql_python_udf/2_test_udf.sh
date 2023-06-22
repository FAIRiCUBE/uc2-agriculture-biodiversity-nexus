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
# ./2_test_udf.sh
#
# Description
#  Command-line utility for testing the prediction UDF "fairicube.predictCropClass".
#  It tests three examples. Each example takes a cutout of the coverage 
#  sentinel2_2018_flevopolder_10m_7x4bands and outputs a JSON file with
#  pixel crop classification prediction. The cutouts are as follows:
#  1. 9x9 cutout example
#  2. 250x250 cutout example
#  3. 250x250 cutout example
#  The prediction UDF takes a cutout of the sentinel2_2018_flevopolder_10m_7x4bands 
#  coverage and outputs the crop labels per pixel of that cutout.
#
# PRECONDITIONS
#  1) rasdaman must be running
#  2) fairicube.predictCropClass UDF must be present in rasdaman

RASQL="$RASDAMAN/rasql --user $RAS_USER --passwd $RAS_PASSWD"

############################
# 9x9 cutout example to json
############################
############################
# 9x9 cutout example to json
############################
$RASQL -q 'select encode (fairicube_python.predictCropClass(s2_cutout[250:258,200:208], maxes), "json") from sentinel2_2018_flevopolder_10m_7x4bands as s2_cutout, maxes_sentinel2_2018_flevopolder_10m_7x4bands as maxes' --out file --outfile prediction_9x9
# Expected result (9x9_prediction.json) =
# [[14,14,14,14,14,14,14,14,14],
#  [14,14,14,14,14,14,14,14,14],
#  [14,14,14,14,14,14,14,14,14],
#  [14,14,14,14,14,14,14,14,14],
#  [14,14,14,14,14,14,14,14,14],
#  [14,14,14,14,14,14,14,14,23],
#  [14,14,14,14,14,9,14,23,23],
#  [14,14,14,14,14,3,23,23,23],
#  [14,14,14,14,23,3,3,23,23]
#  ]

