#!/bin/bash

creds="$HOME/.rasadmin_credentials"
[ -f "$creds" ] || { echo "Please create file $creds with rasdaman admin credentials in format <username>:<password>"; exit 1; }

user="$(awk -F: '{ print $1; }' "$HOME/.rasadmin_credentials")"
pw="$(awk -F: '{ print $2; }' "$HOME/.rasadmin_credentials")"

[ -n "$RMANHOME" ] || export RMANHOME=/opt/rasdaman
RASQL="$RMANHOME/bin/rasql --user $user --passwd $pw --quiet"

udf=fc.predict_crop_class
echo -n "creating UDF $udf ... "

if $RASQL -q "create function $udf(array s2_cutout, array maxes_per_band) returns array language python extern \"fc.py\""; then
  echo ok.
else
  echo failed.
  exit 1
fi
