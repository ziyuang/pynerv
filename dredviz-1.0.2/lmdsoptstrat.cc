/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#include "lmdsoptstrat.hh"
#include "separablecostfunction.hh"

#include <iostream>

namespace dredviz {
LMDSOptStrat::LMDSOptStrat (size_t totalRounds):totalRounds (totalRounds),
    currentRound (0)
{
}


void
LMDSOptStrat::optimize (const DistanceMatrix & origDist,
                        DataMatrix & projData,
                        OptimizationStep & optStep,
                        CostFunction & costFunc, std::ostream & feedback)
{
//  std::cout << "Initial cost: " << costFunc.evaluate (projData) << std::endl;

  for (currentRound = 0; currentRound < totalRounds; currentRound++)
  {
    optStep.updateDynamicParameters (currentRound, totalRounds);
    costFunc.updateDynamicParameters (currentRound, totalRounds, projData);
    optStep.perform (projData);
    if (currentRound % 100 == 0)
      std::cout << "round " << currentRound << ", cost: " << costFunc.
      evaluate (projData) << std::endl;
  }

  std::cout << "Final cost: " << costFunc.evaluate (projData) << std::endl;
}
}
