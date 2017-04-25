# for Anaconda paths
PYTHON_VERSION = $(shell python --version 2>&1 |cut -d' ' -f 2|cut -c 1-3)
PYTHON_CONFIG = $(shell which python$(PYTHON_VERSION)-config)
PYTHON_DIR = $(shell dirname $(shell which python))/..
PYTHON_INCLUDE_DIR = $(PYTHON_DIR)/include/python$(PYTHON_VERSION)m
PYTHON_LINK_DIR = $(PYTHON_DIR)/lib
LDFLAGS = -L$(PYTHON_LINK_DIR) -lpython$(PYTHON_VERSION)m
PYBIND_INCLUDE_DIR = pybind11/include
NERV_DIR = dredviz-1.0.2
CXXFLAGS = -g -fPIC -O3 -Wall -std=c++14 -pedantic -I$(PYTHON_INCLUDE_DIR) -I$(NERV_DIR) -I$(PYBIND_INCLUDE_DIR) -I$(NERV_DIR)/newmat
nerv-objects = $(addprefix ${NERV_DIR}/, sompackexporter.o datamatrix.o exception.o calculatedistancematrix.o euclidean.o euclideansquared.o randomdatagenerator.o sompackloader.o dataset.o distancematrix.o commandlineargs.o dynamicdouble.o conjugategradientopt.o goldensectionsearch.o nervcostfunction.o nervoptstrat.o nervprobability.o inputprobentropy.o nerv.o distancematrixloader.o recorder.o)

.PHONY: clean newmat

pynerv : $(nerv-objects) newmat pynerv.cc
	g++ $(CXXFLAGS) $(LDFLAGS) -shared $(nerv-objects) ${NERV_DIR}/newmat/libnewmat.a `$(PYTHON_CONFIG) --cflags --ldflags` pynerv.cc -o pynerv.so

nerv : $(nerv-objects) newmat
	g++ $(CXXFLAGS) -o ${NERV_DIR}/nerv $(nerv-objects) ${NERV_DIR}/newmat/libnewmat.a 

newmat : 
	cd $(NERV_DIR)/newmat; make -f nm_gnu.mak libnewmat.a; 


conjugategradientopt.o : $(addprefix ${NERV_DIR}/, conjugategradientopt.hh optimizationstepbatch.hh optimizationstep.hh costfunction.hh conjugategradientopt.cc distancematrix.o datamatrix.o dataset.o recorder.o Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/conjugategradientopt.cc

goldensectionsearch.o : $(addprefix ${NERV_DIR}/, goldensectionsearch.hh linesearch.hh goldensectionsearch.cc datamatrix.o Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/goldensectionsearch.cc

nervcostfunction.o : $(addprefix ${NERV_DIR}/, nervcostfunction.hh costfunction.hh linesearch.hh nervcostfunction.cc distancematrix.hh dataset.hh dynamicdouble.hh euclideansquared.hh nervprobability.hh Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/nervcostfunction.cc

nervoptstrat.o : $(addprefix ${NERV_DIR}/, nervoptstrat.hh optimizationstrategy.hh optimizationstep.hh costfunction.hh nervoptstrat.cc dataset.o sompackexporter.o Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/nervoptstrat.cc

conjgradoptstrat.o : $(addprefix ${NERV_DIR}/, conjgradoptstrat.hh optimizationstrategy.hh optimizationstep.hh costfunction.hh conjgradoptstrat.cc dataset.o Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/conjgradoptstrat.cc

euclideansquared.o : $(addprefix ${NERV_DIR}/, euclideansquared.hh metric.hh euclideansquared.cc datamatrix.o Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/euclideansquared.cc

exception.o : $(addprefix ${NERV_DIR}/, exception.cc exception.hh Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/exception.cc

datamatrix.o : $(addprefix ${NERV_DIR}/, datamatrix.hh datamatrix.cc exception.hh Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/datamatrix.cc

dynamicdouble.o : $(addprefix ${NERV_DIR}/, dynamicdouble.hh dynamicdouble.cc Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/dynamicdouble.cc

inversedynamicdouble.o : $(addprefix ${NERV_DIR}/, dynamicdouble.hh inversedynamicdouble.hh inversedynamicdouble.cc Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/inversedynamicdouble.cc

dataset.o : $(addprefix ${NERV_DIR}/, dataset.cc dataset.hh datamatrix.hh Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/dataset.cc

calculatedistancematrix.o : $(addprefix ${NERV_DIR}/, calculatedistancematrix.cc calculatedistancematrix.hh datamatrix.o dataset.o Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/calculatedistancematrix.cc

distancematrix.o : $(addprefix ${NERV_DIR}/, distancematrix.cc distancematrix.hh calculatedistancematrix.hh datamatrix.o dataset.o euclidean.o Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/distancematrix.cc

euclidean.o : $(addprefix ${NERV_DIR}/, euclidean.cc euclidean.hh metric.hh dataset.o datamatrix.o Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/euclidean.cc

sompackloader.o : $(addprefix ${NERV_DIR}/, dataloader.hh sompackloader.cc sompackloader.hh dataset.o datamatrix.o Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/sompackloader.cc

sompackexporter.o : $(addprefix ${NERV_DIR}/, dataexporter.hh sompackexporter.cc sompackexporter.hh dataset.o exception.o datamatrix.o  Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/sompackexporter.cc

randomdatagenerator.o : $(addprefix ${NERV_DIR}/, dataloader.hh randomdatagenerator.cc randomdatagenerator.hh dataset.o datamatrix.o  Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/randomdatagenerator.cc

commandlineargs.o : $(addprefix ${NERV_DIR}/, commandlineargs.cc commandlineargs.hh exception.o Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/commandlineargs.cc


nerv.o : $(addprefix ${NERV_DIR}/, sompackloader.o dataset.o distancematrix.o datamatrix.o sompackexporter.o sompackloader.o randomdatagenerator.o euclidean.o commandlineargs.o nervcostfunction.o nervoptstrat.o conjugategradientopt.o nerv.cc pca.hh Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/nerv.cc

numericalgradientcomputer.o : $(addprefix ${NERV_DIR}/, numericalgradientcomputer.hh numericalgradientcomputer.cc costfunction.hh datamatrix.o Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/numericalgradientcomputer.cc

nervgradienttest.o :$(addprefix ${NERV_DIR}/,  nervgradienttest.cc nervcostfunction.hh numericalgradientcomputer.hh sompackloader.o dynamicdouble.o commandlineargs.o goldensectionsearch.o conjugategradientopt.o datamatrix.o Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/nervgradienttest.cc

gradtest.o : $(addprefix ${NERV_DIR}/, gradtest.hh gradtest.cc costfunction.hh datamatrix.o Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/gradtest.cc


simplecostfunction.o : $(addprefix ${NERV_DIR}/, simplecostfunction.hh simplecostfunction.cc costfunction.hh datamatrix.o Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/simplecostfunction.cc

testnumgrad.o : $(addprefix ${NERV_DIR}/, simplecostfunction.o numericalgradientcomputer.o sompackloader.o dataset.o commandlineargs.o testnumgrad.cc Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/testnumgrad.cc

conjgradtest.o : $(addprefix ${NERV_DIR}/, simplecostfunction.o sompackloader.o goldensectionsearch.o commandlineargs.o dataset.o conjugategradientopt.o conjgradtest.cc Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/conjgradtest.cc

nervprobability.o : $(addprefix ${NERV_DIR}/, nervprobability.hh nervprobability.cc calculatedistancematrix.o distancematrix.o goldensectionsearch.o Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/nervprobability.cc

inputprobentropy.o : $(addprefix ${NERV_DIR}/, inputprobentropy.hh inputprobentropy.cc costfunction.hh nervprobability.o Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/inputprobentropy.cc

distancematrixloader.o : $(addprefix ${NERV_DIR}/, distancematrix.o distancematrixloader.hh distancematrixloader.cc Makefile)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/distancematrixloader.cc

rankmatrix.o : $(addprefix ${NERV_DIR}/, rankmatrix.hh rankmatrix.cc distancematrix.o distcomp.hh)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/rankmatrix.cc

recorder.o : $(addprefix ${NERV_DIR}/, recorder.hh recorder.cc sompackexporter.o datamatrix.o)
	g++ -c $(CXXFLAGS) $(NERV_DIR)/recorder.cc

clean :
	-rm -f *.so $(NERV_DIR)/nerv $(NERV_DIR)/*.o $(NERV_DIR)/newmat/*.o $(NERV_DIR)/newmat/libnewmat.a
