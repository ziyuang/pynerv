/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#ifndef OPTIMIZATIONSTRATEGY_HH
#define OPTIMIZATIONSTRATEGY_HH

#include "dataset.hh"
#include "distancematrix.hh"
#include "optimizationstep.hh"
#include "costfunction.hh"

#include <ostream>

namespace dredviz {
class OptimizationStrategy
{
public:
  /* optimize() attempts to find a configuration of points in the desired
     output space that minimizes the cost function.


     const DistanceData& origDist: The original data as a distance matrix,
     that is, origDist(i,j) is the distance between points i and j in the
     original data.

     Matrix& initialProjData: a matrix that represents the original data
     projected into the output space: initialProjData(i,j) is the jth
     coordinate of the ith data point in the output space.

     OptimizationStep& optStep: The algorithm that performs a single
     optimization step.

     CostFunction& costFunc: The cost function to be minimized.

     std::ostream feedback: optimize() will report its progress into this
     stream.
   */

  virtual void optimize (const DistanceMatrix & origDist,
                         DataMatrix & initialProjData,
                         OptimizationStep & optStep, CostFunction & costFunc,
                         std::ostream & feedback) = 0;

  virtual ~ OptimizationStrategy ()
  {
  }
};
}

#endif
