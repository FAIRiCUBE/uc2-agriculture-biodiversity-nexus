#UDF includes
RMANHOME=/opt/rasdaman
RMANUDF=$(RMANHOME)/share/rasdaman/udf
LIBTORCH=/usr/local/lib/libtorch
INCDIR = -I. \
	 -I$(RMANHOME)/include \
	 -I$(LIBTORCH)/include \
	 -I$(LIBTORCH)/include/torch/csrc/api/include
OPT    = -fPIC -shared -std=c++14 -g3
CPP    = g++
SLIB   = $(RMANHOME)/share/rasdaman/udf
CFLAGS = -DELPP_NO_DEFAULT_LOG_FILE -DELPP_FEATURE_CRASH_LOG -DELPP_STACKTRACE -DELPP_FEATURE_CRASH_LOG \
         -DELPP_NO_CHECK_MACROS -DELPP_THREAD_SAFE \
         $(DBG) $(OPT) $(INCDIR) -Wall -Wl,-soname

LINK   = -lm -ldl -L$(RMANHOME)/lib \
			-lrasodmg \
			-lraslib \
			-lcommon_logging \
			-L$(LIBTORCH)/lib \
			-ltorch \
			-ltorch_global_deps \
			-lc10 \
			-ltorch_cpu

#Test UDF includes
tCOMPILER = g++
tLINKER   = g++
tFLAGS    =  -g3 -std=c++14 -fPIC -pedantic -Werror -Wextra -Wshadow  -Wcast-align -Wcast-qual -Wsign-promo -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op  -Wno-strict-aliasing -Wno-missing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wstrict-null-sentinel -Wstrict-overflow=5 -Wundef -Wno-disabled-optimization -Wno-unused -Wno-empty-body -Wno-unused-function -Wno-unused-result -Wno-long-long -Wno-format-nonliteral  -Wno-implicit-fallthrough -fno-stack-protector -fno-strict-overflow
tSLIB=$(RMANHOME)/share/rasdaman/udf

# /usr/lib/libdfalt.so;/usr/lib/libmfhdfalt.so expands to lib1;lib2, the ; needs to be replaced with a ' '
tHDF4LIBS = $(subst ;, ,/usr/lib/libdfalt.so;/usr/lib/libmfhdfalt.so)

tINCS = -I $(RMANHOME)/include

tLIBS = -L$(RMANHOME)/lib \
       -lrasodmg               \
       -lraslib                \
       -lclientcomm            \
       -lrasodmg               \
       -lraslib                \
       -lrasnet                \
       -lcommon_grpc           \
       -lcommon_exceptions     \
       -lcommon_network        \
       -lcommon_logging        \
       -lcommon_string         \
       -leasyloggingpp         \
       -lmymalloc_cln          \
       -lgrpc++_unsecure       \
       -lgrpc_unsecure         \
       -lz                     \
       -lprotobuf              \
       -lpthread               \
       -lboost_thread          \
       -lboost_system          \
       -lnetcdf                \
       -ltiff                  \
       -lgdal                  \
       -ledit                  \
       -ldl                    \
       -lcrypto                \
       -lcrypt                 \
       -ldw /usr/lib/x86_64-linux-gnu/libeccodes.so $(HDF4LIBS) \
       -L$(SLIB) -lfairicube

tDEFS = -DEARLY_TEMPLATE -DELPP_NO_DEFAULT_LOG_FILE -DELPP_FEATURE_CRASH_LOG -DELPP_STACKTRACE \
       -DELPP_FEATURE_CRASH_LOG -DELPP_NO_CHECK_MACROS -DELPP_THREAD_SAFE

#Build all
all: fairicube bin

fairicube:  fairicube.cc
	$(CPP) $(CFLAGS),$(SLIB)/$@.so -o $(SLIB)/$@.so $< $(LINK)
	cp $(SLIB)/$@.so $(SLIB)/lib$@.so


test_rastorch.o: test_rastorch.cc
	$(tCOMPILER) $(tFLAGS) $(tDEFS) $(tINCS) -c test_rastorch.cc


bin: test_rastorch.o
	$(tLINKER) $(tFLAGS) -o test_rastorch test_rastorch.o $(tLIBS)


clean:
	-rm -f *.o *.so test_rastorch $(RMANUDF)/libfairicube.so $(RMANUDF)/fairicube.so
