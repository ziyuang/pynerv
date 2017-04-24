#include <string>
#include <vector>
#include <math.h>
#include <fstream>
#include <iostream>
#include "exception.hh"
#include "commandlineargs.hh"
#include "distancematrix.hh"
#include "distancematrixloader.hh"
#include "sompackloader.hh"
#include "euclidean.hh"
#include "dataset.hh"
#include "nervprobability.hh"

using namespace dredviz;

int main(int argc, char** argv)
{

  std::string inputfile;
  std::string outputfile;
  std::string outputdist;

  std::ofstream outputFile;

  DataSet origData;
  DistanceMatrix origDataDist;
  DistanceMatrix projDataDist;

  std::vector<std::string> switches;
  switches.push_back("--projfile");
  switches.push_back("--outputfile");
  switches.push_back("--datafile");
  switches.push_back("--datadist");
  switches.push_back("--projdist");
  switches.push_back("--neighbors");

  std::set<std::string> recognizedSwitches(switches.begin(), switches.end());

  CommandLineArgs args(argc, argv, recognizedSwitches);

  std::string usage = "\n"
                      "Synopsis: klmeasure [--datafile | --datadist] [INPUT_FILE] [--projfile | --projdist] [OUTPUT_FILE] --neighbors [NEIGHBORHOOD_SIZE]\n"
                      "\n"
                      "Compares the projection in PROJ_FILE with the original data in INPUT_FILE and calculates the smoothed recall and smoothed precision of the projection for neighborhood sizes ranging from 1 to NEIGHBORHOOD_SIZE. The values are printed to stdout.""\n"
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

    size_t effectiveNeighbors = 20;

    if (argc == 1 || args.hasArg ("--help"))
    {
      std::cerr << usage;
    }

    if (args.hasArg("--datafile"))
    {
      args.getString ("--datafile", inputfile);

      SOMPackLoader loader(inputfile);
      loader.loadData (origData);

      Euclidean metric;

      DistanceMatrix dm(origData, metric);
      origDataDist = dm;

      origDataDist.scale (1.0 / origDataDist.getAverage());
    }
    else
    {
      args.getString ("--datadist", inputfile);

      DistanceMatrixLoader dmloader;
      dmloader.loadMatrix (inputfile, origDataDist);

      origDataDist.scale (1.0 / origDataDist.getAverage());
    }


    std::string projDataFile;

    if (args.hasArg("--projfile"))
    {
      DataSet projData;

      args.getString ("--projfile", projDataFile);

      SOMPackLoader loader(projDataFile);
      loader.loadData (projData);

      Euclidean metric;

      DistanceMatrix dm(projData, metric);

      projDataDist = dm;

      projDataDist.scale (1.0 / projDataDist.getAverage());
    }
    else
    {
      args.getString ("--projdist", projDataFile);

      DistanceMatrixLoader dmloader;
      dmloader.loadMatrix (projDataFile, projDataDist);

      projDataDist.scale (1.0 / origDataDist.getAverage());
    }


    //args.getString("--outputfile", outputfile);

    effectiveNeighbors = args.getUInt("--neighbors");

//  std::cerr << "Calculating probabilities... ";

    NeRVProbability origProb(origDataDist);
    NeRVProbability projProb(projDataDist);

    std::vector<double> sigma(origProb.getRows());
    origProb.findSigma(sigma, effectiveNeighbors);

    origProb.update(sigma);
    projProb.update(sigma);

//  std::cerr << "Done.\n";

    //Calculate Kullback-Leibler divergences

    double ED_p_q = 0.0;
    double ED_q_p = 0.0;

    for (size_t i = 0; i < origProb.getRows(); i++)
      for (size_t j = 0; j < origProb.getRows(); j++)
      {
        if (j != i)
        {
          ED_p_q += origProb(i,j) * log(origProb(i,j) / projProb(i,j));
          ED_q_p += projProb(i,j) * log(projProb(i,j) / origProb(i,j));
        }
      }

    //outputFile.open(outputfile.c_str());

    //outputFile << "-1 " << ED_q_p << ", " << ED_p_q << " " << ED_q_p;

    std::cout << "Smoothed precision | Smoothed recall" << std::endl;
    
    std::cout << ED_q_p << " | " << ED_p_q << std::endl;

    //outputFile.close();

  }

  catch (Exception e)
  {
    std::cerr << e.getErrMsg() << std::endl;
  }

  return 0;
}
