RASQL="/opt/rasdaman/bin/rasql --user $RAS_USER --passwd $RAS_PASSWD"

$RASQL -q "select view function list" --out string

