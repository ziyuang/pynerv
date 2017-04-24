lmds-objects = sompackexporter.o datamatrix.o exception.o calculatedistancematrix.o euclidean.o randomdatagenerator.o sompackloader.o dataset.o distancematrix.o commandlineargs.o dynamicdouble.o lmdscostfunction.o lmdsoptstrat.o ccaoptimization.o lmds.o distancematrixloader.o nervprobability.o goldensectionsearch.o inputprobentropy.o conjugategradientopt.o nervoptstrat.o recorder.o

nerv-objects = sompackexporter.o datamatrix.o exception.o calculatedistancematrix.o euclidean.o euclideansquared.o randomdatagenerator.o sompackloader.o dataset.o distancematrix.o commandlineargs.o dynamicdouble.o conjugategradientopt.o goldensectionsearch.o nervcostfunction.o nervoptstrat.o nervprobability.o inputprobentropy.o nerv.o distancematrixloader.o recorder.o

measure-objects = conttrust.o datamatrix.o exception.o calculatedistancematrix.o euclidean.o sompackloader.o sompackexporter.o commandlineargs.o dataset.o distancematrix.o measuremain.o distancematrixloader.o

klmeasure-objects = datamatrix.o distancematrix.o calculatedistancematrix.o exception.o euclidean.o sompackloader.o commandlineargs.o distancematrixloader.o dataset.o nervprobability.o inputprobentropy.o euclideansquared.o goldensectionsearch.o

klrank-objects = datamatrix.o distancematrix.o calculatedistancematrix.o exception.o euclidean.o sompackloader.o commandlineargs.o distancematrixloader.o dataset.o nervprobability.o inputprobentropy.o euclideansquared.o goldensectionsearch.o rankmatrix.o

pca-objects = sompackexporter.o datamatrix.o exception.o sompackloader.o dataset.o commandlineargs.o pcamain.o


#CXXFLAGS = -O3 -g -Wall -std=c++98 -pedantic

#CXXFLAGS = -g -O3 -Wall -std=c++98 -pedantic -march=pentium-m -msse3 -pipe -fomit-frame-pointer
CXXFLAGS = -g -O3 -Wall -std=c++98 -pedantic -I ./newmat #-static -Lnewmat -lnewmat 

.PHONY: all clean newmat


all: lmds measure nerv klmeasure newmat pca klrank

lmds : $(lmds-objects) 
	g++ $(CXXFLAGS) -o lmds $(lmds-objects)

nerv : $(nerv-objects) newmat
	g++ $(CXXFLAGS) -o nerv $(nerv-objects) newmat/libnewmat.a 

newmat : 
	cd newmat; make -f nm_gnu.mak libnewmat.a; 

pca : $(pca-objects) newmat
	g++ $(CXXFLAGS) -o pca $(pca-objects) newmat/libnewmat.a

measure : $(measure-objects)
	g++ $(CXXFLAGS) -o measure $(measure-objects)

klmeasure : klmeasure.o $(klmeasure-objects)
	g++ $(CXXFLAGS) -o klmeasure klmeasure.o $(klmeasure-objects)

klrank : klrank.o $(klrank-objects)
	g++ $(CXXFLAGS) -o klrank klrank.o $(klrank-objects)


pcamain.o : pca.hh pcamain.cc sompackexporter.o datamatrix.o exception.o sompackloader.o dataset.o commandlineargs.o 
	g++ -c $(CXXFLAGS) pcamain.cc

conjugategradientopt.o : conjugategradientopt.hh optimizationstepbatch.hh optimizationstep.hh costfunction.hh conjugategradientopt.cc distancematrix.o datamatrix.o dataset.o recorder.o Makefile
	g++ -c $(CXXFLAGS) conjugategradientopt.cc

goldensectionsearch.o : goldensectionsearch.hh linesearch.hh goldensectionsearch.cc datamatrix.o Makefile
	g++ -c $(CXXFLAGS) goldensectionsearch.cc

nervcostfunction.o : nervcostfunction.hh costfunction.hh linesearch.hh nervcostfunction.cc distancematrix.hh dataset.hh dynamicdouble.hh euclideansquared.hh nervprobability.hh Makefile
	g++ -c $(CXXFLAGS) nervcostfunction.cc

nervoptstrat.o : nervoptstrat.hh optimizationstrategy.hh optimizationstep.hh costfunction.hh nervoptstrat.cc dataset.o sompackexporter.o Makefile
	g++ -c $(CXXFLAGS) nervoptstrat.cc

conjgradoptstrat.o : conjgradoptstrat.hh optimizationstrategy.hh optimizationstep.hh costfunction.hh conjgradoptstrat.cc dataset.o Makefile
	g++ -c $(CXXFLAGS) conjgradoptstrat.cc

euclideansquared.o : euclideansquared.hh metric.hh euclideansquared.cc datamatrix.o Makefile
	g++ -c $(CXXFLAGS) euclideansquared.cc

exception.o : exception.cc exception.hh Makefile
	g++ -c $(CXXFLAGS) exception.cc

datamatrix.o : datamatrix.hh datamatrix.cc exception.hh Makefile
	g++ -c $(CXXFLAGS) datamatrix.cc

dynamicdouble.o : dynamicdouble.hh dynamicdouble.cc Makefile
	g++ -c $(CXXFLAGS) dynamicdouble.cc

inversedynamicdouble.o : dynamicdouble.hh inversedynamicdouble.hh inversedynamicdouble.cc Makefile
	g++ -c $(CXXFLAGS) inversedynamicdouble.cc

dataset.o : dataset.cc dataset.hh datamatrix.hh Makefile
	g++ -c $(CXXFLAGS) dataset.cc

calculatedistancematrix.o : calculatedistancematrix.cc calculatedistancematrix.hh datamatrix.o dataset.o Makefile
	g++ -c $(CXXFLAGS) calculatedistancematrix.cc

distancematrix.o : distancematrix.cc distancematrix.hh calculatedistancematrix.hh datamatrix.o dataset.o euclidean.o Makefile
	g++ -c $(CXXFLAGS) distancematrix.cc

euclidean.o : euclidean.cc euclidean.hh metric.hh dataset.o datamatrix.o Makefile
	g++ -c $(CXXFLAGS) euclidean.cc

sompackloader.o : dataloader.hh sompackloader.cc sompackloader.hh dataset.o datamatrix.o Makefile
	g++ -c $(CXXFLAGS) sompackloader.cc

sompackexporter.o : dataexporter.hh sompackexporter.cc sompackexporter.hh dataset.o exception.o datamatrix.o  Makefile
	g++ -c $(CXXFLAGS) sompackexporter.cc

randomdatagenerator.o : dataloader.hh randomdatagenerator.cc randomdatagenerator.hh dataset.o datamatrix.o  Makefile
	g++ -c $(CXXFLAGS) randomdatagenerator.cc

commandlineargs.o : commandlineargs.cc commandlineargs.hh exception.o Makefile
	g++ -c $(CXXFLAGS) commandlineargs.cc

lmds.o : sompackloader.hh dataset.o distancematrix.o sompackexporter.o sompackloader.o randomdatagenerator.o euclidean.o commandlineargs.o lmdscostfunction.o ccaoptimization.o lmdsoptstrat.o lmds.cc  Makefile
	g++ -c $(CXXFLAGS) lmds.cc

nerv.o : sompackloader.o dataset.o distancematrix.o datamatrix.o sompackexporter.o sompackloader.o randomdatagenerator.o euclidean.o commandlineargs.o nervcostfunction.o nervoptstrat.o conjugategradientopt.o nerv.cc pca.hh Makefile
	g++ -c $(CXXFLAGS) nerv.cc

conttrust.o : conttrust.cc conttrust.hh datamatrix.o distancematrix.o dataset.o exception.o  distcomp.hh Makefile
	g++ -c $(CXXFLAGS) conttrust.cc


measuremain.o : measuremain.cc sompackloader.hh dataset.o distancematrix.o euclidean.o randomdatagenerator.o conttrust.o datamatrix.o commandlineargs.o Makefile
	g++ -c $(CXXFLAGS) measuremain.cc

ccaoptimization.o : optimizationstep.hh ccaoptimization.cc ccaoptimization.hh distancematrix.o dataset.o euclidean.o Makefile
	g++ -c $(CXXFLAGS) ccaoptimization.cc

lmdscostfunction.o : lmdscostfunction.cc lmdscostfunction.hh separablecostfunction.hh costfunction.hh dynamicdouble.o distancematrix.o dataset.o Makefile
	g++ -c $(CXXFLAGS) lmdscostfunction.cc

lmdsoptstrat.o : optimizationstrategy.hh costfunction.hh optimizationstep.hh lmdsoptstrat.cc lmdsoptstrat.hh dataset.o distancematrix.o Makefile
	g++ -c $(CXXFLAGS) lmdsoptstrat.cc

numericalgradientcomputer.o : numericalgradientcomputer.hh numericalgradientcomputer.cc costfunction.hh datamatrix.o Makefile
	g++ -c $(CXXFLAGS) numericalgradientcomputer.cc

nervgradienttest.o : nervgradienttest.cc nervcostfunction.hh numericalgradientcomputer.hh sompackloader.o dynamicdouble.o commandlineargs.o goldensectionsearch.o conjugategradientopt.o datamatrix.o Makefile
	g++ -c $(CXXFLAGS) nervgradienttest.cc

gradtest.o : gradtest.hh gradtest.cc costfunction.hh datamatrix.o Makefile
	g++ -c $(CXXFLAGS) gradtest.cc


simplecostfunction.o : simplecostfunction.hh simplecostfunction.cc costfunction.hh datamatrix.o Makefile
	g++ -c $(CXXFLAGS) simplecostfunction.cc

testnumgrad.o : simplecostfunction.o numericalgradientcomputer.o sompackloader.o dataset.o commandlineargs.o testnumgrad.cc Makefile
	g++ -c $(CXXFLAGS) testnumgrad.cc

conjgradtest.o : simplecostfunction.o sompackloader.o goldensectionsearch.o commandlineargs.o dataset.o conjugategradientopt.o conjgradtest.cc Makefile
	g++ -c $(CXXFLAGS) conjgradtest.cc

nervprobability.o : nervprobability.hh nervprobability.cc calculatedistancematrix.o distancematrix.o goldensectionsearch.o Makefile
	g++ -c $(CXXFLAGS) nervprobability.cc

inputprobentropy.o : inputprobentropy.hh inputprobentropy.cc costfunction.hh nervprobability.o Makefile
	g++ -c $(CXXFLAGS) inputprobentropy.cc

distancematrixloader.o : distancematrix.o distancematrixloader.hh distancematrixloader.cc Makefile
	g++ -c $(CXXFLAGS) distancematrixloader.cc

klmeasure.o : $(klmeasure-objects) klmeasure.cc
	g++ -c $(CXXFLAGS) klmeasure.cc

klrank.o : $(klrank-objects) klrank.cc
	g++ -c $(CXXFLAGS) klrank.cc

rankmatrix.o : rankmatrix.hh rankmatrix.cc distancematrix.o distcomp.hh
	g++ -c $(CXXFLAGS) rankmatrix.cc

recorder.o : recorder.hh recorder.cc sompackexporter.o datamatrix.o
	g++ -c $(CXXFLAGS) recorder.cc

clean :
	-rm lmds nerv measure klmeasure *.o newmat/*.o newmat/libnewmat.a
