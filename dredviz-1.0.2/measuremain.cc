/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#include "measure.hh"
#include "sompackloader.hh"
#include "sompackexporter.hh"
#include "dataset.hh"
#include "distancematrix.hh"
#include "metric.hh"
#include "euclidean.hh"
#include "randomdatagenerator.hh"
#include "commandlineargs.hh"
#include "conttrust.hh"
#include "distancematrixloader.hh"

#include <iostream>
#include <string>

using namespace dredviz;

int
main (int argc, char **argv)
{
  DataSet origData;
  DataSet projData;

  DistanceMatrix origDataDist;
  DistanceMatrix projDataDist;

  std::vector<std::string> switches;
  switches.push_back("--projfile");
  switches.push_back("--datafile");
  switches.push_back("--datadist");
  switches.push_back("--projdist");
  switches.push_back("--neighbors");

  std::set<std::string> recognizedSwitches(switches.begin(), switches.end());

  std::string usage = "\n"
                      "Synopsis: measure [--datafile | --datadist] [INPUT_FILE] [--projfile | --projdist] [OUTPUT_FILE] --neighbors [NEIGHBORHOOD_SIZE]\n"
                      "\n"
                      "Compares the projection in PROJ_FILE with the original data in INPUT_FILE and calculates the trustworthiness and continuity of the projection for neighborhood sizes ranging from 1 to NEIGHBORHOOD_SIZE. The values are printed to stdout, with each row containing the values for one neighborhood size.""\n"
                      "\n""\n"
                      "Required parameters:"
                      "\n"
                      "--datafile DATA_FILE - The name of the file that contains the data points to be visualized in SOM_PAK format. If a distance matrix is provided via --datadist (see below), this switch will be ignored. "
                      "\n"
                      "--datadist INPUT_DIST - The name of the SOM_PAK file that contains a distance matrix of the original data set."
                      "\n"
                      "--projfile PROJ_FILE - The name of the SOM_PAK file that contains the projected data points. If a distance matrix is provided via --projdist, this switch will be ignored."
                      "\n"
                      "--projdist PROJ_DIST - The name of the SOM_PAK file that contains a distance matrix of the projected data set."
                      "\n";

  try
  {
    CommandLineArgs args (argc, argv, recognizedSwitches);
    std::string origDataFile;
    std::string projDataFile;

    size_t lastNeighbor;

    if (argc == 1 || args.hasArg ("--help"))
    {
      std::cerr << usage;
    }

    if (args.hasArg ("--datadist"))
    {
      args.getString ("--datadist", origDataFile);

      DistanceMatrixLoader dmloader;
      dmloader.loadMatrix (origDataFile, origDataDist);
    }
    else
    {
      args.getString ("--datafile", origDataFile);

      SOMPackLoader loader (origDataFile);
      loader.loadData (origData);

      Euclidean metric;

      DistanceMatrix dm (origData, metric);
      origDataDist = dm;
    }



    lastNeighbor = args.getUInt ("--neighbors");


    if (args.hasArg ("--projdist"))
    {
      args.getString ("--projdist", projDataFile);

      DistanceMatrixLoader dmloader;
      dmloader.loadMatrix (projDataFile, projDataDist);
    }
    else
    {
      args.getString ("--projfile", projDataFile);

      SOMPackLoader loader (projDataFile);
      loader.loadData (projData);

      Euclidean metric;

      DistanceMatrix dm (projData, metric);
      projDataDist = dm;
    }


    ContTrust ctmeasure (lastNeighbor);
    ctmeasure.measure (origDataDist, projDataDist, std::cout);
  }
  catch (Exception & foo)
  {
    std::cout << "Error: " << foo.getErrMsg () << std::endl;
  }

  return 0;
}
