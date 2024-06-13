# A rasdaman UDF for crop class prediction

## Deploy

To deploy the UDF run the following:
```
# copies fc.py to /opt/rasdaman/share/rasdaman/udf/, requires user to have sudo privileges
make
# register the rasql UDF in rasdaman
./1_create_udf.sh
```
