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
#include "rankmatrix.hh"

using namespace dredviz;

int main(int argc, char** argv)
{

  std::vector<std::string> switches;
  switches.push_back("--projfile");
  switches.push_back("--datafile");
  switches.push_back("--projdist");
  switches.push_back("--neighbors");
  switches.push_back("--outputfile");

  std::set<std::string> recognizedSwitches(switches.begin(), switches.end());

  std::string inputfile;
  std::string outputfile;
  std::string outputdist;

  std::ofstream outputFile;

  size_t effectiveNeighbors;

  DataSet origData;
  DistanceMatrix* origDataDist = NULL;
  DistanceMatrix* projDataDist = NULL;

  RankMatrix* origRank = NULL;
  RankMatrix* projRankBest = NULL;
  RankMatrix* projRankWorst = NULL;
  RankMatrix* origRankReversed = NULL;


  std::string usage = "\n"
                      "Synopsis: klrank --datafile [INPUT_FILE] [--projfile | --projdist] [OUTPUT_FILE] --neighbors [NEIGHBORHOOD_SIZE]\n"
                      "\n"
                      "Compares the projection in PROJ_FILE with the original data in INPUT_FILE and calculates the smoothed recall and smoothed precision. NEIGHBORHOOD_SIZE sets the 'effective number of neighbors' used to control the width of the Gaussian; see references in README for details. The values are printed to stdout.""\n"
                      "\n""\n"
                      "Required parameters:"
                      "\n"
                      "--datafile DATA_FILE - The name of the file that contains (in SOM_PAK format) the data points to be visualized. "
                      "\n"
                      "--projfile PROJ_FILE - The name of the SOM_PAK file that contains the projected data points. If a distance matrix is provided via --projdist, this switch will be ignored."
                      "\n"
                      "--projdist PROJ_DIST - The name of the SOM_PAK file that contains a distance matrix of the projected data set."
                      "\n";

  try
  {
    CommandLineArgs args(argc, argv, recognizedSwitches);

    if (argc == 1 || args.hasArg ("--help"))
    {
      std::cerr << usage;
    }

    args.getString ("--datafile", inputfile);

    SOMPackLoader loader(inputfile);
    loader.loadData (origData);

    Euclidean metric;

    origDataDist = new DistanceMatrix(origData, metric);

    origDataDist->scale (1.0 / origDataDist->getAverage());

    origRankReversed = new RankMatrix();
    origRank = new RankMatrix(*origDataDist, *origRankReversed);

    origRank->scale (1.0 / origRank->getAverage());
    origRankReversed->scale (1.0 / origRankReversed->getAverage());




    std::string projDataFile;

    if (args.hasArg("--projfile"))
    {
      DataSet* projData = new DataSet();

      args.getString ("--projfile", projDataFile);

      SOMPackLoader loader(projDataFile);
      loader.loadData (*projData);

      Euclidean metric;

      projDataDist = new DistanceMatrix(*projData, metric);

      delete projData;
      projData = NULL;

      projDataDist->scale (1.0 / origDataDist->getAverage());
    }
    else
    {
      args.getString ("--projdist", projDataFile);

      DistanceMatrixLoader dmloader;
      projDataDist = new DistanceMatrix();

      dmloader.loadMatrix (projDataFile, *projDataDist);

      projDataDist->scale (1.0 / origDataDist->getAverage());
    }


    projRankBest = new RankMatrix(*projDataDist, *origRank, true);

    projRankBest->scale(1.0 / projRankBest->getAverage());

    projRankWorst = new RankMatrix(*projDataDist, *origRank, false);

    projRankWorst->scale(1.0 / projRankWorst->getAverage());

    delete projDataDist;
    projDataDist = NULL;


    NeRVProbability inputProb(*origRank);

    NeRVProbability outputProbBest(*projRankBest);

    NeRVProbability outputProbWorst(*projRankWorst);

    NeRVProbability inputProbReversed(*origRankReversed);


    std::vector<double> sigma(inputProb.getRows());

    effectiveNeighbors = args.getUInt("--neighbors");

    double temp = inputProb.findSigma(effectiveNeighbors, 0);

    for (size_t i = 0; i < sigma.size(); i++)
    {
      sigma[i] = temp;
    }

    inputProb.update(sigma);
    outputProbBest.update(sigma);
    outputProbWorst.update(sigma);
    inputProbReversed.update(sigma);

    delete origRankReversed;
    origRankReversed = NULL;


    //Calculate KL divergences

    double ED_p_q_best = 0.0;
    double ED_q_p_best = 0.0;

    double ED_p_q_worst = 0.0;
    double ED_q_p_worst = 0.0;

    double ED_p_q_max = 0.0;
    double ED_q_p_max = 0.0;

    for (size_t i = 0; i < inputProb.getRows(); i++)
      for (size_t j = 0; j < inputProb.getRows(); j++)
      {
        if (j != i)
        {
          // Best case:
          ED_p_q_best += inputProb(i,j)
                         * log(inputProb(i,j) / outputProbBest(i,j));
          ED_q_p_best += outputProbBest(i,j)
                         * log(outputProbBest(i,j) / inputProb(i,j));

          // Worst case:
          ED_p_q_worst += inputProb(i,j)
                          * log(inputProb(i,j) / outputProbWorst(i,j));
          ED_q_p_worst += outputProbWorst(i,j)
                          * log(outputProbWorst(i,j) / inputProb(i,j));

          // Maximum error (for normalization):

          ED_p_q_max += inputProb(i,j)
                        * log(inputProb(i,j) / inputProbReversed(i,j));
          ED_q_p_max += inputProbReversed(i,j)
                        * log(inputProbReversed(i,j) / inputProb(i,j));

        }
      }

    /* Normalize measures between 0 and 1 */

    ED_p_q_best /= ED_p_q_max;
    ED_p_q_worst /= ED_p_q_max;
    ED_q_p_best /= ED_q_p_max;
    ED_q_p_worst /= ED_q_p_max;


    //args.getString("--outputfile", outputfile);


    //outputFile.open(outputfile.c_str());

    //outputFile << "#ED_p_q_best *worst ED_q_p_best *worst\n";

    //outputFile << ED_p_q_best << " " << ED_p_q_worst << " ";

    //outputFile << ED_q_p_best << " " << ED_q_p_worst << "\n";

    std::cout << "#(1-precision_best) (1-precision_worst) (1-recall_best) (1-recall_worst)\n";

    std::cout << ED_p_q_best << " " << ED_p_q_worst << " ";

    std::cout << ED_q_p_best << " " << ED_q_p_worst << "\n";

    //outputFile.close();
  }

  catch (Exception& exc)
  {
    std::cerr << "Error: " << exc.getErrMsg() << "\n";
  }

  delete origRank;
  delete origRankReversed;
  delete projRankBest;
  delete projRankWorst;
  delete origDataDist;
  delete projDataDist;

  return 0;
}
