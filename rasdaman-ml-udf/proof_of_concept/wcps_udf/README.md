# rasdaman WCPS UDF for crop class prediction

- `fairicube` directory contains the version that calls the rasql C++ UDF
- `fc` directory contains the fc.predictCropClass UDF that calls the rasql Python UDF

## Deploy

```
cp -r fc /opt/rasdaman/share/rasdaman/petascope/udf/
cd /opt/rasdaman/share/rasdaman/petascope/udf/
sudo -u rasdaman bash build.sh
```