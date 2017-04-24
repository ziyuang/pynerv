/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#ifndef CONJGRADOPTSTRAT_HH
#define CONJGRADOPTSTRAT_HH

#include "optimizationstrategybatch.hh"
#include "optimizationstepbatch.hh"

namespace dredviz {
class ConjGradOptStrat:public OptimizationStrategyBatch
{
private:
  const size_t MAX_ROUNDS;
public:

  void optimize (const DistanceMatrix & origData,
                 DataMatrix & initialProjData,
                 OptimizationStepBatch & optStep, CostFunction & costFunc,
                 std::ostream & feedback);

  ConjGradOptStrat (size_t maxIterations = 100):MAX_ROUNDS (maxIterations)
  {
  }
};
}

#endif
