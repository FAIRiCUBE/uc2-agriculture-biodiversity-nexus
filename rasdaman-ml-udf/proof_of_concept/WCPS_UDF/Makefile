PETASCOPE=/opt/rasdaman_debug/share/rasdaman/petascope

.PHONY: all clean

all: /media/ocampos/recovery/Repositories/l-sis/projects/FAIRiCUBE/Code/uc2-biodiversity-agriculture/rasdaman-ml-udf/proof_of_concept/WCPS_UDF/UDF/Quantile.java
	javac -cp "$(PETASCOPE)/rasdaman-geo-core.jar:$(PETASCOPE)/rasdaman-geo-api.jar" /UDF/Quantile.java

clean:
	rm -rvf ./UDF/*.class
