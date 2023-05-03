# FAIRICUBE Pytorch UDF Proof of Concept (PoC)

This proof of concept demonstrates how to execute a semantic segmentation machine learning ML prediction in
[rasdaman](https://doc.rasdaman.org/) using rasql, user-defined functions UDFs, and the ML library 
[libtorch](https://pytorch.org/cppdocs/).

## Description

The PoC consists of predicting the per pixel crop label for a preprocessed 28-band sentinel 2 image of a study area of
the Netherlands.

![alt text](http://url/to/img.png)


## Data 

stacked value of 7 Sentinel 2 images 
(R, G, B, NIR)
28-band sentinel 2 image provided by 

## Data Retrieval Example
In the rasql query below we extract a 250x250 pixel cutout of the sentinel2_2018_flevopolder_10m_7x4bands coverage

```
rasql -q 'select encode(c[2000:2250,1000:1250], "tif") from sentinel2_2018_flevopolder_10m_7x4bands as c' --out file --user $RAS_USER --passwd $RAS_PASS
```

## Getting Started

### Dependencies

* rasdaman enterprise (latest version)
* libtorch library: Download from https://download.pytorch.org/libtorch/cpu/libtorch-cxx11-abi-shared
-with-deps-1.13.0%2Bcpu.zip

* libtorch needs to be installed in the target machine and its path added to LD_LIBRARY_PATH environment variable

### Installing

1. Download and unzip https://download.pytorch.org/libtorch/cpu/libtorch-cxx11-abi-shared-with-deps-1
.13.0%2Bcpu.zip

```
curl https://download.pytorch.org/libtorch/cpu/libtorch-cxx11-abi-shared-with-deps-1.13.0%2Bcpu.zip -
-output libtorch.zip
unzip libtorch.zip
```
2. Copy the contents of **libtorch** directory to **/usr/local/lib**. Open all permissions to the con
tents of **/usr/local/lib**

```
cp -r libtorch /usr/local/lib
chmod -R 777 /usr/local/lib/libtorch
```

3. Copy the contents of **/usr/local/lib/libtorch/lib** into **/usr/local/lib/**

```
sudo cp -a . /usr/local/lib/
```

### Executing program

To ease debugging, if necessary, please follow the step in order. Always compile first and then regis
ter the UDF in rasdaman.

To compile the UDF do the following:

- Create/modify a function in rastorch.cc; rastorch.cc is the main entry point for the UDF code. If y
ou require to use code from other files please either reference them like a shared object or add it t
o the file to the compilation of the UDF; the Makefile will have to be updated as well, 
- Clean old compilation files

```
make clean
```

- Compile rastorch


```
make
```

To register the UDF in rasdaman, please follow these steps in order:

- Check that the required collection are ingested in rasdaman, if not then they need to be ingested first

```
./0_check_collections.sh
```

- Check that there are no UDFs are already registered

```
./check_udfs.sh
```

If there are UDFs then delete them from rasdaman. NOTE: Any new UDF that is registered must be added to the scritp for deletion

```
./3_delete_udf.sh
```

- Test the UDF. NOTE: Any test query to rasdaman should be done added to the script.

```
./2_test_udf.sh
```

- (Optional) If there is further code that would like to be added to UDF, then it is better to delete it before adding the extra code.

```
.3_delete_udf.sh
```

## Help

For more information on user-defined functions in rasdaman, please refer to rasdaman enterprise manual [User-Defined Functions - UDFs](https://doc.rasdaman.com/testing/04_ql-guide.html#user-defined-functions-udfs-re) section.

## Authors

ex. Otoniel Campos
ex. [@email](ocamposescobar@constructor.university)

## Version History

* 0.1
    * Initial Release

## License

NA

## Acknowledgments

Inspiration, code snippets, etc.
* [awesome-readme](https://github.com/matiassingers/awesome-readme)
* [PurpleBooth](https://gist.github.com/PurpleBooth/109311bb0361f32d87a2)
* [dbader](https://github.com/dbader/readme-template)
* [zenorocha](https://gist.github.com/zenorocha/4526327)
* [fvcproductions](https://gist.github.com/fvcproductions/1bfc2d4aecb01a834b46)
