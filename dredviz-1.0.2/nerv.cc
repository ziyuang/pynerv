/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#include <iostream>
#include <string>
#include <fstream>

#include "sompackloader.hh"
#include "dataset.hh"
#include "distancematrix.hh"
#include "sompackexporter.hh"
#include "metric.hh"
#include "euclidean.hh"
#include "randomdatagenerator.hh"
#include "commandlineargs.hh"
#include "nervcostfunction.hh"
#include "nervoptstrat.hh"
#include "goldensectionsearch.hh"
#include "conjugategradientopt.hh"
#include "distancematrixloader.hh"
#include "pca.hh"

using namespace dredviz;

int
main (int argc, char **argv)
{
  DataSet projData;
  DataSet origData;
  ConjugateGradientOpt *optStep = NULL;

  DistanceMatrix origDataDist;

  std::vector<std::string> switches;
  switches.push_back("--inputfile");
  switches.push_back("--outputfile");
  switches.push_back("--inputdist");
  switches.push_back("--lambda");
  switches.push_back("--neighbors");
  switches.push_back("--init");
  switches.push_back("--outputdim");
  switches.push_back("--initrad");
  switches.push_back("--iterations");
  switches.push_back("--cg_steps");
  switches.push_back("--cg_steps_final");
  switches.push_back("--record");
  switches.push_back("--randinit");
  switches.push_back("--seed");
  switches.push_back("--weightfile");

  std::set<std::string> recognizedSwitches(switches.begin(), switches.end());

  std::string usage =
    "Synopsis: nerv [OPTION] [--inputfile | --inputdist] [INPUT_FILE] --outputfile [OUTPUT_FILE]"
    "\n"
    "Required parameters:"
    "\n"
    "--inputfile INPUT_FILE - The name of the file that contains the data points to be visualized in SOM_PAK format. If a distance matrix is provided via --inputdist (see below), this switch will be ignored. "
    "\n"
    "--outputfile OUTPUT_FILE - The name of the file to which the output will be written, also in SOM_PAK format."
    "\n\n"
    "Optional parameters:"
    "\n"
    "--inputdist INPUT_DIST - The name of the SOM_PAK file that contains a distance matrix of the data to be visualized."
    "\n"
    "--lambda L - Control the trustworthiness-continuity tradeoff. A value of zero emphasizes trustworthiness only; increasing L shifts the emphasis towards continuity. L must be a floating point number between 0 and 1, but in practice values higher than 0.5 are not recommended. The default value for L is 0.1."
    "\n"
    "--neighbors K - Set the number of neighbors that each point should have: each point will consider the K points closest to it its neighbors. K should be a positive integer. The default value for K is 20."
    "\n"
    "--randinit - Initialize the projection with a random layout instead of PCA. "
    "\n"
    "--weightfile WEIGHT_FILE - Weight each data point's contribution to the cost function with a weight found in WEIGHT_FILE. WEIGHT_FILE should contain as many lines as there are data points, each line containing the weight associated with the data point on the same line in INPUT_FILE. The weights should be (positive) floating numbers."
    "\n"
    "--init INIT_FILE - Initialize the projection with the data in INIT_FILE. The data should be in SOM_PAK format, and naturally it must contain the same number of points as the data in INPUT_FILE (see --inputfile)."
    "\n"
    "--seed SEED - Initialize random number generator using the unsigned integer SEED as the seed value."
    "\n"
    "--outputdim DIM - The desired dimension of the output data. DIM should be a positive integer. The default value for DIM is 2. Note that --outputdim is ignored if initialization data is passed via --init, because the dimension of the output data must have the same dimension as the initialization."
    "\n"
    "--initrad RAD - The initial value for the global radius parameter. The default is 1.5 * the maximum distance in the original data."
    "\n"
    "--iterations ITERATIONS - The number of iterations to perform. If OPTMETHOD='cca', the default is 200 * the number of data points in INPUT_FILE. If OPTMETHOD='nerv', the default is 10."
    "\n"
    "--cg_steps CG_STEPS -  The number of conjugate gradient steps to perform per iteration in NeRV's optimization scheme. The default value is 2."
    "\n"
    "--cg_steps_final CG_STEPS_FINAL -  The number of conjugate gradient steps to perform on the final iteration in NeRV's optimization scheme. The default value is 40."
    "\n"
    "--record FILENAME_STEM - Outputs the current projection to a separate file after each optimization step. The filenames will be of the form ${FILENAME_STEM}_${STEP_NUMBER}.dat."
    "\n";


  try
  {
    CommandLineArgs args (argc, argv, recognizedSwitches);
    std::string inputfile;
    std::string outputfile;
    std::string tmpoutputfile;
    double lambda;
    size_t lastNeighbor;

    if (argc == 1 || args.hasArg ("--help"))
    {
      std::cerr << usage;
    }

    if (args.hasArg("--lambda"))
    {
      lambda = args.getDouble ("--lambda");
    }
    else
    {
      lambda = 0.1;
    }

    std::cout << "lambda = " << lambda << std::endl;

    if (args.hasArg("--neighbors"))
    {
      lastNeighbor = args.getUInt ("--neighbors");
    }
    else
    {
      lastNeighbor = 20;
    }

    std::cout << "Effective number of neighbors = " << lastNeighbor
      << std::endl;

    args.getString ("--outputfile", outputfile);
    SOMPackExporter exporter (outputfile);

    if (args.hasArg ("--inputdist"))
    {
      args.getString ("--inputdist", inputfile);

      DistanceMatrixLoader dmloader;
      dmloader.loadMatrix(inputfile, origDataDist);

      std::cout << "Loaded input distances from " << inputfile << std::endl;
    }
    else
    {
      args.getString ("--inputfile", inputfile);

      SOMPackLoader loader (inputfile);
      loader.loadData (origData);

      std::cout << "Loaded input data from " << inputfile << std::endl;

      Euclidean metric;

      DistanceMatrix dm (origData, metric);
      origDataDist = dm;
    }

    std::vector<double> weights(origDataDist.getRows(), 1);

    if (args.hasArg("--weightfile"))
    {
      std::string weightfname;
      args.getString("--weightfile", weightfname);
      std::ifstream weightfile(weightfname.c_str());
      for(size_t i = 0; i < weights.size(); i++)
        weightfile >> weights[i];
    }

    size_t outputDimension = 2;       /* The default dimension */

    if (args.hasArg ("--outputdim"))
    {
      outputDimension = args.getUInt ("--outputdim");
    }

    if (args.hasArg ("--init"))
    {
      std::string initfile;
      args.getString ("--init", initfile);

      SOMPackLoader initloader (initfile);
      initloader.loadData (projData);

      std::cout << "Initialized projection with data from " << initfile
        << std::endl;
    }
    //If the input data was given in the form of a distance matrix, we
    //don't have a data matrix to perform PCA on, so we'll just use a random
    //initialization.
    else if (args.hasArg("--randinit") || args.hasArg("--inputdist"))
    {
      if (args.hasArg ("--seed"))
      {
        unsigned int seed = args.getUInt("--seed");
        RandomDataGenerator randgen (origDataDist.getRows(), 
            outputDimension, 1, seed);
        randgen.loadData (projData);
      }
      else
      { 
        RandomDataGenerator randgen (origDataDist.getRows(), 
            outputDimension, 1);
        randgen.loadData (projData);
      }
    }
    //Initialize with PCA.
    else
    {
      PCA<DataSet> pca(origData);
      projData = DataSet(origData.getRows(), outputDimension);
      pca.getComponents(projData);
    }



    origDataDist.scale (1.0 / origDataDist.getAverage ());

    double initrad = 0.0;
    if (args.hasArg ("--initrad"))
    {
      initrad = args.getDouble ("--initrad");
    }
    else
    {
      initrad = origDataDist.getMax () / 2.0;
      std::cerr << "Initial radius = " << initrad << std::endl;
      std::cerr << "Diameter of input data set = " << origDataDist.getMax () << std::endl;
    }

    size_t iterations, stepsPerRound, stepsOnLastRound;

    if (args.hasArg ("--iterations"))
    {
      iterations = args.getUInt ("--iterations");
    }
    else
    {
      iterations = 10;
    }

    std::cout << "Number of iterations = " << iterations << std::endl;

    if (args.hasArg ("--cg_steps"))
    {
      stepsPerRound = args.getUInt ("--cg_steps");
    }
    else
    {
      stepsPerRound = 2;
    }

    std::cout << "Conjugate gradient steps per iteration = "
      << stepsPerRound << std::endl;


    if (args.hasArg ("--cg_steps_final"))
    {
      stepsOnLastRound = args.getUInt ("--cg_steps_final");
    }
    else
    {
      stepsOnLastRound = 40;
    }

    std::cout << "Conjugate gradient steps on final iteration = "
      << stepsOnLastRound << std::endl;

    
      

    DynamicDouble initialRadius (initrad, 0);

    GoldenSectionSearch linesearch;

    NeRVCostFunction costFunc (origDataDist, projData, linesearch,
                               initialRadius, lambda, lastNeighbor,
                               weights, std::cout);

    if (args.hasArg("--record"))
    {
      std::string filename_stem;
      args.getString("--record", filename_stem);

      optStep = new ConjugateGradientOpt(costFunc, linesearch, std::cerr,
                                     filename_stem);
    }
    else
    {
      optStep = new ConjugateGradientOpt(costFunc, linesearch, std::cerr);
    }

    NeRVOptStrat optStrategy (iterations, stepsPerRound, stepsOnLastRound,
        tmpoutputfile);

    optStrategy.optimize (origDataDist, projData, *optStep, costFunc,
                          std::cerr);

    exporter.exportData (projData);

  }

  catch (Exception & exc)
  {
    std::cerr << "Error: " << exc.getErrMsg () << std::endl;
  }

  delete optStep;
  return 0;
}
