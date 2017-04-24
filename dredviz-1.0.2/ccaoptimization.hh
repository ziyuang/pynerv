/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#ifndef CCAOPTIMIZATION_HH
#define CCAOPTIMIZATION_HH

#include "optimizationstep.hh"
#include "dynamicdouble.hh"
#include "separablecostfunction.hh"

namespace dredviz {
class CCAOptimization:public OptimizationStep
{
private:
  DynamicDouble alpha;
  SeparableCostFunction & costFunc;

public:
  CCAOptimization (double initialAlpha, SeparableCostFunction & costFunc);

  void perform (DataMatrix & outputData);
  void updateDynamicParameters (size_t currentRound, size_t totalRounds);
};
}
#endif
