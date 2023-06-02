javac -cp "/opt/rasdaman/share/rasdaman/petascope/rasdaman-geo-api.jar:/opt/rasdaman/share/rasdaman/petascope/rasdaman-geo-core.jar" fairicube/PredictCropClass.java

jar cvf fairicube.jar fairicube

cp -rp fairicube.jar /opt/rasdaman/share/rasdaman/petascope/udf
