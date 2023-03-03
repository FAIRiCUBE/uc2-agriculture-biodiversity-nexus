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
# ./3_delete_udf.sh
#
# Description
#  Command-line utility for deleting the prediction UDF fairicube.predictionTest
#
# PRECONDITIONS
#  1) rasdaman must be running
#  2) fairicube.predictionTest UDF must be present in rasdaman

RASQL="/opt/rasdaman/bin/rasql --out string --user rknapen --passwd c9Du7ZHawU47"

$RASQL -q "delete function fairicube.predictionTest" --out string
