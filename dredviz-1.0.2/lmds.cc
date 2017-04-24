/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#include <iostream>
#include <string>
#include <cmath>

#include "sompackloader.hh"
#include "dataset.hh"
#include "distancematrix.hh"
#include "sompackexporter.hh"
#include "metric.hh"
#include "euclidean.hh"
#include "randomdatagenerator.hh"
#include "ccaoptimization.hh"
#include "lmdsoptstrat.hh"
#include "lmdscostfunction.hh"
#include "commandlineargs.hh"
#include "distancematrixloader.hh"

#include "goldensectionsearch.hh"
#include "conjugategradientopt.hh"
#include "nervoptstrat.hh"

using namespace dredviz;

int
main (int argc, char **argv)
{
  DataSet projData;
  DataSet origData;

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
  switches.push_back("--alpha");

  std::set<std::string> recognizedSwitches(switches.begin(), switches.end());

  std::string usage = "\n"
                      "Synopsis: lmds [OPTION] [--inputfile | --inputdist] [INPUT_FILE] --outputfile [OUTPUT_FILE]"
                      "\n"
                      "Required parameters:"
                      "\n"
                      "--inputfile INPUT_FILE - The name of the file that contains the data points to be visualized in SOM_PAK format. If a distance matrix is provided via --inputdist (see below), this switch will be ignored. "
                      "\n"
                      "--outputfile OUTPUT_FILE - The name of the file to which the output will be written, also in SOM_PAK format."
                      "\n\n"
                      "Optional parameters:"
                      "\n\n"
                      "--inputdist INPUT_DIST - The name of the SOM_PAK file that contains a distance matrix of the data to be visualized."
                      "\n"
                      "--lambda L - Control the trustworthiness-continuity tradeoff. A value of zero emphasizes trustworthiness only; increasing L shifts the emphasis towards continuity. L must be a floating point number between 0 and 1, but in practice values higher than 0.5 are not recommended. The default value for L is 0."
                      "\n"
                      "--neighbors K - Set the number of neighbors that each point should have: each point will consider the K points closest to it its neighbors. K should be a positive integer. The default value for K is 20."
                      "\n"
                      "--init INIT_FILE - Initialize the visualization with the data in INIT_FILE instead of using a random initialization as per default. The data in INIT_FILE should be in SOM_PAK format, and naturally it must contain the same number of points as the data in INPUT_FILE (see --inputfile)."
                      "\n"
                      "--outputdim DIM - The desired dimension of the output data. DIM should be a positive integer. The default value for DIM is 2. Note that --outputdim is ignored if initialization data is passed via --init, because the dimension of the output data must have the same dimension as the initialization."
                      "\n"
                      "--initrad RAD - The initial value for the global radius parameter. The default is 1.5 * the maximum distance in the original data."
                      "\n"
                      "--alpha A - The initial value of the learning parameter. A should be a floating point number between 0 and 1. The default value is 0.25."
                      "\n"
                      "--iterations - The number of iterations to perform. The default is 400 * the number of data points in INPUT_FILE."
                      "\n";



  try
  {
    CommandLineArgs args (argc, argv, recognizedSwitches);
    std::string inputfile;
    std::string outputfile;
    double alpha;
    double lambda;
    size_t iterationsToPerform;
    size_t lastNeighbor;

    if (argc == 1 || args.hasArg ("--help"))
    {
      std::cerr << usage;
    }





    if (args.hasArg ("--alpha"))
    {
      alpha = args.getDouble ("--alpha");
    }
    else
    {
      alpha = 0.25;
    }

    if (args.hasArg ("--lambda"))
    {
      lambda = args.getDouble ("--lambda");
    }
    else
    {
      lambda = 0.0;
    }


    if (args.hasArg ("--k"))
    {
      lastNeighbor = args.getUInt ("--k");
    }
    else
    {
      lastNeighbor = 20;
    }

    if (args.hasArg ("--inputdist"))
    {
      args.getString ("--inputdist", inputfile);

      DistanceMatrixLoader dmloader;
      dmloader.loadMatrix(inputfile, origDataDist);
    }
    else
    {
      args.getString ("--inputfile", inputfile);

      SOMPackLoader loader (inputfile);
      loader.loadData (origData);

      Euclidean metric;

      DistanceMatrix dm (origData, metric);
      origDataDist = dm;
    }


    if (args.hasArg ("--iterations"))
    {
      iterationsToPerform = args.getUInt ("--iterations");
    }
    else
    {
      iterationsToPerform = 400 * origData.getRows ();
    }


    args.getString ("--outputfile", outputfile);

    SOMPackExporter exporter (outputfile);


    origDataDist.scale (1.0 / origDataDist.getAverage ());

    double initRad = 0.0;

    if (args.hasArg ("--initrad"))
    {
      initRad = args.getDouble ("--initrad");
    }
    else
    {
      initRad = origDataDist.getMax () * 1.5;
    }

    DynamicDouble initialRadius (initRad, 0);
    //DynamicDouble initialRadius (0, 0);


    if (args.hasArg ("--init"))
    {
      std::string initfile;
      args.getString ("--init", initfile);

      SOMPackLoader initloader (initfile);
      initloader.loadData (projData);
    }
    else                        //Use randomly generated data
    {
      size_t outputDimension = 2;       /* The default dimension */
      double range = 1.0;

      if (args.hasArg ("--outputdim"))
      {
        outputDimension = args.getUInt ("--outputdim");
      }

      if (args.hasArg ("--seed"))
      {
        unsigned int seed = args.getUInt("--seed");
        RandomDataGenerator randgen (origDataDist.getRows(), 
            outputDimension, range, seed);
        randgen.loadData (projData);
      }
      else
      { 
        RandomDataGenerator randgen (origDataDist.getRows(), 
            outputDimension, range);
        randgen.loadData (projData);
      }

    }

      LMDSCostFunction costFunc(origDataDist, origData, initialRadius,
                                lambda, lastNeighbor);
      CCAOptimization optStep (alpha, costFunc);
      LMDSOptStrat optStrategy (iterationsToPerform);

      optStrategy.optimize (origDataDist, projData, optStep, costFunc,
                            std::cerr);
    exporter.exportData (projData);
  }
  catch (Exception & exc)
  {
    std::cerr << "Error: " << exc.getErrMsg () << std::endl;
  }

  return 0;
}
