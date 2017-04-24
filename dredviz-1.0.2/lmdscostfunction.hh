/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#ifndef LMDSCOSTFUNCTION_HH
#define LMDSCOSTFUNCTION_HH

#include "dataset.hh"
#include "distancematrix.hh"
#include "dynamicdouble.hh"
#include "separablecostfunction.hh"

#include <vector>

namespace dredviz {
class LMDSCostFunction:public SeparableCostFunction
{
private:
  const DistanceMatrix & origDist;

  const double lambda;

  DynamicDouble radius;

  vector < double >finalNeighborhoods;


  void calculateFinalNeighborhoods (size_t neighborhoodSize);

  class CompareIndices
  {
    const DistanceMatrix & origDist;
  public:

    size_t i;                 /*Iterator variable; declared here so that CompareIndices can
                                   access it. */


    CompareIndices (const DistanceMatrix & origDist):origDist (origDist),
        i (0)
    {
    }

    bool operator () (const size_t i1, const size_t i2)
    {
      return (origDist (i, i1) < origDist (i, i2));
    }
  };


public:
  const vector < double >&getGradientComponent (const DataMatrix & location,
      size_t i, size_t j);

  /*LMDSCostFunction()

     const DistanceMatrix& origDist: the original data as a distance matrix.

     const DataMatrix& projData: a matrix that represents the original data
     projected into the output space: projData(i,j) is the jth coordinate of
     the ith data point in the output space.

     DynamicDouble initialRadius: the initial radius of the neighborhood of
     each point.

     double lambda: Should be between 0 and 1; 0 means 'penalize
     untrustworthiness only', and similarly 1 means 'penalize discontinuity only.'

     size_t finalNeighborhoodSize: Once the neighborhood radius for a point has
     decreased to the point where it has finalNeighborhoodSize neighbors, it
     will not decrease further.

   */


  LMDSCostFunction (const DistanceMatrix & inputData,
                    const DataMatrix & projData,
                    DynamicDouble & initialRadius, double lambda,
                    size_t finalNeighborhoodSize);

  virtual void updateDynamicParameters (size_t currentRound,
                                        size_t totalRounds,
                                        const DataMatrix & projData);

  double getGradient (const DataMatrix & projData, DataMatrix & target);

  double evaluate (const DataMatrix & location);

  /* Dummy functions */

  void reportParameters(std::string& str) { str = ""; }

  virtual void updateDataRepresentation (const DataMatrix & newData)
  {
  }

};
}

#endif
