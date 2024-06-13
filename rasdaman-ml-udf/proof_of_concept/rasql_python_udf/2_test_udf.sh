#!/bin/bash

creds="$HOME/.rasadmin_credentials"
[ -f "$creds" ] || { echo "Please create file $creds with rasdaman admin credentials in format <username>:<password>"; exit 1; }

user="$(awk -F: '{ print $1; }' "$HOME/.rasadmin_credentials")"
pw="$(awk -F: '{ print $2; }' "$HOME/.rasadmin_credentials")"

[ -n "$RMANHOME" ] || export RMANHOME=/opt/rasdaman
RASQL="$RMANHOME/bin/rasql --user $user --passwd $pw"

[ "$1" = gdb ] && gdbcmd="gdb --args"
[ "$1" = val ] && gdbcmd="valgrind --tool=memcheck"

# RASQL="sudo -u rasdaman $gdbcmd /tmp/rasserver --user $user --passwd $pw"

echo
echo "running python UDF"
udf=fc.predict_crop_class
coll=sentinel2_2018_flevopolder_10m_7x4bands
maxes=maxes_sentinel2_2018_flevopolder_10m_7x4bands

$RASQL -q "select encode ($udf(s2_cutout[250:258,200:208], maxes_per_band), \"json\") from $coll as s2_cutout, $maxes as maxes_per_band" \
       --out string

echo
echo "running C++ UDF"
udf=fairicube.predictCropClass
coll=sentinel2_2018_flevopolder_10m_7x4bands
maxes=maxes_sentinel2_2018_flevopolder_10m_7x4bands

$RASQL -q "select encode ($udf(s2_cutout[250:258,200:208], maxes_per_band), \"json\") from $coll as s2_cutout, $maxes as maxes_per_band" \
       --out string

# Expected result
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