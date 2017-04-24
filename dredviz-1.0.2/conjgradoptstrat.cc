/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#include "conjgradoptstrat.hh"

namespace dredviz {
void
ConjGradOptStrat::optimize (const DistanceMatrix & origData,
                            DataMatrix & initialProjData,
                            OptimizationStepBatch & optStep,
                            CostFunction & costFunc, std::ostream & feedback)
{
  feedback << "Initial cost: " << costFunc.evaluate (initialProjData)
  << std::endl;

  optStep.setIterationsPerStep (MAX_ROUNDS);
  optStep.updateDynamicParameters (MAX_ROUNDS, MAX_ROUNDS);
  costFunc.updateDynamicParameters (MAX_ROUNDS, MAX_ROUNDS, initialProjData);
  optStep.perform (initialProjData);

  feedback << "Done.\n" << std::endl;
}
}
