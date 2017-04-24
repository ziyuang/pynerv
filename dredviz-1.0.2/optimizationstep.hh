/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#ifndef OPTIMIZATIONSTEP_HH
#define OPTIMIZATIONSTEP_HH

#include "distancematrix.hh"
#include "datamatrix.hh"

namespace dredviz {
class OptimizationStep
{
public:
  virtual ~ OptimizationStep ()
  {
  }
  virtual void perform (DataMatrix & outputData) = 0;

  virtual void updateDynamicParameters (size_t currentRound,
                                        size_t totalRounds) = 0;
};

}
#endif
