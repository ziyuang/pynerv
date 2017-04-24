/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#ifndef LMDSOPTSTRAT_HH
#define LMDSOPTSTRAT_HH

#include "optimizationstrategy.hh"

#include <iostream>

namespace dredviz {
class LMDSOptStrat:public OptimizationStrategy
{
private:
  size_t totalRounds;
  size_t currentRound;

public:
  LMDSOptStrat (size_t totalRounds);

  virtual void optimize (const DistanceMatrix & origDist,
                         DataMatrix & initialProjData,
                         OptimizationStep & optStep, CostFunction & costFunc,
                         std::ostream & feedback);
};

}
#endif
