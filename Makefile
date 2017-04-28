# for Anaconda paths
PYTHON_VERSION = $(shell python --version 2>&1 |cut -d' ' -f 2|cut -c 1-3)
PYTHON_CONFIG = $(shell which python$(PYTHON_VERSION)-config)
PYTHON_DIR = $(shell dirname $(shell which python))/..
PYTHON_INCLUDE_DIR = $(PYTHON_DIR)/include/python$(PYTHON_VERSION)m
PYTHON_LIB_DIR = $(PYTHON_DIR)/lib
PYBIND_INCLUDE_DIR = pybind11/include
NERV_DIR = dredviz-1.0.2

CXXFLAGS = -fPIC -O3 -Wall -std=c++14 -pedantic -I$(PYTHON_INCLUDE_DIR) -I$(NERV_DIR) -I$(PYBIND_INCLUDE_DIR) -I$(NERV_DIR)/newmat
LDFLAGS = -L$(PYTHON_LIB_DIR) -lpython$(PYTHON_VERSION)m

pynerv-objects = $(addprefix ${NERV_DIR}/, datamatrix.o exception.o calculatedistancematrix.o euclidean.o euclideansquared.o randomdatagenerator.o recorder.o dataset.o distancematrix.o dynamicdouble.o conjugategradientopt.o goldensectionsearch.o nervcostfunction.o nervoptstrat.o nervprobability.o inputprobentropy.o sompackexporter.o)

.PHONY: clean nerv newmat

pynerv : $(pynerv-objects) newmat nerv pynerv.cc
	g++ $(CXXFLAGS) $(LDFLAGS) -shared $(pynerv-objects) ${NERV_DIR}/newmat/libnewmat.a `$(PYTHON_CONFIG) --cflags --ldflags` pynerv.cc -o pynerv.so

nerv : 
	cd $(NERV_DIR); make -f Makefile nerv

newmat : 
	cd $(NERV_DIR)/newmat; make -f nm_gnu.mak libnewmat.a; 

clean :
	-rm -f *.so $(NERV_DIR)/nerv $(NERV_DIR)/*.o $(NERV_DIR)/newmat/*.o $(NERV_DIR)/newmat/libnewmat.a
