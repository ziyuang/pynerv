/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#include "ccaoptimization.hh"
#include <stdlib.h>
#include <locale>

namespace dredviz {
CCAOptimization::CCAOptimization (double initialAlpha,
                                  SeparableCostFunction & costFunc):
    alpha (initialAlpha, 0),
    costFunc (costFunc)
{
  srand (time (0));
}


void
CCAOptimization::perform (DataMatrix & outputData)
{


  //First, pick a point in the output space at random.

  size_t i = int ((double (rand ()) / RAND_MAX) *outputData.getRows ());


  for (size_t j = 0; j < outputData.getRows (); j++)
  {
    if (j != i)
    {
      const vector < double >&gradientComponent =
        costFunc.getGradientComponent (outputData, i, j);
      for (size_t m = 0; m < outputData.getCols (); m++)
      {
        outputData (j, m) += gradientComponent[m] * alpha.value ();
      }
    }
  }
}


void
CCAOptimization::updateDynamicParameters (size_t currentRound,
    size_t totalRounds)
{
  alpha.update (currentRound, totalRounds);
}
}
